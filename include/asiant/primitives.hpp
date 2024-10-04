#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <asiant/vector.hpp>

namespace asiant{
    struct sphere {
        std::shared_ptr<vector> center_;
        real radius_;
    };
};

#endif
