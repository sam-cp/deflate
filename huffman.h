#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "bitreader.h"

/**
 * Structure for storing a canonical Huffman tree.
 * The maximum codeword length in this model is 15.
 */
typedef struct __HUFFMAN_TREE {
    int min_codeword[16];   // minimum codeword for a given length
    int num_symbols[16];    // number of codewords for a given length
    int* symbol[16];        // symbol for a given length and offset from minimum codeword: length of symbol[i] should equal num_symbols[i]
} HUFFMAN_TREE;


/**
 * Reads bits from a bit reader and decodes them according to a canonical Huffman tree.
 * 
 * @param reader: the bit reader
 * @param tree: the canonical Huffman tree
 * @returns the decoded symbol, or -1 if there was an error
 */
int huffman_read_codeword(BITREADER* reader, const HUFFMAN_TREE tree);

/**
 * Generates the minimum codewords for a Huffman tree based on the number of symbols for each codeword length.
 * 
 * @param tree: a pointer to the Huffman tree
 * @returns 1 on success, or 0 for an invalid tree
*/
int huffman_calculate_min_codewords(HUFFMAN_TREE* tree);

/**
 * Adds a symbol to the given Huffman tree.
 * This function requires that each element of the symbol array be dynamically allocated.
 * Statically allocated symbol arrays will cause undefined behavior.
 * 
 * @param tree: a pointer to the Huffman tree
 * @param symbol: the symbol to add
 * @param length: the length of the symbol's codeword
*/
void huffman_add_symbol(HUFFMAN_TREE* tree, int symbol, int length);

#endif