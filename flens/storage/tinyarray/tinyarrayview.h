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

#ifndef FLENS_STORAGE_TINYARRAY_TINYARRAYVIEW_H
#define FLENS_STORAGE_TINYARRAY_TINYARRAYVIEW_H 1

namespace flens {

template <typename T, int n, int inc, int indexBase=1>
class TinyArrayView
{
    public:
        typedef T    ElementType;
        typedef int  IndexType;

        // std:: typedefs
        typedef IndexType                                 size_type;
        typedef T                                         value_type;
        typedef value_type*                               pointer;
        typedef const value_type*                         const_pointer;
        typedef value_type&                               reference;
        typedef const value_type&                         const_reference;

        static const int length = n;
        static const int stride = inc;
        static const int firstIndex = indexBase;
        static const int lastIndex = firstIndex+length-1;

        TinyArrayView(ElementType *data);

        ~TinyArrayView();

        //-- operators ---------------------------------------------------------

        const_reference
        operator()(IndexType index) const;

        reference
        operator()(IndexType index);

        //-- methods -----------------------------------------------------------

        const_pointer
        data() const;

        pointer
        data();

        void
        fill(const ElementType &value = ElementType());

    private:
        ElementType *data_;
};

} // namespace flens

#endif // FLENS_STORAGE_TINYARRAY_TINYARRAYVIEW_H
