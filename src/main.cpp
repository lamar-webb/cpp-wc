using namespace std;
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

int main(int argc, char* argv[]) {

    if (argc > 2) {
        if (argv[1] and strcmp(argv[1], "-c") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
            uintmax_t file_size = filesystem::file_size(argv[2]);
            cout << file_size << " " << argv[2] << "\n";
        }

    }else {
        cout << "Please add required command line arguments";
    }


}