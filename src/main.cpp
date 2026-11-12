using namespace std;
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

static uintmax_t calculate_file_size(const filesystem::path& file) {
    uintmax_t file_size =  filesystem::file_size(file);
    return file_size;
}

static std::uintmax_t calculate_line_count(const filesystem::path& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error, file could not be opened";
        return 1;
    }
    string line;
    int line_count{0};
    while (getline(file, line)) {
        ++line_count;
    }
    file.close();
    return line_count;
}

static std::uintmax_t calculate_word_count(const filesystem::path& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error, file could not be opened.";
        return 1;
    }
    char ch{};
    int word_count{0};
    char prev_char{' '};
    while (file.get(ch)) {
        char current_char = ch;
        if (isspace(ch)) {
            prev_char = current_char;
        }else {
            if (!isspace(prev_char)) {
                prev_char = current_char;
            }else {
                ++word_count;
                prev_char = current_char;
            }
        }
    }
    return word_count;
    file.close();
}

static uintmax_t calculate_character_count(const filesystem::path& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error, file could not be opened.";
        return 1;
    }
    char ch{};
    int char_count{0};
    while (file.get(ch)) {
        ++char_count;
    }

    file.close();
    return char_count;
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        if (argv[1] and strcmp(argv[1], "-c") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
            uintmax_t size = calculate_file_size(argv[2]);
            cout << size << " " << argv[2] << "\n";
            return 0;
        }
        if (argv[1] and strcmp(argv[1], "-l") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
            const uintmax_t line_count = calculate_line_count(argv[2]);
            cout << line_count << " " << argv[2] << "\n";
            return 0;
        }
        if (argv[1] and strcmp(argv[1], "-w") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
            const uintmax_t word_count = calculate_word_count(argv[2]);
            cout << word_count << " " << argv[2] << "\n";
            return 0;
        }

        if (argv[1] and strcmp(argv[1], "-m") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
            uintmax_t char_count = calculate_character_count(argv[2]);
            cout  << char_count << " " << argv[2] <<  "\n";
            return 0;
        }

    }else {
        cout << "Please add required command line arguments";
    }
}


