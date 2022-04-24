#pragma once

#include <cstdint>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>

template<typename T>
class Poly {
public:
    Poly() {
    }

    Poly(T c) {
        if (c != 0) {
            coefficients_[0] = c;
        }
    }

    Poly(std::vector<T> coefficients) {
        for (size_t i = 0; i < coefficients.size(); ++i) {
            if (coefficients[i] != T{0}) {
                coefficients_[i] = coefficients[i];
            }
        }
    }

    Poly(std::vector<std::pair<int, T>> coefficients) {
        for (const auto& [i, ai] : coefficients) {
            if (ai) {
                coefficients_[i] = ai;
            }
        }
    }

    Poly(const Poly& from) = default;
    Poly(Poly && from) = default;

    Poly& operator=(const Poly & from) = default;
    Poly& operator=(Poly && from) = default;

    T operator()(T x) const {
        T result = 0;

        const auto quick_power = [](T x, size_t k) -> T {
            T result = 1, pi = x;
            for (size_t i = 0; (1 << i) <= k; ++i) {
                if (i != 0) {
                    pi *= pi;
                }
                if ((k >> i) & 1) {
                    result *= pi;
                }
            }

            return result;
        };

        for (const auto& [i, ai] : coefficients_) {
            result += ai * quick_power(x, i);
        }

        return result;
    }

    bool operator==(const Poly& second) const {
        return coefficients_ == second.coefficients_;
    }

    bool operator!=(const Poly& second) const {
        return coefficients_ != second.coefficients_;
    }

    Poly operator+() const {
        return *this;
    }

    Poly operator-() const {
        Poly result;
        result.coefficients_ = coefficients_;
        for (auto& [i, ai] : result.coefficients_) {
            ai = -ai;
        }
        return result;
    }

    Poly operator+(const Poly& second) const {
        Poly result;
        result.coefficients_ = coefficients_;
        for (const auto& [i, ai] : second.coefficients_) {
            result.coefficients_[i] += ai;
            if (result.coefficients_[i] == T{ 0 }) {
                result.coefficients_.erase(i);
            }
        }

        return result;
    }

    Poly operator-(const Poly& second) const {
        Poly result;
        result.coefficients_ = coefficients_;
        for (const auto& [i, ai] : second.coefficients_) {
            result.coefficients_[i] -= ai;
            if (result.coefficients_[i] == T{ 0 }) {
                result.coefficients_.erase(i);
            }
        }

        return result;
    }

    Poly& operator+=(const Poly& second) {
        for (const auto& [i, ai] : second.coefficients_) {
            coefficients_[i] += ai;
            if (coefficients_[i] == T{ 0 }) {
                coefficients_.erase(i);
            }
        }

        return *this;
    }

    Poly& operator-=(const Poly& second) {
        for (const auto& [i, ai] : second.coefficients_) {
            coefficients_[i] -= ai;
            if (coefficients_[i] == T{ 0 }) {
                coefficients_.erase(i);
            }
        }

        return *this;
    }

    Poly operator*(const Poly& second) const {
        Poly result;

        for (const auto& [i, ai] : coefficients_) {
            for (const auto& [j, bj] : second.coefficients_) {
                result.coefficients_[i + j] += ai * bj;
                if (result.coefficients_[i] == T{ 0 }) {
                    result.coefficients_.erase(i);
                }
            }
        }

        return result;
    }

    Poly& operator*=(const Poly& second) {
        return *this = (*this) * second;
    }

    friend std::ostream& operator<<(std::ostream& out, const Poly& poly) {
        if (poly.coefficients_.empty()) {
            return out << "0";
        }

        std::vector<std::pair<size_t, T>> sorted_coefficients;
        for (const auto& coefficient : poly.coefficients_) {
            sorted_coefficients.push_back(coefficient);
        }
        std::sort(sorted_coefficients.begin(), sorted_coefficients.end(), std::greater<std::pair<size_t, T>>());

        std::ostringstream tout;
        for (const auto& [i, ai] : sorted_coefficients) {
            if (i != sorted_coefficients.front().first) {
                if (ai > 0) {
                    tout << "+";
                }
                else {
                    tout << "-";
                }
            }
            else {
                if (ai < 0) {
                    tout << '-';
                }
            }

            if (std::max(ai, -ai) != T{1} || i == 0) {
                tout << std::max(ai, -ai);
            }

            if (i != 0) {
                tout << 't';
            }

            if (i >= 2) {
                tout << '^' << i;
            }
        }

        return out << tout.str();
    }

    T operator[](size_t i)
    {
        T result = coefficients_[i];
        if (result == T{ 0 })
            coefficients_.erase(i);
        return result;
    }

private:
    std::unordered_map<int, T> coefficients_;
};
