/*---------------------------------------------------------------------------*\

        .----------------.  .----------------.  .----------------.
       | .--------------. || .--------------. || .--------------. |
       | |  _________   | || | ____    ____ | || |     ______   | |
       | | |  _   _  |  | || ||_   \  /   _|| || |   .' ___  |  | |
       | | |_/ | | \_|  | || |  |   \/   |  | || |  / .'   \_|  | |
       | |     | |      | || |  | |\  /| |  | || |  | |         | |
       | |    _| |_     | || | _| |_\/_| |_ | || |  \ `.___.'\  | |
       | |   |_____|    | || ||_____||_____|| || |   `._____.'  | |
       | |              | || |              | || |              | |
       | '--------------' || '--------------' || '--------------' |
        '----------------'  '----------------'  '----------------'

 ------------------------------------------------------------------------------
 Original Copyright (C) 2007-2011 Soeren Textor, Sebastian Geller (VirtualFluids, TU Braunschweig, Germany)
 Modifications Copyright (C) 2022-2023 Sebastian Geller

 This software is distributed WITHOUT ANY WARRANTY.

 License
    This file has been incorporated in TMC.

    TMC is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TMC is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with TMC (see LICENSE.txt). If not, see <http://www.gnu.org/licenses/>.

 Description
     basic math

\*---------------------------------------------------------------------------*/

#ifndef TmcMath_H_
#define TmcMath_H_

#include <complex>
#include <algorithm>
#include <common/utilities/TmcSystem.h>
#include <common/math/TmcEqual.h>
#include <boost/serialization/access.hpp>
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkFieldData.h"

#include <TmcMacroFile.h>

namespace TmcMath
{

    static const rType GREAT = 1.0e+6;
    static const rType VGREAT = 1.0e+37;
    static const rType SMALL = 1.0e-6;
    static const rType VSMALL = 1.0e-37;

    rType Pi();

    //////////////////////////////////////////////////////////////////////////
    /// start TmcMath from SG
    //////////////////////////////////////////////////////////////////////////

    TMC_DLL_EXPORT extern const double PI;

    // helper function for accuracy
    template <typename T>
    struct Epsilon
    {
    };

    //////////////////////////////////////////////////////////////////////////
    //  SPECIALIZATIONS von Epsilon
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct Epsilon<double>
    {
        static inline double val() { return 1.0E-11; }
    };
    template <>
    struct Epsilon<float>
    {
        static inline float val() { return 1.0E-7f; }
    };
    template <>
    struct Epsilon<long double>
    {
        static inline long double val() { return 1.0E-15; }
    };
    template <>
    struct Epsilon<int>
    {
        static inline int val() { return 0; }
    };

