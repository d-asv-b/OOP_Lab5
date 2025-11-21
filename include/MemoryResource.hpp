#pragma once

#include <memory_resource>
#include <list>

class MemoryResource : public std::pmr::memory_resource {
    struct BufferBlock {
        BufferBlock() {};
        BufferBlock(size_t memoryOffset, size_t memorySize) : memOffset(memoryOffset), memSize(memorySize) {}
        
        size_t memOffset{0};
        size_t memSize{0};
    };

private:
    size_t _bufSize = 0;
    char* _memBuffer = 0;
    std::list<BufferBlock> _usedMemBlocks;

public:
    MemoryResource(size_t = 5000);
    ~MemoryResource();

    void* do_allocate(size_t, size_t) override;
    void do_deallocate(void*, size_t, size_t) override;
    bool do_is_equal(const std::pmr::memory_resource&) const noexcept override;
};