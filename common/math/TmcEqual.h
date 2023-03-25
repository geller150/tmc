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
     compares equality between to values a and b

\*---------------------------------------------------------------------------*/

#ifndef TMCEQUAL_H
#define TMCEQUAL_H

#include<cmath>

//////////////////////////////////////////////////////////////////////////
//isTmcEqual<T1,T2>(a,b)
//compares the equality of the two values a and b
//
//the operator== is used for this purpose.
//
//exception: floating-points
// here the "more precise type is cast to the less precise one and then compared".
//e.g.: double d=1.2; int i=1; bool check = isTmcEqual(d,i); -> true
//
//for classes, operator== must be implemented here for const objecte!!!
//e.g.: bool operator==(const Test&) const { if(blabla) return true; else return false; }
//////////////////////////////////////////////////////////////////////////

//std-trait, for all not specified types
template < typename T1, typename T2 >
struct TmcEqualTrait
{
   typedef T1 High;
   typedef T1 Low;
};

//std-trait, fuer gleiche T
template < typename T >
struct TmcEqualTrait< T, T >
{
   typedef T High;
   typedef T Low;
};

//specialization for multiple type tuples
template<> struct TmcEqualTrait< short, int >          { typedef int         High; typedef short  Low; };
template<> struct TmcEqualTrait< short, long >         { typedef long        High; typedef short  Low; };
template<> struct TmcEqualTrait< short, float >        { typedef float       High; typedef short  Low; };
template<> struct TmcEqualTrait< short, double >       { typedef double      High; typedef short  Low; };
template<> struct TmcEqualTrait< short, long double >  { typedef long double High; typedef short  Low; };

template<> struct TmcEqualTrait< int, short >          { typedef int         High; typedef short  Low; };
template<> struct TmcEqualTrait< int, long >           { typedef long        High; typedef int    Low; };
template<> struct TmcEqualTrait< int, float >          { typedef float       High; typedef int    Low; };
template<> struct TmcEqualTrait< int, double >         { typedef double      High; typedef int    Low; };
template<> struct TmcEqualTrait< int, long double >    { typedef long double High; typedef int    Low; };

template<> struct TmcEqualTrait< long, short >         { typedef long        High; typedef short  Low; };
template<> struct TmcEqualTrait< long, int >           { typedef long        High; typedef int    Low; };
template<> struct TmcEqualTrait< long, float >         { typedef float       High; typedef long   Low; };
template<> struct TmcEqualTrait< long, double >        { typedef double      High; typedef long   Low; };
template<> struct TmcEqualTrait< long, long double >   { typedef long double High; typedef long   Low; };

template<> struct TmcEqualTrait< float, short >        { typedef float       High; typedef short  Low; };
template<> struct TmcEqualTrait< float, int >          { typedef float       High; typedef int    Low; };
template<> struct TmcEqualTrait< float, long >         { typedef float       High; typedef long   Low; };
template<> struct TmcEqualTrait< float, double >       { typedef double      High; typedef float  Low; };
template<> struct TmcEqualTrait< float, long double >  { typedef long double High; typedef float  Low; };

template<> struct TmcEqualTrait< double, short >       { typedef double      High; typedef short  Low; };
template<> struct TmcEqualTrait< double, int >         { typedef double      High; typedef int    Low; };
template<> struct TmcEqualTrait< double, long >        { typedef double      High; typedef long   Low; };
template<> struct TmcEqualTrait< double, float >       { typedef double      High; typedef float  Low; };
template<> struct TmcEqualTrait< double, long double > { typedef long double High; typedef double Low; };

template<> struct TmcEqualTrait< long double, short >  { typedef long double High; typedef short  Low; };
template<> struct TmcEqualTrait< long double, int >    { typedef long double High; typedef int    Low; };
template<> struct TmcEqualTrait< long double, long >   { typedef long double High; typedef long   Low; };
template<> struct TmcEqualTrait< long double, float >  { typedef long double High; typedef float  Low; };
template<> struct TmcEqualTrait< long double, double > { typedef long double High; typedef double Low; };

//////////////////////////////////////////////////////////////////////////
//for general types ( operator== ):
template< typename T1, typename T2 >
inline bool specific_equal(const T1& a, const T2& b) { return a==b; }

//////////////////////////////////////////////////////////////////////////
//for floating point build-in-type
//float.float
template< /*float,float*/>
inline bool specific_equal< float, float >(const float& a, const float& b) {  return std::fabs( a - b ) < 1E-8; }

template</*double,double*/>
inline bool specific_equal< double, double >(const double& a, const double& b) { return std::fabs( a - b ) < 1E-13; }

template</*long double,long double*/>
inline bool specific_equal< long double, long double >(const long double& a, const long double& b) { return std::fabs( a - b ) < 1E-16; }

//////////////////////////////////////////////////////////////////////////
//global isTmcEqual - funktion
template< typename T1, typename T2 >
inline bool isTmcEqual(const T1& a, const T2& b)
{
   typedef typename TmcEqualTrait<T1,T2>::Low Low;
   return specific_equal< Low, Low >(static_cast< Low >( a ),static_cast< Low >( b ));
};

//////////////////////////////////////////////////////////////////////////
//TmcEqual-Functor
template< typename T1, typename T2 >
struct TmcEqual
{
   bool operator()(const T1& a, const T2& b)
   {
      return isTmcEqual(a,b);
   }
};

#endif //TMCEQUAL_H
