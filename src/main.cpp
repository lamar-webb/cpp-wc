#include <iostream>

int main(int argc, char* argv[]) {

    std::cout << "Argument count: " << argc << "\n";

    for (int i{0}; i < argc; i++) {
        std::cout << "Arg [" << i << "] ->" << argv[i] << "\n";
    }

}