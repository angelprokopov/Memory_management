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
	void retain(void* ptr); // function to retain the ownership of a block
	int getReferenceCount(void* ptr) const; // function to get the reference count
	void defragment(); // compact memory function
	void displayMemoryState() const; // debugging memory state

private:
	static const size_t POOL_SIZE = 1024; // total size of the memory pool
	char memoryPool[POOL_SIZE]; // memory pool
	bool allocated[POOL_SIZE]; // allocation tracker
	std::unordered_map<void*, size_t> allocations; // tracking the allocations (for debugging purposes)
	std::unordered_map<void*, int> referenceCounts; // Tracker for reference count
	std::mutex allocatorMutex; // Mutex for thread safety
};


#endif // !MEMORY_ALLOCATOR_H
