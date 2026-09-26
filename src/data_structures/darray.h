#pragma once

#include "../utils/ddefines.h"

/*
    NOTE: Usage required you to create a single translation unit with the following:
        #define DARRAY_IMPLEMENTATION
        #include "darray.h"
        You actually need the ddefines in path. You can find it at `src/utils/ddefines.h`


    When using darray as contiguous block you can use:
        int * my_arr = (int*)darr_create(128, sizeof(int), DARR_TYPE_CONTIGUOUS, NULL);
        int x = 41;
        if (!darr_push_back(&my_arr, &x))
        {
            printf("ERROR: failed to reallocate.\n");
            exit(1);
        }
        ...
        if (darrlen(my_arr) > 10)
        {
            my_arr[ 10 ] = 1729;
        }
        ...
        for (int i = 0; i < darrlen(my_arr); i++)
        {
            printf("%d\n", my_arr[ i ]);
        }
    But if you use darray as linked list you can not do this directly, but:
        void* my_arr = darr_create(128, sizeof(int), DARR_TYPE_LINKED_LIST, NULL);
        int x = 41;
        if (!darr_push_back(&my_arr, &x))
        {
            printf("ERROR: failed to reallocate.\n");
            exit(1);
        }
        ...
        if (darrlen(my_arr) > 10)
        {
            int* p = (int*)darr_at(darr, 10);
            *p = 1729;
        }
        ...
        for (int i = 0; i < darrlen(my_arr); i++)
        {
            printf("%d\n", *(int*)darr_at(my_arr, i));
        }
    NOTE: the function darr_at will work in both cases so it might be a good practice to allways use darr_at.
        I did keep the option to cast the darr to int* for better readability in simple cases.

    NOTE: In defines.h we define:
            typedef struct dallocator {
                void * (*allocate) ( u64 );
                void * (*reallocate) ( void *, u64 );
                void   (*free) ( void * );
            } * dallocator_t;

            #include <stdlib.h>
            #define DALLOCATOR_DEFAULT (struct dallocator){ .allocate = malloc, .reallocate = realloc, .free = free }
        So when ever i ask an allocator from you, you can use NULL for default.
    
    ======== This section is for me! ================

    TODO: Add sorting functions and other arrays algorithms
    TODO: Check if your implementation of linked list is actually better then regular pointer based.
    TODO: Remove the ddefines dependency
    TODO: Add compilation option for thread safe impl
    TODO: Add more backends implementations 

*/

typedef enum darr_sort_type : u8
{
    DARR_SORT_TYPE_QUICK_SORT,
    // TODO: Add more
    DARR_SORT_TYPE_COUNT
} darr_sort_type_e;

typedef enum darr_type_e : u8 {
    DARR_TYPE_CONTIGUOUS,
    DARR_TYPE_LINKED_LIST,
    // TODO: Add more
    DARR_TYPE_COUNT
} darr_type_e;

typedef int (*darr_compare)(const void*, const void*);

#if defined(DARRAY_CONVENIENT)

#if !defined(DARRAY_DEFAULT_CAPACITY)
#define DARRAY_DEFAULT_CAPACITY 64UL
#endif

#define expand(x) x
#define choose(args, m2, m1, ...) m1
#define _darr_create_null(type, backend_type) darr_create(DARRAY_DEFAULT_CAPACITY, sizeof(type), backend_type, NULL); 
#define _darr_create_allocator(type, backend_type, allocator) darr_create(DARRAY_DEFAULT_CAPACITY, sizeof(type), backend_type, allocator); 
#define darr_new(type, ...) \
    expand(choose(__VA_ARGS__, _darr_create_allocator, _darr_create_null))(type, __VA_ARGS__)

#endif

