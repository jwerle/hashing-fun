
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
  uint32_t c = 0;
  size_t size = 0;
  int i = 0;

  while (0 != in[i]) {
    a = in[i];
    b = in[i+1];
    c = mat[i][i];
    out[size++] = (a << 16) | (b << 8);
    i += 2;
  }

  out[size] = 0;

  return size;
}

size_t
decode (const char *key,
        uint32_t len,
        uint32_t mat[][len],
        uint32_t *in,
        uint32_t out[]) {

  uint32_t a = 0;
  uint32_t b = 0;
  uint32_t c = 0;
  uint32_t buf = 0;
  size_t size = 0;
  int i = 0;

  while (0 != in[i]) {
    c = mat[i][i];
    buf = in[i++];
    out[size++] = buf >> 16 & 0xff;
    out[size++] = buf >> 8 & 0xff;
  }

  out[size] = 0;

  return size;
}

int
main (void) {
  const char *key = "apple";
  uint32_t len = 0xf;
  uint32_t mat[len][len];
  uint32_t enc[BUFSIZ];
  uint32_t dec[BUFSIZ];
  uint32_t in[] = {'k', 'i', 'n', 'k', 'a', 'j', 'o', 'u', 'z', 'z', 'z', '!', '?', 0};
  size_t size = 0;

  printf("in=(%ls)\n", (int *) in);

  generate(len, mat);

  do {

    int x = 0;
    int y = 0;

    for (; y < len; ++y) {
      for (x = 0; x < len; ++x) {
        printf("%u\t", mat[y][x]);
      }
      printf("\n");
    }
  } while (0);

  size = encode(key, len, mat, in, enc);

  do {

    int i = 0;
    for (; i < size; ++i) {
      printf("%u ", enc[i]);
    }
    printf("\n");
  } while (0);

  size = decode(key, len, mat, enc, dec);
  printf("dec=(%ls)\n", (int *) dec);

  return 0;
}
