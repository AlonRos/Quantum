#include "Matrix2.h"
#include <algorithm>
#include <iostream>

using namespace std;

Matrix2::Matrix2(int m, int n) : m(m), n(n), rowwise(true), jump(n) {
	elements = (complex_t*) calloc(m * n * sizeof(complex_t), 1);
}

Matrix2::Matrix2(int m, int n, bool rowwise) : m(m), n(n), rowwise(rowwise) {
	jump = rowwise ? n : m;

	elements = (complex_t*) malloc(m * n * sizeof(complex_t));
}

Matrix2::Matrix2(int m, int n, complex_t* elements, bool rowwise, int jump) : m(m), n(n), elements(elements), rowwise(rowwise), jump(jump) {}

Matrix2::Matrix2(int m, int n, complex_t* arr) : m(m), n(n), rowwise(true), jump(n) {
	elements = (complex_t*)malloc(m * n * sizeof(complex_t));
	copy(arr, arr + m * n, elements);
}

Matrix2::~Matrix2() {
	free(elements);
}

complex_t& Matrix2::entry(int rowIndex, int colIndex) {
	if (rowwise) {
		return elements[rowIndex * jump + colIndex];
	}
	else {
		return elements[colIndex * jump + rowIndex];
	}
}

Matrix2& Matrix2::rows(int i, int j) {
	complex_t* rowsElements = &entry(i, 0);
	return *new Matrix2(j - i, n, rowsElements, rowwise, jump);
}

Matrix2& Matrix2::cols(int i, int j) {
	complex_t* colsElements = &entry(0, i);
	return *new Matrix2(m, j - i, colsElements, rowwise, jump);
}

Matrix2& Matrix2::transpose() {
	return *new Matrix2(n, m, elements, !rowwise, jump);
}

Matrix2& Matrix2::conjTranspose() {
	Matrix2* retMatrix = new Matrix2(n, m, !rowwise);

	int newJump;
	if (rowwise) {
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				retMatrix->entry(j, i) = conj(entry(i, j));
			}
		}
		newJump = n;
	}

	else {
		for (int j = 0; j < n; ++j) {
			for (int i = 0; i < m; ++i) {
				retMatrix->entry(j, i) = conj(entry(i, j));
			}
		}
		newJump = m;
	}

	return *retMatrix;
}

double Matrix2::normSquared() {
	double res = 0;

	if (rowwise) {
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				res += complexNormSquared(entry(i, j));
			}
		}
	}
	else {
		for (int j = 0; j < n; ++j) {
			for (int i = 0; i < m; ++i) {
				res += complexNormSquared(entry(i, j));
			}
		}
	}

	return res;
}

void Matrix2::cpuAddIn(Matrix2& A, Matrix2& B, Matrix2& saveIn) {
	if (A.m != B.m or A.n != B.n) {
		throw Exception(runtime_error, "Cannot multiply a {} x {} matrix with a {} x {} matrix", A.m, A.n, B.m, B.n);
	}

	int m = A.m, n = A.n;

	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			saveIn.entry(i, j) = A.entry(i, j) + B.entry(i, j);
		}
	}
}

void Matrix2::cpuMultIn(Matrix2& A, Matrix2& B, Matrix2& saveIn) {
	if (A.n != B.m) {
		throw Exception(runtime_error, "Cannot multiply a {} x {} matrix with a {} x {} matrix", A.m, A.n, B.m, B.n);
	}

	complex_t* res = new complex_t[A.m * B.n];
	register int help1 = 0, help2 = 0, sum, i = 0, j = 0, k = 0;
	complex_t temp;

	for (i = 0; i < A.n; ++i) {
		help2 = 0;
		for (k = 0; k < A.n; ++k) {
			temp = A.elements[help1 + k];
			for (j = 0; j < A.n; j++) {
				res[help1 + j] += temp * B.elements[help2 + j];
			}
			help2 += A.n;
		}
		help1 += A.n;
	}

	for (int i = 0; i < A.m; ++i) {
		for (int j = 0; j < B.n; ++j) {
			saveIn.entry(i, j) = res[B.n * i + j];
		}
	}
}

Matrix2& Matrix2::operator*(complex_t scalar) {
	Matrix2* retMatrix = new Matrix2(m, n, rowwise);

	int newJump;

	if (rowwise) {
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				retMatrix->entry(i, j) = entry(i, j) * scalar;
			}
		}
		newJump = n;
	}

	else {
		for (int j = 0; j < n; ++j) {
			for (int i = 0; i < m; ++i) {
				retMatrix->entry(i, j) = entry(i, j) * scalar;
			}
		}
		newJump = m;
	}

	return *retMatrix;
}

Matrix2& Matrix2::randomMatrix(int m, int n, int bound) {
	Matrix2* returnMatrix = new Matrix2(m, n, true);

	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			returnMatrix->entry(i, j) = complex_t(randBound(bound), randBound(bound));
		}
	}

	return *returnMatrix;

}

void Matrix2::print() {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (entry(i, j).imag() > 0) {
				cout << entry(i, j).real() << "+" << entry(i, j).imag() << "i" << " ";
			}
			else if (entry(i, j).imag() < 0) {
				cout << entry(i, j).real() << entry(i, j).imag() << "i" << " ";
			}
			else {
				cout << entry(i, j).real() << " ";

			}
		}
		cout << '\n';
	}
}