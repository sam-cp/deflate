#include "bitreader.h"
#include "deflate.h"
#include "huffman.h"
#include <stdlib.h>

#define BTYPE_STORE 0
#define BTYPE_FIXED_HUFFMAN 1
#define BTYPE_DYNAMIC_HUFFMAN 2

// Fixed Huffman trees

int sym7[24]  = {256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267,
                 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279};

int sym8[152] = {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
                  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,
                  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,
                  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,
                  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,
                  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
                 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
                 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 280, 281, 282, 283, 284, 285, 286, 287};

int sym9[112] = {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
                 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171,
                 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185,
                 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 
                 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 
                 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 
                 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

HUFFMAN_TREE fixed_tree_ll = {{   0,    0,    0,    0,    0,    0,    0,    0,   48,  400,    0,    0,    0,    0,    0,    0},
                                    {   0,    0,    0,    0,    0,    0,    0,   24,  152,  112,    0,    0,    0,    0,    0,    0},
                                    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, sym7, sym8, sym9, NULL, NULL, NULL, NULL, NULL, NULL}};

int sym5[32] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 
                16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

HUFFMAN_TREE fixed_tree_d = {{   0},
                             {   0,    0,    0,    0,    0,   32,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0},
                             {NULL, NULL, NULL, NULL, NULL, sym5, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};

const int dynamic_tree_order[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

// Extra bits references

const int extra_length_bits[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
const int base_lengths[29] = { 3,   4,   5,   6,   7,   8,   9,  10,  11,  13,  15,  17,  19,  23, 
                              27,  31,  35,  43,  51,  59,  67,  83,  99, 115, 131, 163, 195, 227, 258};

/**
 * Decodes the dynamic huffman trees from the bit reader.
 * 
 * @param reader: the bit reader
 * @param tree_ll: pointer to the literal-length tree to read into
 * @param tree_d: pointer to the distance tree to read into
 * 
 * @return 1 on success, 0 otherwise
*/
int decode_dynamic_trees(BITREADER* reader, HUFFMAN_TREE* tree_ll, HUFFMAN_TREE* tree_d) {
    // First, decode the nested tree
    HUFFMAN_TREE nested_tree = {{0}, {0}, {NULL}};
    int hlit = brreadbits(reader, 5);
    if (hlit == EOF) return 0;
    int hdist = brreadbits(reader, 5);
    if (hdist == EOF) return 0;
    int hclen = brreadbits(reader, 4);
    if (hclen == EOF) return 0;

    int tree_values[19] = {0};
    for (int i = 0; i < hclen + 4; i++) {
        int length_of_code = brreadbits(reader, 3);
        if (length_of_code == EOF) return 0;
        tree_values[dynamic_tree_order[i]] = length_of_code;
    }
    for (int i = 0; i < 19; i++) {
        huffman_add_symbol(&nested_tree, i, tree_values[i]);
    }
    if (!huffman_calculate_min_codewords(&nested_tree)) return 0;

    int prev_length;
    int i = 0;
    while (i < hlit + hdist + 258) {
        int symbol = huffman_read_codeword(reader, nested_tree);
        if (symbol < 0) return 0;
        if (symbol < 15) {
            prev_length = symbol;
            huffman_add_symbol((i < hlit + 257) ? tree_ll : tree_d, (i < hlit + 257) ? i : i - hlit - 257, symbol);
            i++;
        } else if (symbol == 16) {
            if (i == 0) return 0;
            int extra_bits = brreadbits(reader, 2);
            if (extra_bits == EOF) return 0;
            int end_pos = i + extra_bits + 3;
            if (end_pos > hlit + hdist + 258) end_pos = hlit + hdist + 258;
            for (; i < end_pos; i++) {
                huffman_add_symbol((i < hlit + 257) ? tree_ll : tree_d, (i < hlit + 257) ? i : i - hlit - 257, prev_length);
            }
        } else if (symbol == 17) {
            prev_length = 0;
            int extra_bits = brreadbits(reader, 3);
            if (extra_bits == EOF) return 0;
            int end_pos = i + extra_bits + 3;
            if (end_pos > hlit + hdist + 258) end_pos = hlit + hdist + 258;
            for (; i < end_pos; i++) {
                huffman_add_symbol((i < hlit + 257) ? tree_ll : tree_d, (i < hlit + 257) ? i : i - hlit - 257, 0);
            }
        } else if (symbol == 18) {
            prev_length = 0;
            int extra_bits = brreadbits(reader, 7);
            if (extra_bits == EOF) return 0;
            int end_pos = i + extra_bits + 11;
            if (end_pos > hlit + hdist + 258) end_pos = hlit + hdist + 258;
            for (; i < end_pos; i++) {
                huffman_add_symbol((i < hlit + 257) ? tree_ll : tree_d, (i < hlit + 257) ? i : i - hlit - 257, 0);
            }
        } else {
            return 0;
        }
    }
    return huffman_calculate_min_codewords(tree_ll) && huffman_calculate_min_codewords(tree_d);
}

/**
 * Decodes a Huffman-coded message from a bit reader into a vector.
 * 
 * @param reader: the bit reader to read from
 * @param vector: the vector to read into
 * @param tree_ll: the literal-length Huffman tree
 * @param tree_d: the distance Huffman tree
 * @returns 1 on success, 0 otherwise
*/
int huffman_decode(BITREADER* reader, VECTOR* vector, const HUFFMAN_TREE tree_ll, const HUFFMAN_TREE tree_d) {
    while(1) {
        int symbol = huffman_read_codeword(reader, tree_ll);
        if (symbol == -1) return 0;
        else if (symbol < 256) vec_push_back(vector, (char) symbol);  // literal
        else if (symbol == 256) return 1;  // end of block
        else if (symbol < 286) {  // length
            int extra_length = brreadbits(reader, extra_length_bits[symbol - 257]);
            if (extra_length == EOF) return 0;
            int length = base_lengths[symbol - 257] + extra_length;

            int distance_symbol = huffman_read_codeword(reader, tree_d);
            if (distance_symbol == -1 || distance_symbol > 29) return 0;
            int extra_distance_bits = (distance_symbol >= 2) ? distance_symbol / 2 - 1 : 0;
            int base_distance = (distance_symbol >= 2) ? ((2 + distance_symbol % 2) << extra_distance_bits) + 1 : distance_symbol + 1;
            int extra_distance = brreadbits(reader, extra_distance_bits);
            if (extra_distance == EOF) return 0;
            int distance = base_distance + extra_distance;
            for (int i = 0; i < length; i++) {
                if (vec_size(vector) < distance) return 0;
                char c = vec_at(vector, vec_size(vector) - distance);
                vec_push_back(vector, c);
            }
        } else return 0;
    }
    return 0;
}

/**
 * Private function to inflate a block of DEFLATED data from a bit reader into a vector.
 *
 * @param reader: the bit reader
 * @param vector: the vector to push to
 * @returns 0 if the decompressor should inflate the next block,
 *          1 if the final block was successful,
 *         -1 if an error was encountered
 */
int inflate_block(BITREADER* reader, VECTOR* vector) {
    int final = brreadbits(reader, 1);
    int size, size_c, result;
    HUFFMAN_TREE tree_ll = {{0}, {0}, {0}}, tree_d = {{0}, {0}, {0}};
    switch(brreadbits(reader, 2)) {
        case BTYPE_STORE:
            size = brread_uint16(reader);
            if (size == EOF) return -1;
            size_c = brread_uint16(reader);
            if (size_c == EOF) return -1;
            if ((size ^ size_c) != 0xFFFF) return -1;
            for (int i = 0; i < size; i++) {
                int byte = brread_uint8(reader);
                if (byte == EOF) return -1;
                vec_push_back(vector, byte);
            }
            break;
        case BTYPE_FIXED_HUFFMAN:
            if (!huffman_decode(reader, vector, fixed_tree_ll, fixed_tree_d)) return -1;
            break;
        case BTYPE_DYNAMIC_HUFFMAN:
            result = decode_dynamic_trees(reader, &tree_ll, &tree_d);
            if (result) result = huffman_decode(reader, vector, tree_ll, tree_d);
            for (int i = 0; i < 16; i++) {
                free(tree_ll.symbol[i]);
                free(tree_d.symbol[i]);
            }
            if (!result) return -1;
            break;
        default:
            return -1;
    }
    return final;
}

VECTOR* inflate(FILE* stream) {
    BITREADER* br = brattach(stream);
    VECTOR* vec = vec_construct_empty();
    int result;
    do {
        result = inflate_block(br, vec);
    } while(!result);
    brfree(br);
    if (result == -1) {
        vec_free(vec);
        return NULL;
    }
    return vec;
}

int inflate_to_file(FILE* input_stream, FILE* output_stream) {
    VECTOR* inflated = inflate(input_stream);
    if (!inflated) return 0;
    int result = fwrite(vec_data(inflated), 1, vec_size(inflated), output_stream) == vec_size(inflated);
    vec_free(inflated);
    return result;
}