void * darr_create(u32 capacity, u32 item_size, darr_type_e type , const dallocator_t allocator);
void * darr_from_darr(const void* darr); // keeps the same backend & allocator
void * darr_from_carr(const void* carr, u32 item_size, u32 length, darr_type_e type, const dallocator_t allocator);
void * darr_from_darr_items(const void* darr, darr_type_e type , const dallocator_t allocator); // This will copy the darr but let you modify the backend type and allocator if allocator is null then the allocator is copied.
bool   darr_push_back(void** darr, void* item);
bool   darr_pop_back(void** darr, void* out_item);
bool   darr_push_at(void ** darr, void* item, u32 index);
bool   darr_pop_at(void ** darr, void* out_item, u32 index);
void   darr_sort(void* darr, darr_sort_type_e sort_type, darr_compare compare);
void * darr_at(void* darr, u32 index);
u32    darrlen(const void* darr);
void   darr_destroy(void* darr);

#define darr_is_empty(darr) (darrlen((darr)) == 0)

#if defined(DARRAY_IMPLEMENTATION)

#include <string.h>
#include <assert.h>
#include <stdlib.h>


typedef struct darr_contiguous_head {
    u32 count;
    u32 capacity;
    u32 item_size;
    struct dallocator allocator;
    darr_type_e type;
    u8 data [ 0 ];
} darr_contiguous_head;

typedef struct darr_linked_list_head {
    u32 count;
    u32 capacity;
    u32 item_size;
    i32 head, tail;
    struct dallocator allocator;
    darr_type_e type;
    u8 data [ 0 ];
} darr_linked_list_head;

typedef struct darr_linked_list_entry {
    i32 next, prev;
    u8 value [ 0 ];
} darr_linked_list_entry;


#define DARR_UNREACHABLE assert(0 && "Unreachable")
#define DARR_OFFSET_OF(s, e) (u64)(&(((s*)0)->e))

#define JUMP_TO_CONTIGUOUS(head, idx) \
    ((head)->data + (head)->item_size * (idx))
#define JUMP_TO_LINKED_LIST(head, idx) \
    ((darr_linked_list_entry*)((head)->data + (sizeof(darr_linked_list_entry) + (head)->item_size) * (idx)))
#define DARR_TO_HEAD_CONTIGUOUS(darr) \
    ((darr_contiguous_head*)(((u8*)(darr)) - DARR_OFFSET_OF(darr_contiguous_head, data)))
#define DARR_TO_HEAD_LINKED_LIST(darr) \
    ((darr_linked_list_head*)(((u8*)(darr)) - DARR_OFFSET_OF(darr_linked_list_head, data)))
#define DARR_TYPE(darr) (*(((darr_type_e*)(darr)) - sizeof(darr_type_e)))


void * darr_create(u32 capacity, u32 item_size, darr_type_e type , const dallocator_t allocator)
{
    struct dallocator A = allocator ? *allocator : DALLOCATOR_DEFAULT;
    void * head = NULL;
    
    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS: 
        {
            head = A.allocate(sizeof(darr_contiguous_head) + item_size * capacity);
            if (!head) return NULL;
            darr_contiguous_head* h = (darr_contiguous_head*)head;
            h->allocator  = A;
            h->capacity   = capacity;
            h->count      = 0;
            h->item_size  = item_size;
            h->type       = DARR_TYPE_CONTIGUOUS;
            return h->data;
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            head = A.allocate(
                sizeof(darr_linked_list_head) + (sizeof(darr_linked_list_entry) + item_size) * capacity
            );
            if (!head) return NULL;
            darr_linked_list_head* h = (darr_linked_list_head*)head;
            h->allocator = A;
            h->capacity  = capacity;
            h->count     = 0;
            h->item_size = item_size;
            h->type      = DARR_TYPE_LINKED_LIST;
            h->head      = 0;
            h->tail      = 0;
            return h->data;
        } break;
        default: DARR_UNREACHABLE; break;
    }
    
    return NULL;
}

