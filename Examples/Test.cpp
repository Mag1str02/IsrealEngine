#include <iostream>

int main() {
    for (size_t i = 0; i < 100; ++i) {
        int* a = new int();
        *a     = 1;
        delete a;
        std::cout << *a << std::endl;
    }
}