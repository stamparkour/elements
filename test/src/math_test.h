#ifndef SRC_MATH_TEST_H
#define SRC_MATH_TEST_H

#include <gtest/gtest.h>
#include <stamp/math/cayley_dickson.h>

using stamp::math::cayley_dickson_construct;
using stamp::math::conjugate;
using stamp::math::norm;
using stamp::math::norm2;
using stamp::math::inverse;
using namespace stamp::math::literals;

TEST(cayley_dickson, constructor) {
	stamp::math::cayley_dickson_construct<double, 1> a{};
	stamp::math::cayley_dickson_construct<double, 2> b{};
	stamp::math::cayley_dickson_construct<double, 3> c{};
	stamp::math::cayley_dickson_construct<double, 4> d{};
	stamp::math::cayley_dickson_construct<double, 8> e{};
	stamp::math::cayley_dickson_construct<double, 16> f{};
}

// Helper: compare component-wise, works for integral and floating value types.
template<typename T, unsigned Dim>
void expect_components_equal(const cayley_dickson_construct<T, Dim>& a, std::initializer_list<T> vals) {
    auto it = vals.begin();
    for (unsigned i = 0; i < Dim; ++i) {
        T expected = (it != vals.end() ? *it : T{});
        if constexpr (std::is_floating_point_v<T>) {
            EXPECT_DOUBLE_EQ(static_cast<double>(a[i]), static_cast<double>(expected)) << "component " << i;
        }
        else {
            EXPECT_EQ(a[i], expected) << "component " << i;
        }
        if (it != vals.end()) ++it;
    }
}

// Group: construction and element access
TEST(stamp_complex_Construction, DefaultAndValueCtor) {
    cayley_dickson_construct<double, 1> a1{};
    expect_components_equal(a1, { 0.0 });

    cayley_dickson_construct<double, 2> a2{ 1.5 };
    // value ctor places value in the scalar (first) component
    expect_components_equal(a2, { 1.5, 0.0 });

    cayley_dickson_construct<int, 4> a4{ 3 };
    expect_components_equal(a4, { 3, 0, 0, 0 });
}

TEST(stamp_complex_Construction, InitializerListAndFieldAccess) {
    // initializer-list construction (assume it fills from index 0)
    cayley_dickson_construct<double, 4> v{ 2.0, 3.0, 5.0, 7.0 };
    expect_components_equal(v, { 2.0, 3.0, 5.0, 7.0 });

    // named fields for small dimensions (r,x,y,z) should reflect components
    EXPECT_DOUBLE_EQ(v[0], v.r);
    EXPECT_DOUBLE_EQ(v[1], v.x);
    EXPECT_DOUBLE_EQ(v[2], v.y);
    EXPECT_DOUBLE_EQ(v[3], v.z);
}

// Group: basic arithmetic and compound assignment
TEST(stamp_complex_Arithmetic, AddSubtractScalarAndConstruct) {
    cayley_dickson_construct<double, 3> a{ 1.0, 2.0, 3.0 };
    cayley_dickson_construct<double, 3> b{ 4.0, 1.0, -1.0 };

    auto sum = a + b;
    expect_components_equal(sum, { 5.0, 3.0, 2.0 });

    auto diff = a - b;
    expect_components_equal(diff, { -3.0, 1.0, 4.0 });

    a += b;
    expect_components_equal(a, { 5.0, 3.0, 2.0 });

    a -= cayley_dickson_construct<double, 2>{1.0, 1.0}; // smaller-dim additive projection
    expect_components_equal(a, { 4.0, 2.0, 2.0 });
}

TEST(stamp_complex_Arithmetic, ScalarMultiplyDivide) {
    cayley_dickson_construct<double, 2> q{ 2.0, 3.0 };
    auto mul = q * 2.0;
    expect_components_equal(mul, { 4.0, 6.0 });

    auto div = q / 2.0;
    expect_components_equal(div, { 1.0, 1.5 });
}

// Group: conjugation, norm, inverse
TEST(stamp_complex_ConjugateNorm, ConjugateFlipsImaginaryParts) {
    cayley_dickson_construct<double, 4> q{ 1.0, 2.0, -3.0, 4.0 };
    auto qc = conjugate(q);
    expect_components_equal(qc, { 1.0, -2.0, 3.0, -4.0 });
}

TEST(stamp_complex_ConjugateNorm, NormAndNorm2) {
    cayley_dickson_construct<double, 3> q{ 1.0, 2.0, 2.0 };
    // norm2 should equal q * conjugate(q) (scalar)
    auto n2 = norm2(q);
    // For assumed semantics, norm2 equals sum of squares of components
    double expected_n2 = 1.0 * 1.0 + 2.0 * 2.0 + 2.0 * 2.0;
    EXPECT_DOUBLE_EQ(static_cast<double>(n2), expected_n2);

    double n = norm(q);
    EXPECT_NEAR(n, std::sqrt(expected_n2), 1e-12);
}

TEST(stamp_complex_ConjugateNorm, InverseRelation) {
    cayley_dickson_construct<double, 2> q{ 3.0, 4.0 };
    auto qinv = inverse(q);
    // q * qinv should have scalar approx 1 and zero imaginary parts (assumed behavior)
    auto prod = q * qinv;
    // scalar part approx 1
    EXPECT_NEAR(static_cast<double>(prod[0]), 1.0, 1e-10);
    // imaginary parts approx 0
    for (unsigned i = 1; i < 2; ++i) {
        EXPECT_NEAR(static_cast<double>(prod[i]), 0.0, 1e-10);
    }
}

// Group: user-defined literals
TEST(stamp_complex_Literals, ImaginaryLiteralsProduceProperConstructs) {
    auto li = 1.5i; // long double literal -> cayley_dickson_construct<long double,2>
    static_assert(std::is_same_v<decltype(li), cayley_dickson_construct<long double, 2>>);
    expect_components_equal(li, { 0.0L, 1.5L });

    auto lj = 2.25j;
    static_assert(std::is_same_v<decltype(lj), cayley_dickson_construct<long double, 3>>);
    expect_components_equal(lj, { 0.0L, 0.0L, 2.25L });

    auto lk = 0.5k;
    static_assert(std::is_same_v<decltype(lk), cayley_dickson_construct<long double, 4>>);
    expect_components_equal(lk, { 0.0L, 0.0L, 0.0L, 0.5L });
}

// Group: iterators / range access
TEST(stamp_complex_Iterators, BeginEndAndRangeFor) {
    cayley_dickson_construct<double, 4> q{ 1.0, 2.0, 3.0, 4.0 };
    double sum = 0.0;
    for (auto it = q.begin(); it != q.end(); ++it) sum += *it;
    EXPECT_DOUBLE_EQ(sum, 10.0);

    // modify via iterator
    for (auto it = q.begin(); it != q.end(); ++it) *it = 1.0;
    expect_components_equal(q, { 1.0, 1.0, 1.0, 1.0 });
}

#endif // SRC_MATH_TEST_H