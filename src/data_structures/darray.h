#pragma once

#include "../utils/ddefines.h"

typedef struct darray_memory_allocator
{
    void* (*allocate)(u64);
    void* (*reallocate)(void*, u64);
    void  (*free)(void*); 
} darray_memory_allocator;

typedef enum darray_sort_type_enum
{
    DARRAY_SORT_TYPE_MERGE_SORT,
    DARRAY_SORT_TYPE_QUICK_SORT,
    DARRAY_SORT_TYPE_COUNT
} darray_sort_type_enum;

typedef void* darr_t;

/*
    TODO: Add definitions (macros) for using those functions without providing an allocator for simplicity.
    TODO: Add more utils
    TODO: Add more sort options
    TODO: Add another backend based on pointers not actual arrayes for better preformance for (insert\delete)_at..
    TODO: Add option for compiling with thread safe implementation of those functions.
    TODO: Think if it is a good thing to add macros like:
        #define darr_create(type) darr_create_empty(DARR_DEFAULT_INITIAL_CAPACITY * sizeof(type), sizeof(type), NULL)
        for usage as:
            darr_t int_arr = darr_create(int);
            darr_t char_arr = darr_create(char);
*/

darr_t darr_create_empty(u64 initial_capacity, u64 type_size_in_bytes, darray_memory_allocator* allocator); // O(initial_capacity)
darr_t darr_create_from_carr(const void * carray, u64 length, u64 type_size_in_bytes, darray_memory_allocator* allocator); // O(length)
darr_t darr_create_from_darr(const darr_t darray, darray_memory_allocator* allocator); // O(darrlen(darray))
void   darr_insert_at(darr_t* darray, const void * element, u64 index); // O(darrlen(darray))
void   darr_remove_at(darr_t* darray, u64 index); // O(darrlen(darray))
void   darr_remove(darr_t* darray, const void * element); // O(darrlen(darray)) - compare byte by byte
void   darr_push(darr_t* darray, const void * element); // O(1) amortized
void   darr_pop(darr_t* darray, void* out_element); // O(1) amortized
void   darr_sort(darr_t darray, int (*compare)(const void*, const void*), darray_sort_type_enum sort_type); // qsort - O(darrlen(darray) * log(darralen(darray))) on avg
u64    darrlen(const darr_t darray); // O(1)
u64    darr_capacity(const darr_t darray); // O(1)


#if defined(DARRAY_IMPLEMENTATION)

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DARRAY_DEFAULT_ALLOCATOR (darray_memory_allocator){ .allocate = malloc, .free = free, .reallocate = realloc }
#define _head_from_block(block) (((_darray_header*)(block))-1)
#define _block_from_head(head) (void*)((head) + 1)

typedef struct _darray_header
{
    darray_memory_allocator allocator;
    u64 capacity;
    u64 count;
    u64 type_size_in_bytes;
} _darray_header;

darr_t darr_create_empty(u64 initial_capacity, u64 type_size_in_bytes, darray_memory_allocator* allocator)
{
    darray_memory_allocator _allocator = (allocator == NULL) ? DARRAY_DEFAULT_ALLOCATOR : *allocator;
    
    _darray_header* header = _allocator.allocate(sizeof(_darray_header) + initial_capacity * type_size_in_bytes);
    if (!header) return NULL;

    header->allocator          = _allocator;
    header->capacity           = initial_capacity;
    header->count              = 0;
    header->type_size_in_bytes = type_size_in_bytes;
    
    return _block_from_head(header);
}

darr_t darr_create_from_carr(const void *carray, u64 length, u64 type_size_in_bytes, darray_memory_allocator *allocator)
{
    if (!carray) return NULL;
    darray_memory_allocator _allocator = (allocator == NULL) ? DARRAY_DEFAULT_ALLOCATOR : *allocator;
    
    _darray_header* header = _allocator.allocate(sizeof(_darray_header) + length * type_size_in_bytes);
    if (!header) return NULL;
    
    header->allocator          = _allocator;
    header->capacity           = length;
    header->count              = length;
    header->type_size_in_bytes = type_size_in_bytes;
    
    void* block = _block_from_head(header);
    memcpy(block, carray, length * type_size_in_bytes);
    
    return block;
}