void * darr_from_darr(const void *darr)
{
    if (!darr) return NULL;

    darr_type_e type = DARR_TYPE(darr);
    
    
    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            darr_contiguous_head* h = DARR_TO_HEAD_CONTIGUOUS(darr);
            struct dallocator A = h->allocator;

            u64 len = sizeof(darr_contiguous_head) + h->item_size * h->capacity;
            darr_contiguous_head* nh = (darr_contiguous_head*)A.allocate(len);
            if (!nh) return NULL;

            memcpy(nh, h, len);

            return nh->data;
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            darr_linked_list_head* h = DARR_TO_HEAD_LINKED_LIST(darr);
            struct dallocator A = h->allocator;

            u64 len = sizeof(darr_linked_list_head) + 
                (sizeof(darr_linked_list_entry) + h->item_size) * h->capacity;
            darr_contiguous_head* nh = (darr_contiguous_head*)A.allocate(len);
            if (!nh) return NULL;

            memcpy(nh, h, len);
            
            return nh->data;
        } break;
        default: DARR_UNREACHABLE; break;
    }

    return NULL;
}

void * darr_from_carr(const void *carr, u32 item_size, u32 length, darr_type_e type, const dallocator_t allocator)
{
    if (!carr || length == 0) return NULL;
    
    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            struct dallocator A = allocator ? *allocator : DALLOCATOR_DEFAULT;

            u64 len = sizeof(darr_contiguous_head) + item_size * length;
            darr_contiguous_head* h = (darr_contiguous_head*)A.allocate(len);

            if (!h) return NULL;

            h->allocator = A;
            h->capacity  = length;
            h->count     = length;
            h->item_size = item_size;
            h->type      = type;

            memcpy(h->data, carr, item_size * length);

            return h->data;
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            struct dallocator A = allocator ? *allocator : DALLOCATOR_DEFAULT;

            u64 len = sizeof(darr_linked_list_head) + item_size * length;
            darr_linked_list_head* h = (darr_linked_list_head*)A.allocate(len);

            if (!h) return NULL;

            h->allocator = A;
            h->capacity  = length;
            h->count     = length;
            h->item_size = item_size;
            h->type      = type;
            h->head      = 0;
            h->tail      = length - 1;

            
            for (i32 i = 0; i < (i32)length; i++)
            {
                darr_linked_list_entry* e = ((darr_linked_list_entry*)h->data) + i;
                e->prev = i - 1;
                e->next = i + 1 < (i32)length ? i + 1 : -1;
                memcpy(e->value, (u8*)carr + i * item_size, item_size);
            }

            return h->data;
        } break;
        default: DARR_UNREACHABLE; break;
    }

    return NULL;
}

void * darr_from_darr_items(const void *darr, darr_type_e type, const dallocator_t allocator)
{
    if (!darr) return NULL;

    u32 capacity = 0, item_size = 0;

    darr_type_e darr_type = DARR_TYPE(darr);

    switch (darr_type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            darr_contiguous_head* h = DARR_TO_HEAD_CONTIGUOUS(darr);
            capacity  = h->count;
            item_size = h->item_size;

            return darr_from_carr(
                h->data, 
                item_size, 
                capacity, 
                type, 
                allocator ? allocator : &h->allocator
            );
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            darr_linked_list_head* h = DARR_TO_HEAD_LINKED_LIST(darr);
            capacity  = h->capacity;
            item_size = h->item_size;

            void * ndarr = darr_create(
                capacity, 
                item_size, 
                type, 
                allocator ? allocator : &h->allocator
            );
            if (!ndarr) return NULL;

            i32 i = h->head;
            while (i != -1)
            {
                darr_push_back(ndarr, JUMP_TO_LINKED_LIST(h, i));
                i = JUMP_TO_LINKED_LIST(h, i)->next;
            }

            return ndarr; 
        } break;
        default: DARR_UNREACHABLE; break;
    }

    return NULL;
}

