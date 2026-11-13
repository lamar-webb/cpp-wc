using namespace std;
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

enum class Command {
    FILE_SIZE,
    CHAR_COUNT,
    WORD_COUNT,
    LINE_COUNT,
};


static vector<char> load_stream_data(istream& input_stream) {
    vector<char> data;
    char ch;
    while (input_stream.get(ch)) {
        data.push_back(ch);
    }
    return data;
}

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
    uintmax_t line_count{0};
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
    uintmax_t word_count{0};
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
    file.close();
    return word_count;
}

static uintmax_t calculate_character_count(const filesystem::path& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error, file could not be opened.";
        return 1;
    }
    char ch{};
    uintmax_t char_count{0};
    while (file.get(ch)) {
        ++char_count;
    }

    file.close();
    return char_count;
}

static void print_output(const vector<string>& results, const filesystem::path& file_path) {
    string output;
    for (size_t i{0}; i < results.size(); ++i) {
        output.append(results[i]);
        output.append(" ");
    }
    output.append(file_path);
    cout << output;
}

int main(int argc, char* argv[]) {
    vector<char> data;
    unordered_map<string, Command> commands = {
        {"-c", Command::FILE_SIZE},
        {"-l", Command::LINE_COUNT},
        {"-w", Command::WORD_COUNT},
        {"-m", Command::CHAR_COUNT}
    };


    if (argc >= 2 and commands.count(argv[1])== 1) {
        // flag was supplied? yes
        if (argc > 2) {
            // filename supplied? yes
            // behavior - Read file, selected count based on flag
        }else{
            // filename supplied? no
            // behavior - Read cin, selected count based on flag
        }
    }else{
        // flag was supplied? no
        if (argc > 1) {
            // filename supplied? yes
            // behavior - Read file, default counts

        }else {
            // filename supplied? no
            // behavior - Read cin, default counts
        }
    }


    // if (argc == 3) {
    //     if (argv[1] and strcmp(argv[1], "-c") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
    //         uintmax_t size = calculate_file_size(argv[2]);
    //         print_output({to_string(size)}, argv[2] );
    //         return 0;
    //     }
    //     if (argv[1] and strcmp(argv[1], "-l") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
    //         const uintmax_t line_count = calculate_line_count(argv[2]);
    //         print_output({to_string(line_count)}, argv[2] );
    //         return 0;
    //     }
    //     if (argv[1] and strcmp(argv[1], "-w") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
    //         const uintmax_t word_count = calculate_word_count(argv[2]);
    //         print_output({to_string(word_count)}, argv[2] );
    //         return 0;
    //     }
    //
    //     if (argv[1] and strcmp(argv[1], "-m") == 0 and argv[2] and filesystem::is_regular_file(argv[2])) {
    //         const uintmax_t char_count = calculate_character_count(argv[2]);
    //         print_output({to_string(char_count)}, argv[2] );
    //         return 0;
    //     }
    // }else if (argc == 2) {
    //     const uintmax_t size = calculate_file_size(argv[1]);
    //     const uintmax_t word_count = calculate_word_count(argv[1]);
    //     const uintmax_t line_count = calculate_line_count(argv[1]);
    //     print_output( {to_string(line_count),to_string(word_count), to_string(size)}, argv[1] );
    // }else {
    //     cout << "Please add required command line arguments";
    // }
}