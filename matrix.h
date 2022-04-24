#pragma once

#include "poly.h"
#include "permutation.h"

#include <iomanip>
#include <iostream>
#include <functional>
#include <sstream>
#include <vector>
#include <exception>

class UnsuitableMatrixSizes : public std::exception {
public:
	UnsuitableMatrixSizes(const char* whatStr = "unsuitable matrix sizes") : whatStr(whatStr) {}

	const char* what() const override {
		return whatStr;
	}

private:
	const char* whatStr;
};

template<typename T>
class Matrix {
public:
	Matrix(): data() {}
	Matrix(size_t height, size_t width) : data(height, std::vector<T>(width)) {};
	Matrix(size_t height, size_t width, T value) : data(height, std::vector<T>(width, value)) {};
	explicit Matrix(std::vector<std::vector<T>> data): data(data) {};
	template<typename T2>
	Matrix(const Matrix<T2>& second): data(second.Height(), second.Width()) {
		for (size_t i = 0; i < second.Height(); ++i) {
			for (size_t j = 0; j < second.Width(); ++j) {
				data[i][j] = static_cast<T2>(second.data[i][j]);
			}
		}
	}

	static Matrix E(size_t height, size_t width) {
		Matrix result(height, width);
		for (size_t i = 0; i < height && i < width; ++i) {
			result[i][i] = static_cast<T>(1);
		}

		return result;
	}

	operator std::vector<std::vector<T>>& () {
		return data;
	};
	operator const std::vector<std::vector<T>>&() const {
		return data;
	};
	std::vector<std::vector<T>>& GetData() {
		return data;
	}
	const std::vector<std::vector<T>>& GetData() const {
		return data;
	}

	size_t Height() const {
		return data.size();
	}
	size_t Width() const {
		return (Height() ? data.front().size() : 0);
	}

	typename std::vector<std::vector<T>>::iterator begin() {
		return data.begin();
	}
	typename std::vector<std::vector<T>>::const_iterator begin() const {
		return data.begin();
	}
	typename std::vector<std::vector<T>>::iterator end() {
		return data.end();
	}
	typename std::vector<std::vector<T>>::const_iterator end() const {
		return data.end();
	}

	std::vector<T>& operator[](size_t i) {
		return data[i];
	}
	const std::vector<T>& operator[](size_t i) const {
		return data[i];
	}

	friend bool operator==(const Matrix& first, const Matrix& second) {
		return first.GetData() == second.GetData();
	}
	friend bool operator!=(const Matrix& first, const Matrix& second) {
		return first.GetData() != second.GetData();
	}

	Matrix operator+() const {
		return *this;
	}
	Matrix operator-() const {
		Matrix result = *this;
		for (auto& i : result) {
			for (auto& j : i) {
				j = -j;
			}
		}

		return result;
	}

	friend Matrix operator+(const Matrix& first, const Matrix& second) {
		if (first.Height() != second.Height() || first.Width() != second.Width()) {
			throw UnsuitableMatrixSizes("operator+ must take two matrices of the same length");
		}

		Matrix result(first.Height(), first.Width());
		for (size_t i = 0; i < first.Height(); ++i) {
			for (size_t j = 0; j < first.Width(); ++j) {
				result[i][j] = first[i][j] + second[i][j];
			}
		}

		return result;
	}
	friend Matrix operator-(const Matrix& first, const Matrix& second) {
		return first + (-second);
	}
	friend Matrix operator*(const Matrix& first, const Matrix& second) {
		if (first.Width() != second.Height()) {
			throw UnsuitableMatrixSizes("operator* must take two matrices such that the width of the first matrix is ​​equal to the height of the second");
		}
		Matrix result(first.Height(), second.Width());
		for (size_t i = 0; i < first.Height(); ++i) {
			for (size_t j = 0; j < second.Width(); ++j) {
				for (size_t k = 0; k < first.Width(); ++k) {
					result[i][j] += first[i][k] * second[k][j];
				}
			}
		}

		return result;
	}
	friend Matrix operator^(const Matrix& first, long long second) {
		if (first.Height() != first.Width()) {
			throw UnsuitableMatrixSizes("operator^ must take squere matrix");
		}

		if (second < 0) return (first ^ (-second)).Inverse();

		Matrix result = E(first.Height(), first.Width());
		int i = 0;
		for (Matrix curr = first; 0 < (second >> i); curr *= curr, ++i) {
			if ((second >> i) & 1) {
				result *= curr;
			}
		}

		return result;
	}

	Matrix& operator+=(const Matrix& second) {
		return *this = *this + second;
	}
	Matrix& operator-=(const Matrix& second) {
		return *this = *this - second;
	}
	Matrix& operator*=(const Matrix& second) {
		return *this = *this * second;
	}
	Matrix& operator^=(long long second) {
		return *this = ((*this) ^ second);
	}

