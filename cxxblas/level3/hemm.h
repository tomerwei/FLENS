/*
 *   Copyright (c) 2010, Michael Lehn
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

#ifndef CXXBLAS_LEVEL3_HEMM_H
#define CXXBLAS_LEVEL3_HEMM_H 1

#include <cxxblas/drivers/drivers.h>
#include <cxxblas/typedefs.h>

#define HAVE_CXXBLAS_HEMM 1

namespace cxxblas {

template <typename IndexType, typename ALPHA, typename MA, typename MB,
          typename BETA, typename MC>
    void
    hemm(StorageOrder order,
         Side side, StorageUpLo upLo,
         IndexType m, IndexType n,
         const ALPHA &alpha,
         const MA *A, IndexType ldA,
         const MB *B, IndexType ldB,
         const BETA &beta,
         MC *C, IndexType ldC);

#ifdef HAVE_CBLAS

template <typename IndexType>
    typename If<IndexType>::isBlasCompatibleInteger
    hemm(StorageOrder order,
         Side side, StorageUpLo upLo,
         IndexType m, IndexType n,
         const ComplexFloat &alpha,
         const ComplexFloat *A, IndexType ldA,
         const ComplexFloat *B, IndexType ldB,
         const ComplexFloat &beta,
         ComplexFloat *C, IndexType ldC);

template <typename IndexType>
    typename If<IndexType>::isBlasCompatibleInteger
    hemm(StorageOrder order,
         Side side, StorageUpLo upLo,
         IndexType m, IndexType n,
         const ComplexDouble &alpha,
         const ComplexDouble *A, IndexType ldA,
         const ComplexDouble *B, IndexType ldB,
         const ComplexDouble &beta,
         ComplexDouble *C, IndexType ldC);

#endif // HAVE_CBLAS

#ifdef HAVE_CUBLAS

// chemm
template <typename IndexType>
    typename If<IndexType>::isBlasCompatibleInteger
    hemm(StorageOrder order, Side side, StorageUpLo upLo,
         IndexType m, IndexType n,
         const ComplexFloat &alpha,
         const flens::device_ptr<const ComplexFloat, flens::StorageType::CUDA> A, IndexType ldA,
         const flens::device_ptr<const ComplexFloat, flens::StorageType::CUDA> B, IndexType ldB,
         const ComplexFloat &beta,
	 flens::device_ptr<ComplexFloat, flens::StorageType::CUDA> C, IndexType ldC);
	 

// zhemm
template <typename IndexType>
    typename If<IndexType>::isBlasCompatibleInteger
    hemm(StorageOrder order, Side side, StorageUpLo upLo,
         IndexType m, IndexType n,
         const ComplexDouble &alpha,
         const flens::device_ptr<const ComplexDouble, flens::StorageType::CUDA> A, IndexType ldA,
         const flens::device_ptr<const ComplexDouble, flens::StorageType::CUDA> B, IndexType ldB,
         const ComplexDouble &beta,
	 flens::device_ptr<ComplexDouble, flens::StorageType::CUDA> C, IndexType ldC);

#endif // HAVE_CUBLAS

} // namespace cxxblas

#endif // CXXBLAS_LEVEL3_HEMM_H