bool darr_push_back(void** darr, void* item)
{
    if (!darr || !*darr || !item) return false;

    darr_type_e type = DARR_TYPE(*darr);
    
    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            darr_contiguous_head* head = DARR_TO_HEAD_CONTIGUOUS(*darr);

            if (head->count >= head->capacity)
            {
                u64 new_capacity = head->capacity > 0 ? head->capacity * 2 : 32;
                void * block = head->allocator.reallocate(
                    head, 
                    sizeof(darr_contiguous_head) + head->item_size * new_capacity
                );

                if (!block)
                {
                    return false;
                }

                head = (darr_contiguous_head*)block;
                head->capacity = new_capacity;
                *darr = head->data;
            }

            u8* p = JUMP_TO_CONTIGUOUS(head, head->count);
            memcpy(p, item, head->item_size);
            head->count++;

            return true; 
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            darr_linked_list_head* head = DARR_TO_HEAD_LINKED_LIST(*darr);

            if (head->count >= head->capacity)
            {
                u64 new_capacity = head->capacity > 0 ? head->capacity * 2 : 32;
                void * block = head->allocator.reallocate(
                    head, 
                    sizeof(darr_linked_list_head) + new_capacity * (head->item_size + sizeof(darr_linked_list_entry))
                );

                if (!block)
                {
                    return false;
                }

                head = (darr_linked_list_head*)block;
                head->capacity = new_capacity;
                *darr = head->data;
            }

            if (head->count == 0)
            {
                darr_linked_list_entry* e = (darr_linked_list_entry*)head->data;
                e->next = -1;
                e->prev = -1;
                memcpy(e->value, item, head->item_size);
                head->count++;
                head->head = 0;
                head->tail = 0;
                return true;
            }

            darr_linked_list_entry* e = JUMP_TO_LINKED_LIST(head, head->tail);
            darr_linked_list_entry* ne = JUMP_TO_LINKED_LIST(head, head->count);
            
            e->next = head->count;
            
            ne->next = -1;
            ne->prev = head->tail;

            memcpy(ne->value, item, head->item_size);

            head->tail = head->count;
            head->count++;

            return true;
        } break;
        default: DARR_UNREACHABLE; break;
    }

    return false;
}

bool darr_pop_back(void **darr, void *out_item)
{
    if (!darr || !*darr) return false;

    darr_type_e type = DARR_TYPE(*darr);
    
    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            darr_contiguous_head* head = DARR_TO_HEAD_CONTIGUOUS(*darr);

            // TODO: if (head->count < head->capacity / 4) realloc ...            

            if (head->count == 0) return false;

            u8* p = JUMP_TO_CONTIGUOUS(head, head->count - 1);
            if (out_item) memcpy(out_item, p, head->item_size);
            
            head->count--;
            return true; 
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            darr_linked_list_head* head = DARR_TO_HEAD_LINKED_LIST(*darr);

            // TODO: if (head->count < head->capacity / 4) realloc ...

            if (head->count == 0) return false;

            darr_linked_list_entry* e = JUMP_TO_LINKED_LIST(head, head->tail);

            if (head->count > 1)
            {
                darr_linked_list_entry* ep = JUMP_TO_LINKED_LIST(head, e->prev);
                ep->next = -1;
                head->tail = e->prev;
            }
            else
            {
                if (head->count == 0) { head->head = head->tail = -1; }
                else head->tail = head->head;
            }
            
            if (out_item) memcpy(out_item, e->value, head->item_size);
            
            head->count--;

            return true;
        } break;
        default: DARR_UNREACHABLE; break;
    }

    return false;
}