	Matrix& Transpose() {
		Matrix<T> res(Width(), Height());
		for (int i = 0; i < Height(); ++i)
			for (int j = 0; j < Width(); ++j)
				res[j][i] = (*this)[i][j];

		return *this = res;
	}

	Matrix& ToLadderForm() {
		for (int i = 0, j = 0; i < Height() && j < Width(); ++j) {
			for (int i1 = i; i1 < Height(); ++i1) {
				if ((*this)[i1][j] != 0) {
					swap((*this)[i], (*this)[i1]);
					break;
				}
			}

			if ((*this)[i][j] == 0) {
				continue;
			}

			T d = 1 / (*this)[i][j];
			for (auto& k : (*this)[i]) {
				k *= d;
			}

			for (int i1 = 0; i1 < Height(); ++i1) {
				if (i1 == i) {
					continue;
				}

				T d = (*this)[i1][j];
				for (int j1 = 0; j1 < Width(); ++j1) {
					(*this)[i1][j1] -= d * (*this)[i][j1];
				}
			}
			++i;
		}

		return *this;
	}

	Matrix& Inverse() {
		if (Height() != Width()) {
			throw UnsuitableMatrixSizes("Inverse must take squere matrix");
		}

		Matrix help = *this;
		*this = E(Height(), Width());
		for (int i = 0, j = 0; i < Height() && j < Width(); ++j) {
			for (int i1 = i; i1 < Height(); ++i1) {
				if (help[i1][j] != 0) {
					swap(help[i], help[i1]);
					swap((*this)[i], (*this)[i1]);
					break;
				}
			}

			if (help[i][j] == 0) {
				continue;
			}

			T d = 1 / help[i][j];
			for (auto& k : help[i]) {
				k *= d;
			}
			for (auto& k : (*this)[i]) {
				k *= d;
			}

			for (int i1 = 0; i1 < Height(); ++i1) {
				if (i1 == i) {
					continue;
				}

				T d = help[i1][j];
				for (int j1 = 0; j1 < Width(); ++j1) {
					help[i1][j1] -= d * help[i][j1];
					(*this)[i1][j1] -= d * (*this)[i][j1];
				}
			}
			++i;
		}

		if (help != E(Height(), Width()))
		{
			throw std::exception("Degenerate matrix");
		}

		return *this;
	}

	Poly<T> CharacteristicPoly()
	{
		if (Width() != Height()) {
			throw UnsuitableMatrixSizes("CharacteristicPoly must take squere matrix");
		}
		Poly<T> result{};

		Permutation c(Width());
		do
		{
			Poly<T> current{c.Sgn()};
			for (size_t i = 0; i < Width(); ++i)
				current *= Poly<T>(std::vector<T>{ T{-data[i][c[i]]}, T{int(i == c[i])}});
			result += current;
		} while (c.Next());

		return result;
	}

	/*
	T Det()
	{
		if (Width() != Height()) {
			throw UnsuitableMatrixSizes("Det must take squere matrix");
		}
		return (Width() % 2 ? -1 : +1) * CharacteristicPoly()(T{ 0 });
	}
	*/

	T Det()
	{
		T result{ 0 };
		Permutation c(Width());
		do
		{
			T current{ c.Sgn() };
			for (size_t i = 0; i < Width(); ++i)
				current *= data[i][c[i]];
			result = result + current;
		} while (c.Next());

		return result;
	}

	friend std::ifstream& operator>>(std::istream& in, Matrix& m) {
		size_t n, m;
		in >> n >> m;
		m = std::vector<std::vector<T>>(n, m);
		for (auto& i : m) {
			for (auto& j : i) {
				in >> j;
			}
		}

		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const Matrix& m) {
		std::vector<size_t> colum_len(m.Width());
		for (size_t j = 0; j < m.Width(); ++j) {
			for (const auto& i : m) {
				std::ostringstream curr;
				curr << i[j];
				colum_len[j] = std::max(colum_len[j], 
					curr.str().size());
			}
		}

		for (size_t i = 0; i < m.Height(); ++i) {
			out << "|";
			for (size_t j = 0; j < m.Width(); ++j) {
				out << std::setw(colum_len[j] + (j != 0)) << m[i][j];
			}
			out << "|";
			if (i + 1 < m.Height()) {
				out << '\n';
			}
		}

		return out;
	}

private:
	std::vector<std::vector<T>> data;
};

template<typename T>
class DynamicMatrix {
public:
	DynamicMatrix() : get([](size_t, size_t) -> T { return {}; }) {}
	explicit DynamicMatrix(std::function<T(size_t, size_t)> get): get(get) {}
	explicit DynamicMatrix(T value) : get([=](size_t i, size_t j) -> T { return (i == j ? value : T{}); }) {}

