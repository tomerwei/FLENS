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

#ifndef CXXBLAS_LEVEL2_TBSV_TCC
#define CXXBLAS_LEVEL2_TBSV_TCC 1

#include <cxxstd/complex.h>
#include <cxxblas/cxxblas.h>

namespace cxxblas {

template <typename IndexType, typename MA, typename VX>
void
tbsv_generic(StorageOrder order, StorageUpLo upLo,
             Transpose transA, Diag diag,
             IndexType n, IndexType k,
             const MA *A, IndexType ldA,
             VX *x, IndexType incX)
{
    using std::max;
    using std::min;

    if (order==ColMajor) {
        transA = Transpose(transA^Trans);
        upLo = (upLo==Upper) ? Lower : Upper;
    }

    if (incX<0) {
        x -= incX*(n-1);
    }

    if (transA==NoTrans) {
        if (upLo==Upper) {
            if (diag==NonUnit) {
                for (IndexType i=n-1, iX=i*incX; i>=0; --i, iX-=incX) {
                    IndexType len = min(k+1, n-i);

                    VX x_;
                    dotu_generic(len-1, A+ldA*i+1, IndexType(1),
                                        x+iX+incX, IndexType(incX),
                                        x_);
                    x[iX] -= x_;
                    x[iX] /= *(A+ldA*i);
                }
            } else { /* diag==Unit */
                for (IndexType i=n-1, iX=i*incX; i>=0; --i, iX-=incX) {
                    IndexType len = min(k+1, n-i);

                    VX x_;
                    dotu_generic(len-1, A+ldA*i+1, IndexType(1),
                                        x+iX+incX, IndexType(incX),
                                        x_);
                    x[iX] -= x_;
                }
            }
        } else { /* upLo==Lower */
            if (diag==NonUnit) {
                for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
                    IndexType iA = max(k-i, IndexType(0));
                    IndexType len = min(k, i) + 1;
                    IndexType i_ = max(i-k, IndexType(0));

                    VX x_;
                    dotu_generic(len-1, A+ldA*i+iA, IndexType(1),
                                        x+i_*incX, IndexType(incX),
                                        x_);
                    x[iX] -= x_;
                    x[iX] /= *(A+ldA*i+iA + len-1);
                }
            } else { /* diag==Unit */
                for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
                    IndexType iA = max(k-i, IndexType(0));
                    IndexType len = min(k, i) + 1;
                    IndexType i_ = max(i-k, IndexType(0));

                    VX x_;
                    dotu_generic(len-1, A+ldA*i+iA, IndexType(1),
                                        x+i_*incX, IndexType(incX),
                                        x_);
                    x[iX] -= x_;
                }
            }
        }
    }
    if (transA==Conj) {
        if (upLo==Upper) {
            if (diag==NonUnit) {
                for (IndexType i=n-1, iX=i*incX; i>=0; --i, iX-=incX) {
                    IndexType len = min(k+1, n-i);

                    VX x_;
                    dot_generic(len-1, A+ldA*i+1, IndexType(1),
                                       x+iX+incX, IndexType(incX),
                                       x_);
                    x[iX] -= x_;
                    x[iX] /= conjugate(A[ldA*i]);
                }
            } else { /* diag==Unit */
                for (IndexType i=n-1, iX=i*incX; i>=0; --i, iX-=incX) {
                    IndexType len = min(k+1, n-i);

                    VX x_;
                    dot_generic(len-1, A+ldA*i+1, IndexType(1),
                                       x+iX+incX, IndexType(incX),
                                       x_);
                    x[iX] -= x_;
                }
            }
        } else { /* upLo==Lower */
            if (diag==NonUnit) {
                for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
                    IndexType iA = max(k-i, IndexType(0));
                    IndexType len = min(k, i) + 1;
                    IndexType i_ = max(i-k, IndexType(0));

                    VX x_;
                    dot_generic(len-1, A+ldA*i+iA, IndexType(1),
                                       x+i_*incX, IndexType(incX),
                                       x_);
                    x[iX] -= x_;
                    x[iX] /= conjugate(A[ldA*i+iA + len-1]);
                }
            } else { /* diag==Unit */
                for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
                    IndexType iA = max(k-i, IndexType(0));
                    IndexType len = min(k, i) + 1;
                    IndexType i_ = max(i-k, IndexType(0));

                    VX x_;
                    dot_generic(len-1, A+ldA*i+iA, IndexType(1),
                                       x+i_*incX, IndexType(incX),
                                       x_);
                    x[iX] -= x_;
                }
            }
        }
    }
    if (transA==Trans) {
        if (upLo==Upper) {
            if (diag==NonUnit) {
                for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
                    IndexType len = min(k+1, n-i);

                    x[iX] /= *(A+ldA*i);
                    axpy_generic(len-1, -x[iX],
                                        A+ldA*i+1, IndexType(1),
                                        x+iX+incX, incX);
                }
            } else { /* diag==Unit */
                for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
                    IndexType len = min(k+1, n-i);

                    axpy_generic(len-1, -x[iX],
                                        A+ldA*i+1, IndexType(1),
                                        x+iX+incX, incX);
                }
            }
        } else { /* upLo==Lower */
            if (diag==NonUnit) {
                for (IndexType i=n-1, iX=i*incX; i>=0; --i, iX-=incX) {
                    IndexType iA = max(k-i, IndexType(0));
                    IndexType len = min(k, i) + 1;
                    IndexType iY = max(i-k, IndexType(0))*incX;

                    x[iX] /= *(A+ldA*i+iA+len-1);
                    axpy_generic(len-1, -x[iX],
                                        A+ldA*i+iA, IndexType(1),
                                        x+iY, incX);
                }
            } else {
                for (IndexType i=n-1, iX=i*incX; i>=0; --i, iX-=incX) {
                    IndexType iA = max(k-i, IndexType(0));
                    IndexType len = min(k, i) + 1;
                    IndexType iY = max(i-k, IndexType(0))*incX;

                    axpy_generic(len-1, -x[iX],
                                        A+ldA*i+iA, IndexType(1),
                                        x+iY, incX);
                }
            }
        }
    }
    if (transA==ConjTrans) {
        if (upLo==Upper) {
            if (diag==NonUnit) {
                for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
                    IndexType len = min(k+1, n-i);

                    x[iX] /= conjugate(A[ldA*i]);
                    acxpy_generic(len-1, -x[iX],
                                         A+ldA*i+1, IndexType(1),
                                         x+iX+incX, incX);
                }
            } else { /* diag==Unit */
                for (IndexType i=0, iX=0; i<n; ++i, iX+=incX) {
                    IndexType len = min(k+1, n-i);

                    acxpy_generic(len-1, -x[iX],
                                         A+ldA*i+1, IndexType(1),
                                         x+iX+incX, incX);
                }
            }
        } else { /* upLo==Lower */
            if (diag==NonUnit) {
                for (IndexType i=n-1, iX=i*incX; i>=0; --i, iX-=incX) {
                    IndexType iA = max(k-i, IndexType(0));
                    IndexType len = min(k, i) + 1;
                    IndexType iY = max(i-k, IndexType(0))*incX;

                    x[iX] /= conjugate(A[ldA*i+iA+len-1]);
                    acxpy_generic(len-1, -x[iX],
                                         A+ldA*i+iA, IndexType(1),
                                         x+iY, incX);
                }
            } else {
                for (IndexType i=n-1, iX=i*incX; i>=0; --i, iX-=incX) {
                    IndexType iA = max(k-i, IndexType(0));
                    IndexType len = min(k, i) + 1;
                    IndexType iY = max(i-k, IndexType(0))*incX;

                    acxpy_generic(len-1, -x[iX],
                                         A+ldA*i+iA, IndexType(1),
                                         x+iY, incX);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------

template <typename IndexType, typename MA, typename VX>
void
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const MA *A, IndexType ldA,
     VX *x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("tbsv_generic");

    if (n==0) {
        return;
    }

    tbsv_generic(order, upLo, transA, diag, n, k, A, ldA, x, incX);
}

#ifdef HAVE_CBLAS

// stbsv
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const float *A, IndexType ldA,
     float *x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_stbsv");

    cblas_stbsv(CBLAS::getCblasType(order), CBLAS::getCblasType(upLo),
                CBLAS::getCblasType(transA), CBLAS::getCblasType(diag),
                n, k,
                A, ldA,
                x, incX);
}

// dtbsv
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const double *A, IndexType ldA,
     double *x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_dtbsv");

    cblas_dtbsv(CBLAS::getCblasType(order), CBLAS::getCblasType(upLo),
                CBLAS::getCblasType(transA), CBLAS::getCblasType(diag),
                n, k,
                A, ldA,
                x, incX);
}