    /*=======================================================*/
    // -------------------------------------------------------------------------------------------------
    // Funktion berechnet den Logarithmus einer Zahl z bzgl. der Basis b
    // -------------------------------------------------------------------------------------------------
    template <typename T>
    inline T log(const T &z, const T &base)
    {
        if (::log(base) == 0)
            return 1.0f;
        return ::log(z) / ::log(base);
    }
    /*=======================================================*/
    // double x = TmcMath::getNegativeInfinity<double>();
    template <typename T>
    inline T getNegativeInfinity()
    {
        // assert(std::numeric_limits<T>::has_infinity);
        UB_STATIC_ASSERT(std::numeric_limits<T>::has_infinity);
        return -std::numeric_limits<T>::infinity();
    }
    /*=======================================================*/
    // double x = TmcMath::getPositiveInfinity<double>();
    template <typename T>
    inline T getPositiveInfinity()
    {
        // assert(std::numeric_limits<T>::has_infinity);
        UB_STATIC_ASSERT(std::numeric_limits<T>::has_infinity);
        return std::numeric_limits<T>::infinity();
    }
    /*=======================================================*/
    // double x; bool b = TmcMath::isInfinity(x);
    template <typename T>
    inline bool isInfinity(const T &value)
    {
        if (value == getNegativeInfinity<T>())
            return true;
        if (value == getPositiveInfinity<T>())
            return true;
        return false;
    }
    /*=======================================================*/
    // double x = TmcMath::getNaN<double>(x);
    template <typename T>
    inline T getNaN()
    {
        UB_STATIC_ASSERT(std::numeric_limits<T>::has_quiet_NaN);
        return std::numeric_limits<T>::quiet_NaN();
    }
    /*=======================================================*/
    // double x; bool b = TmcMath::isNaN(x);
    //  x!=x liefert bei #QNAN "true"!
    template <typename T>
    inline bool isNaN(const T &x)
    {
        UB_STATIC_ASSERT(std::numeric_limits<T>::has_quiet_NaN);
        return (x != x);
    }
    /*=======================================================*/
    template <typename T>
    inline T getEqualityEpsilon()
    {
        return Epsilon<T>::val();
    }
    /*=======================================================*/
    template <typename T>
    inline bool zero(const T &value)
    {
        return std::fabs(value) < Epsilon<T>::val();
        // return value >= -TmcMath::EPSILON && value <= TmcMath::EPSILON;
    }
    /*=======================================================*/
    // spezialisierung fuer ints
    template <>
    inline bool zero(const int &value)
    {
        return value == 0;
    }
    /*=======================================================*/
    template <typename T1, typename T2>
    inline bool zero(const T1 &value1, const T2 &value2)
    {
        return !(!TmcMath::zero(value1) || !TmcMath::zero(value2));
    }
    /*=======================================================*/
    template <typename T1, typename T2, typename T3>
    inline bool zero(const T1 &value1, const T2 &value2, const T3 &value3)
    {
        return !(!TmcMath::zero(value1) || !TmcMath::zero(value2, value3));
    }
    /*=======================================================*/
    template <typename T>
    inline bool negative(const T &value)
    {
        return value < -Epsilon<T>::val();
    }
    /*=======================================================*/
    template <typename T>
    inline bool nonPositive(const T &value)
    {
        return value <= Epsilon<T>::val();
    }
    /*=======================================================*/
    template <typename T>
    inline bool positive(const T &value)
    {
        return value > +Epsilon<T>::val();
    }
    /*=======================================================*/
    template <typename T>
    inline bool nonNegative(const T &value)
    {
        return value >= -Epsilon<T>::val();
    }
    /*=======================================================*/
    template <typename T1, typename T2>
    inline bool equal(const T1 &value, const T2 &reference)
    {
        typedef typename TmcEqualTrait<T1, T2>::High High;
        return std::fabs(value - reference) < Epsilon<High>::val();
    }
    /*=======================================================*/
    template <typename T1, typename T2, typename T3>
    inline bool equal(const T1 &val1, const T2 &val2, const T3 &val3)
    {
        return (TmcMath::equal(val1, val2) && TmcMath::equal(val1, val3));
    }
    /*=======================================================*/
    template <typename T1, typename T2>
    inline bool less(const T1 &value, const T2 &reference)
    {
        typedef typename TmcEqualTrait<T1, T2>::High High;
        return value < reference - Epsilon<High>::val();
    }
    /*=======================================================*/
    template <typename T1, typename T2>
    inline bool lessEqual(const T1 &value, const T2 &reference)
    {
        typedef typename TmcEqualTrait<T1, T2>::High High;
        return value <= reference + Epsilon<High>::val();
    }
    /*=======================================================*/
    template <typename T1, typename T2>
    inline bool greater(const T1 &value, const T2 &reference)
    {
        typedef typename TmcEqualTrait<T1, T2>::High High;
        return value > reference + Epsilon<High>::val();
    }
    /*=======================================================*/
    template <typename T1, typename T2>
    inline bool greaterEqual(const T1 &value, const T2 &reference)
    {
        typedef typename TmcEqualTrait<T1, T2>::High High;
        return value >= reference - Epsilon<High>::val();
    }
    /*=======================================================*/
    template <typename T>
    inline T round(const T &value, const int &decimalPlaces)
    {
        return static_cast<T>(floor(value * pow(10.0, decimalPlaces) + 0.5) * pow(10.0, -decimalPlaces));
    }
    /*=======================================================*/
    template <typename T>
    inline int integerRounding(const T &value)
    {
        return static_cast<int>(TmcMath::zero(value) ? 0 : ((value < 0.0) ? (value - 0.5) : (value + 0.5)));
    }
    /*=======================================================*/
    template <typename T>
    inline T getRad(const T &degrees)
    {
        return degrees * static_cast<T>(TmcMath::PI / 180.0);
    }
    /*=======================================================*/
    template <typename T>
    inline T getDegrees(const T &rad)
    {
        return rad * static_cast<T>(180.0 / TmcMath::PI);
    }
    /*=======================================================*/
    // aus wildmagic
    template <typename T>
    inline T ACos(const T &fValue)
    {
        if (-1.0 < fValue)
        {
            if (fValue < 1.0)
                return static_cast<T>(acos(fValue));
            else
                return static_cast<T>(0.0);
        }
        else
            return static_cast<T>(PI);
    }
    /*=======================================================*/
    template <typename T>
    inline T ASin(const T &fValue)
    {
        double HALF_PI = 0.5 * TmcMath::PI;
        if (-1.0 < fValue)
        {
            if (fValue < 1.0)
                return static_cast<T>(asin(fValue));
            else
                return static_cast<T>(HALF_PI);
        }
        else
            return -static_cast<T>(HALF_PI);
    }
    /*=======================================================*/
    template <typename T>
    inline T invSqrt(const T &fValue)
    {
        return static_cast<T>(1.0 / sqrt(fValue));
    }