	Matrix<T> FixSizes(size_t height, size_t width) const {
		Matrix<T> result(height, width);
		for (size_t i = 0; i < height; ++i) {
			for (size_t j = 0; j < width; ++j) {
				result[i][j] = get(i, j);
			}
		}

		return result;
	}

	DynamicMatrix operator+() const {
		return *this;
	}
	DynamicMatrix operator-() const {
		return DynamicMatrix([=](size_t i, size_t j) {
			return -(this->get(i, j));
		});	
	}

	friend DynamicMatrix operator+(const DynamicMatrix& first, const DynamicMatrix& second) {
		return DynamicMatrix([first, second](size_t i, size_t j) { 
			return first.get(i, j) + second.get(i, j); 
		});
	}
	friend DynamicMatrix operator-(const DynamicMatrix& first, const DynamicMatrix& second) {
		return first + (-second);
	}

	// In pair with Matrix
public:
	friend bool operator==(const Matrix<T>& first, const DynamicMatrix& second) {
		return first == second.FixSizes(first.Height(), first.Width());
	}
	friend bool operator==(const  DynamicMatrix& first, const Matrix<T>& second) {
		return first.FixSizes(second.Height(), second.Width()) == second;
	}
	friend bool operator!=(const Matrix<T>& first, const DynamicMatrix& second) {
		return first == second.FixSizes(first.Height(), first.Width());
	}
	friend bool operator!=(const  DynamicMatrix& first, const Matrix<T>& second) {
		return first.FixSizes(second.Height(), second.Width()) != second;
	}

	friend Matrix<T> operator+(const Matrix<T>& first, const DynamicMatrix& second) {
		return first + second.FixSizes(first.Height(), first.Width());
	}
	friend Matrix<T> operator+(const DynamicMatrix& first, const Matrix<T>& second) {
		return first.FixSizes(second.Height(), second.Width()) + second;
	}
	friend Matrix<T> operator-(const Matrix<T>& first, const DynamicMatrix& second) {
		return first - second.FixSizes(first.Height(), first.Width());
	}
	friend Matrix<T> operator-(const DynamicMatrix& first, const Matrix<T>& second) {
		return first.FixSizes(second.Height(), second.Width()) - second;
	}
	friend Matrix<T> operator*(const Matrix<T>& first, const DynamicMatrix& second) {
		return first * second.FixSizes(first.Width(), first.Width());
	}
	friend Matrix<T> operator*(const DynamicMatrix& first, const Matrix<T>& second) {
		return first.FixSizes(second.Height(), second.Height()) * second;
	}

private:
	std::function<T(size_t, size_t)> get;
};

template<typename T, typename T2>
bool operator==(const Matrix<T>& first, const T2& second) {
	return first == static_cast<DynamicMatrix<T>>(static_cast<T>(second)).FixSizes(first.Height(), first.Width());
}
template<typename T, typename T2>
bool operator==(const T2& first, const Matrix<T>& second) {
	return static_cast<DynamicMatrix<T>>(static_cast<T>(first)).FixSizes(second.Height(), second.Width()) == second;
}
template<typename T, typename T2>
bool operator!=(const Matrix<T>& first, const T2& second) {
	return first != static_cast<DynamicMatrix<T>>(static_cast<T>(second)).FixSizes(first.Height(), first.Width());
}
template<typename T, typename T2>
bool operator!=(const T2& first, const Matrix<T>& second) {
	return static_cast<DynamicMatrix<T>>(static_cast<T>(first)).FixSizes(second.Height(), second.Width()) != second;
}


template<typename T, typename T2>
Matrix<T> operator+(const Matrix<T>& first, const T2& second) {
	return first + static_cast<DynamicMatrix<T>>(static_cast<T>(second)).FixSizes(first.Height(), first.Width());
}
template<typename T, typename T2>
Matrix<T> operator+(const T2& first, const Matrix<T>& second) {
	return static_cast<DynamicMatrix<T>>(static_cast<T>(first)).FixSizes(second.Height(), second.Width()) + second;
}
template<typename T, typename T2>
Matrix<T> operator-(const Matrix<T>& first, const T2& second) {
	return first - static_cast<DynamicMatrix<T>>(static_cast<T>(second)).FixSizes(first.Height(), first.Width());
}
template<typename T, typename T2>
Matrix<T> operator-(const T2& first, const Matrix<T>& second) {
	return static_cast<DynamicMatrix<T>>(static_cast<T>(first)).FixSizes(second.Height(), second.Width()) - second;
}
template<typename T, typename T2>
Matrix<T> operator*(const Matrix<T>& first, const T2& second) {
	return first * static_cast<DynamicMatrix<T>>(static_cast<T>(second)).FixSizes(first.Width(), first.Width());
}
template<typename T, typename T2>
Matrix<T> operator*(const T2& first, const Matrix<T>& second) {
	return static_cast<DynamicMatrix<T>>(static_cast<T>(first)).FixSizes(second.Height(), second.Height()) * second;
}