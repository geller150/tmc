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
 Original Copyright (C) 2008-2011 Soeren Textor (VirtualFluids, TU Braunschweig, Germany)
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
     limits

\*---------------------------------------------------------------------------*/

#ifndef TMCLIMITS_H
#define TMCLIMITS_H

//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <limits>

//////////////////////////////////////////////////////////////////////////
//  CLASS DEFINITION
//////////////////////////////////////////////////////////////////////////
template <typename T>
struct TmcLimits
{
};

//////////////////////////////////////////////////////////////////////////
//  SPECIALIZATIONS
//////////////////////////////////////////////////////////////////////////
template <>
struct TmcLimits<unsigned char>
{
    // return the largest possible positive unsigned char value
    static inline unsigned char inf() { return std::numeric_limits<unsigned char>::max(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<char>
{
    // return the largest possible positive char value. */
    static inline char inf() { return std::numeric_limits<char>::max(); }
    // return the largest possible negative char value
    static inline char ninf() { return std::numeric_limits<char>::min(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<signed char>
{
    // return the largest possible positive signed char value
    static inline signed char inf() { return std::numeric_limits<signed char>::max(); }

    // return The largest possible negative signed char value
    static inline signed char ninf() { return std::numeric_limits<signed char>::min(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<wchar_t>
{
    // return The largest possible positive wchar_t value
    static inline wchar_t inf() { return std::numeric_limits<wchar_t>::max(); }
    // return The largest possible negative wchar_t value
    static inline wchar_t ninf() { return std::numeric_limits<wchar_t>::min(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<unsigned short>
{
    // return The largest possible positive unsigned short value
    static inline unsigned short inf() { return std::numeric_limits<unsigned short>::max(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<short>
{
    // return The largest possible positive short value
    static inline short inf() { return std::numeric_limits<short>::max(); }
    // return The largest possible negative short value
    static inline short ninf() { return std::numeric_limits<short>::min(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<unsigned int>
{
    // return The largest possible positive unsigned int value
    static inline unsigned int inf() { return std::numeric_limits<unsigned int>::max(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<int>
{
    // return The largest possible positive int value
    static inline int inf() { return std::numeric_limits<int>::max(); }

    // return The largest possible negative int value
    static inline int ninf() { return std::numeric_limits<int>::min(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<unsigned long>
{
    // return The largest possible positive unsigned long value
    static inline unsigned long inf() { return std::numeric_limits<unsigned long>::max(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<long>
{
    // return The largest possible positive long value
    static inline long inf() { return std::numeric_limits<long>::max(); }

    // return The largest possible negative long value
    static inline long ninf() { return std::numeric_limits<long>::min(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<float>
{
    // return The largest possible positive float value
    static inline float inf() { return std::numeric_limits<float>::max(); }

    // return The largest possible negative float value
    static inline float ninf() { return -std::numeric_limits<float>::max(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<double>
{
    // return The largest possible positive double value
    static inline double inf() { return std::numeric_limits<double>::max(); }
    // return The largest possible negative double value
    static inline double ninf() { return -std::numeric_limits<double>::max(); }
};
//*************************************************************************************************
template <>
struct TmcLimits<long double>
{
    // return The largest possible positive long double value
    static inline long double inf() { return std::numeric_limits<long double>::max(); }
    // return The largest possible negative long double value
    static inline long double ninf() { return -std::numeric_limits<long double>::max(); }
};

#endif // TMCLIMITS_H
