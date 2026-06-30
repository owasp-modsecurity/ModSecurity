#ifndef DBG_PRINT_BYTES_H
#define DBG_PRINT_BYTES_H

#include <stddef.h>

/**
 * @brief Format an arbitrary byte sequence for debug output into a fixed-size buffer
 *
 * Bytes in the safe printable range (0x20-0x7E except backslash 0x5C) are output
 * as-is. Backslash is escaped to \\ to avoid ambiguity. All other bytes are
 * escaped in \xNN format (lowercase hexadecimal, zero-padded).
 * If the escaped output does not fit in the buffer, the middle part is omitted
 * and replaced with the omission marker, keeping the head and tail portions.
 *
 * @param dest       Output buffer. If NULL, returns 0 without doing anything.
 * @param dest_size  Size of the output buffer in bytes. If 0, returns 0.
 * @param src        Input byte sequence. If NULL or src_len is 0, treated as empty.
 * @param src_len    Length of the input byte sequence in bytes.
 * @param omit       Omission marker string inserted between head and tail when
 *                   truncation occurs. If NULL, defaults to "...".
 * @return The total length of the fully escaped string (excluding null terminator).
 *         If the return value is >= dest_size, truncation (omission) occurred.
 */
size_t dbg_print_bytes(char *dest, size_t dest_size,
                       const unsigned char *src, size_t src_len,
                       const char *omit);

#endif /* DBG_PRINT_BYTES_H */
