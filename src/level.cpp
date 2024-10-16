#include <asiant/level.hpp>

#include <iostream>
#include <fstream>

namespace asiant {
    level::level(std::string file, int width, int height) {
        std::ifstream f(file);
        f >> rows_ >> cols_;

        data_.assign(rows_ * cols_, 0);

        for(int j = 0; j < rows_; ++j) {
            for(int i = 0; i < cols_; ++i) {
                f >> data_[j * cols_ + i];
            }
        }
    }

    void level::print() {
        for(int j = 0; j < rows_; ++j) {
            for(int i = 0; i < cols_; ++i) {
                std::cout << data_[j * cols_ + i] << " ";
            }
            std::cout << std::endl;
        }
    }
};
