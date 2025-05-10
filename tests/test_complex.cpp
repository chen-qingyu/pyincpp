#include "../sources/complex.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Complex")
{
    SECTION("basics")
    {
        // Complex(double real = 0, double imag = 0)
        Complex c1;
        Complex c2(2);
        Complex c3(2, 3);

        // Complex(const Complex& that)
        Complex c4(c3);

        // Complex(Complex&& that)
        Complex c5(std::move(c4));

        // ~Complex()
    }

    Complex zero;
    Complex positive(1, 2);
    Complex negative(-1, 2);

    SECTION("compare")
    {
        REQUIRE(zero == zero);
        REQUIRE(positive == positive);

        REQUIRE(zero != positive);
        REQUIRE(positive != negative);
    }

    SECTION("assignment")
    {
        positive = negative; // copy
        REQUIRE(positive == Complex(-1, 2));
        REQUIRE(negative == Complex(-1, 2));

        zero = std::move(negative); // move
        REQUIRE(zero == Complex(-1, 2));
        REQUIRE(negative == Complex());
    }

    SECTION("examination")
    {
        REQUIRE(zero.real() == 0);
        REQUIRE(positive.real() == 1);
        REQUIRE(negative.real() == -1);

        REQUIRE(zero.imag() == 0);
        REQUIRE(positive.imag() == 2);
        REQUIRE(negative.imag() == 2);

        REQUIRE(zero.abs() == 0);
        REQUIRE(positive.abs() == 2.23606797749979);
        REQUIRE(negative.abs() == 2.23606797749979);

        REQUIRE(zero.arg() == 0);
        REQUIRE(positive.arg() == 1.1071487177940904);
        REQUIRE(negative.arg() == 2.0344439357957027);
    }

    SECTION("unary")
    {
        REQUIRE(+zero == Complex(0));
        REQUIRE(+positive == Complex(1, 2));
        REQUIRE(+negative == Complex(-1, 2));

        REQUIRE(-zero == Complex(0));
        REQUIRE(-positive == Complex(-1, -2));
        REQUIRE(-negative == Complex(1, -2));

        REQUIRE(zero.conjugate() == Complex(0));
        REQUIRE(positive.conjugate() == Complex(1, -2));
        REQUIRE(negative.conjugate() == Complex(-1, -2));
    }

    SECTION("plus")
    {
        REQUIRE(positive + positive == Complex(2, 4));
        REQUIRE(positive + zero == Complex(1, 2));
        REQUIRE(positive + negative == Complex(0, 4));

        REQUIRE(negative + positive == Complex(0, 4));
        REQUIRE(negative + zero == Complex(-1, 2));
        REQUIRE(negative + negative == Complex(-2, 4));

        REQUIRE(zero + positive == Complex(1, 2));
        REQUIRE(zero + zero == Complex(0));
        REQUIRE(zero + negative == Complex(-1, 2));
    }

    SECTION("minus")
    {
        REQUIRE(positive - positive == Complex(0));
        REQUIRE(positive - zero == Complex(1, 2));
        REQUIRE(positive - negative == Complex(2));

        REQUIRE(negative - positive == Complex(-2));
        REQUIRE(negative - zero == Complex(-1, 2));
        REQUIRE(negative - negative == Complex(0));

        REQUIRE(zero - positive == Complex(-1, -2));
        REQUIRE(zero - zero == Complex(0));
        REQUIRE(zero - negative == Complex(1, -2));
    }

    SECTION("times")
    {
        REQUIRE(positive * positive == Complex(-3, 4));
        REQUIRE(positive * zero == Complex(0));
        REQUIRE(positive * negative == Complex(-5));

        REQUIRE(negative * positive == Complex(-5));
        REQUIRE(negative * zero == Complex(0));
        REQUIRE(negative * negative == Complex(-3, -4));

        REQUIRE(zero * positive == Complex(0));
        REQUIRE(zero * zero == Complex(0));
        REQUIRE(zero * negative == Complex(0));
    }

    SECTION("divide")
    {
        REQUIRE(positive / positive == Complex(1));
        REQUIRE_THROWS_MATCHES(positive / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(positive / negative == Complex(0.6, -0.8));

        REQUIRE(negative / positive == Complex(0.6, 0.8));
        REQUIRE_THROWS_MATCHES(negative / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(negative / negative == Complex(1));

        REQUIRE(zero / positive == Complex(0));
        REQUIRE_THROWS_MATCHES(zero / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(zero / negative == Complex(0));
    }

    SECTION("pow")
    {
        REQUIRE(Complex::pow(positive, zero) == Complex(1));
        REQUIRE(Complex::pow(positive, positive) == Complex(-0.22251715680177267, 0.10070913113607541));
        REQUIRE(Complex::pow(positive, negative) == Complex(0.04281551979798478, 0.023517649351954585));

        REQUIRE(Complex::pow(negative, zero) == Complex(1));
        REQUIRE(Complex::pow(negative, positive) == Complex(-0.0335067906880002, -0.018404563532749985));
        REQUIRE(Complex::pow(negative, negative) == Complex(0.006965545047800022, -0.0031525388861500334));

        REQUIRE(Complex::pow(zero, zero) == Complex(1));
        REQUIRE_THROWS_MATCHES(Complex::pow(zero, positive), std::runtime_error, Message("Error: Math domain error."));
        REQUIRE_THROWS_MATCHES(Complex::pow(zero, negative), std::runtime_error, Message("Error: Math domain error."));
    }

    SECTION("print")
    {
        std::ostringstream oss;

        oss << zero;
        REQUIRE(oss.str() == "(0+0j)");
        oss.str("");

        oss << positive;
        REQUIRE(oss.str() == "(1+2j)");
        oss.str("");

        oss << negative;
        REQUIRE(oss.str() == "(-1+2j)");
        oss.str("");
    }

    SECTION("input")
    {
        Complex c1, c2, c3, c4;
        std::istringstream("  +1-2j  \n  233.33 \t -1234-4321j  3j") >> c1 >> c2 >> c3 >> c4;

        REQUIRE(c1 == Complex(1, -2));
        REQUIRE(c2 == Complex(233.33));
        REQUIRE(c3 == Complex(-1234, -4321));
        REQUIRE(c4 == Complex(0, 3));

        std::istringstream("  .1+.2j  \n  .1+2.j \t 1.+.2j  1.+2.j") >> c1 >> c2 >> c3 >> c4;

        REQUIRE(c1 == Complex(0.1, 0.2));
        REQUIRE(c2 == Complex(0.1, 2.0));
        REQUIRE(c3 == Complex(1.0, 0.2));
        REQUIRE(c4 == Complex(1.0, 2.0));

        Complex err;
        REQUIRE_THROWS_MATCHES(std::istringstream("z1+2j") >> err, std::runtime_error, Message("Error: Wrong complex literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("1z+2j") >> err, std::runtime_error, Message("Error: Wrong complex literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("1+z2j") >> err, std::runtime_error, Message("Error: Wrong complex literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("1+2zj") >> err, std::runtime_error, Message("Error: Wrong complex literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("123jj") >> err, std::runtime_error, Message("Error: Wrong complex literal."));
    }
}
