#pragma once
#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <mutex>
#include <chrono>

class MemoryAllocator
{
public:
	MemoryAllocator();
	~MemoryAllocator();
	
	// Basic memory management functions
	void* allocate(size_t size); // function for allocating the memory 
	void deallocate(void* ptr); // function for deallocating the memory
	
	// Advanced memory management functions
	void* allocateAligned(size_t size, size_t alignment);
	void retain(void* ptr);
	int getReferenceCount(void* ptr) const;
	void defragment();
	void displayMemoryState() const;

private:
	static const size_t POOL_SIZE = 1024; // total size of the memory pool
	char memoryPool[POOL_SIZE];
	bool allocated[POOL_SIZE];
	std::unordered_map<void*, size_t> allocations;
	std::unordered_map<void*, int>referenceCounts;
	std::mutex allocatorMutex;
};


#endif // !MEMORY_ALLOCATOR_H
