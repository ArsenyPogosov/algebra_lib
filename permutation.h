#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

class Permutation
{
public:
	explicit Permutation(int n = 0);
	Permutation(std::vector<int> _data);

	friend bool operator==(const Permutation& first, const Permutation& second);

	friend Permutation operator*(const Permutation& first, const Permutation& second);
	friend Permutation operator/(const Permutation& first, const Permutation& second);
	Permutation& operator*=(const Permutation& second);
	Permutation& operator/=(const Permutation& second);

	Permutation& Reverse();
	Permutation Pow(int k) const;

	int Sgn() const;

	bool Next();

	size_t size() const;
	int operator[](size_t i) const;

	friend std::ostream& operator<<(std::ostream& out, const Permutation& perm);

private:
	std::vector<int> data;
};

Permutation Cycle(std::vector<int>&& c, int n = -1);