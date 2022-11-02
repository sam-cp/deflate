#ifndef DEFLATE_H
#define DEFLATE_H

#include "vector.h"
#include <stdio.h>

/**
 * Decompresses a file with the DEFLATE algorithm and writes its output to a vector.
 * 
 * @param stream: the file stream to inflate
 * @returns a vector with the inflated content, or NULL if the content could not be inflated
 */
VECTOR* inflate(FILE* stream);

/**
 * Decompresses a file with the DEFLATE algorithm, and writes the output to a file.
 * 
 * @param input_stream: the file stream to inflate
 * @param output_stream: the file stream to write to
 * @returns 1 on success, or 0 on failure
*/
int inflate_to_file(FILE* input_stream, FILE* output_stream);

#endif