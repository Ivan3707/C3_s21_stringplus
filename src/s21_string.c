#include "s21_string.h"
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <stdint.h>

#if defined(__linux__)

static const char *s21_error_messages[134] = {
    [0] = "Success",
    [1] = "Operation not permitted",
    [2] = "No such file or directory",
    [3] = "No such process",
    [4] = "Interrupted system call",
    [5] = "Input/output error",
    [6] = "No such device or address",
    [7] = "Argument list too long",
    [8] = "Exec format error",
    [9] = "Bad file descriptor",
    [10] = "No child processes",
    [11] = "Resource temporarily unavailable",
    [12] = "Cannot allocate memory",
    [13] = "Permission denied",
    [14] = "Bad address",
    [15] = "Block device required",
    [16] = "Device or resource busy",
    [17] = "File exists",
    [18] = "Invalid cross-device link",
    [19] = "No such device",
    [20] = "Not a directory",
    [21] = "Is a directory",
    [22] = "Invalid argument",
    [23] = "Too many open files in system",
    [24] = "Too many open files",
    [25] = "Inappropriate ioctl for device",
    [26] = "Text file busy",
    [27] = "File too large",
    [28] = "No space left on device",
    [29] = "Illegal seek",
    [30] = "Read-only file system",
    [31] = "Too many links",
    [32] = "Broken pipe",
    [33] = "Numerical argument out of domain",
    [34] = "Numerical result out of range",
    [35] = "Resource deadlock avoided",
    [36] = "File name too long",
    [37] = "No locks available",
    [38] = "Function not implemented",
    [39] = "Directory not empty",
    [40] = "Too many levels of symbolic links",
    [42] = "No message of desired type",
    [43] = "Identifier removed",
    [44] = "Channel number out of range",
    [45] = "Level 2 not synchronized",
    [46] = "Level 3 halted",
    [47] = "Level 3 reset",
    [48] = "Link number out of range",
    [49] = "Protocol driver not attached",
    [50] = "No CSI structure available",
    [51] = "Level 2 halted",
    [52] = "Invalid exchange",
    [53] = "Invalid request descriptor",
    [54] = "Exchange full",
    [55] = "No anode",
    [56] = "Invalid request code",
    [57] = "Invalid slot",
    [59] = "Bad font file format",
    [60] = "Device not a stream",
    [61] = "No data available",
    [62] = "Timer expired",
    [63] = "Out of streams resources",
    [64] = "Machine is not on the network",
    [65] = "Package not installed",
    [66] = "Object is remote",
    [67] = "Link has been severed",
    [68] = "Advertise error",
    [69] = "Srmount error",
    [70] = "Communication error on send",
    [71] = "Protocol error",
    [72] = "Multihop attempted",
    [73] = "RFS specific error",
    [74] = "Bad message",
    [75] = "Value too large for defined data type",
    [76] = "Name not unique on network",
    [77] = "File descriptor in bad state",
    [78] = "Remote address changed",
    [79] = "Can not access a needed shared library",
    [80] = "Accessing a corrupted shared library",
    [81] = ".lib section in a.out corrupted",
    [82] = "Attempting to link in too many shared libraries",
    [83] = "Cannot exec a shared library directly",
    [84] = "Invalid or incomplete multibyte or wide character",
    [85] = "Interrupted system call should be restarted",
    [86] = "Streams pipe error",
    [87] = "Too many users",
    [88] = "Socket operation on non-socket",
    [89] = "Destination address required",
    [90] = "Message too long",
    [91] = "Protocol wrong type for socket",
    [92] = "Protocol not available",
    [93] = "Protocol not supported",
    [94] = "Socket type not supported",
    [95] = "Operation not supported",
    [96] = "Protocol family not supported",
    [97] = "Address family not supported by protocol",
    [98] = "Address already in use",
    [99] = "Cannot assign requested address",
    [100] = "Network is down",
    [101] = "Network is unreachable",
    [102] = "Network dropped connection on reset",
    [103] = "Software caused connection abort",
    [104] = "Connection reset by peer",
    [105] = "No buffer space available",
    [106] = "Transport endpoint is already connected",
    [107] = "Transport endpoint is not connected",
    [108] = "Cannot send after transport endpoint shutdown",
    [109] = "Too many references: cannot splice",
    [110] = "Connection timed out",
    [111] = "Connection refused",
    [112] = "Host is down",
    [113] = "No route to host",
    [114] = "Operation already in progress",
    [115] = "Operation now in progress",
    [116] = "Stale file handle",
    [117] = "Structure needs cleaning",
    [118] = "Not a XENIX named type file",
    [119] = "No XENIX semaphores available",
    [120] = "Is a named type file",
    [121] = "Remote I/O error",
    [122] = "Disk quota exceeded",
    [123] = "No medium found",
    [124] = "Wrong medium type",
    [125] = "Operation canceled",
    [126] = "Required key not available",
    [127] = "Key has expired",
    [128] = "Key has been revoked",
    [129] = "Key was rejected by service",
    [130] = "Owner died",
    [131] = "State not recoverable",
    [132] = "Operation not possible due to RF-kill",
    [133] = "Memory page has hardware error"
};