// ctbsv
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const ComplexFloat *A, IndexType ldA,
     ComplexFloat *x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_ctbsv");

    if (transA==Conj) {
        CXXBLAS_DEBUG_OUT("tbsv_generic");
        tbsv_generic(order, upLo, transA, diag, n, k, A, ldA, x, incX);
        return;
    }

    cblas_ctbsv(CBLAS::getCblasType(order), CBLAS::getCblasType(upLo),
                CBLAS::getCblasType(transA), CBLAS::getCblasType(diag),
                n, k,
                reinterpret_cast<const float *>(A), ldA,
                reinterpret_cast<float *>(x), incX);
}

// ztbsv
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const ComplexDouble *A, IndexType ldA,
     ComplexDouble *x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_ztbsv");

    if (transA==Conj) {
        CXXBLAS_DEBUG_OUT("tbsv_generic");
        tbsv_generic(order, upLo, transA, diag, n, k, A, ldA, x, incX);
        return;
    }

    cblas_ztbsv(CBLAS::getCblasType(order), CBLAS::getCblasType(upLo),
                CBLAS::getCblasType(transA), CBLAS::getCblasType(diag),
                n, k,
                reinterpret_cast<const double *>(A), ldA,
                reinterpret_cast<double *>(x), incX);
}
#endif // HAVE_CBLAS

