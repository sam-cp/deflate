#include "huffman.h"
#include <stdlib.h>

int huffman_read_codeword(BITREADER* reader, HUFFMAN_TREE tree) {
    int fetched = 0;
    for (int i = 1; i < 16; i++) {
        fetched <<= 1;
        int next_bit = brreadbits(reader, 1);
        if (next_bit == EOF) return -1;
        fetched += next_bit;
        if (tree.num_symbols[i] && fetched < tree.min_codeword[i] + tree.num_symbols[i]) {
            return tree.symbol[i][fetched - tree.min_codeword[i]];
        }
    }
    return -1;
}

int huffman_calculate_min_codewords(HUFFMAN_TREE* tree) {
    int current_min = 0;
    for (int i = 1; i < 16; i++) {
        current_min <<= 1;
        tree->min_codeword[i] = current_min;
        current_min += tree->num_symbols[i];
        if (current_min > (1 << i)) return 0;
    }
    return 1;
}

void huffman_add_symbol(HUFFMAN_TREE* tree, int symbol, int length) {
    ++tree->num_symbols[length];
    tree->symbol[length] = reallocarray(tree->symbol[length], tree->num_symbols[length], sizeof(int));
    tree->symbol[length][tree->num_symbols[length] - 1] = symbol;
}