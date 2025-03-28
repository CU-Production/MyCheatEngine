#include <iostream>

int main() {
    int data = 12345;

    std::cout << "data addr: " << &data << std::endl;

    std::cin.get();

    while (true) {
        std::cout << data << std::endl;
    }

    return 0;
}