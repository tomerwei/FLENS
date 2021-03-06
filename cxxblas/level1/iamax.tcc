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

#ifndef CXXBLAS_LEVEL1_IAMAX_TCC
#define CXXBLAS_LEVEL1_IAMAX_TCC 1

#include <cxxstd/complex.h>
#include <cxxstd/cmath.h>
#include <cxxblas/cxxblas.h>

namespace cxxblas {

template <typename IndexType, typename X>
void
iamax_generic(IndexType n, const X *x, IndexType incX, IndexType &iAbsMaxX)
{
    CXXBLAS_DEBUG_OUT("iamax_generic");

    using std::abs;

    iAbsMaxX = 0;
    X absMaxX = abs(x[iAbsMaxX]);
    for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
        if (abs(x[iX])>absMaxX) {
            iAbsMaxX = i;
            absMaxX = abs(x[iX]);
        }
    }
}

template <typename IndexType, typename X>
void
iamax_generic(IndexType n, const Complex<X> *x, IndexType incX,
              IndexType &iAbsMaxX)
{
    CXXBLAS_DEBUG_OUT("iamax_generic");

    using std::abs;

    iAbsMaxX = 0;
    X absMaxX = abs1(x[iAbsMaxX]);
    for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
        if (abs1(x[iX])>absMaxX) {
            iAbsMaxX = i;
            absMaxX = abs1(x[iX]);
        }
    }
}


template <typename IndexType, typename X>
void
iamax(IndexType n, const X *x, IndexType incX, IndexType &iAbsMaxX)
{
    if (incX<0) {
        x -= incX*(n-1);
    }
    if (n<=0) {
        iAbsMaxX = -1;
        return;
    }
    iamax_generic(n, x, incX, iAbsMaxX);
}

template <typename IndexType, typename X>
IndexType
iamax(IndexType n, const X *x, IndexType incX)
{
    IndexType iAbsMaxX = IndexType(0);

    iamax(n, x, incX, iAbsMaxX);
    return iAbsMaxX;
}

#ifdef HAVE_CBLAS

// isamax
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
iamax(IndexType n, const float *x, IndexType incX, IndexType &i)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_isamax");

    i = cblas_isamax(n, x, incX);
}

// idamax
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
iamax(IndexType n, const double *x, IndexType incX, IndexType &i)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_idamax");

    i = cblas_idamax(n, x, incX);
}

// icamax
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
iamax(IndexType n, const ComplexFloat *x, IndexType incX, IndexType &i)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_icamax");

    i = cblas_icamax(n, reinterpret_cast<const float *>(x), incX);
}

// izamax
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
iamax(IndexType n, const ComplexDouble *x, IndexType incX, IndexType &i)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_izamax");

    i = cblas_izamax(n, reinterpret_cast<const double *>(x), incX);
}

#endif // HAVE_CBLAS

#ifdef HAVE_CUBLAS

// isamax
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
iamax(IndexType n,
      const thrust::device_ptr<const float> x, IndexType incX,
      IndexType &result)
{
    CXXBLAS_DEBUG_OUT("cublasIsamax");

    checkStatus(cublasIsamax(CublasEnv::handle(),
                             n, x.get(), incX,
                             &result));

    // We have to correct the result -> only syncMode allowed
    ASSERT(CublasEnv::isSyncCopyEnabled());
    CublasEnv::syncCopy();
    // Correct Indexing
    --result; // cuBLAS is one-based, cblas is zero-based
}


// idamax
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
iamax(IndexType n,
      const thrust::device_ptr<const double> x, IndexType incX,
      IndexType &result)
{
    CXXBLAS_DEBUG_OUT("cublasIdamax");

    checkStatus(cublasIdamax(CublasEnv::handle(),
                             n, x.get(), incX,
                             &result));

    // We have to correct the result -> only syncMode allowed
    ASSERT(CublasEnv::isSyncCopyEnabled());
    CublasEnv::syncCopy();
    // Correct Indexing
    --result; // cuBLAS is one-based, cblas is zero-based
}


// icamax
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
iamax(IndexType n,
      const thrust::device_ptr<const ComplexFloat> x, IndexType incX,
      IndexType &result)
{
    CXXBLAS_DEBUG_OUT("cublasIcamax");

    checkStatus(cublasIcamax(CublasEnv::handle(),
                             n, reinterpret_cast<const cuFloatComplex*>(x.get()), incX,
                             &result));

    // We have to correct the result -> only syncMode allowed
    ASSERT(CublasEnv::isSyncCopyEnabled());
    CublasEnv::syncCopy();
    // Correct Indexing
    --result; // cuBLAS is one-based, cblas is zero-based
}

// izamax
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
iamax(IndexType n,
      const thrust::device_ptr<const ComplexDouble> x, IndexType incX,
      IndexType &result)
{
    CXXBLAS_DEBUG_OUT("cublasIzamax");

    checkStatus(cublasIzamax(CublasEnv::handle(),
                             n, reinterpret_cast<const cuDoubleComplex*>(x.get()), incX,
                             &result));

    // We have to correct the result -> only syncModed allowed
    ASSERT(CublasEnv::isSyncCopyEnabled());
    CublasEnv::syncCopy();
    // Correct Indexing
    --result; // cuBLAS is one-based, cblas is zero-based
}

#endif // HAVE_CUBLAS

} // namespace cxxblas

#endif // CXXBLAS_LEVEL1_IAMAX_TCC
