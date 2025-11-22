#include <gtest/gtest.h>
#include "../include/LinkedList.hpp"
#include "../include/MemoryResource.hpp"

// Тесты операций LinkedList (push, pop, итератор)
class LinkedListOperationsTest : public ::testing::Test {
protected:
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc{&mres};
    
    using ListType = LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>>;
};

// ============ Тесты для pushFront ============
TEST_F(LinkedListOperationsTest, PushFrontToEmpty) {
    ListType list(polyAlloc);
    int value = 42;
    
    list.pushFront(value);
    
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list[0].value, 42);
}

TEST_F(LinkedListOperationsTest, PushFrontMultiple) {
    ListType list(polyAlloc);
    
    int v1 = 10, v2 = 20, v3 = 30;
    list.pushFront(v1);
    list.pushFront(v2);
    list.pushFront(v3);
    
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list[0].value, 30);  // v3 находится в начале
    EXPECT_EQ(list[1].value, 20);
    EXPECT_EQ(list[2].value, 10);
}

TEST_F(LinkedListOperationsTest, PushFrontMaintainsOrder) {
    ListType list(polyAlloc);
    
    for (int i = 1; i <= 5; ++i) {
        list.pushFront(i);
    }
    
    EXPECT_EQ(list[0].value, 5);
    EXPECT_EQ(list[1].value, 4);
    EXPECT_EQ(list[2].value, 3);
    EXPECT_EQ(list[3].value, 2);
    EXPECT_EQ(list[4].value, 1);
}

// ============ Тесты для pushBack ============
TEST_F(LinkedListOperationsTest, PushBackToEmpty) {
    ListType list(polyAlloc);
    int value = 42;
    
    list.pushBack(value);
    
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list[0].value, 42);
}

TEST_F(LinkedListOperationsTest, PushBackMultiple) {
    ListType list(polyAlloc);
    
    int v1 = 10, v2 = 20, v3 = 30;
    list.pushBack(v1);
    list.pushBack(v2);
    list.pushBack(v3);
    
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list[0].value, 10);
    EXPECT_EQ(list[1].value, 20);
    EXPECT_EQ(list[2].value, 30);
}

TEST_F(LinkedListOperationsTest, PushBackMaintainsOrder) {
    ListType list(polyAlloc);
    
    for (int i = 1; i <= 5; ++i) {
        list.pushBack(i);
    }
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(list[i].value, i + 1);
    }
}

// ============ Тесты для popFront ============
TEST_F(LinkedListOperationsTest, PopFrontSingleElement) {
    ListType list({42}, polyAlloc);
    
    int value = list.popFront();
    EXPECT_EQ(value, 42);
    EXPECT_EQ(list.getSize(), 0);
}

TEST_F(LinkedListOperationsTest, PopFrontMultiple) {
    ListType list({10, 20, 30}, polyAlloc);
    
    int v1 = list.popFront();
    EXPECT_EQ(v1, 10);
    EXPECT_EQ(list.getSize(), 2);
    
    int v2 = list.popFront();
    EXPECT_EQ(v2, 20);
    EXPECT_EQ(list.getSize(), 1);
    
    int v3 = list.popFront();
    EXPECT_EQ(v3, 30);
    EXPECT_EQ(list.getSize(), 0);
}

TEST_F(LinkedListOperationsTest, PopFrontAllElements) {
    ListType list({1, 2, 3, 4, 5}, polyAlloc);
    
    for (int i = 1; i <= 5; ++i) {
        int val = list.popFront();
        EXPECT_EQ(val, i);
    }
    
    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}

// ============ Тесты для popBack ============
TEST_F(LinkedListOperationsTest, PopBackSingleElement) {
    ListType list({42}, polyAlloc);
    
    int value = list.popBack();
    EXPECT_EQ(value, 42);
    EXPECT_EQ(list.getSize(), 0);
}

TEST_F(LinkedListOperationsTest, PopBackMultiple) {
    ListType list({10, 20, 30}, polyAlloc);
    
    int v1 = list.popBack();
    EXPECT_EQ(v1, 30);
    EXPECT_EQ(list.getSize(), 2);
    
    int v2 = list.popBack();
    EXPECT_EQ(v2, 20);
    EXPECT_EQ(list.getSize(), 1);
    
    int v3 = list.popBack();
    EXPECT_EQ(v3, 10);
    EXPECT_EQ(list.getSize(), 0);
}

TEST_F(LinkedListOperationsTest, PopBackAllElements) {
    ListType list({1, 2, 3, 4, 5}, polyAlloc);
    
    for (int i = 5; i >= 1; --i) {
        int val = list.popBack();
        EXPECT_EQ(val, i);
    }
    
    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}

// ============ Комбинированные тесты push/pop ============
TEST_F(LinkedListOperationsTest, PushFrontAndPopBack) {
    ListType list(polyAlloc);
    
    int v1 = 10, v2 = 20, v3 = 30;
    list.pushFront(v1);
    list.pushFront(v2);
    list.pushFront(v3);
    
    EXPECT_EQ(list.getSize(), 3);
    
    int val = list.popBack();
    EXPECT_EQ(val, 10);
    EXPECT_EQ(list.getSize(), 2);
}

