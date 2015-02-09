
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

float m_ = 0.0625;

// [0-255]
uint8_t seed_ = 0x00;
uint8_t key_ = 0;

void
init (uint8_t seed, uint8_t key) {
  seed_ = seed & 0xff;
  key_ = key & 0xff;
}

size_t
encode (const uint8_t *in, uint8_t *out, size_t len) {
  size_t size = 0;
  uint8_t next = seed_;
  uint8_t ch = 0;
  uint8_t k = 1;
  int r = 0;
  int i = 0;
  int j = 0;

  while (i < len) {
    out[size] = 0;

    ch = in[i++];
    k = ch + next;

    if (ch <= 0x0ff) {
      r = 1;
    } else if ((unsigned int) ch <= 0xfff) {
      r = 2;
    } else if ((unsigned int) ch <= 0xffff) {
      r = 3;
    }

    out[size++] = k;

    for (j = 0; j < r; ++j) {
      out[size++] = key_;
    }

    next = (next + 4) & 0xff;
  }

  return size;
}

size_t
decode (const uint8_t *in, uint8_t *out, size_t len) {
  size_t size = 0;
  uint8_t next = seed_;
  uint8_t ch = 0;
  uint8_t k = 0;
  int r = 0;
  int i = 0;
  int j = 0;

  while (i < len) {
    out[size] = 0;
    ch = in[i++];
    k = (ch - next);

    if (k <= 0x0ff) {
      r = 1;
    } else if ((unsigned int) k <= 0xfff) {
      r = 2;
    } else if ((unsigned int) k <= 0xffff) {
      r = 3;
    }

    out[size++] = k;

    for (j = 0; j < r; ++j) { i += 1; }
    next = (next + 4) & 0xff;
  }

  return size;
}

int
main (void) {
  size_t size = 0;
  uint8_t in[] = "kinkajouz!@@@@©☃eeee\x80";
  uint8_t enc[BUFSIZ];
  uint8_t dec[BUFSIZ];
  uint8_t key = 0xff;

  init(0xff, key);

  size = encode(in, enc, 24);
  printf("size=(%zu)\nenc=(%s)\n", size, enc);
  size = decode(enc, dec, size);
  printf("size=(%zu)\ndec=(%s)\n", size, dec);
  assert(0 == strcmp((char *) in, (char *) dec));
  return 0;
}
