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
#define SET_NEXT(p,n)  p->next = (void *) (((uintptr_t) (n) & ~0x1) | ((uintptr_t) GET_FREE(p))) /* Preserve free flag */
#define GET_FREE(p)    (uint8_t) ( (uintptr_t) (p->next) & 0x1 )   /* OK -- do not change */
#define SET_FREE(p, f)  p->next = (void *) ((uintptr_t) GET_NEXT(p) | ((f) & 0x1)) /* Set free bit of p->next to f */
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
  // address +- (8 - address % 8) should align our addresses to 8 byte alignment
  // This only works, if the address is not already aligned, hence the ternary operator
  uintptr_t aligned_memory_start = (memory_start % 8) ? memory_start + (8 - memory_start % 8) : memory_start ;
  uintptr_t aligned_memory_end   = (memory_end % 8) ? memory_end - (8 - memory_end % 8) : memory_end ;
  BlockHeader * last;

  /* Already initalized ? */
  if (first == NULL) {
    // Check that we have room for at least one free block and an end header
    if (aligned_memory_start + 2*sizeof(BlockHeader) + MIN_SIZE <= aligned_memory_end) {
      first = (BlockHeader *) aligned_memory_start;
      last = (BlockHeader *) (aligned_memory_end - sizeof(BlockHeader));

      SET_NEXT(first, last);
      SET_FREE(first, 1);

      SET_NEXT(last, first);
      SET_FREE(last, 0);

    }

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

  if (first == NULL) {

    simple_init();
    if (first == NULL) return NULL;
  }

  size_t aligned_size = (size % 8) ? size + (8 - size % 8) : size;  // Add padding

  /* Search for a free block */
  BlockHeader * search_start = current;

  do {
 
    if (GET_FREE(current)) {
      // Coalesce consecutive free blocks
      BlockHeader * next_block = GET_NEXT(current);
      while (GET_FREE(next_block) && next_block != first) {
        SET_NEXT(current, GET_NEXT(next_block));
        next_block = GET_NEXT(current);
      }
      /* Check if free block is large enough */
      if (SIZE(current) >= aligned_size) {
        void * block_ptr;
        /* Will the remainder be large enough for a new block? */
        if (SIZE(current) - aligned_size < sizeof(BlockHeader) + MIN_SIZE) {
          //Use block as is

          SET_FREE(current, 0);
          block_ptr = (void *) current->user_block;
          current = GET_NEXT(current);

        } else {
          // Carve aligned size from block
          BlockHeader * new_free_block = (BlockHeader *) ((uintptr_t) current + sizeof(BlockHeader) + aligned_size);
          BlockHeader * old_next = GET_NEXT(current);

          // Set up the new free block
          SET_NEXT(new_free_block, old_next);
          SET_FREE(new_free_block, 1);

          // Set up the allocated block
          SET_NEXT(current, new_free_block);
          SET_FREE(current, 0);

          block_ptr = (void *) current->user_block;
          current = new_free_block;
        }
        return block_ptr;
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
  if (ptr ==NULL) {
    return;
  }

  BlockHeader * block = (BlockHeader * ) ((uintptr_t) ptr - sizeof(BlockHeader));

  if (GET_FREE(block)) {
    /* Block is not in use -- probably an error */
    return;
  }

  SET_FREE(block, 1);

  BlockHeader * next_block = GET_NEXT(block);

  // Set next block until the memory is circular
  while (GET_FREE(next_block) && next_block != first) {

    SET_NEXT(block, GET_NEXT(next_block));
    next_block = GET_NEXT(block);
  }

}


/* Include test routines */

#include "mm_aux.c"
