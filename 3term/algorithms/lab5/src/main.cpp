#include <iostream>

#include <fort.hpp>

#include "Matrix.hpp"
#include "Random.hpp"
#include "Strassen.hpp"
#include "Benchmark.hpp"
#include "Console/Color.hpp"

template <typename T>
void Rand(Ng::Matrix<T>& matrix, const T& left, const T& right) {
    for (std::size_t i = 0; i < matrix.Rows(); ++i)
        for (std::size_t j = 0; j < matrix.Columns(); ++j)
            matrix[i][j] = Ng::Random::Irand(left, right);
}

fort::char_table CreateTable(std::size_t pow2) {
    std::cout << "Table creation..." << std::endl;

    fort::char_table table;

    table.set_border_style(FT_SOLID_ROUND_STYLE);

    for (int i = 0; i < pow2; ++i) {
        if (i == 0) {
            table << fort::header << "S / L";
        } else {
            table << fort::header << static_cast<int>(std::pow(2, i)) << "";
            table[table.cur_row()][i * 2 - 1].set_cell_span(2);

            if (i == pow2 - 1)
                table << fort::endr;
        }
    }

    for (int i = 0; i < pow2; ++i) {
        if (i == 0) {
            table << "";
        } else {
            table << "Com" << "St";

            if (i == pow2 - 1)
                table << fort::endr;
        }
    }

    for (int i = 1; i < pow2; ++i) {
        table.cur_cell().set_cell_text_align(fort::text_align::right);
        table.cur_cell().set_cell_content_fg_color(fort::color::blue);
        table << static_cast<int>(std::pow(2, i));

        for (std::size_t j = 1; j <= i; ++j) {
            std::size_t size = std::pow(2, i);
            std::size_t limit = std::pow(2, j);
            Ng::Matrix<int> m1(size, size);
            Ng::Matrix<int> m2(size, size);

            Rand(m1, -10, 10);
            Rand(m2, -10, 10);
            table << std::to_string(Ng::Benchmark<std::milli>([=] { m1 * m2; }).count()) + "ms";

            Rand(m1, -10, 10);
            Rand(m2, -10, 10);
            table << std::to_string(Ng::Benchmark<std::milli>(Ng::Strassen::Run<int>, m1, m2, limit).count()) + "ms";
        }

        table[i].set_cell_text_align(fort::text_align::right);
        table << fort::endr;
    }

    table.row(0).set_cell_text_align(fort::text_align::center);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::magenta);

    table.column(12).set_cell_content_fg_color(fort::color::green);
    table.column(12).set_cell_content_text_style(fort::text_style::bold);

    return table;
}

int main() {
    using Color = Ng::Console::Color;

    Ng::Matrix<int> matrix1(321, 658);
    Ng::Matrix<int> matrix2(658, 1023);

    Rand(matrix1, -10, 10);
    Rand(matrix2, -10, 10);

    if (matrix1 * matrix2 == Ng::Strassen::Run(matrix1, matrix2))
        std::cout << Color::Magenta << "Multiplication is OK" << Color::Reset << std::endl;

    std::cout << CreateTable(10).to_string() << std::endl;

    return 0;
}
