#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

namespace functional {

    struct Tape {

        std::string filename;
        std::fstream file;
        int size = 0;
        int end = 0;
        std::vector<int> chunks_position;
        int current_chunk = 0;
        int current_position = 0;

    };

    struct Peak {

        int value = 0;
        int tape = -1;

    };

    bool end_of_chunk(Tape& tape, const int& current);
    void generate_source(const std::string& filepath, const int& n);
    void init_tapes_capacity(std::vector<Tape>& tapes, int files_count, int min_chunks);
    int get_chunks_count(const std::vector<Tape>& tapes);
    std::vector<int> read_chunk(std::ifstream& infile, const int& chunk_size);
    void read_num(Tape& tape, int& num);
    void write_num(Tape& tape, const int& num);
    void write_chunk(Tape& tape, const std::vector<int>& chunk);
    void init_tapes(std::vector<Tape>& tapes, const std::string& filepath, const int& files_count, const int& chunk_size);
    void merge(std::vector<Tape>& tapes, const int& empty);
    void sort(std::vector<Tape>& tapes, int& full_file);
    void rename_tape(std::vector<Tape>& tapes, const int& full_file, const std::string& outpath);
    void delete_tapes(std::vector<Tape>& tapes, const int& full_file);

    int main();

}