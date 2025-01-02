#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "circular_buffer.h"

// Simulated input data generator
void Simulate_Input(CircularBuffer* cb, const char* input) {
    printf("Simulating input: %s\n", input);
    while (*input) {
        if (CircularBuffer_Put(cb, *input) == BUFFER_FULL) {
            printf("Buffer Full! Discarding: %c\n", *input);
        }
        input++;
    }
}

// Simulated output processor
void Process_Buffer(CircularBuffer* cb) {
    uint8_t data;
    printf("Processing Buffer: ");
    while (CircularBuffer_Get(cb, &data) == BUFFER_OK) {
        printf("%c", data);  // Print each character
    }
    printf("\n");
}

// Main Test Program
int main(void) {
    // Declare a circular buffer instance
    CircularBuffer testBuffer;

    // Initialize the circular buffer with size 10 and overwrite disabled
    if (CircularBuffer_Init(&testBuffer, 10, false) != BUFFER_OK) {
        printf("Failed to initialize buffer!\n");
        return 1;
    }

    printf("Circular Buffer Test Started\n");

    // Test Case 1: Add data and process it
    Simulate_Input(&testBuffer, "HELLO");
    Process_Buffer(&testBuffer);


    // Test Case 2: Test buffer wrap-around
    Simulate_Input(&testBuffer, "WORLD12345");
    Process_Buffer(&testBuffer);

    // Test Case 3: Buffer overflow (without overwrite mode)
    Simulate_Input(&testBuffer, "BUFFEROVERFLOW");
    Process_Buffer(&testBuffer);

    // Test Case 4: Peek data without removing
    printf("Peeking Data: ");
    uint8_t data;
    while (CircularBuffer_Peek(&testBuffer, &data)) {
        printf("%c", data);
        CircularBuffer_Get(&testBuffer, &data); // Consume the data
    }
    printf("\n");

    // Cleanup
    CircularBuffer_Free(&testBuffer);
    printf("Circular Buffer Test Completed\n");

    return 0;
}