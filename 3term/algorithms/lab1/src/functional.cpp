#include "functional.hpp"



#include <Random.hpp>

namespace functional {

    bool end_of_chunk(Tape& tape, const int& current) {

        int next;
        std::fstream::pos_type current_position = tape.file.tellg();

        tape.file.read(reinterpret_cast<char*>(&next), sizeof(next));

        if (tape.file.tellg() == -1)
            return true;

        tape.file.seekg(current_position);
        return tape.file.tellg() == tape.end || current > next;

    }

    void generate_source(const std::string& filepath, const int& n) {

        std::ofstream outfile(filepath, std::ios_base::binary);

        if (!outfile.is_open())
            throw std::invalid_argument("failed to open the file | functional::generate_source");

        for (int i = n; i >= 1; --i)
            outfile.write(reinterpret_cast<char*>(&i), sizeof(i));

        outfile.close();

    }

    void init_tapes_capacity(std::vector<Tape>& tapes, int files_count, int min_chunks) {

        int sum = 1;
        int maxIndex = 0;
        int max;

        tapes.resize(files_count);
        tapes[0].size = 1;

        while (sum <= min_chunks) {

            for (int i = 0; i < tapes.size(); ++i)
                if (tapes[maxIndex].size < tapes[i].size)
                    maxIndex = i;

            max = tapes[maxIndex].size;

            for (int i = 0; i < tapes.size(); ++i)
                i == maxIndex ? tapes[i].size = 0 : tapes[i].size += max;

            sum += max * static_cast<int>(tapes.size() - 2);
            maxIndex = 0;

        }

        for (int i = 0; i < tapes.size(); ++i)
            std::cout << i << " -> " << tapes[i].size << std::endl;

    }

    int get_chunks_count(const std::vector<Tape>& tapes) {

        int sum = 0;

        for (const auto& tape : tapes)
            sum += tape.size;

        return sum;

    }

    std::vector<int> read_chunk(std::ifstream& infile, const int& chunk_size) {

        std::vector<int> chunk(chunk_size);

        for (auto& e : chunk)
            infile.read(reinterpret_cast<char*>(&e), sizeof(e));

        return chunk;

    }

    void read_num(Tape& tape, int& num) {

        tape.file.read(reinterpret_cast<char*>(&num), sizeof(num));
        tape.current_position += sizeof(int);

    }

    void write_num(Tape& tape, const int& num) {

        tape.file.write(reinterpret_cast<const char*>(&num), sizeof(num));
        tape.end += sizeof(int);

    }

    void write_chunk(Tape& tape, const std::vector<int>& chunk) {

        for (const auto& e : chunk)
            tape.file.write(reinterpret_cast<const char*>(&e), sizeof(e));

        tape.end += static_cast<int>(chunk.size() * sizeof(int));
        tape.chunks_position.emplace_back(tape.end);

//        int cur = -1;
//        tape.file.write(reinterpret_cast<char*>(&cur), sizeof(cur));

    }

    void init_tapes(std::vector<Tape>& tapes, const std::string& filepath, const int& files_count, const int& chunk_size) {

        std::ifstream infile(filepath, std::ios_base::binary);

        if (!infile.is_open())
            throw std::invalid_argument("failed to open the file | functional::init_tapes");

        int numbers;
        int min_chunks;
        int chunks;
        int new_chunk_size;
        int oversize_chunks;
        std::vector<int> chunk;

        infile.seekg(0, std::ios_base::end);
        numbers = static_cast<int>(infile.tellg() / sizeof(int));
        infile.seekg(0, std::ios_base::beg);

        min_chunks = (numbers + chunk_size - 1) / chunk_size;
        init_tapes_capacity(tapes, files_count, min_chunks);
        chunks = get_chunks_count(tapes);

        new_chunk_size = numbers / chunks;
        oversize_chunks = numbers % chunks;

        std::cout << "tapes size = " << tapes.size() << std::endl;

        for (int i = 0; i < tapes.size(); ++i) {

            std::cout << "i = " << i << std::endl;

            tapes[i].filename = "../files/" + std::to_string(i) + ".bin";
            tapes[i].file.open(tapes[i].filename, std::ios_base::out | std::ios_base::binary);

            if (!tapes[i].file.is_open())
                throw std::invalid_argument("failed to open the tape file | functional::init_tapes");

            for (int j = 0; j < tapes[i].size; ++j) {

                if (oversize_chunks != 0) {

                    chunk = read_chunk(infile, new_chunk_size + 1);
                    std::sort(chunk.begin(), chunk.end());
                    write_chunk(tapes[i], chunk);
                    --oversize_chunks;

                } else {

                    chunk = read_chunk(infile, new_chunk_size);
                    std::sort(chunk.begin(), chunk.end());
                    write_chunk(tapes[i], chunk);

                }

            }

            tapes[i].file.close();
            tapes[i].file.open(tapes[i].filename, std::ios_base::in | std::ios_base::binary);

        }

    }

