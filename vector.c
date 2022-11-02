#include <stdlib.h>
#include "vector.h"

/* A private function to increase a vector's capacity.
 * It will double the vector's capacity if non-zero, or set it to 1 if it is zero.
 *
 * @param vector: the vector to grow.
 */
void grow(VECTOR* vector) {
    vector->capacity = vector->capacity ? vector->capacity * 2 : 1;
    vector->data = realloc(vector->data, vector->capacity);
}


VECTOR* vec_construct_empty() {
    VECTOR* vec = malloc(sizeof(VECTOR));
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
    return vec;
}

VECTOR* vec_construct_capacity(size_t capacity) {
    VECTOR* vec = malloc(sizeof(VECTOR));
    vec->data = malloc(capacity);
    vec->size = 0;
    vec->capacity = capacity;
    return vec;
}

VECTOR* vec_construct_fill(size_t capacity, char fill) {
    VECTOR* vec = malloc(sizeof(VECTOR));
    vec->data = malloc(capacity);
    for (int i = 0; i < capacity; i++) {
        vec->data[i] = fill;
    }
    vec->size = capacity;
    vec->capacity = capacity;
    return vec;
}

void vec_free(VECTOR* vector) {
    if(vector) {
        free(vector->data);
        free(vector);
    }
}


void vec_clear(VECTOR* vector) {
    vector->size = 0;
}

int vec_empty(const VECTOR* vector) {
    return !vector->size;
}

size_t vec_size(const VECTOR* vector) {
    return vector->size;
}

const char* vec_data(const VECTOR* vector) {
    return vector->data;
}

char vec_at(const VECTOR* vector, size_t index) {
    return (index < vector->size) ? vector->data[index] : 0;
}

char vec_back(const VECTOR* vector) {
    return vector->size ? vector->data[vector->size - 1] : 0;
}

int vec_set(VECTOR* vector, char value, size_t index) {
    if (index >= vector->size) return 0;
    vector->data[index] = value;
    return 1;
}

void vec_push_back(VECTOR* vector, char value) {
    if (vector->size == vector->capacity) grow(vector);
    vector->data[vector->size++] = value;
}

char vec_pop_back(VECTOR* vector) {
    return vector->size ? vector->data[--vector->size] : 0;
}

int vec_insert(VECTOR* vector, char value, size_t index) {
    if (index > vector->size) return 0;
    if (vector->capacity == vector->size) grow(vector);
    for (size_t i = vector->size; i > index; i--) {
        vector->data[i] = vector->data[i - 1];
    }
    vector->data[index] = value;
    ++vector->size;
    return 1;
}

char vec_remove(VECTOR* vector, size_t index) {
    if (index >= vector->size) return 0;
    char result = vector->data[index];
    for (size_t i = index; i < vector->size - 1; i++) {
        vector->data[i] = vector->data[i + 1];
    }
    --vector->size;
    return result;
}