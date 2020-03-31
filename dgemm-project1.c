/* 
		Please include compiler name below (you may also include any other modules you would like to be loaded)

COMPILER=icc

		Please include All compiler flags and libraries as you want them run. You can simply copy this over from the Makefile's first few lines

CC = icc
OPT = -Ofast
CFLAGS = -Wall -std=gnu99 $(OPT) -march=core-avx2 -restrict -ipo -unroll[=15] -fma -fp-model fast -qopt-malloc-options=3 -qopt-matmul# -funroll-all-loops -unroll-aggressive 
#MKLROOT = /opt/intel/composer_xe_2013.1.110loops7/mkl
#LDLIBS = -lrt -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm
LDLIBS = -lrt  -I$(MKLROOT)/include -Wl,-L$(MKLROOT)/lib/intel64/ -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm -ldl

*/
#include <stdlib.h>

// #define BLOCK_SIZE 32

// #define min(a,b) (((a)<(b))?(a):(b))
const char * dgemm_desc = "Project1";

// void warm(int n, double * array){
// 	double t;
// 	for(int i = 0; i < n; i++){
// 		for(int j = 0; j < n; j++){
// 			t= array[j+i*n];
// 		}
// 	}
// }

// void square_dgemm(int n, double * A, double * B, double * restrict C) {
// 	int i, j, k; //long
//     // #pragma prefetch A:1
//     // #pragma prefetch B:1
//     // #pragma prefetch C:1

// 	warm(n,A);
// 	if(n > 95){
// 		double * copy = NULL;
// 		copy = (double * ) malloc(n * n * sizeof(double));
// 		for(i = 0; i < n; i++){
// 			for(j = 0; j < n; j++){
// 				copy[j+i*n] = A[i+j*n];
// 			}
// 		}
		
// 		for (j = 0; j < n; ++j) {
// 			for (k = 0; k < n; ++k) {
// 				// double cij = B[k + j * n];
// 				// #pragma vector always
// 				// #pragma ivdep
// 				// #pragma simd vectorlength(16)
// 				for (i = 0; i < n; i++) {
// 					C[i + j * n] += copy[k+i* n] * B[k + j * n];
// 				}
// 			}
// 		}
// 	}else{
// 		for (j = 0; j < n; ++j) {
// 			for (k = 0; k < n; ++k) {
// 				double cij = B[k + j * n];
// 				// #pragma vector always
// 				// #pragma ivdep
// 				// #pragma simd vectorlength(16)
// 				for (i = 0; i < n; i++) {
// 					C[i + j * n] += A[i+k* n] * cij;
// 				}
// 			}
// 		}
// 	}	
//     // for (j = 0; j < n; ++j) {
//     //     for (k = 0; k < n; ++k) {
//     //         // double cij = B[k + j * n];
//     //         // #pragma vector always
//     //         // #pragma ivdep
//     //         // #pragma simd vectorlength(16)
//     //         for (i = 0; i < n; i++) {
//     //             C[i + j * n] += copy[k+i* n] * B[k + j * n];
//     //         }
//     //     }
//     // }
// }


// void square_dgemm(int n, double * A, double * B, double * restrict C) {
// 	int i, j, k; //long
//     // #pragma prefetch A:1
//     // #pragma prefetch B:1
//     // #pragma prefetch C:1


// 	if(n > 95){
// 		double * copy = NULL;
// 		copy = (double * ) malloc(n * n * sizeof(double));
// 		for(i = 0; i < n; i++){
// 			for(j = 0; j < n; j++){
// 				copy[j+i*n] = A[i+j*n];
// 			}
// 			copy = A;
// 		}
// 		for (j = 0; j < n; ++j) {
// 			for (k = 0; k < n; ++k) {
// 				// double cij = B[k + j * n];
// 				// #pragma vector always
// 				// #pragma ivdep
// 				// #pragma simd vectorlength(16)
// 				for (i = 0; i < n; i++) {
// 					C[i + j * n] += A[k+i* n] * B[k + j * n];
// 				}
// 			}
// 		}
// 	}else{
// 	// double * copy = NULL;
// 	// copy = (double * ) malloc(n * n * sizeof(double));
// 	// for(i = 0; i < n; i++){
// 	// 	for(j = 0; j < n; j++){
// 	// 		copy[j+i*n] = A[i+j*n];
// 	// 	}
// 	// 	copy = A;
// 	// }
		
// 		for (j = 0; j < n; ++j) {
// 			for (k = 0; k < n; ++k) {
// 				// double cij = B[k + j * n];
// 				// #pragma vector always
// 				// #pragma ivdep
// 				// #pragma simd vectorlength(16)
// 				for (i = 0; i < n; i++) {
// 					C[i + j * n] += A[i+k* n] * B[k + j * n];
// 				}
// 			}
// 		}
// 	}
// }

//This is the good one don't fuck it up please
void square_dgemm(int n, double * A, double * B, double * restrict C) {
	int i, j, k; //long

	double * copy = (double * ) malloc(n * sizeof(double));
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			copy[j+i*n] = A[i+j*n];
		}
		copy = A;
	}
	
	for (j = 0; j < n; ++j) {
		for (k = 0; k < n; ++k) {
			// double cij = B[k + j * n];
			#pragma prefetch A:1
			#pragma prefetch B:1
			#pragma prefetch C:1
			for (i = 0; i < n; i++) {
				C[i + j * n] += A[k+i* n] * B[k + j * n];
			}
		}
	}
}