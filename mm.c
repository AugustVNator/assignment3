/**
 * @file   mm.c
 * @Author 02335 team
 * @date   September, 2024
 * @brief  Memory management skeleton.
 * 
 */

#include <stdint.h>

#include "mm.h"



/* Proposed data structure elements */

typedef struct header {
  struct header * next;     // Bit 0 is used to indicate free block 
  uint64_t user_block[0];   // Standard trick: Empty array to make sure start of user block is aligned
} BlockHeader;

/* Macros to handle the free flag at bit 0 of the next pointer of header pointed at by p */
// We type cast to uintptr_t so bitwise operations are possible on n and f
#define GET_NEXT(p)    (void *) ((uintptr_t) p->next & ~0x1 )    /* Mask out free flag */
#define SET_NEXT(p,n)  p->next = (void *) (((uintptr_t) (n) & ~0x1) | ((uintptr_t) p->next & 0x1)) /* Preserve free flag */
#define GET_FREE(p)    (uint8_t) ( (uintptr_t) (p->next) & 0x1 )   /* OK -- do not change */
#define SET_FREE(p, f)  p->next = (void *) (((uintptr_t) p->next & ~0x1) | ((f) & 0x1)) /* Set free bit of p->next to f */
#define SIZE(p)        (size_t) ((uintptr_t) GET_NEXT(p) - ((uintptr_t) p + sizeof(BlockHeader)) )/* Calculate size of block from p and p->next */

#define MIN_SIZE     (8)   // A block should have at least 8 bytes available for the user


static BlockHeader * first = NULL;
static BlockHeader * current = NULL;

/**
 * @name    simple_init
 * @brief   Initialize the block structure within the available memory
 *
 */
void simple_init() {
  // address + (8 - address % 8) should align our addresses to 8 byte alignment
  uintptr_t aligned_memory_start = (memory_start % 8) ? memory_start + (8 - memory_start % 8) : memory_start ;  /* TODO: Alignment */
  uintptr_t aligned_memory_end   = (memory_end % 8) ? memory_end - (8 - memory_end % 8) : memory_end ; /* TODO: Alignment */
  BlockHeader * last;

  /* Already initalized ? */
  if (first == NULL) {
    /* Check that we have room for at least one free block and an end header */
    if (aligned_memory_start + 2*sizeof(BlockHeader) + MIN_SIZE <= aligned_memory_end) {
      /* TODO: Place first and last blocks and set links and free flags properly */
      first = (BlockHeader *) aligned_memory_start;
      last = (BlockHeader *) (aligned_memory_end - sizeof(BlockHeader));
      printf("Setting next and free \n");
      SET_NEXT(first, last);
      printf("Next 2 \n");
      SET_FREE(first, 1);
      SET_NEXT(last, first);
      printf("Free 1 \n");

      printf("Free 2 \n");
      SET_FREE(last, 0);
      printf("Set next and free \n");
    }
    printf("Current = first \n");
    current = first;     
  } 
}


/**
 * @name    simple_malloc
 * @brief   Allocate at least size contiguous bytes of memory and return a pointer to the first byte.
 *
 * This function should behave similar to a normal malloc implementation. 
 *
 * @param size_t size Number of bytes to allocate.
 * @retval Pointer to the start of the allocated memory or NULL if not possible.
 *
 */
void* simple_malloc(size_t size) {
  printf("Is first == null? \n");
  if (first == NULL) {
    printf("init simple \n");
    simple_init();
    if (first == NULL) return NULL;
  }

  size_t aligned_size = (size % 8) ? size + (8 - size % 8) : size;  // Add padding /* TODO: Alignment */

  /* Search for a free block */
  BlockHeader * search_start = current;

  do {
 
    if (GET_FREE(current)) {

      /* Possibly coalesce consecutive free blocks here */

      /* Check if free block is large enough */
      if (SIZE(current) >= aligned_size) {
        void * user_ptr;
        /* Will the remainder be large enough for a new block? */
        if (SIZE(current) - aligned_size < sizeof(BlockHeader) + MIN_SIZE) {
          /* TODO: Use block as is, marking it non-free*/
          printf("Use block as is \n");
          SET_FREE(current, 0);
          user_ptr = (void *) current->user_block;
          current = GET_NEXT(current);

        } else {
          /* TODO Carve aligned_size from block and allocate new free block for the rest */
          printf("Carve aligned_size from block \n");
          BlockHeader * new_free_block = (BlockHeader *) ((uintptr_t) current + sizeof(BlockHeader) + aligned_size);
          BlockHeader * old_next = GET_NEXT(current);

          /* Set up the new free block */
          SET_NEXT(new_free_block, old_next);
          SET_FREE(new_free_block, 1);

          /* Set up the allocated block */
          SET_NEXT(current, new_free_block);
          SET_FREE(current, 0);

          user_ptr = (void *) current->user_block;
          current = new_free_block;

        }
        return user_ptr;
        current = GET_NEXT(current);
        return (void *) current; /* TODO: Return address of current's user_block and advance current */
      }
    }

    current = GET_NEXT(current);
  } while (current != search_start);

 /* None found */
  return NULL;
}


/**
 * @name    simple_free
 * @brief   Frees previously allocated memory and makes it available for subsequent calls to simple_malloc
 *
 * This function should behave similar to a normal free implementation. 
 *
 * @param void *ptr Pointer to the memory to free.
 *
 */
void simple_free(void * ptr) {
  printf("Corresponding block \n");
  BlockHeader * block = (BlockHeader * ) ((uintptr_t) ptr - sizeof(BlockHeader)); /* TODO: Find block corresponding to ptr */
  if (GET_FREE(block)) {
    printf("Block was free, should not happen \n ");
    /* Block is not in use -- probably an error */
    return;
  }
  printf("Setting block free");
  SET_FREE(block, 1);

  /* TODO: Free block */
  BlockHeader * next_block = GET_NEXT(block);

  /* Keep merging with next block while it's free */
  printf("While \n");
  while (GET_FREE(next_block) && next_block != first) {
    /* Merge: skip over next_block by pointing to its next */
    SET_NEXT(block, GET_NEXT(next_block));
    next_block = GET_NEXT(block);
  }
  /* Possibly coalesce consecutive free blocks here */
}


/* Include test routines */

#include "mm_aux.c"
