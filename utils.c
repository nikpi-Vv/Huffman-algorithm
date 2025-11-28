#include "utils.h"
#include <stdio.h>

long get_file_size(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return -1;
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return -1;
    }
    long size = ftell(f);
    fclose(f);
    return size;
}

int files_equal(const char *f1, const char *f2) {
    FILE *a = fopen(f1, "rb");
    FILE *b = fopen(f2, "rb");
    if (!a || !b) {
        if (a) fclose(a);
        if (b) fclose(b);
        return 0;
    }

    int ca, cb;
    do {
        ca = fgetc(a);
        cb = fgetc(b);
        if (ca != cb) {
            fclose(a);
            fclose(b);
            return 0;
        }
    } while (ca != EOF && cb != EOF);

    fclose(a);
    fclose(b);
    return 1;
}
