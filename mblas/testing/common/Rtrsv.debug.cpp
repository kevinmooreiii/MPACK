/*
 * Copyright (c) 2008-2010
 *	Nakata, Maho
 * 	All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#include <mblas.h>
#include <blas.h>
#include <mpack_debug.h>

#if defined VERBOSE_TEST
#include <iostream>
#endif

#define MIN_N -2
#define MAX_N 50
#define MIN_LDA -2
#define MAX_LDA 7
#define MIN_INCX -3
#define MAX_INCX 3
#define MAX_ITER 5

REAL_REF maxdiff = 0.0;

void Rtrsv_test2(const char *uplo, const char *trans, const char *diag)
{
    int errorflag = FALSE;
    int mpack_errno1, mpack_errno2;
    for (int incx = MIN_INCX; incx <= MAX_INCX; incx++) {
	for (int n = MIN_N; n <= MAX_N; n++) {
	    for (int lda = max(1, n); lda <= MAX_LDA; lda++) {
#if defined VERBOSE_TEST
		printf("#n is %d, lda is %d, incx is %d, uplo is %s, trans is %s, diag is %s \n", n, lda, incx, uplo, trans, diag);
#endif
		REAL_REF *x_ref;
		REAL_REF *A_ref;
		REAL *x;
		REAL *A;

		A_ref = new REAL_REF[matlen(lda, n)];
		x_ref = new REAL_REF[veclen(n, incx)];
		A = new REAL[matlen(lda, n)];
		x = new REAL[veclen(n, incx)];

		for (int i = 0; i < MAX_ITER; i++) {
		    set_random_vector(A_ref, A, matlen(lda, n));
		    set_random_vector(x_ref, x, veclen(n, incx));

		    mpack_errno = 0; blas_errno = 0;
#if defined ___MPACK_BUILD_WITH_MPFR___
		    dtrsv_f77(uplo, trans, diag, &n, A_ref, &lda, x_ref, &incx);
		    mpack_errno1 = blas_errno;
#else
		    Rtrsv(uplo, trans, diag, n, A_ref, lda, x_ref, incx);
		    mpack_errno1 = mpack_errno;
#endif
		    Rtrsv(uplo, trans, diag, n, A, lda, x, incx);
		    mpack_errno2 = mpack_errno;

#if defined VERBOSE_TEST
		    printf("errno: mpack %d, ref %d\n", mpack_errno1, mpack_errno2);
#endif
		    if (mpack_errno1 != mpack_errno2) {
			printf("error in Mxerbla!!\n");
			exit(1);
		    }
		    REAL_REF diff = infnorm(x_ref, x, veclen(n, incx), 1);
		    if (diff > EPSILON) {
			printf("error: "); printnum(diff); printf("\n");
			errorflag = TRUE;
		    }
		    if (maxdiff < diff)
			maxdiff = diff;
		    printf("max error: "); printnum(maxdiff); printf("\n");
		}
		delete[]x;
		delete[]A;
		delete[]x_ref;
		delete[]A_ref;
	    }
	}
    }
    if (errorflag == TRUE) {
	printf("Rtrsv test failed...\n");
	exit(1);
    }
    printf("max error: "); printnum(maxdiff); printf("\n");
}

void Rtrsv_test()
{
    Rtrsv_test2("U", "N", "U");
    Rtrsv_test2("U", "N", "N");
    Rtrsv_test2("U", "T", "U");
    Rtrsv_test2("U", "T", "N");
    Rtrsv_test2("U", "C", "U");
    Rtrsv_test2("U", "C", "N");
    Rtrsv_test2("L", "N", "U");
    Rtrsv_test2("L", "N", "N");
    Rtrsv_test2("L", "T", "U");
    Rtrsv_test2("L", "T", "N");
    Rtrsv_test2("L", "C", "U");
    Rtrsv_test2("L", "C", "N");
}

int main(int argc, char *argv[])
{
    Rtrsv_test();
    printf("Rtrsv test passed...\n");
    return (0);
}
