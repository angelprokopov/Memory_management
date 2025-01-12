#include "MemoryAllocator.h"
#include <algorithm>

MemoryAllocator::MemoryAllocator() {
    std::fill(std::begin(allocated), std::end(allocated), false);
}

MemoryAllocator::~MemoryAllocator() {
    if (!allocations.empty()) {
        std::cerr << "Memory leak is detected! " << std::endl;
        for (const auto& [ptr, size] : allocations) {
            std::cerr << "Leaked block: Address = " << ptr << ", Size = " << size << " bytes" << std::endl;
        }
    }
}

void* MemoryAllocator::allocate(size_t size) {
    std::lock_guard<std::mutex> lock(allocatorMutex);

    for (size_t i = 0; i < POOL_SIZE; i++) {
        if (!allocated[i] && i + size <= POOL_SIZE) {
            bool canAllocate = true;
            for (size_t j = 0; j < size; j++) {
                if (allocated[i + j]) {
                    canAllocate = false;
                    break;
                }
            }
            if (canAllocate) {
                for (size_t j = 0; j < size; j++) {
                    allocated[i + j] = true;
                }
                void* ptr = memoryPool + i;
                allocations[ptr] = size; // Tracking the allocations
                referenceCounts[ptr] = 1;
                return ptr;
            }
        }
    }
    return nullptr; // Memory is not available
}

void MemoryAllocator::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(allocatorMutex);


    if (referenceCounts.find(ptr) != referenceCounts.end()) {
        referenceCounts[ptr]--;

        if (referenceCounts[ptr] == 0) {
            size_t size = allocations[ptr];
            size_t offset = static_cast<char*>(ptr) - memoryPool;
            for (size_t i = 0; i < size; i++) {
                allocated[offset + i] = false;
            }
            allocations.erase(ptr); // Remove from tracking
            referenceCounts.erase(ptr);
        }
    }
    else {
        std::cout << "Error: Attempt to free unallocated memory!" << std::endl;
    }
}

void* MemoryAllocator::allocateAligned(size_t size, size_t allignment) {
    std::lock_guard<std::mutex> lock(allocatorMutex);
    for (size_t i = 0; i < POOL_SIZE; i++) {
        size_t address = reinterpret_cast<size_t>(memoryPool + i);
        if (address % allignment == 0 && !allocated[i] && i + size <= POOL_SIZE) {
            bool canAllocate = true;
            for (size_t j = 0; j < size; j++) {
                if (allocated[i + j]) {
                    canAllocate = false;
                    break;
                }
            }
            if (canAllocate) {
                for (size_t j = 0; j < size; j++) {
                    allocated[i + j] = true;
                }
                void* ptr = memoryPool + i;
                allocations[ptr] = size;
                referenceCounts[ptr] = 1;
                return ptr;
            }
        }
    }
    
    
    return nullptr;
}

void MemoryAllocator::retain(void* ptr) {
    std::lock_guard<std::mutex> lock(allocatorMutex);

    if (referenceCounts.find(ptr) != referenceCounts.end()) {
        referenceCounts[ptr]++;
    }
}

int MemoryAllocator::getReferenceCount(void* ptr) const {
    auto it = referenceCounts.find(ptr);
    return it != referenceCounts.end() ? it->second : 0;
}

void MemoryAllocator::defragment() {
    std::lock_guard<std::mutex> lock(allocatorMutex);
    
    size_t target = 0;
    for (size_t i = 0; i < POOL_SIZE; i++) {
        if (allocated[i]) {
            if (i != target) {
                memoryPool[target] = memoryPool[i];
                allocated[target] = true;
                allocated[i] = false;
            }
            target++;
        }
    }
    std::cout << "Memory is defragmented" << std::endl;
}

void MemoryAllocator::displayMemoryState() const {
    std::cout << "Memory state: ";
    for (bool block : allocated) {
        std::cout << (block ? "1" : "0");
    }
    std::cout << std::endl;
    std::cout << "Current allocations: " << allocations.size() << std::endl;
}

int main()
{
    MemoryAllocator allocator;

    // Allocating 10 bytes
    void* firstBlock = allocator.allocate(10);
    std::cout << "After allocating 10 bytes " << std::endl;
    allocator.displayMemoryState();

    // Allocating 20 bytes
    void* secondBlock = allocator.allocate(20);
    std::cout << "After allocating 20 bytes " << std::endl;
    allocator.displayMemoryState();

    // Deallocating the memory from the first block
    allocator.deallocate(firstBlock);
    std::cout << "After deallocating the first block: " << std::endl;
    allocator.displayMemoryState();

    // Testing the aligned allocation 
    void* thirdBlock = allocator.allocateAligned(16, 8);
    std::cout << "After aligned allocation: " << std::endl;
    allocator.displayMemoryState();

    // Testing the reference counting
    allocator.retain(thirdBlock);
    std::cout << "Reference count for third block of memory: " << allocator.getReferenceCount(thirdBlock) << std::endl;

    allocator.deallocate(thirdBlock);
    std::cout << "Reference count after deallocation of the third block of memory: " << allocator.getReferenceCount(thirdBlock) << std::endl;

    // Defragmenting the memory
    allocator.defragment();
    allocator.displayMemoryState();

    return 0;
}





