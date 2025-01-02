#include "circular_buffer.h"
#include <stdlib.h>
#include <stdio.h>

// Initialize Circular Buffer
int CircularBuffer_Init(CircularBuffer* cb, size_t size, bool overwrite) {
    cb->buffer = (BUFFER_TYPE*)malloc(size * sizeof(BUFFER_TYPE));
    if (cb->buffer == NULL) return BUFFER_ERROR; // Memory allocation failed

    cb->head = 0;
    cb->tail = 0;
    cb->size = size;
    cb->overwrite = overwrite;
    return BUFFER_OK;
}

// Free allocated memory
void CircularBuffer_Free(CircularBuffer* cb) {
    free(cb->buffer);
    cb->buffer = NULL;
}

// Add data to buffer
int CircularBuffer_Put(CircularBuffer* cb, BUFFER_TYPE data) {
    size_t nextHead = (cb->head + 1) % cb->size;

    if (nextHead == cb->tail) {
        if (cb->overwrite) {
            cb->tail = (cb->tail + 1) % cb->size; // Overwrite oldest data
        } else {
            return BUFFER_FULL; // Buffer full
        }
    }

    cb->buffer[cb->head] = data;
    cb->head = nextHead;
    return BUFFER_OK;
}

// Retrieve data from buffer
int CircularBuffer_Get(CircularBuffer* cb, BUFFER_TYPE* data) {
    if (cb->head == cb->tail) return BUFFER_EMPTY; // Buffer empty

    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->size;
    return BUFFER_OK;
}

// Check if buffer is empty
bool CircularBuffer_IsEmpty(CircularBuffer* cb) {
    return cb->head == cb->tail;
}

// Check if buffer is full
bool CircularBuffer_IsFull(CircularBuffer* cb) {
    return ((cb->head + 1) % cb->size) == cb->tail;
}

// Get size of buffer
size_t CircularBuffer_Size(CircularBuffer* cb) {
    if (cb->head >= cb->tail) {
        return cb->head - cb->tail;
    } else {
        return cb->size - (cb->tail - cb->head);
    }
}

// Peek at next data without removing it
bool CircularBuffer_Peek(CircularBuffer* cb, BUFFER_TYPE* data) {
    if (cb->head == cb->tail) return false; // Buffer empty
    *data = cb->buffer[cb->tail];
    return true;
}

// Debug buffer state
void CircularBuffer_Debug(CircularBuffer* cb) {
    printf("Head: %zu, Tail: %zu, Size: %zu\n", cb->head, cb->tail, CircularBuffer_Size(cb));
}
