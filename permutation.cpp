#include "permutation.h"

Permutation::Permutation(int n) : data{std::vector<int>(n)}
{
	for (int i = 0; i < n; ++i)
		data[i] = i;
}

Permutation::Permutation(std::vector<int> _data) : data{ _data } {}

bool operator==(const Permutation& first, const Permutation& second)
{
	return first.data == second.data;
}

Permutation operator*(const Permutation& first, const Permutation& second)
{
	int n = std::max(first.size(), second.size());
	std::vector<int> res(n);
	for (int i = 0; i < n; ++i)
	{
		int j = (i < second.size() ? second[i] : i);
		res[i] = (j < first.size() ? first[j] : j);
	}

	return res;
}

Permutation operator/(const Permutation& first, const Permutation& second)
{
	return first * second.Pow(-1);
}

Permutation& Permutation::operator*=(const Permutation& second)
{
	return *this = *this * second;
}

Permutation& Permutation::operator/=(const Permutation& second)
{
	return *this = *this / second;
}

Permutation& Permutation::Reverse()
{
	std::vector<int> temp = data;
	for (int i = 0; i < size(); ++i)
		data[temp[i]] = i;

	return *this;
}

Permutation Permutation::Pow(int k) const
{
	if (k < 0)
		return (this->Pow(-k)).Reverse();
	if (k == 0)
		return Permutation(size());

	Permutation res = this->Pow(k / 2);
	res *= res;
	if (k % 2) res *= *this;

	return res;
}

int Permutation::Sgn() const
{
	bool even = true;
	std::vector<bool> used(size());
	for (auto& i : data)
		if (!used[i])
		{
			even ^= 1;
			for (int j = i; !used[j]; j = data[j])
			{
				even ^= 1;
				used[j] = true;
			}
		}

	return (even ? +1 : -1);
}

bool Permutation::Next()
{
	if (size() <= 1) return true;

	int i;
	for (i = size() - 2; i >= 0 && data[i] > data[i + 1]; --i);
	if (i >= 0)
		std::swap(data[i], *(upper_bound(data.begin() + i + 1, data.end(), data[i], std::greater<int>()) - 1));

	std::reverse(data.begin() + (i + 1), data.end());

	return (i >= 0);
}

size_t Permutation::size() const
{
	return data.size();
}

int Permutation::operator[](size_t i) const
{
	return data[i];
}

std::ostream& operator<<(std::ostream& out, const Permutation& perm)
{
	for (auto& i : perm.data)
		out << i << ' ';
	return out;
}

Permutation Cycle(std::vector<int>&& c, int n)
{
	if (n == -1)
		n = *std::max_element(c.begin(), c.end()) + 1;

	std::vector<int> res(n);
	for (int i = 0; i < n; ++i)
		res[i] = i;
	for (int i = 0; i < c.size(); ++i)
		res[c[i]] = c[(i + 1) % c.size()];

	return res;
}