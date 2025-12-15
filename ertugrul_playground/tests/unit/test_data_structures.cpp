#include <Array.h>
#include <iostream>

int main() {
    int size = 5;
    data_structures::Array<int, 5> data;

    memset(&data[0], 0, data.Size() * sizeof(int));
    data[1] = 5;
    data[3] = 8;
    
    for (size_t i = 0; i < data.Size(); i++) {
        std::cout << data[i] << std::endl;
    }

    return 0;
}