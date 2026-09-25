#include "../include/mystrfunctions.h"

int mystrlen(const char* s) {
    int count = 0;
    while (s[count] != '\0') {
        count++;
    }
    return count;
}

int mystrcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';  // copy the null terminator too, or dest is unterminated
    return i;        // number of characters copied (excluding null)
}

int mystrncpy(char* dest, const char* src, int n) {
    int i = 0;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    // If src was shorter than n, real strncpy pads the rest with '\0'.
    // We replicate that so dest is always fully defined up to n bytes.
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return i;
}

int mystrcat(char* dest, const char* src) {
    int dest_len = mystrlen(dest);  // find where dest currently ends
    int i = 0;
    while (src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
    return dest_len + i;  // new total length of dest
}
