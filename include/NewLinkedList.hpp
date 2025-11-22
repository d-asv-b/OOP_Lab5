#pragma once

#include <memory>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <memory_resource>
#include <stdexcept>
#include <type_traits>
#include <iterator>
#include <utility>
#include <vector>

#pragma region LinkedListIterator
template <typename Type>
class LinkedListIterator {
private:
    Type* _pointer;
    size_t _size;
    size_t _currIdx;

public:
    // type_traits, требуются для forward_iterator
    using value_type = typename Type::elementType;
    using reference = value_type&;
    using pointer   = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    LinkedListIterator() : _pointer(nullptr), _size(0), _currIdx(0) {}

    LinkedListIterator(Type* listPtr, size_t listSize, size_t elemIdx) :
        _pointer(listPtr), _size(listSize), _currIdx(elemIdx) {}

    typename Type::elementType operator*() {
        if (this->_currIdx >= this->_size) {
            throw std::out_of_range("List index is out of range!");
        }

        return (*this->_pointer)[this->_currIdx].value;
    }

    typename Type::elementType operator->() {
        return *this;
    }

    LinkedListIterator<Type>& operator++() {
        ++this->_currIdx;
        return *this;
    }

    LinkedListIterator<Type> operator++(int) {
        LinkedListIterator<Type> temp(*this);
        ++this->_currIdx;
        return temp;
    }
    
    bool operator==(const LinkedListIterator<Type>& other) const {
        return (
            this->_currIdx == other._currIdx &&
            this->_pointer == other._pointer
        );
    }

    bool operator!=(const LinkedListIterator<Type>& other) const {
        return !(*this == other);
    }
};

// требуется для forward_iterator
namespace std {
    template <typename Type>
    struct iterator_traits<LinkedListIterator<Type>> {
        using difference_type = std::ptrdiff_t;
        using value_type = typename Type::elementType;
        using pointer = typename LinkedListIterator<Type>::pointer;
        using reference = typename LinkedListIterator<Type>::reference;
        using iterator_category = std::forward_iterator_tag;
    };
}
#pragma endregion LinkedListIterator

#pragma region LimitedUniquePtr
template <typename T>
struct PolymorphicDeleter {
    void operator()(T* ptr) {}
};
template <typename T>
using LimitedUniquePtr = std::unique_ptr<T, PolymorphicDeleter<T>>;
#pragma endregion

#pragma region LinkedList
template <typename T>
struct ListItem {
    T value;
    LimitedUniquePtr<ListItem<T>> nextItem;
};

template <typename T, typename AllocatorType>
requires std::is_default_constructible_v<T> && std::is_same_v<AllocatorType, std::pmr::polymorphic_allocator<ListItem<T>>>
class LinkedList {
private:
    LinkedListIterator<ListItem<T>> _head;
    size_t _listSize;
    AllocatorType _allocator;

public:
    using elementType = T;

    LinkedList(AllocatorType alloc = {}) : _head(nullptr), _listSize(0), _allocator(alloc) {}

    

    size_t getSize() const {
        return this->_listSize;
    }

    void pushFront(T& value) {
        LimitedUniquePtr<ListItem<T>> newItem = LimitedUniquePtr<ListItem<T>>(this->_allocator.allocate(1));
        this->_allocator.construct(newItem.get());

        newItem.get()->value = value;
        newItem.get()->nextItem = std::move(this->_head);

        this->_head = std::move(newItem);

        ++this->_listSize;
    }

    bool isEmpty() const {
        return this->getSize() == 0;
    }

    LinkedListIterator<LinkedList<T, AllocatorType>> begin() {
        return LinkedListIterator<LinkedList<T, AllocatorType>>(this, this->_listSize, 0);
    }

    LinkedListIterator<LinkedList<T, AllocatorType>> end() {
        return LinkedListIterator<LinkedList<T, AllocatorType>>(this, this->_listSize, this->_listSize);
    }
};
#pragma endregion 
