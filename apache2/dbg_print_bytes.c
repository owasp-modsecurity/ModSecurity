#include "dbg_print_bytes.h"
#include <stddef.h>
#include <string.h>

/* ---- internal helpers ---- */

/**
 * Return non-zero if byte c needs escaping in the output.
 * Backslash (0x5C) is escaped as \\ to avoid ambiguity
 * with escape sequences. Other non-printable/control/high bytes
 * are escaped as \xNN.
 */
static int needs_escape(unsigned char c) {
    return !(0x20 <= c && c <= 0x7E) || c == '\\';
}

/**
 * Escaped output length for byte c:
 *   1  when kept as-is (safe printable)
 *   2  for backslash (escaped as \\)
 *   4  otherwise (escaped as \xNN)
 */
static size_t esc_len(unsigned char c) {
    if (c == '\\') return 2;
    return needs_escape(c) ? 4 : 1;
}

/**
 * Write byte c into dest[pos..] with proper escaping.
 * Returns the number of bytes written.
 */
static size_t write_byte(char *dest, size_t pos, unsigned char c) {
    if (!needs_escape(c)) {
        dest[pos] = (char)c;
        return 1;
    }
    if (c == '\\') {
        dest[pos + 0] = '\\';
        dest[pos + 1] = '\\';
        return 2;
    }
    /* \xNN (lowercase) */
    static const char hex[] = "0123456789abcdef";
    dest[pos + 0] = '\\';
    dest[pos + 1] = 'x';
    dest[pos + 2] = hex[c >> 4];
    dest[pos + 3] = hex[c & 0x0f];
    return 4;
}

/**
 * Escape and write src[0..len) into dest starting at pos.
 * Returns the new position after writing.
 */
static size_t write_bytes(char *dest, size_t pos,
                          const unsigned char *src, size_t len) {
    for (size_t i = 0; i < len; i++) {
        pos += write_byte(dest, pos, src[i]);
    }
    return pos;
}

/* ---- public API ---- */

size_t dbg_print_bytes(char *dest, size_t dest_size,
                       const unsigned char *src, size_t src_len,
                       const char *omit)
{
    /* error / empty input */
    if (dest == NULL || dest_size == 0) {
        return 0;
    }
    if (src == NULL || src_len == 0) {
        dest[0] = '\0';
        return 0;
    }

    /* compute total escaped length */
    size_t total = 0;
    for (size_t i = 0; i < src_len; i++) {
        total += esc_len(src[i]);
    }

    const size_t max_out = dest_size - 1;   /* reserve room for null terminator */

    /* ---- fits without truncation ---- */
    if (total <= max_out) {
        size_t pos = write_bytes(dest, 0, src, src_len);
        dest[pos] = '\0';
        return total;
    }

    /* ---- truncation needed ---- */
    const char *mid = omit ? omit : "...";
    size_t mid_len = strlen(mid);
    size_t head_len, tail_len;
    int use_mid = 1;   /* 1: include marker, 0: omit marker */

    if (max_out < mid_len + 2) {
        /* not enough room for marker + at least 2 chars */
        use_mid = 0;
        head_len = max_out;
        tail_len = 0;
    } else {
        head_len = (max_out - mid_len + 1) / 2;      /* round up */
        tail_len = (max_out - mid_len) - head_len;   /* remainder */
    }

    /* --- determine head range (up to head_len escaped chars from start) --- */
    size_t head_end = 0;   /* index into src (exclusive) */
    {
        size_t acc = 0;
        for (size_t i = 0; i < src_len; i++) {
            size_t len = esc_len(src[i]);
            if (acc + len > head_len) break;
            acc += len;
            head_end = i + 1;
        }
    }

    /* --- determine tail range (up to tail_len escaped chars from end) --- */
    size_t tail_start = src_len;   /* index into src (start of tail portion) */
    if (tail_len > 0) {
        size_t acc = 0;
        for (size_t i = src_len; i > 0; i--) {
            size_t len = esc_len(src[i - 1]);
            if (acc + len > tail_len) break;
            acc += len;
            tail_start = i - 1;
        }
    }

    /* --- write to output buffer --- */
    size_t pos = 0;

    /* head portion */
    pos = write_bytes(dest, pos, src, head_end);

    /* middle omission marker (only if there is a gap between head and tail) */
    if (use_mid && head_end < tail_start) {
        memcpy(dest + pos, mid, mid_len);
        pos += mid_len;
    }

    /* tail portion */
    if (tail_start < src_len) {
        pos = write_bytes(dest, pos, src + tail_start,
                          src_len - tail_start);
    }

    dest[pos] = '\0';
    return total;
}
