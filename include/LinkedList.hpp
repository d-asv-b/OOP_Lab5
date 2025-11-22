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

template <typename T>
struct PolymorphicDeleter {
    void operator()(T* ptr) {}
};
template <typename T>
using LimitedUniquePtr = std::unique_ptr<T, PolymorphicDeleter<T>>;

template <typename T>
struct ListItem {
    T value;
    LimitedUniquePtr<ListItem<T>> nextItem;
};

template <typename T, typename AllocatorType>
requires std::is_default_constructible_v<T> && std::is_same_v<AllocatorType, std::pmr::polymorphic_allocator<ListItem<T>>>
class LinkedList {
private:
    LimitedUniquePtr<ListItem<T>> _head;
    size_t _listSize;
    AllocatorType _allocator;

public:
    using elementType = T;

    LinkedList(AllocatorType alloc = {}) : _head(nullptr), _listSize(0), _allocator(alloc) {}

    LinkedList(size_t size, AllocatorType alloc = {}) : _listSize(size), _allocator(alloc) {
        LimitedUniquePtr<ListItem<T>> currItem = LimitedUniquePtr<ListItem<T>>(this->_allocator.allocate(1));
        this->_allocator.construct(currItem.get());

        
        for (long long i = this->_listSize - 2; i >= 0; --i) {
            LimitedUniquePtr<ListItem<T>> newItem = std::move(LimitedUniquePtr<ListItem<T>>(this->_allocator.allocate(1)));
            this->_allocator.construct(newItem.get());

            newItem.get()->nextItem = std::move(currItem);

            currItem = std::move(newItem);
        }

        this->_head = std::move(currItem);
    }

    LinkedList(std::initializer_list<T> params, AllocatorType alloc = {}) : _listSize(params.size()), _allocator(alloc) {
        LimitedUniquePtr<ListItem<T>> currItem = LimitedUniquePtr<ListItem<T>>(this->_allocator.allocate(1));
        this->_allocator.construct(currItem.get());
        currItem.get()->value =*(params.begin() + this->_listSize - 1);
       
        for (long long i = this->_listSize - 2; i >= 0; --i) {
             LimitedUniquePtr<ListItem<T>> newItem = std::move(LimitedUniquePtr<ListItem<T>>(this->_allocator.allocate(1)));
            this->_allocator.construct(newItem.get());

            newItem.get()->value = *(params.begin() + i);
            newItem.get()->nextItem = std::move(currItem);

            currItem = std::move(newItem);
        }

        this->_head = std::move(currItem);
    }

    LinkedList(LinkedList& other) = delete;
    LinkedList(LinkedList&& other) noexcept = default;

    ~LinkedList() {
        if (this->_listSize > 0) {
            if constexpr (std::is_destructible_v<T>) {
                LimitedUniquePtr<ListItem<T>> currentItem = std::move(this->_head);

                while (currentItem != nullptr) {
                    std::allocator_traits<AllocatorType>::destroy(
                        this->_allocator,
                        &(currentItem.get()->value)
                    );

                    LimitedUniquePtr<ListItem<T>> tmp = std::move(currentItem.get()->nextItem);
                    this->_allocator.deallocate(currentItem.get(), 1);
                    currentItem = std::move(tmp);
                }
            }
        }

        this->_head = nullptr;
        this->_listSize = 0;
    }

    // Доступ к массиву (изменение)
    ListItem<T>& operator[](size_t idx) {
        if (idx >= this->_listSize) {
            throw std::out_of_range("List index is out of range!");
        }

        ListItem<T>* returnItem = (this->_head.get());

        for (size_t i = 1; i <= idx; ++i) {
            returnItem = (*returnItem).nextItem.get();
        }

        return *returnItem;
    }

    // Доступ к массиву (получение элемента)
    ListItem<T> operator[](size_t idx) const {
        if (idx >= this->_listSize) {
            throw std::out_of_range("List index is out of range!");
        }

        ListItem<T> returnItem = *(this->_head);

        for (size_t i = 1; i <= idx; ++i) {
            returnItem = *(returnItem.nextItem);
        }

        return returnItem;
    }

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

    void pushBack(T& value) {
        LimitedUniquePtr<ListItem<T>> newItem = LimitedUniquePtr<ListItem<T>>(this->_allocator.allocate(1));
        this->_allocator.construct(newItem.get());

        newItem.get()->value = value;

        if (this->_listSize == 0) {
            this->_head = std::move(newItem);
        } else {
            (*this)[this->_listSize - 1].nextItem = std::move(newItem);
        }

        ++this->_listSize;
    }

    T popFront() {
        if (this->_listSize == 0) {
            throw std::out_of_range("Cannot pop from empty list!");
        }

        T tmpValue = this->_head.get()->value;

        ListItem<T>* oldHead = this->_head.get();
        if (this->_listSize == 1) {
            this->_head = nullptr;
        }
        else {
            auto tmp = std::move(this->_head.get()->nextItem);

            this->_head = std::move(tmp);
        }

        this->_allocator.deallocate(oldHead, 1);

        --this->_listSize;

        return tmpValue;
    }

    T popBack() {
        if (this->_listSize == 0) {
            throw std::out_of_range("Cannot pop from empty list!");
        }

        T tmp = (*this)[this->_listSize - 1].value;

        if (this->_listSize == 1) {
            this->_allocator.deallocate(this->_head.get(), 1);
            this->_head = nullptr;
        } else {
            ListItem<T>* prevItem = this->_head.get();
            for (size_t i = 1; i < this->_listSize - 1; ++i) {
                prevItem = prevItem->nextItem.get();
            }
            
            this->_allocator.deallocate(prevItem->nextItem.get(), 1);
            prevItem->nextItem = nullptr;
        }

        --this->_listSize;

        return tmp;
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