bool darr_push_at(void **darr, void *item, u32 index)
{
    if (!darr || !*darr || !item || index > darrlen(*darr)) return false;
    else if (index == darrlen(*darr)) return darr_push_back(darr, item);

    darr_type_e type = DARR_TYPE(*darr);

    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            darr_contiguous_head* h = DARR_TO_HEAD_CONTIGUOUS(*darr);
            
            if (h->count >= h->capacity)
            {
                u64 new_capacity = h->capacity > 0 ? h->capacity * 2 : 32;
                void * block = h->allocator.reallocate(
                    h, 
                    sizeof(darr_contiguous_head) + h->item_size * new_capacity
                );

                if (!block)
                {
                    return false;
                }

                h = (darr_contiguous_head*)block;
                h->capacity = new_capacity;
                *darr = h->data;
            }

            memmove(
                h->data + (index + 1) * h->item_size, 
                h->data + index * h->item_size, 
                (h->count - index) * h->item_size
            );
            memcpy(h->data + index, item, h->item_size);
            h->count++;
            return true;
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            darr_linked_list_head* h = DARR_TO_HEAD_LINKED_LIST(*darr);

            if (h->count >= h->capacity)
            {
                u64 new_capacity = h->capacity > 0 ? h->capacity * 2 : 32;
                void * block = h->allocator.reallocate(
                    h, 
                    sizeof(darr_linked_list_head) + (h->item_size + sizeof(darr_linked_list_entry)) * new_capacity
                );

                if (!block)
                {
                    return false;
                }

                h = (darr_linked_list_head*)block;
                h->capacity = new_capacity;
                *darr = h->data;
            }

            darr_linked_list_entry* ne = JUMP_TO_LINKED_LIST(h, h->count);
            darr_linked_list_entry* pe = JUMP_TO_LINKED_LIST(h, h->head);
            darr_linked_list_entry* ppe = NULL;
            
            for (i32 i = 0; i < (i32)index; i++)
            {
                ppe = pe;
                pe = JUMP_TO_LINKED_LIST(h, pe->next);
            }

            if (!ppe) // index must be 0 
            {
                ne->prev = -1;
                ne->next = h->head;
                pe->prev = h->count;
                h->head  = h->count;
                h->count++;
                memcpy(ne->value, item, h->item_size);
                return true;
            }
            
            ne->next  = ppe->next;
            ne->prev  = pe->prev;
            ppe->next = h->count;
            pe->prev  = h->count;
            h->count++;
            memcpy(ne->value, item, h->item_size);
            return true;
        } break;
        default: DARR_UNREACHABLE; break;
    }

    return false;
}

bool darr_pop_at(void **darr, void *out_item, u32 index)
{
    if (!darr || !*darr || index >= darrlen(*darr)) return false;
    else if(index + 1 == darrlen(*darr)) return darr_pop_back(darr, out_item);

    darr_type_e type = DARR_TYPE(*darr);

    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            darr_contiguous_head* h = DARR_TO_HEAD_CONTIGUOUS(*darr);
            
            // TODO: if (h->count < h->capacity / 4) reallocate ...

            if (out_item) memcpy(out_item, h->data + index * h->item_size, h->item_size);
            
            memmove(
                h->data + index * h->item_size, 
                h->data + (index + 1) * h->item_size, 
                (h->count - index) * h->item_size
            );
            h->count--;
            return true;
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            darr_linked_list_head* h = DARR_TO_HEAD_LINKED_LIST(*darr);

            // TODO: if (h->count < h->capacity / 4) reallocate ...

            darr_linked_list_entry* pe = JUMP_TO_LINKED_LIST(h, h->head);
            darr_linked_list_entry* ppe = NULL;
            
            for (i32 i = 0; i < (i32)index; i++)
            {
                ppe = pe;
                pe = JUMP_TO_LINKED_LIST(h, pe->next);
            }

            if (!ppe) // index must be 0
            {
                i32 idx = h->head;
                h->head = pe->next;

                if (pe->next != -1) JUMP_TO_LINKED_LIST(h, pe->next)->prev = pe->prev;

                h->count--;

                if (h->count == 0) { h->tail = h->head = -1; }
                else if (h->count == 1) { h->tail = h->head; }
                else
                {
                    darr_linked_list_entry* tail = JUMP_TO_LINKED_LIST(h, h->tail);
                    if (tail->prev > 0 && tail->prev < (i32)h->count)
                    {
                        darr_linked_list_entry* tn = JUMP_TO_LINKED_LIST(h, tail->prev);
                        tn->next = idx;
                        memmove(JUMP_TO_LINKED_LIST(h, idx), tail, sizeof(darr_linked_list_entry) + h->item_size);
                    }
                }

                if (out_item) memcpy(out_item, pe->value, h->item_size);

                return true;
            }
            
            i32 idx = ppe->next;
            ppe->next = pe->next;
            JUMP_TO_LINKED_LIST(h, pe->next)->prev = pe->prev;
            h->count--;

            if (h->count == 1) { h->tail = h->head; }
            else
            {
                darr_linked_list_entry* tail = JUMP_TO_LINKED_LIST(h, h->tail);
                if (tail->prev > 0 && tail->prev < (i32)h->count)
                {
                    darr_linked_list_entry* tn = JUMP_TO_LINKED_LIST(h, tail->prev);
                    tn->next = idx;
                    memmove(JUMP_TO_LINKED_LIST(h, idx), tail, sizeof(darr_linked_list_entry) + h->item_size);
                }
            }

            if (out_item) memcpy(out_item, pe->value, h->item_size);

            return true;
        } break;
        default: DARR_UNREACHABLE; break;
    }

    return false;
}

