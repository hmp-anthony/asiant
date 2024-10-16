#include <asiant/level.hpp>

#include <iostream>
#include <fstream>

namespace asiant {
    level::level(std::string file, int width, int height) {
        int m , n;
        std::cout << "creating level" << std::endl;

        std::ifstream f(file);
        f >> m >> n;

        std::cout << m <<  " " << n << std::endl;

    }
};
