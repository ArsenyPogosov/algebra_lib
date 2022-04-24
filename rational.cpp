#include "rational.h"
#include <string>
using namespace std;

Rational::Rational() : a(0), b(1) {}

Rational& Rational::operator++() {
	a += b;
	return *this;
}

Rational Rational::operator++(int) {
	Rational result = *this;
	a += b;
	return result;
}

Rational& Rational::operator--() {
	a -= b;
	return *this;
}

Rational Rational::operator--(int) {
	Rational result = *this;
	a -= b;
	return result;
}

Rational Rational::operator+() const {
	return Rational(+a, b);
}

Rational Rational::operator-() const {
	return Rational(-a, b);
}

Rational operator+(const Rational& first, const Rational& second) {
	return Rational(first.a * second.b + second.a * first.b, first.b * second.b);
}

Rational operator-(const Rational& first, const Rational& second) {
	return Rational(first.a * second.b - second.a * first.b, first.b * second.b);
}

Rational operator*(const Rational& first, const Rational& second) {
	return Rational(first.a * second.a, first.b * second.b);
}

Rational operator/(const Rational& first, const Rational& second) {
	return Rational(first.a * second.b, first.b * second.a);
}

Rational& Rational::operator+=(const Rational& second) {
	return *this = *this + second;
}

Rational& Rational::operator-=(const Rational& second) {
	return *this = *this - second;
}

Rational& Rational::operator*=(const Rational& second) {
	return *this = *this * second;
}

Rational& Rational::operator/=(const Rational& second) {
	return *this = *this / second;
}

bool operator==(const Rational& first, const Rational& second) {
	return first.a == second.a && first.b == second.b;
}

bool operator!=(const Rational& first, const Rational& second) {
	return first.a != second.a || first.b != second.b;
}

bool operator<(const Rational& first, const Rational& second) {
	return (first - second).a < 0;
}

bool operator<=(const Rational& first, const Rational& second) {
	return (first - second).a <= 0;
}

bool operator>(const Rational& first, const Rational& second) {
	return (first - second).a > 0;
}

bool operator>=(const Rational& first, const Rational& second) {
	return (first - second).a >= 0;
}

std::ostream& operator<<(std::ostream& out, const Rational& r) {
	if (r.b == 1)
		return out << r.a;
	return out << to_string(r.a) + '/' + to_string(r.b);
}

void Rational::Reduce() {
	const auto count_gcd = [](long long a, long long b) -> long long {
		a = abs(a); b = abs(b);
		while (a > 0) {
			b %= a;
			swap(a, b);
		}
		return b;
	};

	long long gcd = count_gcd(a, b);
	a /= gcd; b /= gcd;

	if (b < 0) {
		a = -a; b = -b;
	}
}

namespace std {
	Rational abs(const Rational& r) {
		return (r < 0 ? -r : r);
	}

	Rational min(const Rational& first, const Rational& second) {
		return (first < second ? first : second);
	}

	Rational max(const Rational& first, const Rational& second) {
		return (first > second ? first : second);
	}
}
