# Advanced Memory Management System in C++

This project implements an advanced memory allocator in C++ with features like memory alignment, reference counting, leak detection, defragmentation, and performance metrics. It provides a deeper understanding of memory management concepts and practices.

## Features
1. **Custom Memory Pool**: A fixed-size block of memory for efficient allocation and deallocation.
2. **Leak Detection**: Reports unfreed memory blocks at the program's end.
3. **Memory Alignment**: Ensures allocated memory starts at specified byte boundaries.
4. **Reference Counting**: Tracks ownership of memory blocks and prevents premature deallocation.
5. **Defragmentation**: Reduces fragmentation by compacting the memory pool.
6. **Performance Metrics**: Measures and logs allocation/deallocation times.

## Code Structure
- `MemoryAllocator.h`: Defines the memory allocator class.
- `MemoryAllocator.cpp`: Implements allocation, deallocation, and advanced features. Also demonstrates the usage and tests the allocator

## Getting Started

### Prerequisites
- A C++17-compatible compiler (e.g., GCC, Clang, MSVC).