#elif defined(__APPLE__)

static const char *s21_error_messages[] = {
    "Undefined error: 0",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "Device not configured",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource deadlock avoided",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Resource busy",
    "File exists",
    "Cross-device link",
    "Operation not supported by device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Result too large",
    "Resource temporarily unavailable",
    "Operation now in progress",
    "Operation already in progress",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol family",
    "Address already in use",
    "Can't assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Socket is already connected",
    "Socket is not connected",
    "Can't send after socket shutdown",
    "Too many references: cannot splice",
    "Operation timed out",
    "Connection refused",
    "Too many levels of symbolic links",
    "File name too long",
    "Host is down",
    "No route to host",
    "Directory not empty",
    "Too many processes",
    "Too many users",
    "Disc quota exceeded",
    "Stale NFS file handle",
    "Too many levels of remote in path",
    "RPC struct is bad",
    "RPC version wrong",
    "RPC program unavailable",
    "RPC program version wrong",
    "RPC prog. not avail",
    "No locks available",
    "Function not implemented",
    "Inappropriate file type or format",
    "Authentication error",
    "Need authenticator",
    "Device power is off",
    "Device error",
    "Value too large for defined data type",
    "Bad executable",
    "Bad CPU type in executable",
    "Shared library version mismatch",
    "Malformed Mach-o file",
    "Operation canceled",
    "Identifier removed",
    "No message of desired type",
    "Illegal byte sequence",
    "Attribute not found",
    "Bad message",
    "Reserved",
    "No message available",
    "Reserved",
    "No STREAM resources",
    "Not a STREAM",
    "Protocol error",
    "STREAM ioctl timeout",
    "Operation not supported on socket",
    "No such policy registered",
    "State not recoverable",
    "Previous owner died",
    "Interface output queue is full"
};

#endif

s21_size s21_strlen(const char *str) {
    s21_size length = 0;

    while (str[length] != '\0') {
        length++;
    }

    return length;
}
void *s21_memset(void *destination, int value, s21_size n) {
    unsigned char *ptr = (unsigned char *)destination;
    unsigned char val = (unsigned char)value;

    for (s21_size i = 0; i < n; i++) {
        ptr[i] = val;
    }

    return destination;
}
int s21_memcmp(const void *str1, const void *str2, s21_size n){
    const unsigned char *ptr1 = (const unsigned char *)str1;
    const unsigned char *ptr2 = (const unsigned char *)str2;

    for (s21_size i = 0; i < n; i++) {
        if (ptr1[i] != ptr2[i]) {
            return (ptr1[i] - ptr2[i]);
        }
    }

    return 0;
}
void *s21_memchr(const void *str, int c, s21_size n){
    const unsigned char *ptr = (const unsigned char *)str;
    unsigned char target = (unsigned char)c;

    for (s21_size i = 0; i < n; i++) {
        if (ptr[i] == target) {
            return (void *)(ptr + i);
        }
    }

    return S21_NULL;
}
void *s21_memcpy(void *destination, const void *source, s21_size n){
    unsigned char *dest = (unsigned char *)destination;
    const unsigned char *src = (const unsigned char *)source;

    for (s21_size i = 0; i < n; i++) {
        dest[i] = src[i];
    }

    return destination;
}
s21_size s21_strcspn(const char *str1, const char *str2){
    s21_size count = 0;

    while (str1[count] != '\0') {
        const char *ptr = str2;
        int found = 0;

        while (*ptr != '\0') {
            if (str1[count] == *ptr) {
                found = 1;
                break;
            }
            ptr++;
        }

        if (found) {
            break;
        }

        count++;
    }

    return count;
}
char *s21_strchr(const char *str, int c){
    const char *ptr = str;
    unsigned char target = (unsigned char)c;

    while (*ptr != '\0') {
        if (*ptr == target) {
            return (char *)ptr;
        }
        ptr++;
    }

    if (target == '\0') {
        return (char *)ptr;
    }

    return S21_NULL;
}
char *s21_strrchr(const char *str, int c){
    const char *last_occurrence = S21_NULL;
    unsigned char target = (unsigned char)c;

    while (*str != '\0') {
        if (*str == target) {
            last_occurrence = str;
        }
        str++;
    }

    if (target == '\0') {
        return (char *)str;
    }

    return (char *)last_occurrence;
}

