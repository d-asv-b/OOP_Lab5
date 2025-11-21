#include "../include/MemoryResource.hpp"
#include <algorithm>
#include <cstddef>
#include <new>
#include <stdexcept>

MemoryResource::MemoryResource(size_t bufferSize) : _bufSize(bufferSize) {
    this->_usedMemBlocks = std::list<BufferBlock>();
    this->_memBuffer = new char[this->_bufSize]();
}

MemoryResource::~MemoryResource() {
    this->_usedMemBlocks.clear();
    delete this->_memBuffer;
}

void* MemoryResource::do_allocate(size_t allocationSize, size_t alingment) {
    if (allocationSize > this->_bufSize) {
        throw std::bad_alloc();
    }

    size_t allocOffset = 0;

    for (auto block : this->_usedMemBlocks) {
        if (
            allocOffset + allocationSize <= block.memOffset ||
            allocOffset >= block.memOffset + block.memSize 
        ) {
        }
        else {
            allocOffset += (block.memOffset + block.memSize) + 1;
        }
    }

    if (allocOffset + allocationSize > this->_bufSize) {
        throw std::bad_alloc();
    }

    this->_usedMemBlocks.emplace_back(allocOffset, allocationSize);
    this->_usedMemBlocks.sort(
        [](const BufferBlock& first, const BufferBlock& second) {
            return first.memOffset <= second.memOffset;
        }
    );

    return this->_memBuffer + allocOffset;
}

void MemoryResource::do_deallocate(void *ptr, size_t deallocationSize, size_t alignment) {
    for (auto blockIt = this->_usedMemBlocks.begin(); blockIt != this->_usedMemBlocks.end();) {
        if (ptr == this->_memBuffer + blockIt->memOffset) {
            this->_usedMemBlocks.erase(blockIt);
            return;
        }

        ++blockIt;
    }

    throw std::logic_error("Attempt to deallocate unallocated memory");
}

bool MemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}