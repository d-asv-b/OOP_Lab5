#include "./include/LinkedList.hpp"
#include "./include/MemoryResource.hpp"

#include <iostream>
#include <memory_resource>

int main() {
    MemoryResource mres;
    std::pmr::polymorphic_allocator<ListItem<int>> polyAlloc(&mres);

    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> list({ 1, 2, 3, 5 }, polyAlloc);

    for (size_t i = 0; i < list.getSize(); ++i) {
        std::cout << list[i].value << " ";
    }

    return 0;
}