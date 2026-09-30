#include <iostream>
#include "utils.hpp"

int main(int argc, char *argv[]) {
    std::cout << argc << std::endl;

    while (--argc > 0)
        std::cout << argv[argc];
        
    return EXIT_SUCCESS; 
}