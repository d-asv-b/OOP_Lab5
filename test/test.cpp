#include <gtest/gtest.h>
#include "../include/LinkedList.hpp"
#include "../include/MemoryResource.hpp"

// Тесты MemoryResource
TEST(MemoryResourceTest, AllocateSingleBlock) {
    MemoryResource mres(1000);
    
    void* ptr1 = mres.do_allocate(100, 1);
    EXPECT_NE(ptr1, nullptr);
}

TEST(MemoryResourceTest, AllocateMultipleBlocks) {
    MemoryResource mres(1000);
    
    void* ptr1 = mres.do_allocate(100, 1);
    void* ptr2 = mres.do_allocate(100, 1);
    void* ptr3 = mres.do_allocate(100, 1);
    
    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr3, nullptr);
    EXPECT_NE(ptr1, ptr2);
    EXPECT_NE(ptr2, ptr3);
}

TEST(MemoryResourceTest, DeallocateAndReallocate) {
    MemoryResource mres(1000);
    
    void* ptr1 = mres.do_allocate(100, 1);
    void* ptr2 = mres.do_allocate(100, 1);
    
    mres.do_deallocate(ptr1, 100, 1);
    
    // После deallocate первого блока, новый блок должен занять его место
    void* ptr3 = mres.do_allocate(100, 1);
    EXPECT_EQ(ptr1, ptr3);
}

TEST(MemoryResourceTest, ThrowOnExceedBufferSize) {
    MemoryResource mres(100);
    
    EXPECT_THROW(mres.do_allocate(150, 1), std::bad_alloc);
}

TEST(MemoryResourceTest, ThrowOnFragmentation) {
    MemoryResource mres(300);
    
    void* ptr1 = mres.do_allocate(100, 1);
    void* ptr2 = mres.do_allocate(100, 1);
    void* ptr3 = mres.do_allocate(100, 1);
    
    // Используем всю память
    EXPECT_THROW(mres.do_allocate(1, 1), std::bad_alloc);
}

// Тесты LinkedList
TEST(LinkedListTest, ConstructorWithSize) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list(5, polyAlloc);
    
    EXPECT_EQ(list.getSize(), 5);
}

TEST(LinkedListTest, ConstructorWithInitializerList) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3, 4, 5}, polyAlloc);
    
    EXPECT_EQ(list.getSize(), 5);
}

TEST(LinkedListTest, OperatorIndexAccess) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3, 4, 5}, polyAlloc);
    
    EXPECT_EQ(list[0].value, 1);
    EXPECT_EQ(list[1].value, 2);
    EXPECT_EQ(list[2].value, 3);
    EXPECT_EQ(list[3].value, 4);
    EXPECT_EQ(list[4].value, 5);
}

TEST(LinkedListTest, OperatorIndexThrowOutOfRange) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3}, polyAlloc);
    
    EXPECT_THROW(list[5], std::out_of_range);
}

TEST(LinkedListTest, ModifyElement) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3}, polyAlloc);
    
    list[0].value = 10;
    list[1].value = 20;
    list[2].value = 30;
    
    EXPECT_EQ(list[0].value, 10);
    EXPECT_EQ(list[1].value, 20);
    EXPECT_EQ(list[2].value, 30);
}

TEST(LinkedListTest, EmptyList) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list(polyAlloc);
    
    EXPECT_EQ(list.getSize(), 0);
}

TEST(LinkedListTest, TraversalLoop) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3, 4, 5}, polyAlloc);
    
    int sum = 0;
    for (size_t i = 0; i < list.getSize(); ++i) {
        sum += list[i].value;
    }
    
    EXPECT_EQ(sum, 15);
}

// Тесты для forward_iterator_tag
TEST(IteratorTraitsTest, ForwardIteratorCategory) {
    using ListType = LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>>;
    using IteratorType = LinkedListIterator<ListType>;
    
    // Проверяем, что iterator_category является forward_iterator_tag
    static_assert(
        std::is_same_v<
            typename std::iterator_traits<IteratorType>::iterator_category,
            std::forward_iterator_tag
        >,
        "LinkedListIterator should be forward_iterator_tag"
    );
}

TEST(IteratorTraitsTest, IteratorTraitsTypes) {
    using ListType = LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>>;
    using IteratorType = LinkedListIterator<ListType>;
    using Traits = std::iterator_traits<IteratorType>;
    
    // Проверяем типы traits
    static_assert(std::is_same_v<Traits::value_type, int>);
    static_assert(std::is_same_v<Traits::difference_type, std::ptrdiff_t>);
}

TEST(IteratorTest, DefaultConstructor) {
    using ListType = LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>>;
    using IteratorType = LinkedListIterator<ListType>;
    
    // Конструктор по умолчанию (требуется для forward_iterator)
    IteratorType it;
    // Проверяем что итератор создан и может быть использован
    EXPECT_NO_THROW({
        auto copy = it;  // Проверяем копирование
    });
}

TEST(IteratorTest, PrefixIncrement) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3}, polyAlloc);
    
    auto it = list.begin();
    EXPECT_EQ(*it, 1);  // operator* возвращает значение элемента
    
    // Префиксный инкремент
    auto& result = ++it;
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(&result, &it);  // Должен возвращать ссылку на себя
}

TEST(IteratorTest, PostfixIncrement) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3}, polyAlloc);
    
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    
    // Постфиксный инкремент - должен вернуть копию ДО инкремента
    auto old = it++;
    EXPECT_EQ(*old, 1);
    EXPECT_EQ(*it, 2);
}

TEST(IteratorTest, EqualityComparison) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3}, polyAlloc);
    
    auto it1 = list.begin();
    auto it2 = list.begin();
    auto it3 = it1;
    
    // Оператор == (требуется для forward_iterator)
    EXPECT_TRUE(it1 == it2);
    EXPECT_TRUE(it1 == it3);
}

TEST(IteratorTest, InequalityComparison) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3}, polyAlloc);
    
    auto it1 = list.begin();
    auto it2 = list.begin();
    ++it2;
    
    EXPECT_TRUE(it1 != it2);
    EXPECT_FALSE(it1 == it2);
}

TEST(IteratorTest, RangeBasedForLoop) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({10, 20, 30}, polyAlloc);
    
    int sum = 0;
    // Range-based for loop работает с forward_iterator
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }
    
    EXPECT_EQ(sum, 60);
}

TEST(IteratorTest, MultipleIterators) {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);
    
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({1, 2, 3, 4, 5}, polyAlloc);
    
    // Forward iterator позволяет использовать несколько независимых итераторов (многопроходность)
    auto it1 = list.begin();
    auto it2 = list.begin();
    
    ++it1;  // it1 указывает на 2
    ++it1;  // it1 указывает на 3
    
    // it2 по-прежнему указывает на 1
    EXPECT_EQ(*it1, 3);
    EXPECT_EQ(*it2, 1);
}
