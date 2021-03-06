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

#ifndef FLENS_IO_COORDSTORAGE_OUT_TCC
#define FLENS_IO_COORDSTORAGE_OUT_TCC 1

#include <cxxstd/iostream.h>
#include <flens/io/coordstorage/out.h>

namespace flens {

template <typename CS>
std::ostream &
operator<<(std::ostream &out, const GeCoordMatrix<CS> &A)
{
    const auto &coord = A.engine().coordVector();

    out << "isSorted_ = " << A.engine().isSorted_ << std::endl;
    for (size_t k=0; k<coord.size(); ++k) {
        out << "  #" << k << ": ("
            << coord[k].row << ", " << coord[k].col
            << ") = "
            << coord[k].value << std::endl;
    }
    return out;
}

template <typename CS>
std::ostream &
operator<<(std::ostream &out, const SyCoordMatrix<CS> &A)
{
    const auto &coord = A.engine().coordVector();

    out << "lastSortedCoord_ = " << A.engine().lastSortedCoord_ << std::endl;
    out << std::endl;
    for (size_t k=0; k<coord.size(); ++k) {
        out << "  #" << k << ": ("
            << coord[k].row << ", " << coord[k].col
            << ") = "
            << coord[k].value << std::endl;
    }
    return out;
}

} // namespace flens

#endif // FLENS_IO_COORDSTORAGE_OUT_TCC
