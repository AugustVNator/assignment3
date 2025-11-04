
#include <stdio.h>
#include <stdint.h>

#include "mm.h"


/** 
 * Test program that makes some simple allocations and enables
 * you to inspect the result.
 *
 * Elaborate on your own.
 */

int main(int argc, char ** argv) {

  /* Ensure that macros are working */
  int ret = simple_macro_test();
  printf(ret == 0 ? "SUCCESS\n" : "FAILURE\n");
  printf("%d\n", ret);
  if (ret > 0) {
    printf("Macro test returned %d\n", ret);
    return 1;
  }

    printf("Start simple test\n");



  void * a = simple_malloc(0x200);

  void * b = simple_malloc(0x100);

  simple_free(a);

  simple_malloc(0x100);

  simple_free(b);

  simple_block_dump(); 

  return 0;
}
