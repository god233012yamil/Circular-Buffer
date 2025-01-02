# Embedded Circular Buffer Implementation

A robust, thread-safe circular buffer implementation in C, designed for embedded systems with UART/USART communication. Features include configurable buffer size, optional overwrite mode, and comprehensive error handling.

## Features

- Dynamic buffer allocation with configurable size
- Thread-safe implementation with volatile indices
- Configurable data type (default: uint8_t)
- Optional overwrite mode for full buffers
- UART/USART integration example for AVR microcontrollers
- Complete error handling and status codes
- Peek functionality without data removal
- Buffer state debugging capabilities

## Installation

1. Clone the repository:
```bash
git clone https://github.com/god233012yamil/Circular-Buffer/circular_buffer.git
```

2. Include the header file in your project:
```c
#include "circular_buffer.h"
```

## Usage

### Basic Buffer Operations

```c
CircularBuffer buffer;
uint8_t data;

// Initialize buffer (size: 64 bytes, overwrite: disabled)
CircularBuffer_Init(&buffer, 64, false);

// Add data
CircularBuffer_Put(&buffer, 'A');

// Retrieve data
CircularBuffer_Get(&buffer, &data);

// Free buffer when done
CircularBuffer_Free(&buffer);
```

### UART Integration Example

```c
// Initialize UART with circular buffer
volatile CircularBuffer rxBuffer;
CircularBuffer_Init(&rxBuffer, 64, false);
USART_Init(9600);

// In UART ISR
ISR(USART0_RXC_vect) {
    uint8_t data = USART0.RXDATAL;
    CircularBuffer_Put(&rxBuffer, data);
}
```

## API Reference

### Initialization and Cleanup

- `CircularBuffer_Init(CircularBuffer* cb, size_t size, bool overwrite)`
  - Initializes buffer with specified size and overwrite mode
  - Returns: BUFFER_OK or BUFFER_ERROR

- `CircularBuffer_Free(CircularBuffer* cb)`
  - Frees allocated buffer memory

### Data Operations

- `CircularBuffer_Put(CircularBuffer* cb, BUFFER_TYPE data)`
  - Adds data to buffer
  - Returns: BUFFER_OK or BUFFER_FULL

- `CircularBuffer_Get(CircularBuffer* cb, BUFFER_TYPE* data)`
  - Retrieves and removes data
  - Returns: BUFFER_OK or BUFFER_EMPTY

- `CircularBuffer_Peek(CircularBuffer* cb, BUFFER_TYPE* data)`
  - Views next data without removal
  - Returns: true if data available

### Status Functions

- `CircularBuffer_IsEmpty(CircularBuffer* cb)`
- `CircularBuffer_IsFull(CircularBuffer* cb)`
- `CircularBuffer_Size(CircularBuffer* cb)`
- `CircularBuffer_Debug(CircularBuffer* cb)`

## Configuration

Modify buffer configuration in `circular_buffer.h`:

```c
// Change buffer data type
#define BUFFER_TYPE uint8_t

// Status codes
#define BUFFER_OK     0
#define BUFFER_FULL   1
#define BUFFER_EMPTY  2
#define BUFFER_ERROR  3
```

## Testing

Run the example program (`circular_buffer_example.c`) to test basic functionality:

```bash
gcc circular_buffer_example.c circular_buffer.c -o buffer_test
./buffer_test
```

## License

[Add your chosen license here]

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
