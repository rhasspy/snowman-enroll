#include "blas.h"
#include <cstddef>

void cblas_sscal(const int N, const float alpha, float* X, const int stride) {
	int i;
	int m;

	if (N <= 0 || stride <= 0 || alpha == 1.0f) {
		return;
	}
	// Use loop unrolling if the stride is equal to `1`...
	if (stride == 1) {
		m = N % 5;

		// If we have a remainder, run a clean-up loop...
		if (m > 0) {
			for (i = 0; i < m; i++) {
				X[i] *= alpha;
			}
		}
		if (N < 5) {
			return;
		}
		for (i = m; i < N; i += 5) {
			X[i] *= alpha;
			X[i + 1] *= alpha;
			X[i + 2] *= alpha;
			X[i + 3] *= alpha;
			X[i + 4] *= alpha;
		}
		return;
	}
	for (i = 0; i < N * stride; i += stride) {
		X[i] *= alpha;
	}
	return;
}

float cblas_sdot(const int N, const float* X, const int strideX, const float* Y, const int strideY) {
	float dot;
	int ix;
	int iy;
	int m;
	int i;

	dot = 0.0f;
	if (N <= 0) {
		return dot;
	}
	// If both strides are equal to `1`, use unrolled loops...
	if (strideX == 1 && strideY == 1) {
		m = N % 5;

		// If we have a remainder, do a clean-up loop...
		if (m > 0) {
			for (i = 0; i < m; i++) {
				dot += X[i] * Y[i];
			}
		}
		if (N < 5) {
			return dot;
		}
		for (i = m; i < N; i += 5) {
			dot += (X[i] * Y[i]) + (X[i + 1] * Y[i + 1]) + (X[i + 2] * Y[i + 2]) + (X[i + 3] * Y[i + 3]) + (X[i + 4] * Y[i + 4]);
		}
		return dot;
	}
	if (strideX < 0) {
		ix = (1 - N) * strideX;
	} else {
		ix = 0;
	}
	if (strideY < 0) {
		iy = (1 - N) * strideY;
	} else {
		iy = 0;
	}
	for (i = 0; i < N; i++) {
		dot += X[ix] * Y[iy];
		ix += strideX;
		iy += strideY;
	}
	return dot;
}

void cblas_sgemm(
	const enum CBLAS_ORDER Order,	   // {CblasRowMajor=101, CblasColMajor=102} CBLAS_ORDER;
	const enum CBLAS_TRANSPOSE TransA, // {CblasNoTrans=111, CblasTrans=112, CblasConjTrans=113, CblasConjNoTrans=114} CBLAS_TRANSPOSE;
	const enum CBLAS_TRANSPOSE TransB, // {CblasNoTrans=111, CblasTrans=112, CblasConjTrans=113, CblasConjNoTrans=114} CBLAS_TRANSPOSE;
	const int M,
	const int N,
	const int K,
	const float alpha,
	const float* A, // m x k (after transpose if TransA)
	const int lda,	// leading dimension of a
	const float* B, // k x n (after transpose if TransB)
	const int ldb,	// leading dimension of b
	const float beta,
	float* C,	  // m x n
	const int ldc // leading dimension of c
) {
	bool transpose_A = false;
	bool transpose_B = false;
	/* assert_sgemm_parameters(Order, TransA, TransB, N, M, K, lda, ldb, ldc, transpose_A, transpose_B); */

	for (int m = 0; m < M; ++m)
	{
		for (int n = 0; n < N; ++n)
		{
			C[m * N + n] += beta * C[m * N + n];
			for (int k = 0; k < K; ++k)
			{
				size_t A_idx = 0, B_idx = 0;
				if (transpose_A) {
					A_idx = k * M + m; // A is k x m
				} else {
					A_idx = m * K + k; // A is m x k
				}
				if (transpose_B) {
					B_idx = k + n * K; // B is n x k
				} else {
					B_idx = n + k * N; // B is k x n
				}
				C[m * N + n] += alpha * A[A_idx] * B[B_idx];
			}
		}
	}
}

void cblas_saxpy(const int N, const float alpha, const float* X, const int strideX, float* Y, const int strideY) {
	int ix;
	int iy;
	int i;
	int m;

	if (N <= 0) {
		return;
	}
	// If `alpha` is `0`, then `y` is unchanged...
	if (alpha == 0.0f) {
		return;
	}
	// If both strides are equal to `1`, use unrolled loops...
	if (strideX == 1 && strideY == 1) {
		m = N % 4;

		// If we have a remainder, do a clean-up loop...
		if (m > 0) {
			for (i = 0; i < m; i++) {
				Y[i] += alpha * X[i];
			}
			if (N < 4) {
				return;
			}
		}
		for (i = m; i < N; i += 4) {
			Y[i] += alpha * X[i];
			Y[i + 1] += alpha * X[i + 1];
			Y[i + 2] += alpha * X[i + 2];
			Y[i + 3] += alpha * X[i + 3];
		}
		return;
	}
	if (strideX < 0) {
		ix = (1 - N) * strideX;
	} else {
		ix = 0;
	}
	if (strideY < 0) {
		iy = (1 - N) * strideY;
	} else {
		iy = 0;
	}
	for (i = 0; i < N; i++) {
		Y[iy] += alpha * X[ix];
		ix += strideX;
		iy += strideY;
	}
	return;
}

void cblas_sgemv(const enum CBLAS_ORDER order,
				 const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
				 const float alpha, const float* A, const int lda,
				 const float* X, const int incX, const float beta,
				 float* Y, const int incY) {
}

void cblas_sger(const enum CBLAS_ORDER order, const int M, const int N,
				const float alpha, const float* X, const int incX,
				const float* Y, const int incY, float* A, const int lda) {
}