    /*=======================================================*/
    /**
     * Returns true, if specified values a and b are less both values c and d.
     * @param a the first value to check
     * @param b the second value to check
     * @param c the first value to check against
     * @param d the second value to check against
     * @return true, if specified values a and b are less both values c and d
     **/
    template <typename T1, typename T2, typename T3, typename T4>
    inline bool less2(const T1 &value1, const T2 &value2, T3 toBeLessAs1, T4 toBeLessAs2)
    {
        return (less(value1, toBeLessAs1) && less(value1, toBeLessAs2) && less(value2, toBeLessAs1) && less(value2, toBeLessAs2));
    }
    /*=======================================================*/
    template <typename T1, typename T2, typename T3, typename T4>
    inline bool greater2(const T1 &value1, const T2 &value2, T3 toBeGreaterAs1, T4 toBeGreaterAs2)
    {
        return (greater(value1, toBeGreaterAs1) && greater(value1, toBeGreaterAs2) && greater(value2, toBeGreaterAs1) && greater(value2, toBeGreaterAs2));
    }
    /*=======================================================*/
    template <typename T1, typename T2, typename T3>
    inline bool inClosedInterval(const T1 &value, const T2 &threshold1, const T3 &threshold2)
    {
        if (threshold1 < threshold2)
        {
            return (greaterEqual(value, threshold1) && lessEqual(value, threshold2));
        }

        return (greaterEqual(value, threshold2) && lessEqual(value, threshold1));
    }
    /*=======================================================*/
    template <typename T1, typename T2, typename T3>
    inline bool inOpenInterval(const T1 &value, const T2 &threshold1, const T3 &threshold2)
    {
        if (threshold1 < threshold2)
        {
            return (greater(value, threshold1) && less(value, threshold2));
        }

        return (greater(value, threshold2) && less(value, threshold1));
    }
    /*=======================================================*/
    template <typename T1, typename T2, typename T3>
    inline double adaptToClosedInterval(const T1 &value, const T2 &threshold1, const T3 &threshold2)
    {
        if (threshold1 < threshold2)
        {
            if (less(value, threshold1))
                return threshold1;
            else if (greater(value, threshold2))
                return threshold2;
        }
        else
        {
            if (less(value, threshold2))
                return threshold2;
            else if (greater(value, threshold1))
                return threshold1;
        }
        return value;
    }
    /*=======================================================*/
    // -------------------------------------------------------------------------------------------------
    // Funktion berechnet den groessten gemeinsamen Teiler zweier Zahlen (MK)
    // -------------------------------------------------------------------------------------------------
    /*=======================================================*/
    inline int calcGgt(int val1, int val2)
    {
        if (val1 < val2)
            std::swap(val1, val2);
        int ggt = val2;
        while (ggt > 1)
        {
            if ((val1 % ggt) == 0 && (val2 % ggt) == 0)
                break;

            ggt -= 1;
        }
        return ggt;
    }
    /*=======================================================*/
    // -------------------------------------------------------------------------------------------------
    // Funktion berechnet den groessten gemeinsamen Teiler von drei Zahlen (MK)
    // -------------------------------------------------------------------------------------------------
    inline int calcGgt(int val1, const int &val2, int val3)
    {
        return TmcMath::calcGgt(TmcMath::calcGgt(val1, val2), val3);
    }
    /*=======================================================*/
    // returns the max of two values
    // to avoid errors at mixed argument-types use: double myMax = max<double>(2,2.3);
    template <typename T>
    inline const T &max(const T &a1, const T &a2)
    {
        return (a1 < a2) ? a2 : a1;
    }
    /*=======================================================*/
    template <typename T>
    inline const T &max(const T &a1, const T &a2, const T &a3)
    {
        return max(max(a1, a2), a3);
    }
    /*=======================================================*/
    template <typename T>
    inline const T &max(const T &a1, const T &a2, const T &a3, const T &a4)
    {
        return max(max(max(a1, a2), a3), a4);
    }
    /*=======================================================*/
    template <typename T>
    inline const T &max(const T &a1, const T &a2, const T &a3, const T &a4, const T &a5, const T &a6)
    {
        return max(max(max(max(max(a1, a2), a3), a4), a5), a6);
    }
    /*=======================================================*/
    template <typename T>
    inline const T &max(const T &a1, const T &a2, const T &a3, const T &a4, const T &a5, const T &a6, const T &a7, const T &a8)
    {
        return max(max(max(max(max(max(max(a1, a2), a3), a4), a5), a6), a7), a8);
    }
    /*=======================================================*/
    template <typename T>
    inline const T &min(const T &a1, const T &a2)
    {
        return (a1 < a2) ? a1 : a2;
    }
    /*=======================================================*/
    template <typename T>
    inline const T &min(const T &a1, const T &a2, const T &a3)
    {
        return min(min(a1, a2), a3);
    }
    /*=======================================================*/
    template <typename T>
    inline const T &min(const T &a1, const T &a2, const T &a3, const T &a4)
    {
        return min(min(min(a1, a2), a3), a4);
    }
    /*=======================================================*/
    template <typename T>
    inline const T &min(const T &a1, const T &a2, const T &a3, const T &a4, const T &a5, const T &a6)
    {
        return min(min(min(min(min(a1, a2), a3), a4), a5), a6);
    }
    /*=======================================================*/
    template <typename T>
    inline const T &min(const T &a1, const T &a2, const T &a3, const T &a4, const T &a5, const T &a6, const T &a7, const T &a8)
    {
        return min(min(min(min(min(min(min(a1, a2), a3), a4), a5), a6), a7), a8);
    }
    /*=======================================================*/
    //////////////////////////////////////////////////////////////////////////
    //
    // constants
    //
    //////////////////////////////////////////////////////////////////////////
    static const double c1o72 = 1. / 72.; // 0.01388888
    static const double c1o36 = 1. / 36.; // 0.02777777
    static const double c1o48 = 1. / 48.; // 0.02083333
    static const double c1o32 = 1. / 32.; // 0.03125
    static const double c1o24 = 1. / 24.; // 0.04166666
    static const double c1o20 = 1. / 20.; // 0.05
    static const double c1o18 = 1. / 18.; // 0.05555555
    static const double c1o16 = 1. / 16.; // 0.0625
    static const double c1o12 = 1. / 12.; // 0.08333333
    static const double c1o9 = 1. / 9.;   // 0.11111111
    static const double c1o8 = 1. / 8.;   // 0.125
    static const double c1o6 = 1. / 6.;   // 0.16666666
    static const double c1o5 = 1. / 5.;   // 0.2
    static const double c1o4 = 1. / 4.;   // 0.25
    static const double c5o16 = 5. / 16.; // 0.3125
    static const double c1o3 = 1. / 3.;   // 0.33333333
    static const double c3o8 = 3. / 8.;   // 0.375
    static const double c4o9 = 4. / 9.;   // 0.44444444
    static const double c1o2 = 1. / 2.;   // 0.5
    static const double c9o16 = 9. / 16.; // 0.5625
    static const double c2o3 = 2. / 3.;   // 0.66666666
    static const double c3o4 = 3. / 4.;   // 0.75
    static const double c4o3 = 4. / 3.;   // 1.33333333
    static const double c5o3 = 5. / 3.;   // 1.66666666
    static const double c9o5 = 9. / 5.;   // 1.8
    static const double c2o9 = 2. / 9.;   // 0.22222222
    static const double c8o27 = 8. / 27.;
    static const double c2o27 = 2. / 27.;
    static const double c1o54 = 1. / 54.;
    static const double c1o216 = 1. / 216.;
    static const double one_over_sqrt2 = 1.0 / sqrt(2.0); // 0.707106781
    static const double one_over_sqrt3 = 1.0 / sqrt(3.0); // 0.577350269
    static const double sqrt2 = sqrt(2.0);                // 1.4142135
    static const double sqrt3 = sqrt(3.0);                // 1.7320508

    void randomize(rType col[3]);

} // namespace TmcMath

#endif /* TmcMath_H_ */
