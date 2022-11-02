#ifndef BITREADER_H
#define BITREADER_H

#include <stdio.h>
#include <stdint.h>

/**
 * Structure for reading individual bits from a file.
 * @param file: the file being read from
 * @param current: the character being read
 * @param offset: the current bit being read in the character (0-7)
 */
typedef struct __BITREADER {
    FILE* file;
    char current;
    int offset;
} BITREADER;

/**
 * Creates a new bit reader for the file with the given name.
 * The bit reader must be freed later with brclose() or brfree() to avoid memory leaks.
 * @param filename: name of the file
 * @returns the bit reader, or NULL if the file could not be opened
 */
BITREADER* bropen(const char* filename);

/**
 * Creates a new bit reader for an already-opened file stream.
 * The stream must have read permissions, or the behavior is undefined.
 * The bit reader must be freed later with brclose() or brfree() to avoid memory leaks.
 * @param stream: an opened file stream
 * @returns the bit reader, or NULL if the stream was NULL
 */
BITREADER* brattach(FILE* stream);

/**
 * Reads some number of bits from a bit reader.
 * The first bit will be interpreted as the MSB.
 * @param reader: the bit reader to read from
 * @param n: the number of bits to read (must be less than sizeof(int))
 * @return the integer representing the bits read, or EOF if EOF was reached
 */
int brreadbits(BITREADER* reader, int n);

/**
 * Reads a byte/character from the bit reader.
 * If a byte has been partially read, this function ignores the rest.
 * @param reader: bit reader to read from
 * @returns the unsigned short as an int, or EOF if EOF was reached
 */
int brread_uint8(BITREADER* reader);

/**
 * Reads an unsigned 16-bit integer from a bit reader, assuming it was stored in little-endian format.
 * If a byte has been partially read, this function ignores the rest.
 * 
 * @param reader: bit reader to read from
 * @returns the unsigned short as an int, or EOF if EOF was reached
 */
int brread_uint16(BITREADER* reader);

/**
 * Closes the bit reader's file stream and frees the bit reader.
 * If the bit reader is NULL, nothing will happen.
 * 
 * @param reader: the bit reader to close
 * @returns 0 on success or if the bit reader is NULL, or EOF otherwise
 */
int brclose(BITREADER* reader);

/**
 * Frees the bit reader without closing the file stream.
 * If the bit reader is NULL, nothing will happen.
 * 
 * @param reader: the bit reader to close
 * @returns the file stream, or NULL if the bit reader was null.
 */
FILE* brfree(BITREADER* reader);

#endif