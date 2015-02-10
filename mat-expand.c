
#include <stdio.h>
#include <stdlib.h>

void
generate (uint32_t len, uint32_t mat[][len]) {
  int x = 0;
  int y = 0;
  for (; y < len; ++y) {
    for (x = 0; x < len; ++x) {
      mat[y][x] = (x + y) << 4;
    }
  }
}


size_t
encode (const char *key,
        uint32_t len,
        uint32_t mat[][len],
        uint32_t *in,
        uint32_t out[]) {

  uint32_t a = 0;
  uint32_t b = 0;
  size_t size = 0;
  int i = 0;

  while (0 != in[i]) {
    a = in[i++];
    b = in[i++];
    out[size++] = (a + b) % len;
  }

  return size;
}

size_t
decode (const char *key,
        uint32_t len,
        uint32_t mat[][len],
        uint32_t *in,
        uint32_t out[]) {

  size_t size = 0;
  return size;
}

int
main (void) {
  const char *key = "apple";
  uint32_t len = 0xff;
  uint32_t mat[len][len];
  uint32_t out[BUFSIZ];
  uint32_t in[] = {'k', 'i', 'n', 'k', 'a', 'j', 'o', 'u', 0};
  size_t size = 0;

  generate(len, mat);

  do {
    break;

    int x = 0;
    int y = 0;

    for (; y < len; ++y) {
      for (x = 0; x < len; ++x) {
        printf("%u\t", mat[y][x]);
      }
      printf("\n");
    }
  } while (0);

  size = encode(key, len, mat, in, out);

  {
    int i = 0;
    for (; i < size; ++i) {
      printf("%u ", out[i]);
    }
    printf("\n");
  }

  return 0;
}
