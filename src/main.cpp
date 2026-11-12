using namespace std;
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 2) {
        // file size
        if (argv[1] and strcmp(argv[1], "-c") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
            uintmax_t file_size = filesystem::file_size(argv[2]);
            cout << file_size << " " << argv[2] << "\n";
            return 0;
        }
        if (argv[1] and strcmp(argv[1], "-l") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
            // line count
            ifstream file(argv[2]);
            if (!file.is_open()) {
                cerr << "Error, file could not be opened";
                return 1;
            }
            string line;
            int line_count{0};
            while (getline(file, line)) {
                ++line_count;
            }
            cout << line_count << " " << argv[2] << "\n";
        }
    }else {
        cout << "Please add required command line arguments";
    }
}


