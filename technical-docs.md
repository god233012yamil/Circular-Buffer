# Circular Buffer Technical Documentation

## Implementation Overview

The circular buffer is implemented as a dynamic array with two indices: head and tail. The implementation uses a struct-based approach with thread-safety considerations for embedded systems.

### Core Structure

```c
typedef struct {
    BUFFER_TYPE* buffer;   // Dynamic buffer
    volatile size_t head;  // Write index
    volatile size_t tail;  // Read index
    size_t size;          // Buffer capacity
    bool overwrite;       // Overwrite mode flag
} CircularBuffer;
```

Key aspects:
- `head`: Points to next write position
- `tail`: Points to next read position
- `volatile` qualifiers ensure thread safety in interrupt contexts
- Dynamic allocation allows runtime size configuration

## Memory Management

### Initialization
```c
int CircularBuffer_Init(CircularBuffer* cb, size_t size, bool overwrite) {
    cb->buffer = (BUFFER_TYPE*)malloc(size * sizeof(BUFFER_TYPE));
    if (cb->buffer == NULL) return BUFFER_ERROR;
    
    cb->head = cb->tail = 0;
    cb->size = size;
    cb->overwrite = overwrite;
    return BUFFER_OK;
}
```

Key operations:
1. Dynamic memory allocation based on specified size
2. Initialization of head/tail indices to 0
3. Configuration of overwrite mode
4. Error handling for allocation failures

## Data Operations

### Write Operation (Put)
```c
int CircularBuffer_Put(CircularBuffer* cb, BUFFER_TYPE data) {
    size_t nextHead = (cb->head + 1) % cb->size;
    
    if (nextHead == cb->tail) {
        if (cb->overwrite) {
            cb->tail = (cb->tail + 1) % cb->size;
        } else {
            return BUFFER_FULL;
        }
    }
    
    cb->buffer[cb->head] = data;
    cb->head = nextHead;
    return BUFFER_OK;
}
```

Write algorithm:
1. Calculate next head position using modulo arithmetic
2. Check for buffer full condition (next_head == tail)
3. Handle overwrite mode if enabled
4. Store data and update head index

### Read Operation (Get)
```c
int CircularBuffer_Get(CircularBuffer* cb, BUFFER_TYPE* data) {
    if (cb->head == cb->tail) return BUFFER_EMPTY;
    
    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->size;
    return BUFFER_OK;
}
```

Read algorithm:
1. Check for empty buffer condition (head == tail)
2. Read data from tail position
3. Update tail index using modulo arithmetic

## Thread Safety Considerations

### Interrupt Context Usage
The implementation is ISR-safe due to:
1. Volatile indices preventing compiler optimization
2. Atomic operations for index updates
3. Single-producer/single-consumer design

Example in UART ISR:
```c
ISR(USART0_RXC_vect) {
    uint8_t data = USART0.RXDATAL;
    CircularBuffer_Put(&rxBuffer, data);
}
```

### Race Condition Prevention
- Head index modified only by producer (writer)
- Tail index modified only by consumer (reader)
- No index sharing between operations

## Buffer State Management

### Size Calculation
```c
size_t CircularBuffer_Size(CircularBuffer* cb) {
    if (cb->head >= cb->tail) {
        return cb->head - cb->tail;
    } else {
        return cb->size - (cb->tail - cb->head);
    }
}
```

Size calculation considers:
1. Normal case (head ≥ tail)
2. Wrap-around case (head < tail)

### Full/Empty Detection
```c
bool CircularBuffer_IsFull(CircularBuffer* cb) {
    return ((cb->head + 1) % cb->size) == cb->tail;
}

bool CircularBuffer_IsEmpty(CircularBuffer* cb) {
    return cb->head == cb->tail;
}
```

State detection:
- Full: Next head position equals tail
- Empty: Head equals tail

## Performance Characteristics

### Time Complexity
- Put operation: O(1)
- Get operation: O(1)
- Size calculation: O(1)
- State checks: O(1)

### Memory Usage
- Buffer size: n * sizeof(BUFFER_TYPE)
- Overhead: sizeof(CircularBuffer) struct
- No dynamic allocation during operations

## Error Handling

Status codes:
```c
#define BUFFER_OK     0  // Operation successful
#define BUFFER_FULL   1  // Buffer full (non-overwrite mode)
#define BUFFER_EMPTY  2  // No data available
#define BUFFER_ERROR  3  // Initialization failure
```

Error handling strategy:
1. Initialization failures return BUFFER_ERROR
2. Full buffer in non-overwrite mode returns BUFFER_FULL
3. Empty buffer reads return BUFFER_EMPTY
4. Successful operations return BUFFER_OK

## UART Integration Example

The implementation integrates with UART via:
1. ISR-based data reception
2. Non-blocking circular buffer operations
3. Configurable buffer size for different baud rates
4. Overflow protection with status checking

For detailed UART usage, see `UART Data Reception Using Circular Buffer.c`.
