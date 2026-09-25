#include "../include/myfilefunctions.h"
#include <stdlib.h>
#include <string.h>

int wordCount(FILE* file, int* lines, int* words, int* chars) {
    if (file == NULL) return -1;

    *lines = 0;
    *words = 0;
    *chars = 0;

    int c;
    int in_word = 0;  // tracks whether we're currently inside a word

    while ((c = fgetc(file)) != EOF) {
        (*chars)++;

        if (c == '\n') {
            (*lines)++;
        }

        if (c == ' ' || c == '\n' || c == '\t') {
            in_word = 0;  // we just hit whitespace, so we're "between" words
        } else if (in_word == 0) {
            in_word = 1;   // first non-whitespace char after whitespace = new word starts
            (*words)++;
        }
    }

    return 0;
}

int mygrep(FILE* fp, const char* search_str, char*** matches) {
    if (fp == NULL || search_str == NULL) return -1;

    int capacity = 4;          // start with room for 4 matching lines
    int count = 0;
    char** result = malloc(capacity * sizeof(char*));
    if (result == NULL) return -1;

    char buffer[1024];  // holds one line at a time while reading

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, search_str) != NULL) {
            // grow the array if we've run out of room
            if (count == capacity) {
                capacity *= 2;
                char** temp = realloc(result, capacity * sizeof(char*));
                if (temp == NULL) {
                    // realloc failed: free what we have so far, then bail
                    for (int i = 0; i < count; i++) free(result[i]);
                    free(result);
                    return -1;
                }
                result = temp;
            }

            // strdup makes an independent heap copy of this line,
            // since 'buffer' gets overwritten on the next fgets call
            result[count] = strdup(buffer);
            if (result[count] == NULL) {
                for (int i = 0; i < count; i++) free(result[i]);
                free(result);
                return -1;
            }
            count++;
        }
    }

    *matches = result;
    return count;
}
