// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#define IS_POWER_OF_TWO(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))
#define MIN_BLOCK 2560
#include <BuddyMemoryManager.h>
#include <stdint.h>

typedef struct Node {
    void * start;
    uint64_t size;
    uint64_t state;
    uint64_t index;
    struct Node *left;  
    struct Node *right; 
} Node;

enum { INVALID_ADDRESS=-1, FREE, ALLOCATED, PARCIAL, START_PAGE_BOUND, END_PAGE_BOUND, SINGLE_PAGE };

Node * root;
uint64_t mem_allocated = 0;

void* malloc_rec(Node* node, uint64_t bytes);
void create_children(Node * node);
void * free_rec(Node * node, void * ptr);

void init_mm(void * base_address, uint64_t mem_amount){
    root = (Node *) base_address;
    root->start =  (void*)mem_amount;
    root->size = mem_amount;
    root->state = FREE;
    root->index = 0;
    root->left= NULL;
    root->right= NULL; 
}

void check_state(Node* node){
    if (node->left == NULL || node->right == NULL)
    {   
        node->state = FREE; 
        return;
    }
    
    if (node->right->state == FREE && node->left->state == FREE)
        {
            node->state = FREE;
        }

    else if (node->left->state == ALLOCATED && node->right->state == ALLOCATED)
        {
            node->state = ALLOCATED;
        }
    else
        {
            node->state = PARCIAL;
        }
}

void * mallocBuddy(uint64_t bytes){
  if (root->size < bytes)
    return NULL;
  if (!IS_POWER_OF_TWO(bytes)){
    int i = 1;
    while (i < bytes){
      i = i*2;
    }
    bytes = i;
  }
    void * rta = malloc_rec(root, bytes);  
    return rta;
}

void * malloc_rec(Node* node, uint64_t bytes){    
    if (node->state == ALLOCATED){
        return NULL;
    }
   if(node->left != NULL || node->right != NULL){
    void * new_node = malloc_rec(node->left, bytes);
    if (new_node == NULL){
        new_node = malloc_rec(node->right, bytes);
    }
    check_state(node);
    return new_node;
   }
   else{
    if (bytes > node->size)
    {
        return NULL;
    }
    
    if((node->size)/2 >= bytes && node->size > MIN_BLOCK){
        create_children(node);
        void * new_node = malloc_rec(node->left, bytes);
        check_state(node);
        return new_node;
    }
    node->state = ALLOCATED;
    mem_allocated += bytes;
    return node->start;
   }
}

void create_children(Node * node){
    uint64_t parent_index = node->index;
    uint64_t left_index = parent_index*2 + 1;
    uint64_t right_index = left_index + 1;
    uint64_t new_size = (node->size)/2;

    node->left = node + left_index;
    node->left->start = node->start;
    node->left->size = new_size;
    node->left->state = FREE;
    node->left->index = left_index;
    node->left->left = NULL;
    node->left->right = NULL;

    node->right = node + right_index;
    node->right->start = (void*)((char*) (node->start) + new_size);
    node->right->size = new_size;
    node->right->state = FREE;
    node->right->index = right_index;
    node->right->left = NULL;
    node->right->right = NULL;
}


void freeBuddy(void * mem){
    free_rec(root, mem);
}

void * free_rec(Node * node, void * ptr){
    if(node == NULL){
        return NULL;
    }
    if (node->left != NULL || node->right != NULL){
        if ((uint64_t)node->right->start > (uint64_t) ptr){
            free_rec(node->left, ptr);
        }
        else{
            free_rec(node->right, ptr);
        }
        check_state(node);
        if (node->state == FREE)
        {
            node->left = NULL;
            node->right = NULL;
        }
    }
    else if (node->state == ALLOCATED){
        if (node->start == ptr)
        {
            node->state = FREE;
            mem_allocated -= node->size;
        }
    }
    return NULL;
}

void buddyState(uint64_t *free,uint64_t *total,uint64_t *alloctd){
    *total=root->size;
    *alloctd=mem_allocated;
    *free=root->size-mem_allocated;
}