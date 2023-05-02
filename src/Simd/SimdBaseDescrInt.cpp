/*
* Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2022 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "Simd/SimdDescrInt.h"

namespace Simd
{
    namespace Base
    {
        bool DescrIntValid(size_t size, size_t depth)
        {
            if (depth < 8 || depth > 8)
                return false;
            if (size == 0)
                return false;
            return true;
        }

        //-------------------------------------------------------------------------------------------------

        static void MinMax(const float* src, size_t size, float& min, float& max)
        {
            min = FLT_MAX;
            max = -FLT_MAX;
            for (size_t i = 0; i < size; ++i)
            {
                min = Simd::Min(src[i], min);
                max = Simd::Max(src[i], max);
            }
        }

        static void Encode8(const float* src, float scale, float min, size_t size, uint8_t* dst)
        {
            for (size_t i = 0; i < size; ++i)
                dst[i] = Round((src[i] - min) * scale);
        }

        //-------------------------------------------------------------------------------------------------

        DescrInt::DescrInt(size_t size, size_t depth)
            : _size(size)
            , _depth(depth)
        {
            _encSize = 8 + DivHi(size * depth, 8);
            _minMax = MinMax;
            switch (depth)
            {
            case 8: _encode = Encode8; break;
            default:
                assert(0);
            }
        }

        void DescrInt::Encode(const float* src, uint8_t* dst) const
        {
            float min, max;
            _minMax(src, _size, min, max);
            max = min + Simd::Max(max - min, SIMD_DESCR_INT_EPS);
            float scale = 255.0f / (max - min);
            ((float*)dst)[0] = 1.0f / scale;
            ((float*)dst)[1] = min;
            _encode(src, scale, min, _size, dst + 8);
        }

        void DescrInt::Decode(const uint8_t* src, float* dst) const
        {

        }

        void DescrInt::CosineDistance(const uint8_t* a, const uint8_t* b, float* distance) const
        {

        }

        void DescrInt::CosineDistancesMxNa(size_t M, size_t N, const uint8_t* const* A, const uint8_t* const* B, float* distances) const
        {

        }

        void DescrInt::CosineDistancesMxNp(size_t M, size_t N, const uint8_t* A, const uint8_t* B, float* distances) const
        {

        }

        void DescrInt::VectorNormNa(size_t N, const uint8_t* const* A, float* norms) const
        {

        }

        void DescrInt::VectorNormNp(size_t N, const uint8_t* A, float* norms) const
        {

        }

        //-------------------------------------------------------------------------------------------------

        void* DescrIntInit(size_t size, size_t depth)
        {
            if(!Base::DescrIntValid(size, depth))
                return NULL;
            return new Base::DescrInt(size, depth);
        }
    }
}