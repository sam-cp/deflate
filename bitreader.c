#include <stdlib.h>
#include "bitreader.h"

BITREADER* bropen(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    BITREADER* br = malloc(sizeof(BITREADER));
    br->file = f;
    br->current = 0;
    br->offset = 0;
    return br;
}

BITREADER* brattach(FILE* stream) {
    if (!stream) return NULL;
    BITREADER* br = malloc(sizeof(BITREADER));
    br->file = stream;
    br->current = 0;
    br->offset = 0;
}

int brreadbits(BITREADER* reader, int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        if (!reader->offset) {
            int next_char = fgetc(reader->file);
            if (next_char == EOF) return EOF;
            reader->current = next_char;
        }
        if (reader->current & (1 << reader->offset)) result |= (1 << i);
        ++reader->offset;
        reader->offset %= 8;
    }
    return result;
}

int brread_uint8(BITREADER* reader) {
    reader->offset = 0;
    uint8_t c;
    if (fread(&c, 1, 1, reader->file) == 0) return EOF;
    return c;
}

int brread_uint16(BITREADER* reader) {
    reader->offset = 0;
    uint16_t ls, ms;
    if (fread(&ls, 1, 1, reader->file) == 0 || fread(&ms, 1, 1, reader->file) == 0) return EOF;
    return ls + (ms << 8);
}

int brclose(BITREADER* reader) {
    if (!reader) return 0;
    int result = fclose(reader->file);
    free(reader);
    return result;
}

FILE* brfree(BITREADER* reader) {
    if (!reader) return NULL;
    FILE* file = reader->file;
    free(reader);
    return file;
}