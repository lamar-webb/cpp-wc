using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

enum class Command {
    FILE_SIZE,
    CHAR_COUNT,
    WORD_COUNT,
    LINE_COUNT,
    DEFAULT
};


static vector<char> load_stream_data(istream& input_stream) {
    vector<char> data;
    char ch;
    while (input_stream.get(ch)) {
        data.push_back(ch);
    }
    return data;
}

static uintmax_t calculate_line_count(const std::vector<char>& data) {
    uintmax_t line_count{0};
    uintmax_t i{0};
    while (i < data.size()) {
        if (data[i] == '\n') {
            ++line_count;
        }
        ++i;
    }
    return line_count;
}

static uintmax_t calculate_word_count(const std::vector<char>& data) {

    uintmax_t word_count{0};
    char prev_char{' '};
    uintmax_t i{0};
    while (i < data.size()) {
        char ch = data[i];
        char current_char = ch;
        if (std::isspace(static_cast<unsigned char>(ch))) {
            prev_char = current_char;
        }else {
            if (!std::isspace(static_cast<unsigned char>(prev_char))) {
                prev_char = current_char;
            }else {
                ++word_count;
                prev_char = current_char;
            }
        }
        ++i;
    }
    return word_count;
}

static uintmax_t calculate_character_count(const std::vector<char>& data) {
    uintmax_t char_count{0};
    for (char ch : data) {
        unsigned char byte = static_cast<unsigned char>(ch);
        if (byte < 128 || byte >= 192) {
            ++char_count;
        }
    }
    return char_count;
}

static void print_output(const vector<string>& results, const filesystem::path& file_path) {
    string output;
    for (size_t i{0}; i < results.size(); ++i) {
        output.append(results[i]);
        if (i + 1 < results.size()) {
            output.append(" ");
        }
    }
    if (!file_path.empty()) {
        output.append(" ");
        output.append(file_path);
    }
    cout << output << "\n";
}

static void process_command(Command& command, const vector<char>& data, const filesystem::path& file_path) {
    switch (command) {
        case Command::FILE_SIZE: {
            uintmax_t file_size = data.size();
            print_output({to_string(file_size)}, file_path);
            break;
        }
        case Command::CHAR_COUNT: {
            const uintmax_t char_count = calculate_character_count(data);
            print_output({to_string(char_count)}, file_path);
            break;
        }
        case Command::LINE_COUNT: {
            const uintmax_t line_count = calculate_line_count(data);
            print_output({to_string(line_count)}, file_path);
            break;
        }

        case Command::WORD_COUNT: {
            const uintmax_t word_count = calculate_word_count(data);
            print_output({to_string(word_count)}, file_path);
            break;
        }
        case Command::DEFAULT: {
            const uintmax_t line_count = calculate_line_count(data);
            const uintmax_t word_count = calculate_word_count(data);
            uintmax_t file_size = data.size();
            print_output({to_string(line_count), to_string(word_count), to_string(file_size)}, file_path);
            break;
        }

    }
}


int main(int argc, char* argv[]) {

    if (argc > 3) {
        std::cerr << "cpp_wc: too many arguments\n"
                  << "Usage: cpp_wc [-c|-l|-w|-m] [file]\n";
        return 1;
    }

    unordered_map<string, Command> commands = {
        {"-c", Command::FILE_SIZE},
        {"-l", Command::LINE_COUNT},
        {"-w", Command::WORD_COUNT},
        {"-m", Command::CHAR_COUNT},
        {"", Command::DEFAULT}
    };


    Command command = Command::DEFAULT;
    string file_path;

    if (argc >= 2 and commands.count(argv[1])== 1) {

        unordered_map<string, Command>::iterator command_iter = commands.find(argv[1]);
        command = command_iter->second;
        if (argc > 2) {
            file_path = argv[2];
            ifstream file(file_path, ios::binary); // preserve the original bytes when reading the file.

            if (!file.is_open()) {
                cerr << "cpp_wc: could not open file: " << file_path << '\n';
                return 1;
            }
            vector<char> file_data = load_stream_data(file);
            process_command(command, file_data, file_path);
        }else{
            vector<char> cin_data =  load_stream_data(std::cin);
            process_command(command, cin_data, file_path);
        }
    }else{
        if (argc > 1) {
            if (std::filesystem::is_regular_file(argv[1])) {
                file_path = argv[1];
                ifstream file(file_path, ios::binary); // preserve the original bytes when reading the file.
                if (!file.is_open()) {
                    cerr << "cpp_wc: could not open file: " << file_path << '\n';
                    return 1;
                }
                vector<char> file_data = load_stream_data(file);
                process_command(command, file_data, file_path);
            }else {
                cerr << "cpp_wc: unsupported option or invalid file path: '" << argv[1] << "'\n"
                    << "Usage: cpp_wc [-c|-l|-w|-m] [file]\n";
                return 1;
            }
        }else {
            vector<char> cin_data =  load_stream_data(std::cin);
            process_command(command, cin_data, file_path);
        }
    }
}