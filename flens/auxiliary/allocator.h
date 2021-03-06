/*
 *   Copyright (c) 2010, Michael Lehn, Cris Cecka
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

#ifndef FLENS_AUXILIARY_ALLOCATOR_H
#define FLENS_AUXILIARY_ALLOCATOR_H 1

#include <flens/vectortypes/impl/vectorclosure.h>
#include <flens/matrixtypes/impl/matrixclosure.h>

namespace flens {

struct NoAllocator {};

template <typename A, typename B>
struct CommonAllocator;

template <typename A>
struct CommonAllocator<A, A>
{
    typedef A Type;
};

template <typename A>
struct CommonAllocator<A, NoAllocator>
{
    typedef A Type;
};

template <typename B>
struct CommonAllocator<NoAllocator, B>
{
    typedef B Type;
};

// The type of T::Allocator or T::Engine::Allocator or NoAllocator
template <typename T>
struct AllocatorType
{
    template <typename A>
        static typename A::Allocator
        check(typename A::Allocator *);

    template <typename A>
        static typename A::Engine::Allocator
        check(typename A::Engine::Allocator *);

    template <typename Any>
        static NoAllocator
        check(...);

    typedef decltype(check<T>(0)) Type;
};

// Specialization for VectorClosure
template <typename Op, typename L, typename R>
struct AllocatorType<VectorClosure<Op, L, R> >
{
  typedef typename CommonAllocator<typename AllocatorType<L>::Type,
                                   typename AllocatorType<R>::Type>::Type Type;
};

// Specialization for MatrixClosure
template <typename Op, typename L, typename R>
struct AllocatorType<MatrixClosure<Op, L, R> >
{
  typedef typename CommonAllocator<typename AllocatorType<L>::Type,
                                   typename AllocatorType<R>::Type>::Type Type;
};

} // namespace flens

#endif // FLENS_AUXILIARY_ALLOCATOR_H
