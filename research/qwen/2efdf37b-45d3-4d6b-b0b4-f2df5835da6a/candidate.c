/*
 * Decodes a variable-length unsigned integer from a byte stream.
 * Each byte contributes 7 bits to the result. The most significant
 * bit (bit 7) indicates continuation: 1 means more bytes follow,
 * 0 means this is the last byte.
 *
 * param_1: pointer to current byte in the input stream
 * param_2: pointer to store the decoded unsigned integer
 * Returns: updated pointer to the next byte after the last decoded byte
 */
unsigned char *FUN_00123530(unsigned char *param_1, unsigned int *param_2)
{
  unsigned char b;
  unsigned int result;
  unsigned int shift;

  b = *param_1++;
  result = b & 0x7f;
  shift = 0;

  while ((b & 0x80) != 0) {
    b = *param_1++;
    shift += 7;
    result |= (b & 0x7f) << shift;
  }

  *param_2 = result;
  return param_1;
}
