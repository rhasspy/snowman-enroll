#ifndef BLAS_H_
#define BLAS_H_

#define CBLAS_INDEX size_t /* this may vary between platforms */

enum CBLAS_ORDER { CblasRowMajor = 101,
				   CblasColMajor = 102 };
enum CBLAS_TRANSPOSE { CblasNoTrans = 111,
					   CblasTrans = 112,
					   CblasConjTrans = 113 };
enum CBLAS_UPLO { CblasUpper = 121,
				  CblasLower = 122 };
enum CBLAS_DIAG { CblasNonUnit = 131,
				  CblasUnit = 132 };
enum CBLAS_SIDE { CblasLeft = 141,
				  CblasRight = 142 };

void cblas_sscal(const int N, const float alpha, float* X, const int stride);
float cblas_sdot(const int N, const float* X, const int strideX, const float* Y, const int strideY);
void cblas_saxpy(const int N, const float alpha, const float* X, const int incX, float* Y, const int incY);

void cblas_sgemv(const enum CBLAS_ORDER order,
				 const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
				 const float alpha, const float* A, const int lda,
				 const float* X, const int incX, const float beta,
				 float* Y, const int incY);

void cblas_sger(const enum CBLAS_ORDER order, const int M, const int N,
				const float alpha, const float* X, const int incX,
				const float* Y, const int incY, float* A, const int lda);

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
);

#endif // BLAS_H_
