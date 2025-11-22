#include <gtest/gtest.h>
#include "../include/MemoryResource.hpp"

// Тесты для MemoryResource - управление памятью
class MemoryResourceTest : public ::testing::Test {
protected:
    MemoryResource mres;
};

TEST_F(MemoryResourceTest, AllocateSingleBlock) {
    void* ptr1 = mres.do_allocate(100, 1);
    EXPECT_NE(ptr1, nullptr);
}

TEST_F(MemoryResourceTest, AllocateMultipleBlocks) {
    void* ptr1 = mres.do_allocate(100, 1);
    void* ptr2 = mres.do_allocate(100, 1);
    void* ptr3 = mres.do_allocate(100, 1);
    
    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr3, nullptr);
    EXPECT_NE(ptr1, ptr2);
    EXPECT_NE(ptr2, ptr3);
}

TEST_F(MemoryResourceTest, DeallocateAndReallocate) {
    void* ptr1 = mres.do_allocate(100, 1);
    void* ptr2 = mres.do_allocate(100, 1);
    
    mres.do_deallocate(ptr1, 100, 1);
    
    // После deallocate первого блока, новый блок должен занять его место
    void* ptr3 = mres.do_allocate(100, 1);
    EXPECT_EQ(ptr1, ptr3);
}

TEST_F(MemoryResourceTest, ThrowOnExceedBufferSize) {
    EXPECT_THROW(mres.do_allocate(5001, 1), std::bad_alloc);
}

TEST_F(MemoryResourceTest, AllocateAfterPartialDeallocation) {
    void* ptr1 = mres.do_allocate(200, 1);
    void* ptr2 = mres.do_allocate(200, 1);
    void* ptr3 = mres.do_allocate(200, 1);
    
    // Освобождаем средний блок
    mres.do_deallocate(ptr2, 200, 1);
    
    // Должны выделить блок в освобожденном месте
    void* ptr4 = mres.do_allocate(100, 1);
    EXPECT_EQ(ptr2, ptr4);
}

TEST_F(MemoryResourceTest, MultipleAllocationsDeallocationsCycles) {
    // Первый цикл
    void* ptr1 = mres.do_allocate(100, 1);
    mres.do_deallocate(ptr1, 100, 1);
    
    // Второй цикл
    void* ptr2 = mres.do_allocate(100, 1);
    EXPECT_EQ(ptr1, ptr2);
    
    mres.do_deallocate(ptr2, 100, 1);
    
    // Третий цикл
    void* ptr3 = mres.do_allocate(100, 1);
    EXPECT_EQ(ptr1, ptr3);
}

TEST_F(MemoryResourceTest, MemoryResourceEquality) {
    MemoryResource other;
    
    EXPECT_TRUE(mres.do_is_equal(mres));
    EXPECT_FALSE(mres.do_is_equal(other));
}

TEST_F(MemoryResourceTest, LargeAllocation) {
    void* ptr = mres.do_allocate(4900, 1);
    EXPECT_NE(ptr, nullptr);
    
    // Оставляем 100 байт - должны бросить ошибку при попытке выделить 101
    EXPECT_THROW(mres.do_allocate(101, 1), std::bad_alloc);
}

TEST_F(MemoryResourceTest, DeallocateUnallocatedMemory) {
    MemoryResource mres;
    
    // Попытка deallocate памяти, которая не была выделена
    char dummy[100];
    EXPECT_THROW(mres.do_deallocate(&dummy, 100, 1), std::logic_error);
}