    void merge(std::vector<Tape>& tapes, const int& empty) {

        std::vector<Peak> peaks(tapes.size());

        for (int i = 0; i < peaks.size(); ++i) {

            if (i != empty) {

                read_num(tapes[i], peaks[i].value);
                peaks[i].tape = i;


            }

        }

        int min = 0;

        while (min != -1) {

            min = -1;

            for (int i = 0; i < tapes.size(); ++i) {

                if (i == empty)
                    continue;

                if (peaks[i].tape == -1)
                    continue;

                if (min == -1 || (peaks[min].value > peaks[i].value))
                    min = i;

            }

            if (min != -1) {

                write_num(tapes[empty], peaks[min].value);

                if (tapes[min].chunks_position[tapes[min].current_chunk] != tapes[min].current_position) {

                    read_num(tapes[min], peaks[min].value);

                } else {

                    peaks[min].tape = -1;
                    ++tapes[min].current_chunk;

                }

            }

        }

        tapes[empty].chunks_position.emplace_back(tapes[empty].file.tellp());

    }

    void sort(std::vector<Tape>& tapes, int& full_file) {

        std::cout << "sort" << std::endl;

        bool sorted = false;
        int empty = -1;
        int min = -1;

        while (!sorted) {

            for (int i = 0; i < tapes.size(); ++i) {

                if (tapes[i].size == 0)
                    empty == -1 ? empty = i : sorted = true;
                else if (min == -1 || tapes[min].size > tapes[i].size)
                    min = i;

            }

            if (!sorted) {

                tapes[empty].file.close();
                tapes[empty].file.open(tapes[empty].filename, std::ios_base::out | std::fstream::binary);
                tapes[empty].end = 0;
                tapes[empty].chunks_position.clear();
                tapes[empty].current_chunk = 0;
                tapes[empty].current_position = 0;

                for (int i = 0; i < tapes[min].size; ++i)
                    merge(tapes, empty);

                tapes[empty].file.close();
                tapes[empty].file.open(tapes[empty].filename, std::ios_base::in | std::fstream::binary);
                tapes[empty].size = tapes[min].size;

                for (int i = 0; i < tapes.size(); ++i)
                    if (i != empty)
                        tapes[i].size -= tapes[empty].size;

                empty = -1;
                min = -1;

            }

        }

        for (auto& tape : tapes)
            tape.file.close();

        full_file = min;

    }

    void rename_tape(std::vector<Tape>& tapes, const int& full_file, const std::string& outpath) {

        if (std::fstream(outpath))
            std::remove(outpath.c_str());

        std::rename(tapes[full_file].filename.c_str(), outpath.c_str());

    }

    void delete_tapes(std::vector<Tape>& tapes, const int& full_file) {

        for (int i = 0; i < tapes.size(); ++i)
            if (i != full_file && std::remove(tapes[i].filename.c_str()) != 0)
                std::cerr << "failed to delete " << tapes[i].filename << std::endl;

    }

    int main() {

        std::string inpath = "../files/source.bin";
        std::string outpath = "../files/result.bin";
        int files_count = 5;
        int chunk_size = 1000;
        int full_file = -1;
        std::vector<Tape> tapes;

        // main
        generate_source(inpath, 1000000);

        auto start = std::chrono::steady_clock::now();

        init_tapes(tapes, inpath, files_count, chunk_size);
        sort(tapes, full_file);


        auto end = std::chrono::steady_clock::now();
        std::cout << "TIME = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

        rename_tape(tapes, full_file, outpath);
        delete_tapes(tapes, full_file);

        // output
        std::ifstream iinfile(outpath, std::ios_base::binary);

        if (!iinfile.is_open())
            std::cerr << "failed to open the file" << std::endl;

        iinfile.seekg(0, std::ios_base::end);
        int endc = iinfile.tellg();
        iinfile.seekg(0, std::ios_base::beg);
        int num;

        while (iinfile.tellg() != endc) {

            iinfile.read(reinterpret_cast<char*>(&num), sizeof(num));
            std::cout << num << " ";

        }
        std::cout << std::endl;
        iinfile.close();

        return 0;

    }

}