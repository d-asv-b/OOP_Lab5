#pragma once

#include <memory>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <memory_resource>
#include <stdexcept>
#include <type_traits>

template <typename Type>
class LinkedListIterator {
private:
    Type* _pointer;
    size_t _size;
    size_t _currIdx;

public:
    LinkedListIterator(Type* listPtr, size_t listSize, size_t elemIdx) :
        _pointer(listPtr), _size(listSize), _currIdx(elemIdx) {}

    typename Type::elementType operator*() {
        if (this->_currIdx >= this->_size) {
            throw std::out_of_range("List index is out of range!");
        }

        return (*this->_pointer)[this->_currIdx];
    }

    typename Type::elementType operator->() {
        return *this;
    }

    LinkedListIterator<Type>& operator++() {
        ++this->_currIdx;
        return *this;
    }

    bool operator!=(const LinkedListIterator<Type>& other) const {
        return (
            this->_currIdx != other._currIdx &&
            this->_pointer != other._pointer
        );
    }
};

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
#define ALLOC_MULTIPLE_AT_ONCE
#ifdef ALLOC_MULTIPLE_AT_ONCE
        ListItem<T>* rawPtr = this->_allocator.allocate(this->_listSize);
        this->_allocator.construct(rawPtr + this->_listSize - 1);

        for (long long i = this->_listSize - 2; i >= 0; --i) {
            this->_allocator.construct(rawPtr + i);
            rawPtr[i].nextItem = std::move(LimitedUniquePtr<ListItem<T>>(rawPtr + i + 1, PolymorphicDeleter<ListItem<T>>{}));
        }

        this->_head = std::move(LimitedUniquePtr<ListItem<T>>(rawPtr, PolymorphicDeleter<ListItem<T>>{}));
#else
        LimitedUniquePtr currItem = std::unique_ptr<ListItem<T>>(this->_allocator.allocate(1));
        this->_allocator.construct(currItem.get());

        std::unique_ptr<ListItem<T>> newItem;
        for (long long i = this->_listSize - 2; i >= 0; --i) {
            newItem = std::move(std::unique_ptr<ListItem<T>>(this->_allocator.allocate(1)));
            this->_allocator.construct(newItem.get());

            newItem.get()->nextItem = std::move(currItem);

            currItem = std::move(newItem);
        }

        this->_head = std::move(newItem);
#endif
    }

    LinkedList(std::initializer_list<T> params, AllocatorType alloc = {}) : _listSize(params.size()), _allocator(alloc) {
#ifdef ALLOC_MULTIPLE_AT_ONCE
        ListItem<T>* rawPtr = this->_allocator.allocate(this->_listSize);

        this->_allocator.construct(rawPtr + this->_listSize - 1);
        rawPtr[this->_listSize - 1].value = *(params.begin() + this->_listSize - 1);

        for (long long i = this->_listSize - 2; i >= 0; --i) {
            this->_allocator.construct(rawPtr + i);

            rawPtr[i].value = *(params.begin() + i);
            rawPtr[i].nextItem = std::move(LimitedUniquePtr<ListItem<T>>(rawPtr + i + 1, PolymorphicDeleter<ListItem<T>>{}));
        }

        this->_head = std::move(LimitedUniquePtr<ListItem<T>>(rawPtr, PolymorphicDeleter<ListItem<T>>{}));
#else
        std::unique_ptr<ListItem<T>> currItem = std::unique_ptr<ListItem<T>>(this->_allocator.allocate(1));
        this->_allocator.construct(currItem.get());

        std::unique_ptr<ListItem<T>> newItem;
        for (long long i = this->_listSize - 2; i >= 0; --i) {
            newItem = std::move(std::unique_ptr<ListItem<T>>(this->_allocator.allocate(1)));
            this->_allocator.construct(newItem.get());

            newItem.get()->nextItem = std::move(currItem);

            currItem = std::move(newItem);
        }

        this->_head = std::move(newItem);
#endif
    }

    ~LinkedList() {
#ifdef ALLOC_MULTIPLE_AT_ONCE
        ListItem<T>* ptrToDealloc = this->_head.get();
#endif

        if constexpr (std::is_destructible_v<T>) {
            LimitedUniquePtr<ListItem<T>> currentItem = std::move(this->_head);

            while (currentItem != nullptr) {
                std::allocator_traits<AllocatorType>::destroy(
                    this->_allocator,
                    &(currentItem.get()->value)
                );

                LimitedUniquePtr<ListItem<T>> tmp = std::move(currentItem.get()->nextItem);
#ifndef ALLOC_MULTIPLE_AT_ONCE
                this->_allocator.deallocate(currentItem.get(), 1);
#endif
                currentItem = std::move(tmp);
            }
        }

#ifdef ALLOC_MULTIPLE_AT_ONCE
        this->_allocator.deallocate(ptrToDealloc, this->_listSize);
#endif

        this->_head = nullptr;
        this->_listSize = 0;
    }


    // Доступ к массиву (изменение)
    ListItem<T>& operator[](size_t idx) {
        if (idx >= this->_listSize) {
            throw std::out_of_range("List index is out of range!");
        }

        ListItem<T>* returnItem = _head.get();

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

        ListItem<T> returnItem = *_head;

        for (size_t i = 1; i <= idx; ++i) {
            returnItem = *((*_head).nextItem);
        }

        return returnItem;
    }

    size_t getSize() const {
        return this->_listSize;
    }

    LinkedListIterator<LinkedList<T, AllocatorType>> begin() {
        return LinkedListIterator<LinkedList<T, AllocatorType>>(this, 0, this->_listSize);
    }

    LinkedListIterator<LinkedList<T, AllocatorType>> end() {
        return LinkedListIterator<LinkedList<T, AllocatorType>>(this, this->_listSize, this->_listSize);
    }
};