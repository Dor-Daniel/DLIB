# darray

`darray.h` is a single-header, generic dynamic array for C, with a choice of backend behind one API. You pick the backend once, at creation time, and every other function works the same way regardless of which one you chose.

```c
int* nums = (int*)darr_create(128, sizeof(int), DARR_TYPE_CONTIGUOUS, NULL);
```

Like the rest of this header family, it's untyped: you give it the element size once at creation, and the array is handed back to you as `void*` so you can cast it to whatever you're actually storing.

---

## Table of contents

1. [How it works](#how-it-works)
2. [Backend tradeoffs](#backend-tradeoffs)
3. [Getting started](#getting-started)
4. [Quick tour](#quick-tour)
5. [API guide](#api-guide)
6. [Custom allocators](#custom-allocators)
7. [Future plans](#future-plans)

---

## How it works

### One tagged header per backend

Every darray keeps a small header in the same allocation as its element storage, immediately before it — the pointer you get back from `darr_create` (and friends) points at the first element, not the header. The header's *last* byte is always a `darr_type_e` tag, so any function that only takes a `void*` can step back exactly one byte to read the tag, find out which backend it's looking at, and then step back the rest of the way using that backend's own header shape.

This is what makes the backend a pure implementation detail behind one API: `darrlen`, `darr_at`, `darr_push_back`, and everything else dispatch on that tag internally. You never tell them which backend you're holding.

This design is meant to be extended, not just used as-is: adding a new backend is a matter of giving it its own header struct (ending in the same `darr_type_e type` tag as the last byte), its own `DARR_TO_HEAD_*` macro to step back to that header, and a new case in each function's dispatch switch. `darr_type_e` itself already reserves room for this — it's just an enum you add a value to.

### The contiguous backend

```
 one allocation
 +--------------------------------+-----+-----+-----+------+
 | darr_contiguous_head           |  0  |  1  |  2  | ...  |
 | (count, capacity, item_size,   |
 |  allocator, type)              |
 +--------------------------------+-----+-----+-----+------+
                                    ^
                                    +-- the returned pointer
```

Elements sit back-to-back, so indexing is direct pointer arithmetic — you can even treat the returned pointer as a normal C array (`my_arr[i]`).

### The linked-list backend

```
 one allocation
 +---------------------------+-------+-------+-------+-------+-------+-------+-----+
 | darr_linked_list_head     | next, | value | next, | value | next, | value | ... |
 | (count, capacity,         | prev  |       | prev  |       | prev  |       |     |
 |  item_size, head, tail,   |       |       |       |       |       |       |     |
 |  allocator, type)         |       |       |       |       |       |       |     |
 +---------------------------+-------+-------+-------+-------+-------+-------+-----+
                               \_____________/ \_____________/ \_____________/
                                   entry 0          entry 1          entry 2
                               ^
                               +-- the returned pointer (entry 0)
```

Entries live in the same block as the header, back-to-back like the contiguous backend's elements — but each entry carries its own `next`/`prev` (indices into this same block, not pointers), and the header's `head`/`tail` fields say where the logical list actually starts and ends. That's what lets insertion or removal happen without shifting every following entry, at the cost of no longer being able to jump straight to an arbitrary index — reaching one means walking the chain.

`darr_at` is the one function that works identically for both backends, so it's the way to read or write an element in code that's meant to work with either. Direct indexing (`my_arr[i]`) is a convenience specific to the contiguous backend.

### Growth

`darr_push_back` and `darr_push_at` are the functions that can grow a darray. When there's no room left, the block is reallocated and every field — including the caller's own pointer, via the `void**` these functions take — is updated to the new address.

---

## Backend tradeoffs

| | Contiguous | Linked list |
|---|---|---|
| Random access by index (`darr_at`, `my_arr[i]`) | O(1) | O(index) — walks from the head |
| Push/pop at the end | O(1) amortized | O(1) amortized |
| Insert/remove at a known index | O(n) — shifts everything after it | O(index) to reach the position, O(1) to link it in once there |
| Memory overhead per element | none | one `next` + one `prev` index alongside each value |
| Cache locality | best — one flat block | worse — still one allocation, but entries are visited out of storage order as the list is walked |

The important nuance: on this API, `darr_push_at`/`darr_pop_at` still take a plain index, and the linked-list backend has to walk from `head` to reach it — so switching backends doesn't turn "insert at index 500" into an O(1) operation. What it removes is the cost of shifting every element after the insertion point. That trade only pays off when insertions and removals cluster near the front (where the walk is short) — for example, treating the darray as a work queue where you mostly `darr_push_at(darr, item, 0)` and `darr_pop_back`.

**Use the contiguous backend** when you mostly read by index or iterate front-to-back, and mostly change the array at the end — this is the right default for most uses.

**Consider the linked list backend** when insertions/removals happen close to the front (or you're walking the darray with `darr_at` and removing near where you currently are) often enough that avoiding the shift is worth the extra per-element bookkeeping and the loss of O(1) random access.

---

## Getting started

### Requirements

- A C compiler with support for enums with a fixed underlying type (`enum foo : u8 { ... }`), which recent GCC and Clang accept.
- `"../utils/ddefines.h"` on the include path, providing:
  - `u8`, `i32`, `u32`, `u64`, `bool`
  - `struct dallocator` and the `dallocator_t` pointer typedef (three function pointers: `allocate`, `reallocate`, `free`)
  - `DALLOCATOR_DEFAULT`, a compound literal wrapping `malloc`/`realloc`/`free`
  - `<stdlib.h>` included from within `ddefines.h` itself, since `DALLOCATOR_DEFAULT` needs it

A `ddefines.h` that satisfies this looks like:

```c
// utils/ddefines.h
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint8_t  u8;
typedef int32_t  i32;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct dallocator {
    void* (*allocate)(u64);
    void* (*reallocate)(void*, u64);
    void  (*free)(void*);
} * dallocator_t;

#define DALLOCATOR_DEFAULT (struct dallocator){ .allocate = malloc, .reallocate = realloc, .free = free }
```

### Project layout

```
include/
├── utils/
│   └── ddefines.h
└── darray/
    └── darray.h
src/
├── main.c
└── darray_impl.c
```

### Enabling the implementation

```c
// src/darray_impl.c
#define DARRAY_IMPLEMENTATION
#include "darray/darray.h"
```

Every other file just includes the header normally, without the define.

### Compiling

```bash
gcc -Iinclude src/main.c src/darray_impl.c -o app
```

---

## Quick tour

### Contiguous

```c
#include <stdio.h>
#include "darray/darray.h"

int main(void)
{
    int* nums = (int*)darr_create(128, sizeof(int), DARR_TYPE_CONTIGUOUS, NULL);

    for (int i = 0; i < 5; i++)
        darr_push_back((void**)&nums, &i);

    for (u32 i = 0; i < darrlen(nums); i++)
        printf("%d ", nums[i]);
    printf("\n"); /* 0 1 2 3 4 */

    if (darrlen(nums) > 2)
        nums[2] = 1729;

    int popped;
    darr_pop_back((void**)&nums, &popped);
    printf("popped %d, length is now %u\n", popped, darrlen(nums));

    darr_destroy(nums);
    return 0;
}
```

### Linked list

```c
#include <stdio.h>
#include "darray/darray.h"

int main(void)
{
    void* nums = darr_create(128, sizeof(int), DARR_TYPE_LINKED_LIST, NULL);

    for (int i = 0; i < 5; i++)
        darr_push_back(&nums, &i);

    /* Indexing directly doesn't work for this backend — use darr_at. */
    for (u32 i = 0; i < darrlen(nums); i++)
        printf("%d ", *(int*)darr_at(nums, i));
    printf("\n"); /* 0 1 2 3 4 */

    if (darrlen(nums) > 2)
        *(int*)darr_at(nums, 2) = 1729;

    darr_destroy(nums);
    return 0;
}
```

`darr_at` works for both backends, so it's the one to reach for in code that's meant to work with either — the direct-indexing style in the first example is a convenience specific to the contiguous backend.

---

## API guide

Complexity is given per backend where they differ. `n` is `darrlen(darray)`.

| Function | Contiguous | Linked list | What it does |
|---|---|---|---|
| `darr_create(capacity, item_size, type, allocator)` | O(capacity) | O(capacity) | Creates a new, empty darray of the given backend with room for `capacity` elements. |
| `darr_from_darr(darray)` | O(n) | O(n) | Duplicates an existing darray, keeping the same backend and allocator. |
| `darr_from_carr(carray, item_size, length, type, allocator)` | O(length) | O(length) | Copies a plain C array into a new darray of the given backend. |
| `darr_from_darr_items(darray, type, allocator)` | O(n) | O(n) | Copies an existing darray's elements into a *new* darray, letting you change the backend and/or allocator along the way. |
| `darr_push_back(darray, item)` | O(1) amortized | O(1) amortized | Appends `item` to the end, growing the darray if needed. Returns `false` if it needed to grow and couldn't. |
| `darr_pop_back(darray, out_item)` | O(1) | O(1) | Copies the last element into `out_item` and removes it. |
| `darr_push_at(darray, item, index)` | O(n) | O(index) | Inserts `item` at `index`. `index == darrlen(darray)` appends. |
| `darr_pop_at(darray, out_item, index)` | O(n) | O(index) | Removes the element at `index`, copying it into `out_item` first. |
| `darr_sort(darray, sort_type, compare)` | O(n log n) avg | — | Sorts in place using a `qsort`-style comparator. Currently sorts the contiguous backend; linked-list sorting isn't implemented yet. |
| `darr_at(darray, index)` | O(1) | O(index) | Returns a pointer to the element at `index`. |
| `darrlen(darray)` | O(1) | O(1) | Number of elements currently stored. |
| `darr_is_empty(darray)` | O(1) | O(1) | Macro for `darrlen(darray) == 0`. |
| `darr_destroy(darray)` | O(1) | O(1) | Frees the darray. |

`darr_push_back`, `darr_pop_back`, `darr_push_at`, and `darr_pop_at` all take `void**` (a pointer to your darray variable), since any of them may need to point it at a new address. Everything else takes a plain `void*`.

---

## Custom allocators

```c
typedef struct dallocator {
    void* (*allocate)(u64);
    void* (*reallocate)(void*, u64);
    void  (*free)(void*);
} * dallocator_t;
```

```c
static int live = 0;
static void* my_alloc(u64 n)            { live++; return malloc(n); }
static void* my_realloc(void* p, u64 n) { return realloc(p, n); }
static void  my_free(void* p)           { live--; free(p); }

static struct dallocator tracked = { my_alloc, my_realloc, my_free };

void* arr = darr_create(64, sizeof(int), DARR_TYPE_CONTIGUOUS, &tracked);
```

`NULL` falls back to `DALLOCATOR_DEFAULT` (plain `malloc`/`realloc`/`free`), defined in `ddefines.h`. The allocator is stored in the darray's header at creation time and reused for later growth and for `darr_destroy`.

---

## Future plans

- **More backends.** The tagged-header design already exists to support this — see [How it works](#how-it-works). A chunked/deque-style backend sitting between the two current ones (better cache locality than the linked list, cheaper front insertion than the contiguous block) is a natural next candidate.
- **More sorting algorithms and array utilities.** `darr_sort` currently offers one algorithm (`DARR_SORT_TYPE_QUICK_SORT`) and only for the contiguous backend; more are planned, along with general array algorithms beyond sorting.
- **Remove the `ddefines.h` dependency**, so this gets closer to a true single-header, dependency-free library.
- **A compile-time option for a thread-safe build.**