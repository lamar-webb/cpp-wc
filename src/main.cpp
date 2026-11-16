using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <cctype>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif

enum class Command {
    FILE_SIZE,
    CHAR_COUNT,
    WORD_COUNT,
    LINE_COUNT,
    DEFAULT
};

struct Counts {
    uintmax_t lines{0};
    uintmax_t words{0};
    uintmax_t bytes{0};
    uintmax_t characters{0};
};

struct ParsedArguments {
    Command command{Command::DEFAULT};
    string file_path;
    bool success{true};
};

Counts calculate_counts(std::istream& input_stream) {
    Counts counts{};
    constexpr std::size_t buffer_size = BUFFER_SIZE;
    char buffer[buffer_size];
    bool is_inside_word = false;

    while (input_stream) {
        input_stream.read(buffer, buffer_size);
        std::streamsize bytes_read = input_stream.gcount();

        for (std::streamsize i{0}; i < bytes_read; i++) {
            // count lines
            if (buffer[i] == '\n') {
                ++counts.lines;
            }
            // count character
            if (auto byte = static_cast<unsigned char>(buffer[i]); byte < 128 || byte >= 192) {
                ++counts.characters;
            }
            // count words
            bool is_white_space = std::isspace(static_cast<unsigned char>(buffer[i])) != 0;

            if (is_white_space) {
                is_inside_word = false;
            } else if (!is_inside_word) {
                ++counts.words;
                is_inside_word = true;
            }
        }
        // count bytes
        counts.bytes += bytes_read;
    }
    return counts;
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

static void process_command(Command command, istream& input_stream, const filesystem::path& file_path) {
    Counts counts = calculate_counts(input_stream);
    switch (command) {
        case Command::FILE_SIZE: {
            print_output({to_string(counts.bytes)}, file_path);
            break;
        }
        case Command::CHAR_COUNT: {
            print_output({to_string(counts.characters)}, file_path);
            break;
        }
        case Command::LINE_COUNT: {
            print_output({to_string(counts.lines)}, file_path);
            break;
        }

        case Command::WORD_COUNT: {
            print_output({to_string(counts.words)}, file_path);
            break;
        }
        case Command::DEFAULT: {
            print_output({to_string(counts.lines), to_string(counts.words), to_string(counts.bytes)}, file_path);
            break;
        }

    }
}

ParsedArguments parse_arguments(int argc, char* argv[]) {
    ParsedArguments parsed_arguments;
    const unordered_map<string, Command> commands = {
        {"-c", Command::FILE_SIZE},
        {"-l", Command::LINE_COUNT},
        {"-w", Command::WORD_COUNT},
        {"-m", Command::CHAR_COUNT}
    };
    for (int i{1}; i < argc; ++i) {
        string argument = argv[i];
        unordered_map<string, Command>::const_iterator command_iter = commands.find(argument);
        if (command_iter != commands.end()) {
            if (parsed_arguments.command != Command::DEFAULT) {
                cerr << "cpp_wc: multiple flags are not supported\n"
                     << "Usage: cpp_wc [-c|-l|-w|-m] [file]\n";

                parsed_arguments.success = false;
                return parsed_arguments;
            }

            parsed_arguments.command = command_iter->second;

        } else if (!argument.empty() && argument[0] == '-') {

            cerr << "cpp_wc: unsupported option: " << argument << '\n'
                 << "Usage: cpp_wc [-c|-l|-w|-m] [file]\n";

            parsed_arguments.success = false;
            return parsed_arguments;
        } else {
            if (!parsed_arguments.file_path.empty()) {
                cerr << "cpp_wc: multiple filenames are not supported\n"
                     << "Usage: cpp_wc [-c|-l|-w|-m] [file]\n";

                parsed_arguments.success = false;
                return parsed_arguments;
            }
            parsed_arguments.file_path = argument;
        }
    }
    return parsed_arguments;
}



int main(int argc, char* argv[]) {

    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    if (!parsed_arguments.success) {
        return 1;
    }

    try {
        if (!parsed_arguments.file_path.empty()) {
            std::ifstream file(parsed_arguments.file_path, std::ios::binary);

            if (!file.is_open()) {
                throw std::runtime_error("Could not open file");
            }

            process_command(
                parsed_arguments.command,
                file,
                parsed_arguments.file_path
            );
        } else {
            process_command(
                parsed_arguments.command,
                std::cin,
                parsed_arguments.file_path
            );
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

}