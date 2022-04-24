#pragma once

#include "matrix.h"

#include <vector>

template<typename T>
using Basis = std::vector<Matrix<T>>;

template<typename T>
Matrix<T> BasisToMatrix(const Basis<T>& basis)
{
	if (basis.empty()) return Matrix<T>(0, 0);

	Matrix<T> res(basis[0].Height(), basis.size());
	for (int i = 0; i < res.Height(); ++i)
		for (int j = 0; j < res.Width(); ++j)
			res[i][j] = basis[j][i][0];

	return res;
}

template<typename T>
Basis<T> MatrixToBasis(const Matrix<T>& matrix)
{
	if (matrix.Width() == 0) return Basis<T>(0);

	Basis<T> res(matrix.Width(), Matrix<T>(matrix.Height(), 1));
	for (int i = 0; i < matrix.Height(); ++i)
		for (int j = 0; j < matrix.Width(); ++j)
			res[j][i][0] = matrix[i][j];

	return res;
}

template<typename T>
Basis<T> BasisSimplify(Basis<T> v)
{
	return MatrixToBasis(BasisToMatrix(v).Transpose().ToLadderForm().Transpose());
}

template<typename T>
Basis<T> SpanBasis(Basis<T> v)
{
	if (v.empty()) return v;

	Matrix<T> h = BasisToMatrix(v);
	h.Transpose();

	int i = 0;
	for (int j = 0; i < h.Height() && j < h.Width(); ++j) {
		for (int i1 = i; i1 < h.Height(); ++i1) {
			if (h[i1][j] != 0) {
				// for it to be stable (abcd -> dabc)
				for (int x = i1 - 1; x >= i; --x)
				{
					swap(h[x], h[x + 1]);
					std::swap(v[x], v[x + 1]);
				}
				break;
			}
		}

		if (h[i][j] == 0) {
			continue;
		}

		T d = 1 / h[i][j];
		for (auto& k : h[i]) {
			k *= d;
		}

		for (int i1 = 0; i1 < h.Height(); ++i1) {
			if (i1 == i) {
				continue;
			}

			T d = h[i1][j];
			for (int j1 = 0; j1 < h.Width(); ++j1) {
				h[i1][j1] -= d * h[i][j1];
			}
		}
		++i;
	}
	v.resize(i);

	return v;
}


template<typename T>
Basis<T> SumBasis(Basis<T> u, Basis<T> v)
{
	u.insert(u.end(), v.begin(), v.end());
	return SpanBasis(u);
}

template<typename T>
Basis<T> KerBasis(Matrix<T> A);

template<typename T>
Basis<T> IntersectionBasis(Basis<T> u, Basis<T> v)
{
	v.insert(v.begin(), u.begin(), u.end());
	Basis<T> res = KerBasis(BasisToMatrix(v));

	Matrix<T> U = BasisToMatrix(u);
	for (auto& i : res)
	{
		i.GetData().resize(u.size());
		i = U * i;
	}

	return SpanBasis(res);
}

template<typename T>
Basis<T> KerBasis(Matrix<T> A)
{
	A.ToLadderForm();
	std::vector<int> mainvar, freevar;
	for (int i = 0, j = 0; j < A.Width(); ++j)
	{
		if (i >= A.Height() || A[i][j] == 0)
			freevar.push_back(j);
		else
		{
			mainvar.push_back(j);
			++i;
		}
	}

	Basis<T> res;
	for (auto& j : freevar)
	{
		Matrix<T> cur(A.Width(), 1);
		for (int i = 0; i < mainvar.size(); ++i)
			cur[mainvar[i]][0] = -A[i][j];

		cur[j][0] = 1;

		res.push_back(cur);
	}

	return res;
}

template<typename T>
Basis<T> ImBasis(Matrix<T> A)
{
	return SpanBasis(MatrixToBasis(A));
}

template<typename T>
Basis<T> EigenBasis(Matrix<T> A, T k)
{
	return KerBasis((A - k));
}

template<typename T>
Basis<T> RootBasis(Matrix<T> A, T k)
{
	return KerBasis((A - k) ^ A.Height());
}

template<typename T>
Matrix<T> Projector(Basis<T> u, Basis<T> v)
{
	int k = u.size();
	u.insert(u.end(), v.begin(), v.end());
	
	Matrix<T> F = BasisToMatrix(u), res(F.Height(), F.Width());
	for (int i = 0; i < k; ++i)
		res[i][i] = 1;

	return F * res * (F ^ -1);
}