int s21_strncmp(const char *str1, const char *str2, s21_size n){
    for (s21_size i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
        if (str1[i] == '\0') {
            break;
        }
    }
    return 0;
}
char *s21_strncpy(char *destination, const char *source, s21_size n){
    s21_size i = 0;

    for (; i < n && source[i] != '\0'; i++) {
        destination[i] = source[i];
    }

    for (; i < n; i++) {
        destination[i] = '\0';
    }

    return destination;
}
char *s21_strncat(char *destination, const char *source, s21_size n){
    s21_size dest_len = s21_strlen(destination);
    s21_size i = 0;

    while (i < n && source[i] != '\0') {
        destination[dest_len + i] = source[i];
        i++;
    }

    destination[dest_len + i] = '\0';

    return destination;
}
char *s21_strpbrk(const char *str1, const char *str2){
    while (*str1 != '\0') {
        const char *ptr = str2;

        while (*ptr != '\0') {
            if (*str1 == *ptr) {
                return (char *)str1;
            }
            ptr++;
        }

        str1++;
    }

    return S21_NULL;
}
char *s21_strstr(const char *haystack, const char *needle){
    if (*needle == '\0') {
        return (char *)haystack;
    }

    while (*haystack != '\0') {
        const char *h = haystack;
        const char *n = needle;

        while (*h != '\0' && *n != '\0' && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return (char *)haystack;
        }

        haystack++;
    }

    return S21_NULL;
}
char *s21_strtok(char *str, const char *delim){
    static char *last = S21_NULL;

    if (str != S21_NULL) {
        last = str;
    } else if (last == S21_NULL) {
        return S21_NULL;
    }

    while (*last != '\0' && s21_strchr(delim, *last) != S21_NULL) {
        last++;
    }

    if (*last == '\0') {
        last = S21_NULL;
        return S21_NULL;
    }

    char *token_start = last;

    while (*last != '\0' && s21_strchr(delim, *last) == S21_NULL) {
        last++;
    }

    if (*last != '\0') {
        *last = '\0'; 
        last++;
    } else {
        last = S21_NULL; 
    }

    return token_start;
}
static void s21_int_to_string(int value, char *buffer) {
    unsigned int number;
    int i = 0;

    if (value < 0) {
        buffer[i++] = '-';
        number = 0u - (unsigned int)value;
    } else {
        number = (unsigned int)value;
    }

    if (number == 0) {
        buffer[i++] = '0';
    } else {
        int start = i;

        while (number > 0) {
            buffer[i++] = (char)('0' + number % 10);
            number /= 10;
        }

        for (int left = start, right = i - 1; left < right; left++, right--) {
            char temp = buffer[left];
            buffer[left] = buffer[right];
            buffer[right] = temp;
        }
    }

    buffer[i] = '\0';
}
char *s21_strerror(int errnum) {
#if defined(__linux__) || defined(__APPLE__)
    s21_size count = sizeof(s21_error_messages) /
                     sizeof(s21_error_messages[0]);

    if (errnum >= 0 && (s21_size)errnum < count) {
        return (char *)s21_error_messages[errnum];
    }
#endif

    static char buffer[64];
    const char prefix[] = "Unknown error: ";
    s21_size i = 0;

    while (prefix[i] != '\0') {
        buffer[i] = prefix[i];
        i++;
    }

    s21_int_to_string(errnum, buffer + i);

    return buffer;
}
void *s21_to_upper(const char *str){
    if (str == S21_NULL) {
        return S21_NULL;
    }

    s21_size length = s21_strlen(str);
    char *result = (char *)malloc(length + 1);

    if (result == S21_NULL) {
        return S21_NULL;
    }

    for (s21_size i = 0; i < length; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            result[i] = str[i] - ('a' - 'A');
        } else {
            result[i] = str[i];
        }
    }

    result[length] = '\0';

    return result;
}
void *s21_to_lower(const char *str){
    if (str == S21_NULL) {
        return S21_NULL;
    }

    s21_size length = s21_strlen(str);
    char *result = (char *)malloc(length + 1);

    if (result == S21_NULL) {
        return S21_NULL;
    }

    for (s21_size i = 0; i < length; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            result[i] = str[i] + ('a' - 'A');
        } else {
            result[i] = str[i];
        }
    }

    result[length] = '\0';

    return result;
}
void *s21_insert(const char *src, const char *str, s21_size start_index){
    if (src == S21_NULL || str == S21_NULL) {
        return S21_NULL;
    }

    s21_size src_length = s21_strlen(src);
    s21_size str_length = s21_strlen(str);

    if (start_index > src_length) {
        return S21_NULL;
    }

    char *result = (char *)malloc(src_length + str_length + 1);
    if (result == S21_NULL) {
        return S21_NULL;
    }

    for (s21_size i = 0; i < start_index; i++) {
        result[i] = src[i];
    }

    for (s21_size i = 0; i < str_length; i++) {
        result[start_index + i] = str[i];
    }

    for (s21_size i = start_index; i < src_length; i++) {
        result[str_length + i] = src[i];
    }

    result[src_length + str_length] = '\0';

    return result;
}

