#pragma once

#include <iostream>

class Rational {
public:
    Rational();

    template<typename T>
    Rational(T x) : a(static_cast<long long>(x)), b(1) {
        static_assert(std::is_integral<T>::value, "Integral type required");
    }

    template<typename T>
    Rational(typename std::enable_if<std::is_arithmetic<T>::value, T>::type a, T b) : a(static_cast<long long>(a)), b(static_cast<long long>(b)) {
        if (b == 0)
            throw std::out_of_range("Divide by zero exception");

        Reduce();
    }

    template<typename T>
    explicit operator typename std::enable_if<std::is_integral<T>::value, T>::type() {
        if (std::is_integral<T>::value)
            return static_cast<T>(a / b);

        if (std::is_floating_point<T>::value)
            return static_cast<T>(static_cast<double>(a) / b);
    }

    Rational& operator++();
    Rational operator++(int);
    Rational& operator--();
    Rational operator--(int);

    Rational operator+() const;
    Rational operator-() const;

    friend Rational operator+(const Rational& first, const Rational& second);
    friend Rational operator-(const Rational& first, const Rational& second);
    friend Rational operator*(const Rational& first, const Rational& second);
    friend Rational operator/(const Rational& first, const Rational& second);

    Rational& operator+=(const Rational& second);
    Rational& operator-=(const Rational& second);
    Rational& operator*=(const Rational& second);
    Rational& operator/=(const Rational& second);

    friend bool operator==(const Rational& first, const Rational& second);
    friend bool operator!=(const Rational& first, const Rational& second);
    friend bool operator<(const Rational& first, const Rational& second);
    friend bool operator<=(const Rational& first, const Rational& second);
    friend bool operator>(const Rational& first, const Rational& second);
    friend bool operator>=(const Rational& first, const Rational& second);

    friend std::ostream& operator<<(std::ostream& out, const Rational& r);

    // To resolve multiple condidates issue
public:
    template<typename T>
    friend bool operator==(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first == static_cast<Rational>(second);
    }
    template<typename T>
    friend bool operator==(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) == second;
    }
    template<typename T>
    friend bool operator!=(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first != static_cast<Rational>(second);
    }
    template<typename T>
    friend bool operator!=(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) != second;
    }
    template<typename T>
    friend bool operator<(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first < static_cast<Rational>(second);
    }
    template<typename T>
    friend bool operator<(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) < second;
    }
    template<typename T>
    friend bool operator<=(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first <= static_cast<Rational>(second);
    }
    template<typename T>
    friend bool operator<=(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) <= second;
    }
    template<typename T>
    friend bool operator>(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first > static_cast<Rational>(second);
    }
    template<typename T>
    friend bool operator>(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) > second;
    }
    template<typename T>
    friend bool operator>=(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first >= static_cast<Rational>(second);
    }
    template<typename T>
    friend bool operator>=(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) >= second;
    }

    template<typename T>
    friend Rational operator+(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first * static_cast<Rational>(second);
    }
    template<typename T>
    friend Rational operator+(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) * second;
    }
    template<typename T>
    friend Rational operator-(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first - static_cast<Rational>(second);
    }
    template<typename T>
    friend Rational operator-(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) - second;
    }
    template<typename T>
    friend Rational operator*(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first * static_cast<Rational>(second);
    }
    template<typename T>
    friend Rational operator*(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) * second;
    }
    template<typename T>
    friend Rational operator/(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return first / static_cast<Rational>(second);
    }
    template<typename T>
    friend Rational operator/(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return static_cast<Rational>(first) / second;
    }

private:
    long long a, b;

    void Reduce();
};

namespace std {
    Rational abs(const Rational& r);
    Rational min(const Rational& first, const Rational& second);
    Rational max(const Rational& first, const Rational& second);

    // To resolve multiple condidates issue
    template<typename T>
    Rational min(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return min(first, static_cast<Rational>(second));
    }
    template<typename T>
    Rational min(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return min(static_cast<Rational>(first), second);
    }
    template<typename T>
    Rational max(const Rational& first, typename std::enable_if<std::is_integral<T>::value, T>::type second) {
        return max(first, static_cast<Rational>(second));
    }
    template<typename T>
    Rational max(typename std::enable_if<std::is_integral<T>::value, T>::type first, const Rational& second) {
        return max(static_cast<Rational>(first), second);
    }
}
