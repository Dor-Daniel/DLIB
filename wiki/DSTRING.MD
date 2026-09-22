# dstring

A single-header C library for **dynamic, length-aware strings**, with a **string builder** and a **string reader** (a lightweight cursor for parsing).

- **O(1) length.** Every string remembers its length, so you never call `strlen` on your own strings.
- **C-compatible.** Every string is also null-terminated, so it works directly with `printf("%s", ...)`, `fopen(...)`, and other C APIs.
- **Pluggable memory.** Every function that allocates takes an optional allocator. Pass `NULL` to use `malloc` / `realloc` / `free`.
- **NULL-safe.** Functions return `NULL` (or do nothing) when given `NULL` inputs or when an allocation fails.
- **Builder.** Append text efficiently, then turn it into a string.
- **Reader.** Walk through a string token by token without modifying it.

---

## Table of contents

1. [Getting started](#getting-started)
2. [Core concepts](#core-concepts)
3. [Quick tour](#quick-tour)
4. [API guide](#api-guide)
   - [Creating strings](#creating-strings)
   - [Searching and comparing](#searching-and-comparing)
   - [Modifying in place](#modifying-in-place)
   - [Splitting](#splitting)
   - [Destroying strings](#destroying-strings)
   - [String builder](#string-builder)
   - [String reader](#string-reader)
5. [Custom allocators](#custom-allocators)
6. [Behavior notes](#behavior-notes)
7. [Planned features](#planned-features)

---

## Getting started

### Requirements

- A **C99** (or newer) compiler. The implementation uses designated initializers and compound literals, and it is written as C, not C++.
- The header includes `"../utils/ddefines.h"`. That file must provide the types used by the API: **`u64`**, **`u32`**, and **`bool`**.

### Enabling the implementation (exactly once)

This is a single-header library. The function bodies are compiled only in the one `.c` file that defines `DSTRING_IMPLEMENTATION` before including the header:

```c
// src/dstring_impl.c
#define DSTRING_IMPLEMENTATION
#include "dstring/dstring.h"
```

Every other file just includes the header normally:

```c
// src/main.c
#include "dstring/dstring.h"
```

### Compiling

```bash
gcc -Iinclude src/main.c src/dstring_impl.c -o app
```

```bash
clang -Iinclude src/main.c src/dstring_impl.c -o app
```

If you see linker errors such as *undefined reference to `dstr_create_from_cstr`*, the implementation file is missing from the build, or `DSTRING_IMPLEMENTATION` was not defined before the include.

---

## Core concepts

### What is a `dstr_t`?

`dstr_t` is just `char *`, but it points into a block of memory that has a hidden length header in front of the text:

```
 one allocation
 +----------------+---+---+---+---+---+------+
 | length  (u64)  | H | e | l | l | o | '\0' |
 +----------------+---+---+---+---+---+------+
                    ^
                    +-- the dstr_t points HERE (the first character)
```

Consequences:

- You can use a `dstr_t` anywhere a normal C string is accepted (`printf("%s", s)`, `fputs`, `fopen`, ...).
- Read the length with the **`dstrlen(s)`** macro. It reads the hidden header, and it is an lvalue, so **don't assign to it**.
- **Never** call `free()` or `realloc()` on a `dstr_t`, because the real allocation starts *before* the pointer. Always use [`dstr_destroy`](#destroying-strings).
- `dstrlen(NULL)` is undefined behavior, so check for `NULL` first (see the next section for why a string can be `NULL`).

### Empty strings are `NULL`

**A dstring is never empty.** Wherever an empty string would be produced, the library returns `NULL` instead. This applies everywhere:

- `dstr_create_from_cstr("")` and `dstr_create_from_chars(chars, 0, ...)` return `NULL`.
- A format that produces no text, or a concatenation of two empty inputs, returns `NULL`.
- Substrings, builder contents, and reader tokens that come out empty are `NULL`.

So a `NULL` result means one of three things: invalid input, allocation failure, or "the result is empty". Every function that takes a string treats `NULL` safely, so `dstr_destroy(NULL, ...)` is fine, but check results before calling `dstrlen` or `printf("%s", ...)` on them.

> Note that because `NULL` stands in for an empty string, passing a `NULL` dstring to a function that needs two inputs (for example `dstr_create_from_dstrs_concat`) returns `NULL` rather than treating it as an empty string.

### Handle types

`dstr_builder_t` and `dstr_reader_t` are **already pointer types** (opaque handles). Declare them without an extra `*`:

```c
dstr_builder_t sb = dstr_builder_create(64, NULL);   /* correct */
dstr_reader_t  r  = dstr_reader_create(text, NULL);  /* correct */
```

### Naming conventions

The suffix of a function name tells you what kind of input it takes:

| Suffix | Meaning | Type |
|---|---|---|
| `_dstr` / `_dstrs` | Input is a dstring | `dstr_t` |
| `_cstr` / `_cstrs` | Input is a **null-terminated** C string | `const char *` |
| `_chars` | Input is a raw buffer **plus an explicit count** (need not be null-terminated) | `const char[]`, `u64 count` |

### Ownership

Every `dstr_create_*` function returns a **new allocation that you own**. Destroy it with `dstr_destroy` when you are done, passing **the same allocator** you created it with.

### Allocators

Every function that allocates takes a `const dstr_memory_allocator *allocator` argument:

- Pass **`NULL`** to use the standard `malloc` / `realloc` / `free`.
- Pass a pointer to your own `dstr_memory_allocator` to use a custom one. See [Custom allocators](#custom-allocators).

> There are no default-argument convenience macros yet, so you must always write the `NULL` explicitly.

### Error handling

- **Creating functions** (`dstr_create_*`, `dstr_builder_generate_dstr`, `dstr_reader_read_*`, `dstr_builder_chop_*`) return `NULL` on invalid input, allocation failure, or an empty result. Check the result.
- **Builder mutators** (`append`, `reserve`, and so on) return nothing. If the buffer cannot grow, the operation is **silently skipped**.
- Passing `NULL` for a string, builder or reader is safe for nearly every function (it returns `NULL` / `false` or does nothing).

---

## Quick tour

```c
#include <stdio.h>
#include "dstring/dstring.h"

int main(void)
{
    /* Create and concatenate (with a delimiter between the parts) */
    dstr_t greeting = dstr_create_from_cstr("hello", NULL);
    dstr_t target   = dstr_create_from_cstr("world", NULL);
    dstr_t message  = dstr_create_from_dstrs_concat_with_delim(greeting, target, ' ', NULL);

    printf("%s (%llu chars)\n", message, (unsigned long long)dstrlen(message));
    /* -> hello world (11 chars) */

    /* Inspect */
    u64 where = 0;
    if (dstr_is_contain_char(message, 'w', &where))
        printf("'w' is at index %llu\n", (unsigned long long)where);   /* 6 */

    if (dstr_is_prefix_cstr(message, "hello"))
        puts("starts with \"hello\"");

    /* Modify in place */
    dstr_to_upper(message);
    printf("%s\n", message);                                           /* HELLO WORLD */

    /* Build a new string piece by piece */
    dstr_builder_t sb = dstr_builder_create(64, NULL);
    dstr_builder_append_cstr(sb, "[");
    dstr_builder_append_dstr(sb, message);
    dstr_builder_append_cstr_format(sb, "] x%d", 3);

    dstr_t wrapped = dstr_builder_generate_dstr(sb, NULL);
    printf("%s\n", wrapped);                                           /* [HELLO WORLD] x3 */

    /* Clean up: destroy everything you created */
    dstr_destroy(wrapped, NULL);
    dstr_builder_destroy(sb);
    dstr_destroy(message, NULL);
    dstr_destroy(target, NULL);
    dstr_destroy(greeting, NULL);
    return 0;
}
```

---

## API guide

Throughout this section, `alloc` means `const dstr_memory_allocator *` (use `NULL` for the default).

### Creating strings

| Function | What it does |
|---|---|
| `dstr_create_from_cstr(cstr, alloc)` | Copies a null-terminated C string. |
| `dstr_create_from_chars(chars, count, alloc)` | Copies `count` characters from a buffer. |
| `dstr_create_from_dstr(dstr, alloc)` | Makes an independent copy of another dstring. |
| `dstr_create_from_cstr_format(alloc, fmt, ...)` | `printf`-style formatting into a new dstring. **Note the allocator comes first.** |
| `dstr_create_from_dstrs_concat(a, b, alloc)` | `a + b` |
| `dstr_create_from_cstrs_concat(a, b, alloc)` | `a + b` (both null-terminated) |
| `dstr_create_from_chars_concat(a, b, a_count, b_count, alloc)` | `a + b` from raw buffers |
| `dstr_create_from_dstrs_concat_with_delim(a, b, delim, alloc)` | `a + delim + b` |
| `dstr_create_from_cstrs_concat_with_delim(a, b, delim, alloc)` | `a + delim + b` (both null-terminated) |
| `dstr_create_from_chars_concat_with_delim(a, b, a_count, b_count, delim, alloc)` | `a + delim + b` from raw buffers |
| `dstr_create_substring_by_delim(dstr, delim, from_start, alloc)` | `from_start = true`: the text **before the first** `delim`. `from_start = false`: the text **after the last** `delim`. The delimiter is not included. If `delim` is absent, a copy of the whole string is returned. Returns `NULL` if the piece is empty. |
| `dstr_create_substring_by_delims(dstr, delims, count, from_start, alloc)` | Same idea with a multi-character delimiter (`delims` is `count` characters long). Returns `NULL` if the delimiter is not found, `count == 0`, `count >=` the string length, or the piece is empty. |

```c
dstr_t a = dstr_create_from_cstr("usr", NULL);
dstr_t b = dstr_create_from_cstr("local", NULL);

dstr_t path = dstr_create_from_dstrs_concat_with_delim(a, b, '/', NULL);   /* "usr/local" */
dstr_t copy = dstr_create_from_dstr(path, NULL);                            /* independent copy */
dstr_t raw  = dstr_create_from_chars("abcdef", 3, NULL);                    /* "abc" */

/* Formatting: the allocator comes FIRST */
dstr_t num  = dstr_create_from_cstr_format(NULL, "item-%d", 7);             /* "item-7" */

/* Single-character delimiter */
dstr_t head = dstr_create_substring_by_delim(path, '/', true,  NULL);       /* "usr"   */
dstr_t tail = dstr_create_substring_by_delim(path, '/', false, NULL);       /* "local" */

/* Multi-character delimiter */
dstr_t url    = dstr_create_from_cstr("http://example.com", NULL);
dstr_t scheme = dstr_create_substring_by_delims(url, "://", 3, true,  NULL); /* "http"        */
dstr_t host   = dstr_create_substring_by_delims(url, "://", 3, false, NULL); /* "example.com" */
```

### Searching and comparing

| Function | Returns `true` when... |
|---|---|
| `dstr_is_contain_char(dstr, c, &index)` | `c` occurs in `dstr`. `index` (optional) receives the first position. |
| `dstr_is_contain_dstr(text, pattern, &index)` | `pattern` (a dstring) occurs in `text`. |
| `dstr_is_contain_cstr(text, pattern, &index)` | `pattern` (a C string) occurs in `text`. |
| `dstr_is_prefix_dstr(text, pattern)` | `text` starts with `pattern`. |
| `dstr_is_prefix_cstr(text, pattern)` | `text` starts with `pattern`. |
| `dstr_is_suffix_dstr(text, pattern)` | `text` ends with `pattern`. |
| `dstr_is_suffix_cstr(text, pattern)` | `text` ends with `pattern`. |
| `dstr_is_equal_dstrs(a, b)` | Same length and same characters. |
| `dstr_is_equal_cstrs(dstr, cstr)` | Same length and same characters. |

The `index` out-parameter is optional: pass `NULL` if you only need the yes/no answer. All of these return `false` if any argument is `NULL`. A pattern equal to the whole text counts as a match for `contain`, `prefix`, `suffix` and `equal`.

```c
dstr_t text = dstr_create_from_cstr("a needle in a haystack", NULL);

u64 at;
if (dstr_is_contain_cstr(text, "needle", &at)) { /* at == 2 */ }

if (dstr_is_prefix_cstr(text, "a needle"))     { /* true */ }
if (dstr_is_suffix_cstr(text, "haystack"))     { /* true */ }
if (dstr_is_equal_cstrs(text, "something"))    { /* false */ }
```

### Modifying in place

These change the string's characters directly. None of them can change its length.

| Function | What it does |
|---|---|
| `dstr_to_lower(dstr)` | Converts `A-Z` to `a-z`. Other characters are unchanged (ASCII only). |
| `dstr_to_upper(dstr)` | Converts `a-z` to `A-Z`. Other characters are unchanged (ASCII only). |
| `dstr_replace_char(dstr, to_replace, replacement)` | Replaces the **first** occurrence of a character. |
| `dstr_replace_chars(dstr, to_replace, replacement, length)` | Replaces the **first** occurrence of a `length`-character sequence with another sequence of the **same length**. Does nothing if `length` is greater than the string length. |

```c
dstr_t s = dstr_create_from_cstr("Hello, World", NULL);
dstr_to_lower(s);                       /* "hello, world" */
dstr_replace_char(s, ',', ';');         /* "hello; world" */

char from[] = "wor", to[] = "WOR";
dstr_replace_chars(s, from, to, 3);     /* "hello; WORld" */
```

### Splitting

| Function | What it does |
|---|---|
| `dstr_break_by_delim(dstr, delim, out_array, &out_length, alloc)` | Splits `dstr` at each `delim` into new dstrings. **Empty segments are skipped**, so leading, trailing and consecutive delimiters are ignored. |
| `dstr_break_by_delim_start_end(dstr, start, end, out_array, alloc)` | Splits `dstr` into 3 parts around a `start` and an `end` delimiter: `before + start + middle + end + after`. |

#### `dstr_break_by_delim`

Use it in two passes (count, then fill):

1. Call it with `out_array = NULL` to learn how many pieces there will be.
2. Allocate an array of `dstr_t` yourself.
3. Call it again with the array to fill it.
4. When finished, `dstr_destroy` **each** element, then free the array yourself. The function never allocates or frees the array itself.

Initialize `n` to `0` before the call: if `dstr` is `NULL` the function returns without writing `out_length`.

```c
dstr_t csv = dstr_create_from_cstr("red,green,blue", NULL);

u64 n = 0;
dstr_break_by_delim(csv, ',', NULL, &n, NULL);            /* n == 3 */

dstr_t *parts = malloc(n * sizeof(dstr_t));
dstr_break_by_delim(csv, ',', parts, &n, NULL);           /* { "red", "green", "blue" } */

for (u64 i = 0; i < n; i++) {
    printf("%s\n", parts[i]);
    dstr_destroy(parts[i], NULL);
}
free(parts);
dstr_destroy(csv, NULL);
```

#### `dstr_break_by_delim_start_end`

`out_array` must have room for **3** entries (`dstr_t out[3]`). The function fills all three:

- `out[0]` is the text before the first `start`, `out[1]` the text between it and the first `end` after it, and `out[2]` the text after that `end`.
- Any part that is empty is `NULL`.
- If either delimiter is missing, all three entries are `NULL`.
- `start` and `end` may be the same character (for example `"` to pull out quoted text).

Destroy every non-`NULL` entry when you are done.

```c
dstr_t s = dstr_create_from_cstr("key[index]rest", NULL);

dstr_t out[3];
dstr_break_by_delim_start_end(s, '[', ']', out, NULL);
/* out = { "key", "index", "rest" } */

for (int i = 0; i < 3; i++) {
    if (out[i]) {
        printf("out[%d] = %s\n", i, out[i]);
        dstr_destroy(out[i], NULL);
    }
}
dstr_destroy(s, NULL);
```

### Destroying strings

```c
void dstr_destroy(dstr_t dstr, const dstr_memory_allocator *allocator);
```

Frees a dstring. Pass the **same allocator** that was used to create it (`NULL` for the default). Passing a `NULL` string is safe. Do not use the string afterwards.

### String builder

A builder is a growable character buffer. Use it when you are assembling text from many pieces, because appending is cheaper than repeatedly concatenating immutable strings.

```
create -> append / edit ... -> generate_dstr (optional, can be repeated) -> destroy
```

The builder remembers the allocator you gave it, so `dstr_builder_destroy` needs none.

#### Lifecycle

| Function | What it does |
|---|---|
| `dstr_builder_create(initial_capacity, alloc)` | Creates a builder, or returns `NULL` on failure. An `initial_capacity` of `0` uses a default of 32 bytes. |
| `dstr_builder_generate_dstr(builder, alloc)` | Returns a **new, independent** dstring with the builder's current contents (`NULL` if the builder is empty). The builder stays usable. |
| `dstr_builder_destroy(builder)` | Frees the builder and its buffer. |
| `dstr_builder_reserve(builder, amount)` | Grows the capacity by `amount` bytes. |

#### Appending

| Function | What it does |
|---|---|
| `dstr_builder_append_dstr(builder, dstr)` | Appends a dstring. |
| `dstr_builder_append_cstr(builder, cstr)` | Appends a null-terminated C string. |
| `dstr_builder_append_chars(builder, chars, count)` | Appends `count` characters. |
| `dstr_builder_append_char(builder, c)` | Appends one character. |
| `dstr_builder_append_cstr_format(builder, fmt, ...)` | Appends `printf`-style formatted text. |

The buffer grows automatically. If it cannot grow, the append is silently skipped.

#### Editing

| Function | What it does |
|---|---|
| `dstr_builder_remove_at_char(builder, index)` | Removes one character. Out-of-range indices are ignored. |
| `dstr_builder_remove_at_chars(builder, index, count)` | Removes a range. The range is clamped to the end. |
| `dstr_builder_remove_prefix(builder, n)` | Removes the first `n` characters (clears the builder if `n >=` its length). |
| `dstr_builder_remove_suffix(builder, n)` | Removes the last `n` characters (clears the builder if `n >=` its length). |
| `dstr_builder_remove_all_char(builder, c)` | Removes every occurrence of `c`. |
| `dstr_builder_replace_all_char(builder, from, to)` | Replaces every `from` with `to`. |
| `dstr_builder_replace_at_dstr(builder, replacement, start)` | Overwrites existing content starting at `start`. |
| `dstr_builder_replace_at_cstr(builder, replacement, start)` | Same, with a C string. |
| `dstr_builder_replace_at_chars(builder, replacement, count, start)` | Same, with a buffer and count. |
| `dstr_builder_chop_prefix_by_delim(builder, delim, alloc)` | Returns the text before the **first** `delim` as a new dstring, and removes it **and the delimiter** from the builder. |
| `dstr_builder_chop_suffix_by_delim(builder, delim, alloc)` | Returns the text after the **last** `delim` as a new dstring, and removes it **and the delimiter** from the builder. |

Details:

- `replace_at_*` only overwrites what is already in the builder. If the replacement would run past the current end (`start + length > count`), nothing happens. It never grows the builder.
- The `chop_*` functions return `NULL` if the piece they cut off is empty. If `delim` is absent, they return everything and leave the builder empty.

#### Inspecting

| Function | What it does |
|---|---|
| `dstr_builder_get_chars_count(builder)` | Number of characters currently stored. |
| `dstr_builder_get_capacity(builder)` | Current buffer capacity in bytes. |
| `dstr_builder_get_chars(builder)` | Pointer to the raw buffer. **It is not null-terminated**, and it is invalidated by any call that appends or reserves (the buffer may move). |

To print a builder's raw contents without generating a dstring:

```c
printf("%.*s\n", (int)dstr_builder_get_chars_count(sb), dstr_builder_get_chars(sb));
```

#### Examples

Assembling text:

```c
dstr_builder_t sb = dstr_builder_create(32, NULL);

for (int i = 1; i <= 3; i++) {
    dstr_builder_append_cstr_format(sb, "line %d", i);
    dstr_builder_append_char(sb, '\n');
}

dstr_t text = dstr_builder_generate_dstr(sb, NULL);
fputs(text, stdout);

dstr_destroy(text, NULL);
dstr_builder_destroy(sb);
```

Chopping pieces off the front:

```c
dstr_builder_t b = dstr_builder_create(0, NULL);      /* 0 = default capacity */
dstr_builder_append_cstr(b, "key=value");

dstr_t key = dstr_builder_chop_prefix_by_delim(b, '=', NULL);
/* key == "key", and the builder now holds "value" */

dstr_destroy(key, NULL);
dstr_builder_destroy(b);
```

### String reader

A reader is a cursor over an **existing** dstring. It never modifies or copies the source, so the source string **must stay alive** for as long as the reader is used. The reader's allocator is used for the reader itself and for any strings it returns.

> `dstr_reader_create` returns **`NULL`** if the string is `NULL`. Every reader function accepts a `NULL` reader safely (`dstr_reader_is_finish` returns `true`), so a `while (!dstr_reader_is_finish(r))` loop handles that case for free.

| Function | What it does |
|---|---|
| `dstr_reader_create(dstr, alloc)` | Creates a reader positioned at the start of `dstr`. |
| `dstr_reader_destroy(reader)` | Frees the reader (not the source string). |
| `dstr_reader_skip_spaces(reader)` | Advances past whitespace, **including newlines**. |
| `dstr_reader_check_and_skip_char(reader, c)` | If the next character is `c`, consumes it and returns `true`. Otherwise returns `false` and does not move. |
| `dstr_reader_check_and_skip_chars(reader, chars, count)` | Same, for a sequence of `count` characters. Returns `false` if fewer than `count` characters remain. |
| `dstr_reader_read_and_skip_chars(reader, count)` | Returns the next `count` characters as a new dstring and advances past them. Returns `NULL` if fewer than `count` characters remain (or `count` is `0`). |
| `dstr_reader_read_and_skip_chars_to_delim(reader, delim)` | Returns the text up to (not including) `delim`, or to the end of the input if there is none. It advances **up to** the delimiter but does **not** consume it, so follow it with `dstr_reader_check_and_skip_char`. Returns `NULL` if the token is empty. |
| `dstr_reader_skip_to_next_line(reader)` | Advances **past the next `'\n'`** (or to the end if there is none). |
| `dstr_reader_find_first_char(reader, c, &len)` | If `c` appears ahead, sets `len` to its distance from the cursor and returns `true`. Returns `false` (leaving `len` untouched) if not found. Does not move the reader. |
| `dstr_reader_is_finish(reader)` | `true` when nothing is left to read. |

Strings returned by the reader are owned by you: free each with `dstr_destroy(s, alloc)` using the allocator you gave `dstr_reader_create` (`NULL` for the default).

#### Example: parsing `key: value` lines

```c
dstr_t text = dstr_create_from_cstr("name: Ada\nlang: C\n", NULL);
dstr_reader_t r = dstr_reader_create(text, NULL);

while (!dstr_reader_is_finish(r))
{
    /* key: everything up to ':' */
    dstr_t key = dstr_reader_read_and_skip_chars_to_delim(r, ':');
    dstr_reader_check_and_skip_char(r, ':');
    dstr_reader_skip_spaces(r);

    /* value: everything up to the end of the line */
    dstr_t value = dstr_reader_read_and_skip_chars_to_delim(r, '\n');
    dstr_reader_check_and_skip_char(r, '\n');

    if (key && value)                          /* empty tokens are NULL */
        printf("%s = %s\n", key, value);       /* name = Ada, then lang = C */

    dstr_destroy(key, NULL);
    dstr_destroy(value, NULL);
}

dstr_reader_destroy(r);
dstr_destroy(text, NULL);
```

#### Example: reading a token of known length

`find_first_char` measures the distance to a character without moving, and `read_and_skip_chars` then reads exactly that many characters:

```c
dstr_t rec = dstr_create_from_cstr("ID42:rest", NULL);
dstr_reader_t r = dstr_reader_create(rec, NULL);

u32 len = 0;
if (dstr_reader_find_first_char(r, ':', &len))            /* len == 4 */
{
    dstr_t id = dstr_reader_read_and_skip_chars(r, len);  /* "ID42" */
    printf("%s\n", id);
    dstr_destroy(id, NULL);
}

dstr_reader_destroy(r);
dstr_destroy(rec, NULL);
```

---

## Custom allocators

An allocator is three function pointers:

```c
typedef struct dstr_memory_allocator {
    void* (*allocate)(u64 size);
    void* (*reallocate)(void* block, u64 new_size);
    void  (*free)(void* block);
} dstr_memory_allocator;
```

Example: a tracking allocator that counts live allocations.

```c
static int live = 0;

static void *my_alloc(u64 size)            { live++; return malloc(size); }
static void *my_realloc(void *p, u64 size) { return realloc(p, size); }
static void  my_free(void *p)              { live--; free(p); }

static const dstr_memory_allocator tracked = { my_alloc, my_realloc, my_free };

dstr_t s = dstr_create_from_cstr("tracked", &tracked);
/* live == 1 */
dstr_destroy(s, &tracked);
/* live == 0 */
```

Rules of thumb:

- **Create and destroy with the same allocator.** A dstring made with `&tracked` must be destroyed with `&tracked`.
- **Builders and readers copy your allocator by value** when created, so they keep working if your allocator struct goes out of scope. The function pointers themselves must of course remain valid.
- `NULL` always means the standard `malloc` / `realloc` / `free`.
- `reallocate` receives only the block and the new size, so custom allocators (arenas, for example) must handle that themselves.

---

## Behavior notes

- **Empty means `NULL`.** No dstring is ever empty. See [Empty strings are `NULL`](#empty-strings-are-null).
- **Builder failure is silent.** If a builder cannot grow its buffer, the append is skipped without any error. The builder itself stays valid and consistent.
- **Builder buffers are not strings.** `dstr_builder_get_chars` is not null-terminated. Use `dstr_builder_generate_dstr` for a real dstring.
- **Bytes, not characters.** Strings are plain `char` bytes. Case conversion is ASCII-only, and lengths count bytes.
- **Not thread-safe.** Do not share a string, builder, or reader between threads without your own locking.

---

## Planned features

- Reconsider whether empty strings should be allowed (currently they are `NULL`).
- Conversions between dstrings and other types (for example to `int` and `float`).
- An option for thread-safe compilation.
- Creating a dstring directly from an external function that fills a buffer (for example `GetCurrentDirectory`), without a temporary copy.
- Lexicographic comparison of two dstrings (returning an `int`).
- Optionally storing a reference to the allocator inside the dstring, so `dstr_destroy` can always use the right one.
- Convenience macros so the allocator argument can be omitted.
- More builder and reader functionality.
- Wide characters and code points.