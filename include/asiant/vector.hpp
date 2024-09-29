#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <asiant/precision.hpp>

#include <stdexcept>

namespace asiant {
    class vector {
    public:
        vector() : x(0), y(0), z(0) {}
        vector(const real x, const real y, const real z)
            : x(x), y(y), z(z) {}

        void operator*=(const real a) {
            x *= a;
            y *= a;
            z *= a;
        }

        vector operator*(const real a) const {
            return vector(a*x, a*y, a*z);
        }

        vector(const vector& from, const vector& to)
        {
            x = to.x - from.x;
            y = to.y - from.y;
            z = to.z - from.z;
        }


        void operator+=(const vector& v) {
            x += v[0];
            y += v[1];
            z += v[2];
        }

        vector operator+(const vector& v) const {
            return vector(x + v[0], y + v[1], z + v[2]); 
        }

        void operator-=(const vector& v) {
            x -= v[0];
            y -= v[1];
            z -= v[2];
        }

        vector operator-(const vector& v) const {
            return vector(x - v[0], y - v[1], z - v[2]);
        }

        real operator*(const vector& v) const {
            return x * v[0] + y * v[1] + z * v[2];
        }

        void operator %=(const vector& v) {
            *this = vector_product(v);
        }

        vector operator%(const vector& v) const {
            return vector(y * v[2] - z * v[1],
                          z * v[0] - x * v[2],
                          x * v[1] - y * v[0]);
        }

        real operator [](int i) const {
            if(i == 0) { return x; }
            if(i == 1) { return y; }
            if(i == 2) { return z; }
            throw std::invalid_argument("vector index out of range!");
        }

        real & operator [](int i) {
            if(i == 0) { return x; }
            if(i == 1) { return y; }
            if(i == 2) { return z; }
            throw std::invalid_argument("vector index out of range!");
        }
        
        void invert() {
            x = -x;
            y = -y;
            z = -z;
        }

        real magnitude() const {
            return real_sqrt(x*x + y*y + z*z);
        }
    
        real square_magnitude() const {
            return x*x + y*y + z*z;
        }

        void normalize() {
            real m = magnitude();
            if(m > 0) {
                (*this) *= 1.0/m; 
            }
        }

        void add_scaled_vector(const vector& v, real scale) {
            x += v[0] * scale;
            y += v[1] * scale;
            z += v[2] * scale;
        }
    
        vector component_product(const vector& v) const {
            return vector(x * v[0], y * v[1], z * v[2]);
        }

        void component_product_update(const vector& v) {
            x *= v[0];
            y *= v[1];
            z *= v[2];
        }

        real scalar_product(const vector& v) const {
            return x * v[0] + y * v[1] + z * v[2];
        }

        vector vector_product(const vector& v) const {
            return vector(y * v[2] - z * v[1],
                          z * v[0] - x * v[2],
                          x * v[1] - y * v[0]);
        }

        void clear() {
            x = 0;
            y = 0;
            z = 0;
        }

        real distance(const vector &other) const
        {
            return vector(*this, other).magnitude();
        }
    private:
        real x;
        real y;
        real z;
    };
};

#endif