void *s21_trim(const char *src, const char *trim_chars){
    if (src == S21_NULL || trim_chars == S21_NULL) {
        return S21_NULL;
    }

    s21_size src_length = s21_strlen(src);
    s21_size start_index = 0;
    s21_size end_index = src_length;

    while (start_index < end_index && s21_strchr(trim_chars, src[start_index]) != S21_NULL) {
        start_index++;
    }

    while (end_index > start_index && s21_strchr(trim_chars, src[end_index - 1]) != S21_NULL) {
        end_index--;
    }

    s21_size trimmed_length = end_index - start_index;
    char *result = (char *)malloc(trimmed_length + 1);
    if (result == S21_NULL) {
        return S21_NULL;
    }

    for (s21_size i = 0; i < trimmed_length; i++) {
        result[i] = src[start_index + i];
    }

    result[trimmed_length] = '\0';

    return result;
}

/* ==================== Part 4. s21_sscanf ==================== */

static int s21_ss_is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' ||
           c == '\r' || c == '\f' || c == '\v';
}

static int s21_ss_is_digit(char c) {
    return c >= '0' && c <= '9';
}

static int s21_ss_hex_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int s21_sscanf(const char *str, const char *format, ...) {
    if (str == S21_NULL || format == S21_NULL) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    const char *s = str;
    const char *f = format;
    int matched = 0;

    while (*f) {
        if (s21_ss_is_space(*f)) {
            while (s21_ss_is_space(*s)) s++;
            f++;
            continue;
        }

        if (*f != '%') {
            if (*s != *f) break;
            s++;
            f++;
            continue;
        }

        f++;

        if (*f == '%') {
            if (*s != '%') break;
            s++;
            f++;
            continue;
        }

        int suppress = 0;
        if (*f == '*') {
            suppress = 1;
            f++;
        }

        int width = 0;
        while (s21_ss_is_digit(*f)) {
            width = width * 10 + (*f - '0');
            f++;
        }

        char len_mod = 0;
        if (*f == 'h' || *f == 'l' || *f == 'L') {
            len_mod = *f;
            f++;
        }

        char spec = *f;
        if (spec == '\0') break;
        f++;

        if (spec != 'c' && spec != 'n') {
            while (s21_ss_is_space(*s)) s++;
        }

        if (spec == 'n') {
            if (!suppress) {
                int *p = va_arg(args, int *);
                *p = (int)(s - str);
            }
            continue;
        }

        if (*s == '\0') break;

        int this_match = 0;
        int max_len = (width > 0) ? width : INT_MAX;

        if (spec == 'c') {
            int n = (width > 0) ? width : 1;
            if (!suppress) {
                char *dest = va_arg(args, char *);
                int k = 0;
                while (k < n && s[k]) {
                    dest[k] = s[k];
                    k++;
                }
                if (k == 0) break;
                s += k;
            } else {
                int k = 0;
                while (k < n && s[k]) k++;
                if (k == 0) break;
                s += k;
            }
            this_match = 1;
        } else if (spec == 's') {
            char buf[1024];
            int i = 0;
            while (*s && !s21_ss_is_space(*s) && i < max_len && i < 1023) {
                buf[i++] = *s++;
            }
            buf[i] = '\0';
            if (i == 0) break;
            if (!suppress) {
                char *dest = va_arg(args, char *);
                for (int k = 0; k <= i; k++) dest[k] = buf[k];
            }
            this_match = 1;
        } else if (spec == '[') {
            int negate = 0;
            int charset[256] = {0};

            if (*f == '^') {
                negate = 1;
                f++;
            }

            while (*f && *f != ']') {
                char start = *f++;
                if (*f == '-' && *(f + 1) != ']' && *(f + 1) != '\0') {
                    f++;
                    char end = *f++;
                    for (int c = (unsigned char)start; c <= (unsigned char)end; c++) {
                        charset[c] = 1;
                    }
                } else {
                    charset[(unsigned char)start] = 1;
                }
            }
            if (*f == ']') f++;

            char buf[1024];
            int i = 0;
            while (*s && i < max_len && i < 1023) {
                int in = charset[(unsigned char)*s];
                if (negate) in = !in;
                if (!in) break;
                buf[i++] = *s++;
            }
            buf[i] = '\0';

            if (i == 0) break;

            if (!suppress) {
                char *dest = va_arg(args, char *);
                for (int k = 0; k <= i; k++) dest[k] = buf[k];
            }
            this_match = 1;
        } else if (spec == 'd' || spec == 'i' || spec == 'u' ||
                   spec == 'o' || spec == 'x' || spec == 'X' || spec == 'p') {
            int is_signed = (spec == 'd' || spec == 'i');
            int base = 10;
            char buf[128];
            int i = 0;
            int neg = 0;

            if ((spec == 'd' || spec == 'i') && (*s == '-' || *s == '+')) {
                if (*s == '-') neg = 1;
                s++;
            }

            if (spec == 'p') {
                base = 16;
                if (*s == '0' && (s[1] == 'x' || s[1] == 'X')) s += 2;
            } else if (spec == 'x' || spec == 'X') {
                base = 16;
                if (*s == '0' && (s[1] == 'x' || s[1] == 'X')) s += 2;
            } else if (spec == 'o') {
                base = 8;
            } else if (spec == 'i') {
                if (*s == '0' && (s[1] == 'x' || s[1] == 'X')) {
                    base = 16;
                    s += 2;
                } else if (*s == '0') {
                    base = 8;
                }
            }

            while (*s && i < (int)sizeof(buf) - 1 && i < max_len) {
                int v;
                if (base == 16) v = s21_ss_hex_val(*s);
                else v = s21_ss_is_digit(*s) ? (*s - '0') : -1;
                if (v < 0 || v >= base) break;
                buf[i++] = *s++;
            }
            buf[i] = '\0';

            if (i == 0) break;

            unsigned long long val = 0;
            for (int k = 0; k < i; k++) {
                int v;
                if (base == 16) v = s21_ss_hex_val(buf[k]);
                else v = buf[k] - '0';
                val = val * base + v;
            }

            if (!suppress) {
                if (spec == 'p') {
                    void **pp = va_arg(args, void **);
                    *pp = (void *)(uintptr_t)val;
                } else if (len_mod == 'h') {
                    short *p = va_arg(args, short *);
                    *p = (short)(neg ? -(long long)val : (long long)val);
                } else if (len_mod == 'l') {
                    long *p = va_arg(args, long *);
                    *p = neg ? -(long)val : (long)val;
                } else if (is_signed) {
                    int *p = va_arg(args, int *);
                    *p = neg ? -(int)val : (int)val;
                } else {
                    unsigned int *p = va_arg(args, unsigned int *);
                    *p = (unsigned int)val;
                }
            }
            this_match = 1;
        } else if (spec == 'e' || spec == 'E' || spec == 'f' ||
                   spec == 'g' || spec == 'G') {
            char buf[128];
            int i = 0;
            int has_digit = 0;
            int has_dot = 0;
            int has_exp = 0;

            if ((*s == '-' || *s == '+') && i < 127) {
                buf[i++] = *s++;
            }
            while (*s && i < 127 && i < max_len) {
                char c = *s;
                if (s21_ss_is_digit(c)) {
                    has_digit = 1;
                    buf[i++] = c;
                    s++;
                } else if (c == '.' && !has_dot && !has_exp) {
                    has_dot = 1;
                    buf[i++] = c;
                    s++;
                } else if ((c == 'e' || c == 'E') && has_digit && !has_exp) {
                    has_exp = 1;
                    buf[i++] = c;
                    s++;
                    if ((*s == '-' || *s == '+') && i < 127) {
                        buf[i++] = *s++;
                    }
                } else {
                    break;
                }
            }
            buf[i] = '\0';

            if (!has_digit) break;

            if (!suppress) {
                long double val = strtold(buf, S21_NULL);
                if (len_mod == 'L') {
                    long double *p = va_arg(args, long double *);
                    *p = val;
                } else if (len_mod == 'l') {
                    double *p = va_arg(args, double *);
                    *p = (double)val;
                } else {
                    float *p = va_arg(args, float *);
                    *p = (float)val;
                }
            }
            this_match = 1;
        } else {
            break;
        }

        if (this_match && !suppress) {
            matched++;
        }
    }

    va_end(args);
    return matched;
}