TEST_F(LinkedListOperationsTest, PushBackAndPopFront) {
    ListType list(polyAlloc);
    
    int v1 = 10, v2 = 20, v3 = 30;
    list.pushBack(v1);
    list.pushBack(v2);
    list.pushBack(v3);
    
    EXPECT_EQ(list.getSize(), 3);
    
    int val = list.popFront();
    EXPECT_EQ(val, 10);
    EXPECT_EQ(list.getSize(), 2);
}

TEST_F(LinkedListOperationsTest, AlternatingPushPopOperations) {
    ListType list(polyAlloc);
    
    // push-pop-push-pop...
    int v1 = 100;
    list.pushFront(v1);
    EXPECT_EQ(list.getSize(), 1);
    
    int p1 = list.popFront();
    EXPECT_EQ(p1, 100);
    EXPECT_EQ(list.getSize(), 0);
    
    int v2 = 200;
    list.pushBack(v2);
    EXPECT_EQ(list.getSize(), 1);
    
    int p2 = list.popBack();
    EXPECT_EQ(p2, 200);
    EXPECT_EQ(list.getSize(), 0);
}

// ============ Тесты итератора ============
TEST_F(LinkedListOperationsTest, BeginEndIterators) {
    ListType list({1, 2, 3}, polyAlloc);
    
    auto begin = list.begin();
    auto end = list.end();
    
    EXPECT_EQ(*begin, 1);
    EXPECT_NE(begin, end);
}

TEST_F(LinkedListOperationsTest, IteratorPrefix) {
    ListType list({10, 20, 30}, polyAlloc);
    
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
    
    ++it;
    EXPECT_EQ(*it, 20);
    
    ++it;
    EXPECT_EQ(*it, 30);
}

TEST_F(LinkedListOperationsTest, IteratorPostfix) {
    ListType list({10, 20, 30}, polyAlloc);
    
    auto it = list.begin();
    auto old_it = it++;
    
    EXPECT_EQ(*old_it, 10);
    EXPECT_EQ(*it, 20);
}

TEST_F(LinkedListOperationsTest, IteratorRangeBasedFor) {
    ListType list({10, 20, 30, 40, 50}, polyAlloc);
    
    int sum = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }
    
    EXPECT_EQ(sum, 150);
}

TEST_F(LinkedListOperationsTest, IteratorEquality) {
    ListType list({1, 2, 3}, polyAlloc);
    
    auto it1 = list.begin();
    auto it2 = list.begin();
    
    EXPECT_EQ(it1, it2);
    
    ++it1;
    EXPECT_NE(it1, it2);
}

TEST_F(LinkedListOperationsTest, IteratorMultiplePasses) {
    ListType list({5, 10, 15}, polyAlloc);
    
    // Первый проход
    int sum1 = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum1 += *it;
    }
    
    // Второй проход - должен давать то же самое
    int sum2 = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum2 += *it;
    }
    
    EXPECT_EQ(sum1, 30);
    EXPECT_EQ(sum2, 30);
}

TEST_F(LinkedListOperationsTest, IteratorCopying) {
    ListType list({1, 2, 3, 4, 5}, polyAlloc);
    
    auto it1 = list.begin();
    auto it2 = it1;  // Копируем итератор
    
    ++it1;
    
    // it2 должен остаться на первом элементе
    EXPECT_EQ(*it1, 2);
    EXPECT_EQ(*it2, 1);
}

TEST_F(LinkedListOperationsTest, IteratorForwardIteratorTag) {
    using IteratorType = decltype(std::declval<ListType>().begin());
    using Traits = std::iterator_traits<IteratorType>;
    
    static_assert(
        std::is_same_v<Traits::iterator_category, std::forward_iterator_tag>,
        "Iterator should be forward_iterator"
    );
}

// ============ Сложные сценарии ============
TEST_F(LinkedListOperationsTest, BuildListWithPushBack) {
    ListType list(polyAlloc);
    
    for (int i = 1; i <= 10; ++i) {
        list.pushBack(i);
    }
    
    EXPECT_EQ(list.getSize(), 10);
    
    int sum = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }
    
    EXPECT_EQ(sum, 55);  // 1+2+3+...+10
}

TEST_F(LinkedListOperationsTest, BuildListWithPushFrontThenTraverse) {
    ListType list(polyAlloc);
    
    for (int i = 1; i <= 5; ++i) {
        list.pushFront(i);
    }
    
    std::vector<int> values;
    for (auto it = list.begin(); it != list.end(); ++it) {
        values.push_back(*it);
    }
    
    EXPECT_EQ(values[0], 5);
    EXPECT_EQ(values[1], 4);
    EXPECT_EQ(values[2], 3);
    EXPECT_EQ(values[3], 2);
    EXPECT_EQ(values[4], 1);
}

TEST_F(LinkedListOperationsTest, LongSequenceOfOperations) {
    ListType list(polyAlloc);
    
    // Добавляем элементы
    for (int i = 0; i < 5; ++i) {
        list.pushBack(i);
    }
    
    EXPECT_EQ(list.getSize(), 5);
    
    // Удаляем с начала
    list.popFront();
    EXPECT_EQ(list.getSize(), 4);
    
    // Добавляем спереди
    int val = 100;
    list.pushFront(val);
    EXPECT_EQ(list.getSize(), 5);
    
    // Проверяем
    EXPECT_EQ(list[0].value, 100);
    EXPECT_EQ(list[1].value, 1);
    EXPECT_EQ(list[4].value, 4);
}