darr_t darr_create_from_darr(const darr_t darray, darray_memory_allocator *allocator)
{
    if (!darray) return NULL;
    darray_memory_allocator _allocator = (allocator == NULL) ? DARRAY_DEFAULT_ALLOCATOR : *allocator;

    _darray_header* old_header = _head_from_block(darray);
    _darray_header* header = _allocator.allocate(sizeof(_darray_header) + old_header->capacity * old_header->type_size_in_bytes);
    if (!header) return NULL;

    memcpy(header, old_header, sizeof(_darray_header) + old_header->capacity * old_header->type_size_in_bytes);

    return _block_from_head(header);
}

void darr_insert_at(darr_t* darray, const void *element, u64 index)
{
    if (!darray || !*darray || !element) return; // Silence error
    
    _darray_header * head = _head_from_block((*darray));
    if (!head) return;

    if (head->count <= index) return; // Silence error
    
    if (head->capacity <= head->count + 1)
    {
        head->capacity *= 2;

        void* reallocation = head->allocator.reallocate(head, sizeof(_darray_header) + head->capacity * head->type_size_in_bytes);
        assert(reallocation != NULL);
        
        head = reallocation;
        *darray = _block_from_head(head);
    }

    void * dest = (u8*)(*darray) + (index + 1) * head->type_size_in_bytes;
    void * src  = (u8*)(*darray) +  index      * head->type_size_in_bytes;
    u64 amount  = (head->count - index)   * head->type_size_in_bytes;

    memmove(dest, src, amount);
    memcpy(src, element, head->type_size_in_bytes);

    head->count++;
}

void darr_remove_at(darr_t* darray, u64 index)
{
    if (!darray || !*darray) return;

    _darray_header* head = _head_from_block((*darray));

    if (index >= head->count) return; // Silence error

    memmove(darray + index, darray + index + 1, (head->count - index - 1) *  head->type_size_in_bytes);

    head->count--;
    // TODO: Add realloc when head->count < head->capacity / 4
    // Dont forget to reasign darray to new reallocation
}

void darr_remove(darr_t* darray, const void *element)
{
    if (!darray || !*darray || !element) return; // Silence error
    
    _darray_header* head = _head_from_block((*darray));
    
    for (u64 i = 0; i < head->count; i+= head->type_size_in_bytes)
    {
        bool found = true;
        for (u64 j = 0; j < head->type_size_in_bytes; j++)
        {
            if (*(((char*)(*darray)) + i + j) != *(((char*)element) + j))
            {
                found = false;
                break;
            }
        }
        
        if (found)
        {
            memmove(darray + i, darray + i + 1, (head->count - i - 1) *  head->type_size_in_bytes);
            head->count--;
            // TODO: Add realloc when head->count < head->capacity / 4
            // Dont forget to reasign darray to new reallocation
            return;
        }
    }
}

void darr_push(darr_t*darray, const void *element)
{
    if (!darray || !*darray || !element) return; // Silence error
    
    _darray_header * head = _head_from_block((*darray));
    
    if (head->capacity <= head->count + 1)
    {
        head->capacity *= 2;

        void* reallocation = head->allocator.reallocate(head, sizeof(_darray_header) + head->capacity * head->type_size_in_bytes);
        assert(reallocation != NULL);
        
        head = reallocation;
        *darray = _block_from_head(head);
    }

    void * dest = (u8*)(*darray) + head->count * head->type_size_in_bytes;

    memcpy(dest, element, head->type_size_in_bytes);

    head->count++;
}

void darr_pop(darr_t*darray, void *out_element)
{
    if (!darray || !*darray || !out_element) return; // Silence error

    _darray_header* head = _head_from_block((*darray));


    memmove(out_element, darray + (--head->count), head->type_size_in_bytes);

    head->count--;
    // TODO: Add realloc when head->count < head->capacity / 4
    // Dont forget to reasign darray to new reallocation :)
}

void darr_sort(darr_t darray, int (*compare)(const void *, const void *), darray_sort_type_enum sort_type)
{
    if (!darray || !compare) return;
    _darray_header* head = _head_from_block((darray));
    qsort(darray, head->count, head->type_size_in_bytes, compare);
    return;
    (void)sort_type;
    // TODO: ontinue... 
    // switch (sort_type)
    // {
    // }
}

inline u64 darrlen(const darr_t darray)
{
    if (darray == NULL) return 0; // Silence error
    return _head_from_block(darray)->count;
}

inline u64 darr_capacity(const darr_t darray)
{
    if (darray == NULL) return 0; // Silence error
    return _head_from_block(darray)->capacity;
}

#endif
