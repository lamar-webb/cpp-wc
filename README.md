# cpp-wc

A C++17 partial implementation of the Unix `wc` command-line utility, focused on C++ fundamentals, stream processing, memory efficiency, and performance optimization.

The program counts bytes, lines, words, and UTF-8 characters from files or standard input. It processes input in a single pass using a fixed-size buffer, allowing it to handle large files without loading their contents into memory.

## Project Goals

This project was developed to gain practical experience with:

- Modern C++ fundamentals, including structs, enums, references, and standard library containers.
- Command-line argument parsing and input validation.
- File I/O and standard input using `std::istream`.
- Separation of responsibilities and incremental refactoring.
- Memory-efficient processing of large files.
- Performance optimization through buffered I/O.
- Benchmarking and verifying program output against an established Unix utility.

The implementation evolved from reading entire files into a `std::vector<char>` to processing streams one byte at a time, and finally to fixed-size buffered reads.

## Features

| Option | Description |
|---|---|
| `-c` | Count bytes |
| `-l` | Count newline characters |
| `-w` | Count whitespace-delimited words |
| `-m` | Count UTF-8 characters (simplified code-point counting) |
| No option | Display line, word, and byte counts |

Additional functionality:

- Accepts a filename or reads from standard input.
- Supports Unix pipelines and input redirection.
- Validates unsupported flags and duplicate arguments.
- Reports file-opening errors.
- Processes input in a single pass with constant auxiliary memory.
- Supports configurable buffer sizes at compile time.

## Building the Project

**Requirements:** A C++17-compatible compiler. CMake 4.3 or later is required by the current `CMakeLists.txt`.

Clone the repository:

```bash
git clone https://github.com/lamar-webb/cpp-wc.git
cd cpp-wc
```

Build directly with Clang:

```bash
clang++ -std=c++17 -O2 src/main.cpp -o cpp_wc
```

## Usage

Count lines, words, and bytes:

```bash
./cpp_wc tests/test.txt
```

Example output:

```text
7148 59886 340235 tests/test.txt
```

Count individual measurements:

```bash
./cpp_wc -c tests/test.txt
./cpp_wc -l tests/test.txt
./cpp_wc -w tests/test.txt
./cpp_wc -m tests/test.txt
```

Read from standard input:

```bash
cat tests/test.txt | ./cpp_wc -w
```

Or use input redirection:

```bash
./cpp_wc < tests/test.txt
```

When reading from standard input, the output does not include a filename.

## Implementation

### Single-Pass Stream Processing

The core counting function accepts an `std::istream&`, allowing the same implementation to process both file streams and standard input.

It maintains a `Counts` structure containing four counters:

- **Lines:** Incremented for each newline character (`\n`).
- **Bytes:** Incremented by the number of bytes read.
- **Words:** Counted by detecting transitions from whitespace to non-whitespace.
- **Characters:** Counted using a simplified UTF-8 code-point counting rule.

All four measurements are calculated during one traversal of the input. The requested command determines which results are displayed.

### Buffered Reading

The implementation reads data in fixed-size chunks using:

- `std::istream::read()` to read blocks of bytes.
- `std::istream::gcount()` to determine how many bytes were actually read.
- A loop over the valid bytes in each chunk to update the counters.

Using `gcount()` ensures the final partially filled buffer is processed correctly.

Word-counting state is preserved between chunks so that words crossing buffer boundaries are counted only once.

The buffer size can be configured at compile time using `BUFFER_SIZE`. For example:

```bash
clang++ -std=c++17 -O2 -DBUFFER_SIZE=4096 src/main.cpp -o cpp_wc
```

### Memory Complexity

The initial implementation stored the entire input file in memory, resulting in auxiliary memory usage proportional to file size: **O(n)**.

The buffered implementation uses a fixed-size buffer and a small number of counters, resulting in **O(1) auxiliary memory** relative to input size.

Time complexity remains **O(n)** because every byte must be examined.

## Testing and Validation

The implementation was tested on macOS and compared with the system `wc` utility.

### Reference File Results

All four measurements matched `wc` when tested against `tests/test.txt`.

| Measurement | `cpp-wc` | macOS `wc` |
|---|---:|---:|
| Lines | 7,148 | 7,148 |
| Words | 59,886 | 59,886 |
| Bytes | 340,235 | 340,235 |
| Characters | 333,921 | 333,921 |

Additional manual tests covered empty input, whitespace-only input, consecutive spaces and tabs, words without a trailing newline, UTF-8 text, piped input, unsupported flags, duplicate arguments, missing files, and words spanning buffer boundaries.

## Performance Benchmarks

Buffered reading was compared against the previous byte-by-byte streaming implementation using a **100,000,000-byte (100 MB)** generated text file.

The executables were compiled using Apple Clang with C++17 and `-O2` optimization. Each configuration was timed five times on macOS using the shell `time` command, and the median elapsed time was recorded.

### Results

| Implementation | Buffer size | Median time | Speedup |
|---|---:|---:|---:|
| Byte-by-byte streaming | N/A | 0.662 s | 1.00× |
| Buffered streaming | 1 KB | 0.126 s | 5.25× |
| Buffered streaming | 4 KB | 0.122 s | 5.43× |
| Buffered streaming | 16 KB | 0.122 s | 5.43× |
| Buffered streaming | 64 KB | 0.122 s | 5.43× |

All buffered configurations produced identical counts:

```text
2500000 17500000 100000000
```

On this workload, the 4 KB buffered implementation was approximately **5.4× faster** than byte-by-byte streaming, reducing elapsed time by approximately **81.6%**.

Increasing the buffer beyond 4 KB did not produce a meaningful additional improvement in the recorded measurements. A 4 KB default was selected as a balance between memory usage and measured performance.

These are local measurements from a single machine and workload, not universal performance guarantees.



## Future Improvements

Potential extensions include automated tests, support for multiple files and combined options, more robust I/O error handling, and additional performance profiling across different input types.

## Acknowledgments

Built as part of the [Build Your Own `wc` Tool](https://codingchallenges.fyi/challenges/challenge-wc) coding challenge by John Crickett.