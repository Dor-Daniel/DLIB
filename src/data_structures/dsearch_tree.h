#pragma once

#include "../utils/ddefines.h"

/*
    TODO: implement Red-Black Tree backend + array indices insted of pointers for better caching.
*/

typedef struct dsearch_tree* dsearch_tree_t;
// k1 < k2 -> negative, k1 == k2 -> 0, k1 > k2 -> positive
typedef int (*compare_function_ptr)(void* k1, void* k2); 

typedef struct dsearch_tree_mem_allocator {
    void* (*allocate)(u64);
    void* (*reallocate)(void*,u64);
    void (*free)(void*);
} dsearch_tree_mem_allocator;

dsearch_tree_t dsearch_tree_create   (compare_function_ptr compare_func, u32 key_size, u32 value_size, dsearch_tree_mem_allocator* allocator);
bool           dsearch_tree_insert   (dsearch_tree_t tree, void* key, void* value);
bool           dsearch_tree_delete   (dsearch_tree_t tree, void* key);
bool           dsearch_tree_get      (dsearch_tree_t tree, void* key, void* out_value);
bool           dsearch_tree_is_exist (dsearch_tree_t tree, void* key);
void           dsearch_tree_clear    (dsearch_tree_t tree);
void           dsearch_tree_destroy  (dsearch_tree_t tree);

#if defined(DSEARCH_TREE_IMPLEMENTATION)

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DSEARCH_TREE_DEF_ALLOC (dsearch_tree_mem_allocator){ .allocate = malloc, .reallocate = realloc, .free = free }

typedef struct dsearch_tree_node
{
    struct dsearch_tree_node* left, *right;
    void* key;
    void* value;
} dsearch_tree_node;

typedef struct dsearch_tree
{ 
    u32 count, key_size, value_size;
    dsearch_tree_mem_allocator allocator;
    compare_function_ptr compare_func;
    struct dsearch_tree_node* root;
} * dsearch_tree_t;

dsearch_tree_t dsearch_tree_create(compare_function_ptr compare_func, u32 key_size, u32 value_size, dsearch_tree_mem_allocator* _allocator)
{
    dsearch_tree_mem_allocator allocator = _allocator == NULL ? DSEARCH_TREE_DEF_ALLOC : *_allocator;
    dsearch_tree_t tree = (dsearch_tree_t)allocator.allocate(sizeof(struct dsearch_tree));
    assert(tree);

    tree->allocator  = allocator;
    tree->count      = 0;
    tree->key_size   = key_size;
    tree->value_size = value_size;
    tree->compare_func = compare_func;
    tree->root = NULL;

    return tree;
}

bool dsearch_tree_insert(dsearch_tree_t tree, void *key, void *value)
{
    assert(tree && key && value);

    dsearch_tree_node* n = tree->root;
    
    if (!n)
    {
        tree->root = tree->allocator.allocate(sizeof(dsearch_tree_node));
        tree->root->key = tree->allocator.allocate(tree->key_size);
        tree->root->value = tree->allocator.allocate(tree->value_size);
        assert(tree->root && tree->root->key && tree->root->value);
        memcpy(tree->root->key, key, tree->key_size);
        memcpy(tree->root->value, value, tree->value_size);
        tree->root->left = NULL;
        tree->root->right = NULL;
        tree->count++;
        return true;
    }

    int compare = 0;
    while (n)
    {
        compare = tree->compare_func(n->key, key);
        if (compare == 0)
        {
            return false;
        }
        else if (compare < 0)
        {
            if (n->left) n = n->left;
            else break;
        }
        else
        {
            if (n->right) n = n->right;
            else break;
        }
    }

    dsearch_tree_node* nn = tree->allocator.allocate(sizeof(dsearch_tree_node));
    nn = tree->allocator.allocate(sizeof(dsearch_tree_node));
    nn->key = tree->allocator.allocate(tree->key_size);
    nn->value = tree->allocator.allocate(tree->value_size);
    assert(nn && nn->key && nn->value);
    memcpy(nn->key, key, tree->key_size);
    memcpy(nn->value, value, tree->value_size);
    nn->left = NULL;
    nn->right = NULL;

    if (compare < 0) n->left = nn;
    if (compare > 0) n->right = nn;

    tree->count++;

    return true;
}

bool dsearch_tree_delete(dsearch_tree_t tree, void *key)
{
    assert(tree && key);
    
    // 8888888888888888888888888
    //    TODO: Implement!
    // 8888888888888888888888888
    
    (void)tree; (void)key;

    assert(0 && "NOT IMPLEMENTED");
}

bool dsearch_tree_get(dsearch_tree_t tree, void *key, void *out_value)
{
    assert(tree && key && out_value);

    dsearch_tree_node* n = tree->root;
    
    int compare = 1;
    while (n)
    {
        compare = tree->compare_func(n->key, key);
        if (compare == 0) break;
        if (compare < 0) n = n->left;
        if (compare > 0) n = n->left;
    }

    if (compare == 0)
    {
        memcpy(out_value, n->value, tree->value_size);
        return true;
    }
    
    return false;
}

bool dsearch_tree_is_exist(dsearch_tree_t tree, void *key)
{
    assert(tree && key);

    dsearch_tree_node* n = tree->root;
    
    int compare = 1;
    while (n)
    {
        compare = tree->compare_func(n->key, key);
        if (compare == 0) break;
        if (compare < 0) n = n->left;
        if (compare > 0) n = n->left;
    }

    return compare == 0;
}

static void _destroy_node_and_subtrees_(dsearch_tree_node* n, dsearch_tree_mem_allocator allocator)
{
    if (n)
    {
        if (n->left) _destroy_node_and_subtrees_(n->left, allocator);
        if (n->right) _destroy_node_and_subtrees_(n->right, allocator);
        allocator.free(n);
    }
}

void dsearch_tree_clear(dsearch_tree_t tree)
{
    assert(tree);

    dsearch_tree_node* n = tree->root;

    _destroy_node_and_subtrees_(n, tree->allocator);

    tree->count = 0;
    tree->root = NULL;
}

void dsearch_tree_destroy(dsearch_tree_t tree)
{
    dsearch_tree_clear(tree);
    tree->allocator.free(tree);
}

#endif