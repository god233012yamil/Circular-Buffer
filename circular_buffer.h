#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Status Codes
#define BUFFER_OK         0
#define BUFFER_FULL       1
#define BUFFER_EMPTY      2
#define BUFFER_ERROR      3

// Configurable Buffer Type
#ifndef BUFFER_TYPE
#define BUFFER_TYPE uint8_t
#endif

// Circular Buffer Structure
typedef struct {
    BUFFER_TYPE* buffer;   // Dynamic buffer
    volatile size_t head;  // Head index
    volatile size_t tail;  // Tail index
    size_t size;           // Maximum size
    bool overwrite;        // Overwrite mode
} CircularBuffer;

// Function Prototypes

/**
 * @brief Initializes the circular buffer.
 * @param cb Pointer to CircularBuffer structure.
 * @param size The size of the buffer.
 * @param overwrite Enable or disable overwrite mode (true or false).
 * @return Status code (BUFFER_OK, BUFFER_ERROR).
 */
int CircularBuffer_Init(CircularBuffer* cb, size_t size, bool overwrite);

/**
 * @brief Frees the dynamically allocated memory for the circular buffer.
 * @param cb Pointer to CircularBuffer structure.
 */
void CircularBuffer_Free(CircularBuffer* cb);

/**
 * @brief Adds a byte to the circular buffer.
 * @param cb Pointer to CircularBuffer structure.
 * @param data Byte to add to the buffer.
 * @return Status code (BUFFER_OK, BUFFER_FULL).
 */
int CircularBuffer_Put(CircularBuffer* cb, BUFFER_TYPE data);

/**
 * @brief Removes and returns a byte from the circular buffer.
 * @param cb Pointer to CircularBuffer structure.
 * @param data Pointer to store the removed byte.
 * @return Status code (BUFFER_OK, BUFFER_EMPTY).
 */
int CircularBuffer_Get(CircularBuffer* cb, BUFFER_TYPE* data);

/**
 * @brief Checks if the circular buffer is empty.
 * @param cb Pointer to CircularBuffer structure.
 * @return True if the buffer is empty, False otherwise.
 */
bool CircularBuffer_IsEmpty(CircularBuffer* cb);

/**
 * @brief Checks if the circular buffer is full.
 * @param cb Pointer to CircularBuffer structure.
 * @return True if the buffer is full, False otherwise.
 */
bool CircularBuffer_IsFull(CircularBuffer* cb);

/**
 * @brief Returns the number of bytes currently stored in the buffer.
 * @param cb Pointer to CircularBuffer structure.
 * @return The number of bytes in the buffer.
 */
size_t CircularBuffer_Size(CircularBuffer* cb);

/**
 * @brief Retrieves the next byte without removing it from the buffer.
 * @param cb Pointer to CircularBuffer structure.
 * @param data Pointer to store the peeked byte.
 * @return True if data was retrieved, False if the buffer is empty.
 */
bool CircularBuffer_Peek(CircularBuffer* cb, BUFFER_TYPE* data);

/**
 * @brief Prints the buffer state for debugging purposes.
 * @param cb Pointer to CircularBuffer structure.
 */
void CircularBuffer_Debug(CircularBuffer* cb);

#endif // CIRCULAR_BUFFER_H
