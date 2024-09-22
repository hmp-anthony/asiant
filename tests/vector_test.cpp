#include <asiant/vector.hpp>

#include <cstdlib>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "gtest/gtest.h"

TEST(vector, assigning_values) {
    asiant::vector v;
    v[0] = 0.4;
    v[1] = 0.6;
    v[2] = 0.9;
    ASSERT_NEAR(v[0], 0.4, 0.0001);
    ASSERT_NEAR(v[1], 0.6, 0.0001);
    ASSERT_NEAR(v[2], 0.9, 0.0001);
}

TEST(vector, scalar_multiplication) {
    asiant::vector v(1.0, 2.0, 3.0);
    v *= 5.0;
    ASSERT_EQ(v[0], 5.0);
    ASSERT_EQ(v[1], 10.0);
    ASSERT_EQ(v[2], 15.0);

    auto w = v * 9.0;
    ASSERT_EQ(w[0], 45.0);
    ASSERT_EQ(w[1], 90.0);
    ASSERT_EQ(w[2], 135.0);
}

TEST(vector, addition) {
    asiant::vector v1(1.0, 2.0, 3.0);
    asiant::vector v2(3.0, 2.0, 1.0);
    auto w = v1 + v2;

    ASSERT_EQ(w[0], 4.0);
    ASSERT_EQ(w[1], 4.0);
    ASSERT_EQ(w[2], 4.0);

    asiant::vector v3(1.0, 1.0, 1.0);
    v1 += v3;
    ASSERT_EQ(v1[0], 2.0);
    ASSERT_EQ(v1[1], 3.0);
    ASSERT_EQ(v1[2], 4.0);
}

TEST(vector, subtraction) {
    asiant::vector v1(1.0, 2.0, 3.0);
    asiant::vector v2(3.0, 2.0, 1.0);
    auto w = v1 - v2;

    ASSERT_EQ(w[0], -2.0);
    ASSERT_EQ(w[1], 0.0);
    ASSERT_EQ(w[2], 2.0);

    asiant::vector v3(1.0, 1.0, 1.0);
    v1 -= v3;
    ASSERT_EQ(v1[0], 0.0);
    ASSERT_EQ(v1[1], 1.0);
    ASSERT_EQ(v1[2], 2.0);
}

TEST(vector, dot_product) {
    asiant::vector v1(1.0, 2.0, 3.0);
    asiant::vector v2(3.0, 2.0, 1.0);
    auto s = v1 * v2;
    ASSERT_EQ(s, 10.0);
}

TEST(vector, cross_product) {
    asiant::vector v1(1.0, 2.0, 3.0);
    asiant::vector v2(3.0, 2.0, 1.0);
    auto w = v1 % v2;
    ASSERT_EQ(w[0], -4.0);
    ASSERT_EQ(w[1], 8.0);
    ASSERT_EQ(w[2], -4.0);

    v1 %= v2;
    ASSERT_EQ(v1[0], -4.0);
    ASSERT_EQ(v1[1], 8.0);
    ASSERT_EQ(v1[2], -4.0);
}

TEST(vector, invert) {
    asiant::vector v(1.0, 2.0, 3.0);
    v.invert();
    ASSERT_EQ(-1.0, v[0]);
    ASSERT_EQ(-2.0, v[1]);
    ASSERT_EQ(-3.0, v[2]);
}

TEST(vector, magnitude) {
    asiant::vector v(1.0, 2.0, 3.0);
    ASSERT_NEAR(v.magnitude(), 3.7416 , 0.001);
}

TEST(vector, square_magnitude) {
    asiant::vector v(1.0, 2.0, 3.0);
    ASSERT_NEAR(v.square_magnitude(), 14.0 , 0.001); 
}

TEST(vector, normalize) {
    asiant::vector v(1.0, 2.0, 3.0);
    v.normalize();
    ASSERT_NEAR(v[0], 0.2673, 0.001); 
    ASSERT_NEAR(v[1], 0.5345, 0.001); 
    ASSERT_NEAR(v[2], 0.8018, 0.001); 
}

TEST(vector, add_scaled_vector) {
    asiant::vector v(1.0, 2.0, 6.0);
    asiant::vector w(1.0, 2.0, 1.0);
    asiant::real a = 10.0;
    v.add_scaled_vector(w, a);
    
    ASSERT_NEAR(v[0], 11.0, 0.001); 
    ASSERT_NEAR(v[1], 22.0, 0.001); 
    ASSERT_NEAR(v[2], 16.0, 0.001); 
}

TEST(vector, component_product) {
    asiant::vector v(1.0, 2.0, 6.0);
    asiant::vector w(1.0, 2.0, 1.0);
    auto z = v.component_product(w);
    ASSERT_NEAR(z[0], 1.0, 0.001); 
    ASSERT_NEAR(z[1], 4.0, 0.001); 
    ASSERT_NEAR(z[2], 6.0, 0.001); 
}

TEST(vector, component_product_update) {
    asiant::vector v(1.0, 2.0, 6.0);
    asiant::vector w(1.0, 2.0, 1.0);
    v.component_product_update(w);
    ASSERT_NEAR(v[0], 1.0, 0.001); 
    ASSERT_NEAR(v[1], 4.0, 0.001); 
    ASSERT_NEAR(v[2], 6.0, 0.001); 
}

TEST(vector, scalar_product) {
    asiant::vector v1(1.0, 2.0, 3.0);
    asiant::vector v2(3.0, 2.0, 1.0);
    auto s = v1.scalar_product(v2);
    ASSERT_EQ(s, 10.0);
}

TEST(vector, vector_product) {
    asiant::vector v1(1.0, 2.0, 3.0);
    asiant::vector v2(3.0, 2.0, 1.0);
    auto w = v1.vector_product(v2);
    ASSERT_EQ(w[0], -4.0);
    ASSERT_EQ(w[1], 8.0);
    ASSERT_EQ(w[2], -4.0);
}

TEST(vector, clear) {
    asiant::vector v(1.0, 2.0, 6.0);
    v.clear();
    ASSERT_NEAR(v[0], 0.0, 0.001); 
    ASSERT_NEAR(v[1], 0.0, 0.001); 
    ASSERT_NEAR(v[2], 0.0, 0.001); 
}