#ifdef HAVE_CUBLAS

// stbsv
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const thrust::device_ptr<const float> A, IndexType ldA,
     thrust::device_ptr<float> x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("cublasStbsv");

    if (order==RowMajor) {
        transA = Transpose(transA^Trans);
        upLo = (upLo==Upper) ? Lower : Upper;
        tbsv(ColMajor, upLo, transA, diag, n, k, A, ldA, x, incX);
        return;
    }
    cublasStatus_t status = cublasStbsv(CublasEnv::handle(),
                                        CUBLAS::getCublasType(upLo), CUBLAS::getCublasType(transA),
                                        CUBLAS::getCublasType(diag),
                                        n, k,
                                        A.get(), ldA,
                                        x.get(), incX);

    checkStatus(status);
}

// dtbsv
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const thrust::device_ptr<const double> A, IndexType ldA,
      thrust::device_ptr<double> x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("cublasDtbsv");

    if (order==RowMajor) {
        transA = Transpose(transA^Trans);
        upLo = (upLo==Upper) ? Lower : Upper;
        tbsv(ColMajor, upLo, transA, diag, n, k, A, ldA, x, incX);
        return;
    }
    cublasStatus_t status = cublasDtbsv(CublasEnv::handle(),
                                        CUBLAS::getCublasType(upLo), CUBLAS::getCublasType(transA),
                                        CUBLAS::getCublasType(diag),
                                        n, k,
                                        A.get(), ldA,
                                        x.get(), incX);

    checkStatus(status);
}

// ctbsv
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const thrust::device_ptr<const ComplexFloat> A, IndexType ldA,
     thrust::device_ptr<ComplexFloat> x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("cublasCtbsv");

    if (order==RowMajor) {
        transA = Transpose(transA^Trans);
        upLo = (upLo==Upper) ? Lower : Upper;
        tbsv(ColMajor, upLo, transA, diag, n, k, A, ldA, x, incX);
        return;
    }
    cublasStatus_t status = cublasCtbsv(CublasEnv::handle(),
                                        CUBLAS::getCublasType(upLo), CUBLAS::getCublasType(transA),
                                        CUBLAS::getCublasType(diag),
                                        n, k,
                                        reinterpret_cast<const cuFloatComplex*>(A.get()), ldA,
                                        reinterpret_cast<cuFloatComplex*>(x.get()), incX);

    checkStatus(status);
}

// ztbsv
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
tbsv(StorageOrder order, StorageUpLo upLo,
     Transpose transA, Diag diag,
     IndexType n, IndexType k,
     const thrust::device_ptr<const ComplexDouble> A, IndexType ldA,
     thrust::device_ptr<ComplexDouble> x, IndexType incX)
{
    CXXBLAS_DEBUG_OUT("cublasZtbsv");

    if (order==RowMajor) {
        transA = Transpose(transA^Trans);
        upLo = (upLo==Upper) ? Lower : Upper;
        tbsv(ColMajor, upLo, transA, diag, n, k, A, ldA, x, incX);
        return;
    }
    cublasStatus_t status = cublasZtbsv(CublasEnv::handle(),
                                        CUBLAS::getCublasType(upLo), CUBLAS::getCublasType(transA),
                                        CUBLAS::getCublasType(diag),
                                        n, k,
                                        reinterpret_cast<const cuDoubleComplex*>(A.get()), ldA,
                                        reinterpret_cast<cuDoubleComplex*>(x.get()), incX);

    checkStatus(status);
}

#endif // HAVE_CUBLAS

} // namespace cxxblas

#endif // CXXBLAS_LEVEL2_TBSV_TCC
