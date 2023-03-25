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
     infinity

\*---------------------------------------------------------------------------*/

#ifndef TMCINFINITY_H
#define TMCINFINITY_H
#include <limits>

#include "TmcLimits.h"
#include <common/utilities/TmcSystem.h>


//////////////////////////////////////////////////////////////////////////
//
//  TmcNegInfinity
//  Remark:  small template class, since you end up using an instance "inf"
//           which can be converted into "different"(!!!) types and not
//           only into the template type!
//  Note: The TmcNegInfinity class cannot be instantiated on its own, but works
//        as a base class for the Infinity class.
//////////////////////////////////////////////////////////////////////////
class TmcNegInfinity
{
 public:
   //name Conversion operators
   inline operator signed char() const { return TmcLimits<signed char>::ninf(); }
   inline operator char()        const { return TmcLimits<char>::ninf();        }
   inline operator wchar_t()     const { return TmcLimits<wchar_t>::ninf();     }
   inline operator short()       const { return TmcLimits<short>::ninf();       }
   inline operator int()         const { return TmcLimits<int>::ninf();         }
   inline operator long()        const { return TmcLimits<long>::ninf();        }
   inline operator float()       const { return TmcLimits<float>::ninf();       }
   inline operator double()      const { return TmcLimits<double>::ninf();      }
   inline operator long double() const { return TmcLimits<long double>::ninf(); }

   // This function compares built-in data types with their largest possible value. The function
   // only works for built-in data types. The attempt to compare user-defined class types will
   // result in a compile time error.
   template< typename T >
   inline bool equal( const T& rhs ) const
   {
      UB_STATIC_ASSERT( std::numeric_limits<T>::is_specialized );
      return TmcLimits<T>::ninf() == rhs;
   }
 protected:
    inline TmcNegInfinity() {}

 private:
   TmcNegInfinity( const TmcNegInfinity& ninf );             //copy constructor (private & undefined)
   TmcNegInfinity& operator=( const TmcNegInfinity& ninf );  //copy assignment operator (private & undefined)
   void* operator&() const;                                //address operator (private & undefined)
};

//=================================================================================================
//
//  GLOBAL OPERATORS
//
//=================================================================================================
template< typename T >
inline bool operator==( const TmcNegInfinity& lhs, const T& rhs )
{
   return lhs.equal( rhs );
}
//*************************************************************************************************
template< typename T >
inline bool operator==( const T& lhs, const TmcNegInfinity& rhs )
{
   return rhs.equal( lhs );
}
//*************************************************************************************************
template< typename T >
inline bool operator!=( const TmcNegInfinity& lhs, const T& rhs )
{
   return !lhs.equal( rhs );
}
//*************************************************************************************************
template< typename T >
inline bool operator!=( const T& lhs, const TmcNegInfinity& rhs )
{
   return !rhs.equal( lhs );
}

//////////////////////////////////////////////////////////////////////////
//
//  TmcInfinity
//
//////////////////////////////////////////////////////////////////////////
class TmcInfinity : public TmcNegInfinity //um sp�terTmconInfinity leichter zu implementieren!!!
{
 public:
   inline TmcInfinity()
      : TmcNegInfinity()
    {}

   inline operator unsigned char()  const  { return TmcLimits<unsigned char>::inf();  }
   inline operator signed char()    const  { return TmcLimits<signed char>::inf();    }
   inline operator char()           const  { return TmcLimits<char>::inf();           }
   inline operator wchar_t()        const  { return TmcLimits<wchar_t>::inf();        }
   inline operator unsigned short() const  { return TmcLimits<unsigned short>::inf(); }
   inline operator short()          const  { return TmcLimits<short>::inf();          }
   inline operator unsigned int()   const  { return TmcLimits<unsigned int>::inf();   }
   inline operator int()            const  { return TmcLimits<int>::inf();            }
   inline operator unsigned long()  const  { return TmcLimits<unsigned long>::inf();  }
   inline operator long()           const  { return TmcLimits<long>::inf();           }
   inline operator float()          const  { return TmcLimits<float>::inf();          }
   inline operator double()         const  { return TmcLimits<double>::inf();         }
   inline operator long double()    const  { return TmcLimits<long double>::inf();    }

   inline const TmcNegInfinity& operator-() const { return static_cast<const TmcNegInfinity&>( *this ); }

   /*==========================================================*/
   template< typename T >
   inline bool equal( const T& rhs ) const
   {
      UB_STATIC_ASSERT( std::numeric_limits<T>::is_specialized );
      return TmcLimits<T>::inf() == rhs;
   }

 private:
   TmcInfinity( const TmcInfinity& inf );             //Copy constructor (private & undefined)
   TmcInfinity& operator=( const TmcInfinity& inf );  //Copy assignment operator (private & undefined)
   void* operator&() const;                         //Address operator (private & undefined)
};

//////////////////////////////////////////////////////////////////////////
//  GLOBAL OPERATORS
//////////////////////////////////////////////////////////////////////////
template< typename T >
inline bool operator==( const TmcInfinity& lhs, const T& rhs );

template< typename T >
inline bool operator==( const T& lhs, const TmcInfinity& rhs );

template< typename T >
inline bool operator!=( const TmcInfinity& lhs, const T& rhs );

template< typename T >
inline bool operator!=( const T& lhs, const TmcInfinity& rhs );
//@}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Equality comparison between an Infinity object and a built-in data type.
// \ingroup util
//
// This operator works only for built-in data types. The attempt to compare user-defined class
// types will result in a compile time error.
*/
template< typename T >
inline bool operator==( const TmcInfinity& lhs, const T& rhs )
{
   return lhs.equal( rhs );
}
//*************************************************************************************************
template< typename T >
inline bool operator==( const T& lhs, const TmcInfinity& rhs )
{
   return rhs.equal( lhs );
}
//*************************************************************************************************
template< typename T >
inline bool operator!=( const TmcInfinity& lhs, const T& rhs )
{
   return !lhs.equal( rhs );
}
//*************************************************************************************************
template< typename T >
inline bool operator!=( const T& lhs, const TmcInfinity& rhs )
{
   return !rhs.equal( lhs );
}
//*************************************************************************************************

//////////////////////////////////////////////////////////////////////////
//  GLOBAL INFINITY VALUE
//////////////////////////////////////////////////////////////////////////
namespace Tmc
{
   //e.g. double x = TmcSystem::inf;  float x = -Tmc::inf;
   const TmcInfinity inf;
}

#endif //TMCINFINITY_H
