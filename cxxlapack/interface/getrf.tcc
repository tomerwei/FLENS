/*
 *   Copyright (c) 2012, Michael Lehn
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

#ifndef CXXLAPACK_INTERFACE_GETRF_TCC
#define CXXLAPACK_INTERFACE_GETRF_TCC 1

#include <cxxstd/iostream.h>
#include <cxxlapack/interface/interface.h>
#include <cxxlapack/netlib/netlib.h>

namespace cxxlapack {

template <typename IndexType>
IndexType
getrf(IndexType             m,
      IndexType             n,
      float                 *A,
      IndexType             ldA,
      IndexType             *iPiv)
{
    IndexType info;
    CXXLAPACK_DEBUG_OUT("sgetrf");
    LAPACK_IMPL(sgetrf)(&m,
                        &n,
                        A,
                        &ldA,
                        iPiv,
                        &info);
#   ifndef NDEBUG
    if (info<0) {
        std::cerr << "info = " << info << std::endl;
    }
#   endif
    ASSERT(info>=0);
    return info;
}
template <typename IndexType>
IndexType
getrf(IndexType             m,
      IndexType             n,
      double                *A,
      IndexType             ldA,
      IndexType             *iPiv)
{
    IndexType info;
    CXXLAPACK_DEBUG_OUT("dgetrf");
    LAPACK_IMPL(dgetrf)(&m,
                        &n,
                        A,
                        &ldA,
                        iPiv,
                        &info);
#   ifndef NDEBUG
    if (info<0) {
        std::cerr << "info = " << info << std::endl;
    }
#   endif
    ASSERT(info>=0);
    return info;
}

template <typename IndexType>
IndexType
getrf(IndexType             m,
      IndexType             n,
      std::complex<float >  *A,
      IndexType             ldA,
      IndexType             *iPiv)
{
    IndexType info;
    CXXLAPACK_DEBUG_OUT("cgetrf");
    LAPACK_IMPL(cgetrf)(&m,
                        &n,
                        reinterpret_cast<float  *>(A),
                        &ldA,
                        iPiv,
                        &info);
#   ifndef NDEBUG
    if (info<0) {
        std::cerr << "info = " << info << std::endl;
    }
#   endif
    ASSERT(info>=0);
    return info;
}

template <typename IndexType>
IndexType
getrf(IndexType             m,
      IndexType             n,
      std::complex<double>  *A,
      IndexType             ldA,
      IndexType             *iPiv)
{
    IndexType info;
    CXXLAPACK_DEBUG_OUT("zgetrf");
    LAPACK_IMPL(zgetrf)(&m,
                        &n,
                        reinterpret_cast<double *>(A),
                        &ldA,
                        iPiv,
                        &info);
#   ifndef NDEBUG
    if (info<0) {
        std::cerr << "info = " << info << std::endl;
    }
#   endif
    ASSERT(info>=0);
    return info;
}

#ifdef HAVE_CUSOLVER

template <typename IndexType>
IndexType
getrf(IndexType                                m,
      IndexType                                n,
      thrust::device_ptr<float>                A,
      IndexType                                ldA,
      thrust::device_ptr<IndexType>            iPiv)
{
    CXXLAPACK_DEBUG_OUT("fgetrf [cuda]");

    int work_size;
    cusolverDnSgetrf_bufferSize(CusolverEnv::handle(),
                                m, n,
                                A.get(), ldA,
                                &work_size);
    float* work;
    checkStatus(cudaMalloc(&work, work_size * sizeof(float)));
    int* devInfo;
    checkStatus(cudaMalloc(&devInfo, sizeof(int)));

    cusolverDnSgetrf(CusolverEnv::handle(),
                     m, n, A.get(), ldA,
                     work, iPiv.get(),
                     devInfo);

    // TODO: Check solverInfo, check cusolverStatus

    checkStatus(cudaFree(work));
    checkStatus(cudaFree(devInfo));

    return 0;
}

template <typename IndexType>
IndexType
getrf(IndexType                                 m,
      IndexType                                 n,
      thrust::device_ptr<double>                A,
      IndexType                                 ldA,
      thrust::device_ptr<IndexType>             iPiv)
{
    CXXLAPACK_DEBUG_OUT("dgetrf [cuda]");

    int work_size;
    cusolverDnDgetrf_bufferSize(CusolverEnv::handle(),
                                m, n,
                                A.get(), ldA,
                                &work_size);
    double* work;
    checkStatus(cudaMalloc(&work, work_size * sizeof(double)));
    int* devInfo;
    checkStatus(cudaMalloc(&devInfo, sizeof(int)));

    cusolverDnDgetrf(CusolverEnv::handle(),
                     m, n, A.get(), ldA,
                     work, iPiv.get(),
                     devInfo);

    // TODO: Check solverInfo, check cusolverStatus

    checkStatus(cudaFree(work));
    checkStatus(cudaFree(devInfo));

    return 0;
}

template <typename IndexType>
IndexType
getrf(IndexType                                 m,
      IndexType                                 n,
      thrust::device_ptr<std::complex<float> >  A,
      IndexType                                 ldA,
      thrust::device_ptr<IndexType>             iPiv)
{
    CXXLAPACK_DEBUG_OUT("cgetrf [cuda]");

    int work_size;
    cusolverDnCgetrf_bufferSize(CusolverEnv::handle(),
                                m, n,
                                reinterpret_cast<cuFloatComplex*>(A.get()), ldA,
                                &work_size);
    cuFloatComplex* work;
    checkStatus(cudaMalloc(&work, work_size * sizeof(cuFloatComplex)));
    int* devInfo;
    checkStatus(cudaMalloc(&devInfo, sizeof(int)));

    cusolverDnCgetrf(CusolverEnv::handle(),
                     m, n, reinterpret_cast<cuFloatComplex*>(A.get()), ldA,
                     work, iPiv.get(),
                     devInfo);

    // TODO: Check solverInfo, check cusolverStatus

    checkStatus(cudaFree(work));
    checkStatus(cudaFree(devInfo));

    return 0;
}

template <typename IndexType>
IndexType
getrf(IndexType                                 m,
      IndexType                                 n,
      thrust::device_ptr<std::complex<double> > A,
      IndexType                                 ldA,
      thrust::device_ptr<IndexType>             iPiv)
{
    CXXLAPACK_DEBUG_OUT("zgetrf [cuda]");

    int work_size;
    cusolverDnZgetrf_bufferSize(CusolverEnv::handle(),
                                m, n,
                                reinterpret_cast<cuDoubleComplex*>(A.get()), ldA,
                                &work_size);
    cuDoubleComplex* work;
    checkStatus(cudaMalloc(&work, work_size * sizeof(cuDoubleComplex)));
    int* devInfo;
    checkStatus(cudaMalloc(&devInfo, sizeof(int)));

    cusolverDnZgetrf(CusolverEnv::handle(),
                     m, n, reinterpret_cast<cuDoubleComplex*>(A.get()), ldA,
                     work, iPiv.get(),
                     devInfo);

    // TODO: Check solverInfo, check cusolverStatus

    checkStatus(cudaFree(work));
    checkStatus(cudaFree(devInfo));

    return 0;
}

#endif // HAVE_CUSOLVER

} // namespace cxxlapack

#endif // CXXLAPACK_INTERFACE_GETRF_TCC
