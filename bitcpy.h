#ifndef BITCPY_H_INCLUDED
#define BITCPY_H_INCLUDED
void bitcpy(void *_dest,      /* Address of the buffer to write to */
            size_t _write,    /* Bit offset to start writing to */
            const void *_src, /* Address of the buffer to read from */
            size_t _read,     /* Bit offset to start reading from */
            size_t count)
{
    size_t read_lhs = _read & 7;
    size_t read_rhs = 8 - read_lhs;
    const uint8_t *source = (const uint8_t *) _src + (_read / 8);
    size_t write_lhs = _write & 7;
    size_t write_rhs = 8 - write_lhs;
    uint8_t *dest = (uint8_t *) _dest + (_write / 8);

    static const uint8_t mask_rw[] = {
        0x00, /*    == 0    00000000b   */
        0x80, /*    == 1    10000000b   */
        0x40, /*    == 2    01000000b   */
        0x20, /*    == 3    00100000b   */
        0x10, /*    == 4    00010000b   */
        0x08, /*    == 5    00001000b   */
        0x04, /*    == 6    00000100b   */
        0x02, /*    == 7    00000010b   */
        0x01  /*    == 8    00000001b   */
    };

    while (count > 0) {
        uint8_t data = *source++;
        size_t bitsize = (count > 8) ? 8 : count;
         if (read_lhs > 0) {
            data <<= read_lhs;
            if (bitsize > read_rhs)
                data |= (*source >> read_rhs);
        }

        if (bitsize < 8)
            data &= mask_rw[bitsize];

        uint8_t original = *dest;
        uint8_t mask = mask_rw[write_lhs];
        if (bitsize > write_rhs) {
            /* Cross multiple bytes */
            *dest++ = (original & mask) | (data >> write_lhs);
            original = *dest & mask_rw[bitsize - write_rhs];
            *dest = original | (data << write_rhs);
        } else {
            // Since write_lhs + bitsize is never >= 8, no out-of-bound access.
            mask |= mask_rw[write_lhs + bitsize];
            *dest++ = (original & mask) | (data >> write_lhs);
        }

        count -= bitsize;
    }
}
#endif // BITCPY_H_INCLUDED
