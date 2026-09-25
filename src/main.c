#include <stdio.h>
#include <stdlib.h>
#include "../include/mystrfunctions.h"
#include "../include/myfilefunctions.h"

int main() {
    printf("--- Testing String Functions ---\n");

    // mystrlen
    const char* test1 = "Hello, World!";
    printf("mystrlen(\"%s\") = %d\n", test1, mystrlen(test1));

    // mystrcpy
    char dest1[50];
    mystrcpy(dest1, test1);
    printf("mystrcpy result: \"%s\"\n", dest1);

    // mystrncpy - test both cases: src longer than n, and src shorter than n
    char dest2[50];
    mystrncpy(dest2, "Programming", 5);
    dest2[5] = '\0';  // manually terminate since we only copied 5 chars, no null guaranteed
    printf("mystrncpy(\"Programming\", 5) = \"%s\"\n", dest2);

    char dest3[50];
    mystrncpy(dest3, "Hi", 10);
    printf("mystrncpy(\"Hi\", 10) = \"%s\" (should be null-padded)\n", dest3);

    // mystrcat
    char dest4[50] = "Operating ";
    mystrcat(dest4, "Systems");
    printf("mystrcat result: \"%s\"\n", dest4);

    printf("\n--- Testing File Functions ---\n");

    // wordCount - create a small test file first
    FILE* wf = fopen("test.txt", "w");
    if (wf == NULL) {
        printf("Failed to create test.txt\n");
        return 1;
    }
    fprintf(wf, "Hello world\nThis is a test file\nGrep this line please\n");
    fclose(wf);

    FILE* rf = fopen("test.txt", "r");
    if (rf == NULL) {
        printf("Failed to open test.txt for reading\n");
        return 1;
    }
    int lines, words, chars;
    if (wordCount(rf, &lines, &words, &chars) == 0) {
        printf("wordCount: lines=%d, words=%d, chars=%d\n", lines, words, chars);
    } else {
        printf("wordCount failed\n");
    }
    fclose(rf);

    // mygrep - search test.txt for a keyword
    FILE* gf = fopen("test.txt", "r");
    if (gf == NULL) {
        printf("Failed to open test.txt for grep\n");
        return 1;
    }
    char** matches;
    int match_count = mygrep(gf, "test", &matches);
    fclose(gf);

    if (match_count == -1) {
        printf("mygrep failed\n");
    } else {
        printf("mygrep found %d matching line(s):\n", match_count);
        for (int i = 0; i < match_count; i++) {
            printf("  %s", matches[i]);  // line already has \n from the file
            free(matches[i]);            // free each string mygrep allocated
        }
        free(matches);                    // free the array itself
    }

    return 0;
}
