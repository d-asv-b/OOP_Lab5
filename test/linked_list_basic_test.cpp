#include <gtest/gtest.h>
#include "../include/LinkedList.hpp"
#include "../include/MemoryResource.hpp"

// Тесты базовых операций LinkedList
class LinkedListBasicTest : public ::testing::Test {
protected:
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc{&mres};
    
    using ListType = LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>>;
};

TEST_F(LinkedListBasicTest, ConstructorEmpty) {
    ListType list(polyAlloc);
    
    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}

TEST_F(LinkedListBasicTest, ConstructorWithSize) {
    ListType list(5, polyAlloc);
    
    EXPECT_EQ(list.getSize(), 5);
    EXPECT_FALSE(list.isEmpty());
}

TEST_F(LinkedListBasicTest, ConstructorWithInitializerList) {
    ListType list({1, 2, 3, 4, 5}, polyAlloc);
    
    EXPECT_EQ(list.getSize(), 5);
    EXPECT_EQ(list[0].value, 1);
    EXPECT_EQ(list[4].value, 5);
}

TEST_F(LinkedListBasicTest, ConstructorWithInitializerListSingleElement) {
    ListType list({42}, polyAlloc);
    
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list[0].value, 42);
}

TEST_F(LinkedListBasicTest, OperatorIndexAccess) {
    ListType list({10, 20, 30, 40, 50}, polyAlloc);
    
    EXPECT_EQ(list[0].value, 10);
    EXPECT_EQ(list[1].value, 20);
    EXPECT_EQ(list[2].value, 30);
    EXPECT_EQ(list[3].value, 40);
    EXPECT_EQ(list[4].value, 50);
}

TEST_F(LinkedListBasicTest, OperatorIndexThrowOutOfRange) {
    ListType list({1, 2, 3}, polyAlloc);
    
    EXPECT_THROW(list[3], std::out_of_range);
    EXPECT_THROW(list[10], std::out_of_range);
}

TEST_F(LinkedListBasicTest, ModifyElementByIndex) {
    ListType list({1, 2, 3}, polyAlloc);
    
    list[0].value = 100;
    list[1].value = 200;
    list[2].value = 300;
    
    EXPECT_EQ(list[0].value, 100);
    EXPECT_EQ(list[1].value, 200);
    EXPECT_EQ(list[2].value, 300);
}

TEST_F(LinkedListBasicTest, GetSizeMethod) {
    ListType list1(polyAlloc);
    EXPECT_EQ(list1.getSize(), 0);
    
    ListType list2(5, polyAlloc);
    EXPECT_EQ(list2.getSize(), 5);
    
    ListType list3({1, 2, 3, 4, 5, 6}, polyAlloc);
    EXPECT_EQ(list3.getSize(), 6);
}

TEST_F(LinkedListBasicTest, IsEmptyMethod) {
    ListType emptyList(polyAlloc);
    EXPECT_TRUE(emptyList.isEmpty());
    
    ListType nonEmptyList(1, polyAlloc);
    EXPECT_FALSE(nonEmptyList.isEmpty());
}

TEST_F(LinkedListBasicTest, LargeList) {
    ListType list(100, polyAlloc);
    
    EXPECT_EQ(list.getSize(), 100);
    
    // Проверяем, что все элементы доступны
    for (size_t i = 0; i < 100; ++i) {
        list[i].value = i * 10;
    }
    
    for (size_t i = 0; i < 100; ++i) {
        EXPECT_EQ(list[i].value, i * 10);
    }
}

TEST_F(LinkedListBasicTest, TraversalByIndex) {
    ListType list({5, 10, 15, 20, 25}, polyAlloc);
    
    int sum = 0;
    for (size_t i = 0; i < list.getSize(); ++i) {
        sum += list[i].value;
    }
    
    EXPECT_EQ(sum, 75);
}

TEST_F(LinkedListBasicTest, ModifyAndTraverse) {
    ListType list({1, 2, 3}, polyAlloc);
    
    // Изменяем элементы
    for (size_t i = 0; i < list.getSize(); ++i) {
        list[i].value *= 2;
    }
    
    // Проверяем изменения
    EXPECT_EQ(list[0].value, 2);
    EXPECT_EQ(list[1].value, 4);
    EXPECT_EQ(list[2].value, 6);
}

TEST_F(LinkedListBasicTest, FirstAndLastElements) {
    ListType list({100, 200, 300, 400, 500}, polyAlloc);
    
    // Проверяем первый элемент
    EXPECT_EQ(list[0].value, 100);
    
    // Проверяем последний элемент
    EXPECT_EQ(list[list.getSize() - 1].value, 500);
}

TEST_F(LinkedListBasicTest, ManySmallListsSequentially) {
    for (int i = 1; i <= 10; ++i) {
        ListType list(i, polyAlloc);
        EXPECT_EQ(list.getSize(), i);
        
        for (int j = 0; j < i; ++j) {
            list[j].value = j;
        }
        
        for (int j = 0; j < i; ++j) {
            EXPECT_EQ(list[j].value, j);
        }
    }
}
