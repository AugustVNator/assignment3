
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "io.h"

/*TODO - change for own implementation of malloc*/
/* 
 * This implementation cheats by using stdio
 * 
 */

/* Reads next char from stdin. If no more characters, it returns EOF */
int
read_char() {
  char buf[1];
  ssize_t bytes_read = read(STDIN_FILENO, buf, sizeof(buf));
  if (bytes_read > 0) {
    return buf[0];
  }
  return EOF;
}

/* Writes c to stdout.  If no errors occur, it returns 0, otherwise EOF */
int
write_char(char c) {
  ssize_t bytes_write = write(STDOUT_FILENO, &c, sizeof(c));

  if (bytes_write > 0) {
    return 0;
  }
  return EOF;
}

/* Writes a null-terminated string to stdout.  If no errors occur, it returns 0, otherwise EOF */
int
write_string(char *s) {
  ssize_t bytes_write = write(STDOUT_FILENO, s, strlen(s));

  if (bytes_write > 0) {
    return 0;
  }
  return EOF;
}

/* Writes n to stdout (without any formatting).   
 * If no errors occur, it returns 0, otherwise EOF
 */
int
write_int(int n) {
  int r = printf("%d", n);
  return (r > 0 ? 0 : EOF); 
}
