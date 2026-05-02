#include "../include/common.h"

int format_error(const char* fmt, ...) {
    int save_error_no;
    save_error_no = errno;

    va_list list;
    va_start(list, fmt);

    vfprintf(stdout, fmt, list);
    fprintf(stdout, "\n");
    fflush(stdout);

    if (save_error_no != 0) {
        fprintf(stdout, "(errorno = %d) : %s", save_error_no, strerror(save_error_no));
        fprintf(stdout, "\n");
        fflush(stdout);
    }

    va_end(list);
    return -1;
}

char* bin_to_hex(char* bin_fmt, size_t len) {
    char* hex_digits = "0123456789ABCDEF";

    if (bin_fmt == NULL || len <= 0) { return NULL; }

    // EACH CHARACTER(8 BITS) => (2 HEX NUMBERS + 1 SPACE) + 1 NULL AT THE END
    size_t res_len = (3 * len) + 1;
    char* result = malloc(res_len);;
    memset(result, 0, res_len);

    for (size_t i = 0; i < len; i++) {
        result[3 * i] = hex_digits[bin_fmt[i] >> 4];
        result[(3 * i) + 1] = hex_digits[bin_fmt[i] & 0x0F];
        result[(3 * i) + 2] = ' '; // THIS IS FOR READABILITY
    }

    return result;
}
