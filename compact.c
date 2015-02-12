
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * codec (BE)
 *
 *      0      1 2 3 4   size
 *  +--------+--------+------------+
 *  | chunks |  size  | stream ... |
 *  +--------+--------+------------+
 */

void
write_u32_be (uint32_t in, uint32_t out[]) {
  out[0] = in >> 24 & 0xff;
  out[1] = in >> 16 & 0xff;
  out[2] = in >> 8 & 0xff;
  out[3] = in & 0xff;
}

uint32_t
read_u32_be (uint32_t in[]) {
  return (uint32_t) (
    in[0] << 24 | in[1] << 16 | in[2] << 8 | in[3]
  );
}

size_t
encode (uint32_t in[], uint32_t out[]) {
  uint32_t tmp[BUFSIZ];
  uint32_t a = 0;
  uint32_t b = 0;
  uint32_t c = 0;
  uint32_t d = 0;
  size_t chunks = 0;
  size_t size = 0;
  int i = 0;

  while (0 != in[i]) {
    for (; i < (i + 16); i += 4) {
      a = in[i];
      b = in[i + 1];
      c = in[i + 2];
      d = in[i + 3];

      if (0 == a) { break; }

      tmp[size++] = (a << 24) | (b << 16) | (c << 8) | (d);
    }

    chunks += 1;
  }

  memset(out, 0, size + 4);

  out[0] = chunks;
  out[1] = size >> 24 & 0xff;
  out[2] = size >> 16 & 0xff;
  out[3] = size >> 8 & 0xff;
  out[4] = size & 0xff;

  for (i = 0; i < size; ++i) {
    out[i + 5] = tmp[i];
  }

  return size;
}

size_t
decode (uint32_t in[], uint32_t out[]) {
  uint32_t a = 0;
  uint32_t b = 0;
  uint32_t c = 0;
  uint32_t d = 0;
  size_t size = 0;
  size_t nread = 0;
  int chunks = 0;
  int i = 0;

  chunks = in[0] & 0xff;
  in++;
  size = (size_t) read_u32_be(in);
  in += 4;

  while (i < size) {
    a = in[i] >> 24 & 0xff;
    b = in[i] >> 16 & 0xff;
    c = in[i] >> 8 & 0xff;
    d = in[i] & 0xff;

    if (0 == a) { break; }
    out[nread++] = a;

    if (0 == b) { break; }
    out[nread++] = b;

    if (0 == c) { break; }
    out[nread++] = c;

    if (0 == d) { break; }
    out[nread++] = d;

    i += 1;
  }

  return nread;
}


int
main (void) {
  size_t size = 0;
  uint32_t enc[BUFSIZ];
  uint32_t dec[BUFSIZ];
  char buf[BUFSIZ];
  uint32_t in[] = {
    0xa, 0x04, 0x08, 0x16,
    0xb, 0x08, 0x16, 0x24,
    0xc, 0x16, 0x24, 0x32,
    0xd, 0x24, 0x32, 0x64,
    0xa, 0x04, 0x08, 0x16,
    0xb, 0x08, 0x16, 0x24,
    0xc, 0x16, 0x24, 0x32,
    0xd, 0x24, 0x32, 0x64,
    0xa, 0x04, 0x08, 0x16,
    0xb, 0x08, 0x16, 0x24,
    0xc, 0x16, 0x24, 0x32,
    0xd, 0x24, 0x32, 0x64,
    0xa, 0x04, 0x08, 0x16,
    0xb, 0x08, 0x16, 0x24,
    0xc, 0x16, 0x24, 0x32,
    0xd, 0x24, 0x32, 0x64,
    0xa, 0x04, 0x08, 0x16,
    0xb, 0x08, 0x16, 0x24,
    0xc, 0x16, 0x24, 0x32,
    0xd, 0x24, 0x32, 0x64,
    0xa, 0x04, 0x08, 0x16,
    0xb, 0x08, 0x16, 0x24,
    0xc, 0x16, 0x24, 0x32,
    0xd, 0x24, 0x32, 0x64,
    0
  };

  /*uint32_t in[] = {
    'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h',

    'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h',

    'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h',

    'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h',

    'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h',

    'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h',

    0
  };*/

  size = encode(in, enc);
  printf("encode size=(%zu)\n", size);
  size = decode(enc, dec);
  printf("decode size=(%zu)\n", size);

  {
    int i = 0;
    for (i = 0; i < size; ++i) {
      buf[i] = dec[i];
    }
    buf[i] = 0;
  }

  assert(0 == memcmp(dec, in, size));

  return 0;
}