void darr_sort(void *darr, darr_sort_type_e sort_type, darr_compare compare)
{
    switch (sort_type)
    {
        case DARR_SORT_TYPE_QUICK_SORT:
        {
            darr_type_e type = DARR_TYPE(darr);

            switch (type)
            {
                case DARR_TYPE_CONTIGUOUS:
                {
                    u32 item_size = DARR_TO_HEAD_CONTIGUOUS(darr)->item_size;
                    qsort(darr, darrlen(darr), item_size, compare);
                } break;
                case DARR_TYPE_LINKED_LIST:
                {
                    // TODO: ....
                    DARR_UNREACHABLE; // Not implemented yet.
                } break;
                default: DARR_UNREACHABLE; break;
            }
        } break;
        default: DARR_UNREACHABLE; break;
    }
}

inline void *darr_at(void *darr, u32 index)
{
    if (!darr || darrlen(darr) <= index) return NULL;

    darr_type_e type = DARR_TYPE(darr);

    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            darr_contiguous_head* head = DARR_TO_HEAD_CONTIGUOUS(darr);
            void* p = JUMP_TO_CONTIGUOUS(head, index);
            return p;
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            darr_linked_list_head* head = DARR_TO_HEAD_LINKED_LIST(darr);
            darr_linked_list_entry* e = JUMP_TO_LINKED_LIST(head, head->head);

            for (i32 i = 0; i < (i32)index; i++)
            {
                e = JUMP_TO_LINKED_LIST(head, e->next);
            }

            return e->value;
        } break;
        default: DARR_UNREACHABLE; break;
    }
    
    return NULL;
}

inline u32 darrlen(const void *darr)
{
    if (!darr) return 0;
    
    darr_type_e type = DARR_TYPE(darr);

    switch (type)
    {
        case DARR_TYPE_CONTIGUOUS:
        {
            darr_contiguous_head* head = DARR_TO_HEAD_CONTIGUOUS(darr);
            return head->count;
        } break;
        case DARR_TYPE_LINKED_LIST:
        {
            darr_linked_list_head* head = DARR_TO_HEAD_LINKED_LIST(darr);
            return head->count;
        } break;
        default: DARR_UNREACHABLE; break;
    }

    return 0;
}

inline void darr_destroy(void *darr)
{
    if (darr)
    {
        darr_type_e type = DARR_TYPE(darr);

        switch (type)
        {
            case DARR_TYPE_CONTIGUOUS:
            {
                darr_contiguous_head* h = DARR_TO_HEAD_CONTIGUOUS(darr);
                h->allocator.free(h);
            } break;
            case DARR_TYPE_LINKED_LIST:
            {
                darr_linked_list_head* h = DARR_TO_HEAD_LINKED_LIST(darr);
                h->allocator.free(h);
            } break;
            default: DARR_UNREACHABLE; break;
        }
    }
}

#endif
