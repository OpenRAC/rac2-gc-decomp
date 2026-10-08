/* Decodes a variable-length 32-bit signed integer from a byte stream.
 * Reads bytes from 'in' until a byte with bit 7 clear is found.
 * Each byte contributes 7 bits (bits 0-6), little-endian order.
 * If the last byte's bit 6 is set and final bit position < 32, sign-extend.
 * Stores decoded value in *out and returns pointer to next byte.
 */
unsigned char *FUN_00123578(unsigned char *in, unsigned int *out)
{
    unsigned int result = 0;
    unsigned int shift = 0;
    unsigned char byte;

    do {
        byte = *in++;
        result |= (unsigned int)(byte & 0x7F) << (shift & 0x1F);
        shift += 7;
    } while ((byte & 0x80) != 0);

    /* Sign extension: if sign bit (0x40) set in last byte and not yet 32 bits */
    if ((shift < 0x20U) && (byte & 0x40U)) {
        result |= (unsigned int)(-1) << (shift & 0x1FU);
    }

    *out = result;
    return in;
}