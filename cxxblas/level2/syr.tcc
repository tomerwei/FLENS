/*
 *   Copyright (c) 2009, Michael Lehn
 *
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2) Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3) Neither the name of the FLENS development group nor the names of
 *      its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CXXBLAS_LEVEL2_SYR_TCC
#define CXXBLAS_LEVEL2_SYR_TCC 1

#include <cxxstd/complex.h>
#include <cxxblas/cxxblas.h>

namespace cxxblas {

template <typename IndexType, typename ALPHA, typename VX, typename MA>
void
syr_generic(StorageOrder order, StorageUpLo upLo,
            IndexType n,
            const ALPHA &alpha,
            const VX *x, IndexType incX,
            MA *A, IndexType ldA)
{
    if (order==ColMajor) {
        upLo = (upLo==Upper) ? Lower : Upper;
        syr_generic(RowMajor, upLo, n, alpha, x, incX, A, ldA);
        return;
    }
    #ifdef CXXBLAS_USE_XERBLA
        // insert error check here
    #endif
    if (upLo==Upper) {
        for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
            axpy_generic(n-i, alpha*x[iX], x+iX, incX,
                                           A+i*(ldA+1), IndexType(1));
        }
    } else {
        for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
            axpy_generic(i+1, alpha*x[iX], x, incX,
                                           A+i*ldA, IndexType(1));
        }
    }
}

template <typename IndexType, typename ALPHA, typename VX, typename MA>
void
syr(StorageOrder order, StorageUpLo upLo,
    IndexType n,
    const ALPHA &alpha,
    const VX *x, IndexType incX,
    MA *A, IndexType ldA)
{
    CXXBLAS_DEBUG_OUT("syr_generic");

    if (incX<0) {
        x -= incX*(n-1);
    }
    syr_generic(order, upLo, n, alpha, x, incX, A, ldA);
}


#ifdef HAVE_CBLAS

// ssyr
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
syr(StorageOrder order,   StorageUpLo upLo,
      IndexType n,
      float alpha,
      const float *x, IndexType incX,
      float *A, IndexType ldA)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_ssyr");

    cblas_ssyr(CBLAS::getCblasType(order), CBLAS::getCblasType(upLo),
               n,
               alpha,
               x, incX,
               A, ldA);
}

// dsyr
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
syr(StorageOrder order,   StorageUpLo upLo,
      IndexType n,
      double alpha,
      const double *x, IndexType incX,
      double *A, IndexType ldA)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_dsyr");

    cblas_dsyr(CBLAS::getCblasType(order), CBLAS::getCblasType(upLo),
               n,
               alpha,
               x, incX,
               A, ldA);
}

#endif // HAVE_CBLAS

#ifdef HAVE_CUBLAS

// csyr
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
    syr(StorageOrder order, StorageUpLo upLo,
         IndexType n,
         float alpha,
         const thrust::device_ptr<const float> x, IndexType incX,
         thrust::device_ptr<float> A, IndexType ldA)
{
    CXXBLAS_DEBUG_OUT("cublasSsyr");

    ASSERT (order==ColMajor);

    cublasStatus_t status = cublasSsyr(CublasEnv::handle(), CUBLAS::getCublasType(upLo),
                                        n,
                                        &alpha,
                                        x.get(), incX,
                                        A.get(), ldA);

    checkStatus(status);
}

// zsyr
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
syr(StorageOrder order, StorageUpLo upLo,
      IndexType n,
      double alpha,
      const thrust::device_ptr<const double> x, IndexType incX,
      thrust::device_ptr<double> A, IndexType ldA)
{
    CXXBLAS_DEBUG_OUT("cublasDsyr");

    ASSERT (order==ColMajor);

    cublasStatus_t status = cublasDsyr(CublasEnv::handle(), CUBLAS::getCublasType(upLo),
                                        n,
                                        &alpha,
                                        x.get(), incX,
                                        A.get(), ldA);

    checkStatus(status);
}

// csyr
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
    syr(StorageOrder order, StorageUpLo upLo,
         IndexType n,
         ComplexFloat alpha,
         const thrust::device_ptr<const ComplexFloat> x, IndexType incX,
         thrust::device_ptr<ComplexFloat> A, IndexType ldA)
{
    CXXBLAS_DEBUG_OUT("cublasCsyr");

    ASSERT (order==ColMajor);

    cublasStatus_t status = cublasCsyr(CublasEnv::handle(), CUBLAS::getCublasType(upLo),
                                        n,
                                        reinterpret_cast<const cuFloatComplex*>(&alpha),
                                        reinterpret_cast<const cuFloatComplex*>(x.get()), incX,
                                        reinterpret_cast<const cuFloatComplex*>(A.get()), ldA);

    checkStatus(status);
}

// zsyr
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
syr(StorageOrder order, StorageUpLo upLo,
      IndexType n,
      ComplexDouble alpha,
      const thrust::device_ptr<const ComplexDouble> x, IndexType incX,
      thrust::device_ptr<ComplexDouble> A, IndexType ldA)
{
    CXXBLAS_DEBUG_OUT("cublasZsyr");

    ASSERT (order==ColMajor);

    cublasStatus_t status = cublasZsyr(CublasEnv::handle(), CUBLAS::getCublasType(upLo),
                                        n,
                                        reinterpret_cast<const cuDoubleComplex*>(&alpha),
                                        reinterpret_cast<const cuDoubleComplex*>(x.get()), incX,
                                        reinterpret_cast<cuDoubleComplex*>(A.get()), ldA);

    checkStatus(status);
}

#endif // HAVE_CUBLAS

} // namespace cxxblas

#endif // CXXBLAS_LEVEL2_SYR_TCC
