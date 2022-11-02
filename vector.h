#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

/* A random-access, variable-length list of characters.
 * Only the "vec" functions should be used to modify this structure, or else undefined behavior is possible.
 */
typedef struct __VECTOR {
    char* data;
    size_t size;
    size_t capacity;
} VECTOR;

/**
 * Allocates an empty vector.
 * Must be freed later with vec_free().
 * 
 * @returns an empty vector
 */
VECTOR* vec_construct_empty();

/**
 * Allocates an uninitialized vector with the given capacity.
 * Must be freed later with vec_free().
 *
 * @param capacity: the capacity of the vector
 * @returns an uninitialized vector with the given size
 */
VECTOR* vec_construct_capacity(size_t capacity);

/**
 * Allocates a vector with the given capacity and fills it with the given character.
 * Must be freed later with vec_free().
 *
 * @param capacity: the capacity of the vector
 * @returns an uninitialized vector with the given size
 */
VECTOR* vec_construct_fill(size_t capacity, char fill);

/**
 * Frees a vector and its contents. If the vector is NULL, this does nothing.
 *
 * @param vector: the vector to free
 */
void vec_free(VECTOR* vector);


/**
 * Clears a vector's contents.
 * 
 * @param vector: the vector to empty
 */
void vec_clear(VECTOR* vector);

/**
 * Returns whether a vector is empty.
 *
 * @param vector: the vector
 * @returns 1 if the vector is empty, 0 otherwise
 */
int vec_empty(const VECTOR* vector);

/**
 * Returns a vector's size (the number of elements).
 *
 * @param vector: the vector
 * @returns the size of the vector
 */
size_t vec_size(const VECTOR* vector);

/**
 * Returns an array of a vector's contents.
 * 
 * @param vector: the vector
 * @returns an array of chars representing the vector's contents
*/
const char* vec_data(const VECTOR* vector);

/**
 * Returns the element at a given index of a vector.
 *
 * @param vector: the vector
 * @param index: the index to search
 * @returns the element at that index, or the null character if that index does not exist
 */
char vec_at(const VECTOR* vector, size_t index);

/**
 * Returns the last value of a vector.
 * 
 * @param vector: the vector
 * @returns the back value, or the null character if the vector is empty
 */
char vec_back(const VECTOR* vector);

/**
 * Sets the value at the given index of a vector.
 *
 * @param vector: the vector to modify
 * @param value: the value to push
 * @param index: the index at which to change the value
 * @returns 0 if the index was out-of-bounds, 1 otherwise
 */
int vec_set(VECTOR* vector, char value, size_t index);

/**
 * Pushes a value to the back of a vector.
 *
 * @param vector: the vector to modify
 * @param value: the value to push
 */
void vec_push_back(VECTOR* vector, char value);

/**
 * Pops and returns the last value of a vector.
 * 
 * @param vector: the vector to modify
 * @returns the popped value, or the null character if the vector is empty
 */
char vec_pop_back(VECTOR* vector);

/**
 * Inserts a value into a given index in a vector.
 *
 * @param vector: the vector to modify
 * @param value: the value to push
 * @param index: the index in which to push the value
 * @returns 0 if the index was out-of-bounds (greater than size()), 1 otherwise
 */
int vec_insert(VECTOR* vector, char value, size_t index);

/**
 * Removes the element at a given index of a vector.
 *
 * @param vector: the vector to modify
 * @param index: the index to pop from the vector
 * @returns the removed value, or the null character if the index was out-of-bounds.
 */
char vec_remove(VECTOR* vector, size_t index);

#endif