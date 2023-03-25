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
 Original Copyright (C) 2006-2011 Soeren Textor (VirtualFluids, TU Braunschweig, Germany)
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
    tuple

\*---------------------------------------------------------------------------*/

#ifndef TMCTUPLE_H
#define TMCTUPLE_H

#include <iostream>
#include <string>

/*
usage: ...
////Advanced TmcTuple
//Bsp:
//// create and use tuple with only one field
//TmcTuple<int,int,int,int,int> t1;
//val<1>(t1) += 42;
//std::cout << t1.v1() << std::endl;

//// create and use duo
//TmcTuple<bool,int> t2;
//std::cout << val<1>(t2) << ", ";
//std::cout << t2.v1() << std::endl;

//// create and use triple
//TmcTuple<bool,int,double> t3;
//val<1>(t3) = true;  // new values via: val< pos >(triple) = ...
//val<2>(t3) = 42;
//val<3>(t3) = 0.2;
//t3 = makeTmcTuple(false, 23, 13.13);

//std::cout << val<1>(t3) << ", ";
//std::cout << val<2>(t3) << ", ";
//std::cout << val<3>(t3) << std::endl;

//// create and use quadruple
//TmcType<bool,int,float,double> t4(true,42,13,1.95583);
//std::cout << val<4>(t4) << std::endl;        //<- option 2 (std)
//std::cout << t4.v2().v2().v2() << std::endl; //<- option 2
*/

// typeop.h
//  primary template
/**********************************
 * typeop1.hpp:
 **********************************/
template <typename T>
class TmcTypeOp // primary template
{
public:
    typedef T ArgT;
    typedef T BareT;
    typedef T const ConstT;
    typedef T &RefT;
    typedef T &RefBareT;
    typedef T const &RefConstT;
};
/**** end of typeop1.hpp ****/

// partial specialization for const
/**********************************
 * typeop2.hpp:
 **********************************/
template <typename T>
class TmcTypeOp<T const> // partial specialization for const types
{
public:
    typedef T const ArgT;
    typedef T BareT;
    typedef T const ConstT;
    typedef T const &RefT;
    typedef T &RefBareT;
    typedef T const &RefConstT;
};
/**** end of typeop2.hpp ****/

// partial specialization for references
/**********************************
 * typeop3.hpp:
 **********************************/
template <typename T>
class TmcTypeOp<T &> // partial specialization for references
{
public:
    typedef T &ArgT;
    typedef typename TmcTypeOp<T>::BareT BareT;
    typedef T const ConstT;
    typedef T &RefT;
    typedef typename TmcTypeOp<T>::BareT &RefBareT;
    typedef T const &RefConstT;
};
/**** end of typeop3.hpp ****/

// full specialization for void
/**********************************
 * typeop4.hpp:
 **********************************/
template <>
class TmcTypeOp<void> // full specialization for void
{
public:
    typedef void ArgT;
    typedef void BareT;
    typedef void const ConstT;
    typedef void RefT;
    typedef void RefBareT;
    typedef void RefConstT;
};
/**** end of typeop4.hpp ****/

// duo1.hpp
template <typename T1, typename T2>
class TmcDuo
{
public:
    typedef T1 Type1; // type of first field
    typedef T2 Type2; // type of second field
    enum
    {
        N = 2
    }; // number of fields

public:
    // constructors
    TmcDuo() : value1(), value2() {}
    TmcDuo(T1 const &a, T2 const &b) : value1(a), value2(b) {}

    // for implicit type conversion during construction
    template <typename U1, typename U2>
    TmcDuo(TmcDuo<U1, U2> const &d) : value1(d.v1()), value2(d.v2()) {}

    // for implicit type conversion during assignments
    template <typename U1, typename U2>
    TmcDuo<T1, T2> &operator=(TmcDuo<U1, U2> const &d)
    {
        value1 = d.v1(); // value1;
        value2 = d.v2(); // value2;
        return *this;
    }

    // field access
    T1 &v1() { return value1; }
    T1 const &v1() const { return value1; }

    T2 &v2() { return value2; }
    T2 const &v2() const { return value2; }

#ifdef CAB_RCF
    template <class Archive>
    void SF_SERIALIZE(Archive &ar)
    {
        ar &value1;
        ar &value2;
    }
#endif // CAB_RCF

private:
    T1 value1; // value of first field
    T2 value2; // value of second field
};

// comparison operators (allow mixed types):
template <typename T1, typename T2, typename U1, typename U2>
inline bool operator==(TmcDuo<T1, T2> const &d1, TmcDuo<U1, U2> const &d2)
{
    return d1.v1() == d2.v1() && d1.v2() == d2.v2();
}

template <typename T1, typename T2, typename U1, typename U2>
inline bool operator!=(TmcDuo<T1, T2> const &d1, TmcDuo<U1, U2> const &d2)
{
    return !(d1 == d2);
}

template <typename T1, typename T2, typename U1, typename U2>
inline bool operator<(TmcDuo<T1, T2> const &d1, TmcDuo<U1, U2> const &d2)
{
    if (d1.v1() < d2.v1())
        return true;
    else if (d1.v1() == d2.v1())
        return d1.v2() < d2.v2();

    return false;
}

// convenience function for creation and initialization
template <typename T1, typename T2>
inline TmcDuo<T1, T2> makeTmcDuo(T1 const &a, T2 const &b)
{
    return TmcDuo<T1, T2>(a, b);
}

// duo2.hpp
template <typename A, typename B, typename C>
class TmcDuo<A, TmcDuo<B, C>>
{
public:
    typedef A T1;            // type of first field
    typedef TmcDuo<B, C> T2; // type of second field
    enum
    {
        N = TmcDuo<B, C>::N + 1
    }; // number of fields

public:
    // constructors
    TmcDuo() : value1(), value2() {}
    TmcDuo(T1 const &a, T2 const &b) : value1(a), value2(b) {}

    // for implicit type conversion during construction
    template <typename U1, typename U2>
    TmcDuo(TmcDuo<U1, U2> const &d) : value1(d.v1()), value2(d.v2()) {}

    // for implicit type conversion during assignments
    template <typename U1, typename U2>
    TmcDuo<T1, T2> &operator=(TmcDuo<U1, U2> const &d)
    {
        value1 = d.v1(); // value1;
        value2 = d.v2(); // value2;
        return *this;
    }

    // field access
    T1 &v1() { return value1; }
    T1 const &v1() const { return value1; }

    T2 &v2() { return value2; }
    T2 const &v2() const { return value2; }

#ifdef CAB_RCF
    template <class Archive>
    void SF_SERIALIZE(Archive &ar)
    {
        ar &value1;
        ar &value2;
    }
#endif // CAB_RCF

private:
    T1 value1; // value of first field
    T2 value2; // value of second field
};

// duo3.hpp
//  primary template for type of Nth field of (duo) T
template <int N, typename T>
class TmcDuoT
{
public:
    typedef void ResultT; // in general, the result type is void
};

// specialization for 1st field of a plain duo
template <typename A, typename B>
class TmcDuoT<1, TmcDuo<A, B>>
{
public:
    typedef A ResultT;
};

// specialization for 2nd field of a plain duo
template <typename A, typename B>
class TmcDuoT<2, TmcDuo<A, B>>
{
public:
    typedef B ResultT;
};

// specialization for Nth field of a recursive duo
template <int N, typename A, typename B, typename C>
class TmcDuoT<N, TmcDuo<A, TmcDuo<B, C>>>
{
public:
    typedef typename TmcDuoT<N - 1, TmcDuo<B, C>>::ResultT ResultT;
};

// specialization for 1st field of a recursive duo
template <typename A, typename B, typename C>
class TmcDuoT<1, TmcDuo<A, TmcDuo<B, C>>>
{
public:
    typedef A ResultT;
};

// specialization for 2nd field of a recursive duo
template <typename A, typename B, typename C>
class TmcDuoT<2, TmcDuo<A, TmcDuo<B, C>>>
{
public:
    typedef B ResultT;
};

// duo4.hpp
//  primary template for value of Nth field of (duo) T
template <int N, typename T>
class DuoValue
{
public:
    static void get(T &) {} // in general, we have no value
    static void get(T const &) {}
};

// specialization for 1st field of a plain duo
template <typename A, typename B>
class DuoValue<1, TmcDuo<A, B>>
{
public:
    static A &get(TmcDuo<A, B> &d) { return d.v1(); }
    static A const &get(TmcDuo<A, B> const &d) { return d.v1(); }
};

// specialization for 2nd field of a plain duo
template <typename A, typename B>
class DuoValue<2, TmcDuo<A, B>>
{
public:
    static B &get(TmcDuo<A, B> &d) { return d.v2(); }
    static B const &get(TmcDuo<A, B> const &d) { return d.v2(); }
};

// specialization for Nth field of recursive duo
template <int N, typename A, typename B, typename C>
struct DuoValue<N, TmcDuo<A, TmcDuo<B, C>>>
{
    static typename TmcTypeOp<typename TmcDuoT<N - 1, TmcDuo<B, C>>::ResultT>::RefT get(TmcDuo<A, TmcDuo<B, C>> &d)
    {
        return DuoValue<N - 1, TmcDuo<B, C>>::get(d.v2());
    }
    static typename TmcTypeOp<typename TmcDuoT<N - 1, TmcDuo<B, C>>::ResultT>::RefConstT get(TmcDuo<A, TmcDuo<B, C>> const &d)
    {
        return DuoValue<N - 1, TmcDuo<B, C>>::get(d.v2());
    }
};

// specialization for 1st field of recursive duo
template <typename A, typename B, typename C>
class DuoValue<1, TmcDuo<A, TmcDuo<B, C>>>
{
public:
    static A &get(TmcDuo<A, TmcDuo<B, C>> &d) { return d.v1(); }
    static A const &get(TmcDuo<A, TmcDuo<B, C>> const &d) { return d.v1(); }
};

// specialization for 2nd field of recursive duo
template <typename A, typename B, typename C>
class DuoValue<2, TmcDuo<A, TmcDuo<B, C>>>
{
public:
    static B &get(TmcDuo<A, TmcDuo<B, C>> &d) { return d.v2().v1(); }
    static B const &get(TmcDuo<A, TmcDuo<B, C>> const &d) { return d.v2().v1(); }
};

// duo5.hpp
//  return Nth value of variable duo
template <int N, typename A, typename B>
inline typename TmcTypeOp<typename TmcDuoT<N, TmcDuo<A, B>>::ResultT>::RefT val(TmcDuo<A, B> &d)
{
    return DuoValue<N, TmcDuo<A, B>>::get(d);
}

// return Nth value of constant duo
template <int N, typename A, typename B>
inline typename TmcTypeOp<typename TmcDuoT<N, TmcDuo<A, B>>::ResultT>::RefConstT val(TmcDuo<A, B> const &d)
{
    return DuoValue<N, TmcDuo<A, B>>::get(d);
}

// duo6.hpp
//  partial specialization for TmcDuo<> with only one field
template <typename A>
struct TmcDuo<A, void>
{
public:
    typedef A T1;    // type of first field
    typedef void T2; // type of second field
    enum
    {
        N = 1
    }; // number of fields

private:
    T1 value1; // value of first field

public:
    // constructors
    TmcDuo() : value1() {}
    TmcDuo(T1 const &a) : value1(a) {}

    // field access
    T1 &v1() { return value1; }
    T1 const &v1() const { return value1; }

    void v2() {}
    void v2() const {}

#ifdef CAB_RCF
    template <class Archive>
    void SF_SERIALIZE(Archive &ar)
    {
        ar &value1;
    }
#endif
};

// tupel1.hpp
//  type that represents unused type parameters
class TmcNullT
{
};

// TmcTuple<> in general derives from TmcTuple<> with one more TmcNullT
template <typename P1,
          typename P2 = TmcNullT,
          typename P3 = TmcNullT,
          typename P4 = TmcNullT,
          typename P5 = TmcNullT,
          typename P6 = TmcNullT,
          typename P7 = TmcNullT,
          typename P8 = TmcNullT>
class TmcTuple : public TmcDuo<P1, typename TmcTuple<P2, P3, P4, P5, P6, P7, P8, TmcNullT>::BaseT>
{
public:
    typedef TmcDuo<P1, typename TmcTuple<P2, P3, P4, P5, P6, P7, P8, TmcNullT>::BaseT> BaseT;

    // constructor:
    TmcTuple() {}
    TmcTuple(typename TmcTypeOp<P1>::RefConstT a1,
             typename TmcTypeOp<P2>::RefConstT a2,
             typename TmcTypeOp<P3>::RefConstT a3 = TmcNullT(),
             typename TmcTypeOp<P4>::RefConstT a4 = TmcNullT(),
             typename TmcTypeOp<P5>::RefConstT a5 = TmcNullT(),
             typename TmcTypeOp<P6>::RefConstT a6 = TmcNullT(),
             typename TmcTypeOp<P7>::RefConstT a7 = TmcNullT(),
             typename TmcTypeOp<P8>::RefConstT a8 = TmcNullT())
        : BaseT(a1, TmcTuple<P2, P3, P4, P5, P6, P7, P8, TmcNullT>(a2, a3, a4, a5, a6, a7, a8))
    {
    }

    // for implicit type conversion during assignments
    template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7, typename U8>
    TmcTuple<P1, P2, P3, P4, P5, P6, P7, P8> &operator=(const TmcTuple<U1, U2, U3, U4, U5, U6, U7, U8> &rhs)
    {
        this->BaseT::operator=(typename TmcTuple<U1, U2, U3, U4, U5, U6, U7, U8>::BaseT(rhs));
        return *this;
    }
};

// specialization to end deriving recursion
template <typename P1, typename P2>
class TmcTuple<P1, P2, TmcNullT, TmcNullT, TmcNullT, TmcNullT, TmcNullT, TmcNullT> : public TmcDuo<P1, P2>
{
public:
    typedef TmcDuo<P1, P2> BaseT;

    // constructor:
    TmcTuple() {}
    TmcTuple(typename TmcTypeOp<P1>::RefConstT a1,
             typename TmcTypeOp<P2>::RefConstT a2,
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT())
        : BaseT(a1, a2)
    {
    }

    // for implicit type conversion during assignments
    template <typename U1, typename U2>
    TmcTuple<P1, P2> &operator=(const TmcTuple<U1, U2> &rhs)
    {
        this->BaseT::operator=(typename TmcTuple<U1, U2>::BaseT(rhs));
        return *this;
    }
};

// specialization for singletons
template <typename P1>
class TmcTuple<P1, TmcNullT, TmcNullT, TmcNullT, TmcNullT, TmcNullT, TmcNullT, TmcNullT> : public TmcDuo<P1, void>
{
public:
    typedef TmcDuo<P1, void> BaseT;

    // constructor:
    TmcTuple() {}
    TmcTuple(typename TmcTypeOp<P1>::RefConstT a1,
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT(),
             typename TmcTypeOp<TmcNullT>::RefConstT = TmcNullT())
        : BaseT(a1)
    {
    }

    // for implicit type conversion during assignments
    template <typename U1>
    TmcTuple<P1> &operator=(const TmcTuple<U1> &rhs)
    {
        this->v1() = rhs.v1();
        return *this;
    }
};

// convenience function for 1 argument
template <typename T1>
inline TmcTuple<T1> makeTmcTuple(T1 const &a1)
{
    return TmcTuple<T1>(a1);
}

// convenience function for 2 arguments
template <typename T1, typename T2>
inline TmcTuple<T1, T2> makeTmcTuple(T1 const &a1, T2 const &a2)
{
    return TmcTuple<T1, T2>(a1, a2);
}

// convenience function for 3 arguments
template <typename T1, typename T2, typename T3>
inline TmcTuple<T1, T2, T3> makeTmcTuple(T1 const &a1, T2 const &a2, T3 const &a3)
{
    return TmcTuple<T1, T2, T3>(a1, a2, a3);
}

// convenience function for 4 arguments
template <typename T1, typename T2, typename T3, typename T4>
inline TmcTuple<T1, T2, T3, T4> makeTmcTuple(T1 const &a1, T2 const &a2, T3 const &a3, T4 const &a4)
{
    return TmcTuple<T1, T2, T3, T4>(a1, a2, a3, a4);
}

// convenience function for 5 arguments
template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline TmcTuple<T1, T2, T3, T4, T5> makeTmcTuple(T1 const &a1, T2 const &a2, T3 const &a3, T4 const &a4, T5 const &a5)
{
    return TmcTuple<T1, T2, T3, T4, T5>(a1, a2, a3, a4, a5);
}

// convenience function for 6 arguments
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline TmcTuple<T1, T2, T3, T4, T5, T6> makeTmcTuple(T1 const &a1, T2 const &a2, T3 const &a3, T4 const &a4, T5 const &a5, T6 const &a6)
{
    return TmcTuple<T1, T2, T3, T4, T5, T6>(a1, a2, a3, a4, a5, a6);
}

// convenience function for 7 arguments
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline TmcTuple<T1, T2, T3, T4, T5, T6, T7> makeTmcTuple(T1 const &a1, T2 const &a2, T3 const &a3, T4 const &a4, T5 const &a5, T6 const &a6, T7 const &a7)
{
    return TmcTuple<T1, T2, T3, T4, T5, T6, T7>(a1, a2, a3, a4, a5, a6, a7);
}

// convenience function for 8 arguments
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
inline TmcTuple<T1, T2, T3, T4, T5, T6, T7, T8> makeTmcTuple(T1 const &a1, T2 const &a2, T3 const &a3, T4 const &a4, T5 const &a5, T6 const &a6, T7 const &a7, T8 const &a8)
{
    return TmcTuple<T1, T2, T3, T4, T5, T6, T7, T8>(a1, a2, a3, a4, a5, a6, a7, a8);
}

// some typedefs
typedef TmcTuple<float, float> TmcTupleFloat2;
typedef TmcTuple<float, float, float> TmcTupleFloat3;
typedef TmcTuple<float, float, float, float, float, float> TmcTupleFloat6;
typedef TmcTuple<int, int> TmcTupleInt2;
typedef TmcTuple<int, int, int> TmcTupleInt3;
typedef TmcTuple<int, int, int, int> TmcTupleInt4;
typedef TmcTuple<int, int, int, int, int> TmcTupleInt5;
typedef TmcTuple<int, int, int, int, int, int> TmcTupleInt6;
typedef TmcTuple<int, int, int, int, int, int, int> TmcTupleInt7;
typedef TmcTuple<int, int, int, int, int, int, int, int> TmcTupleInt8;
typedef TmcTuple<double, double> TmcTupleDouble2;
typedef TmcTuple<double, double, double> TmcTupleDouble3;
typedef TmcTuple<double, double, double, double> TmcTupleDouble4;
typedef TmcTuple<double, double, double, double, double, double> TmcTupleDouble6;
typedef TmcTuple<double, double, double, double, double, double, double> TmcTupleDouble7;
typedef TmcTuple<std::string, double, double> TmcTupleStringDouble2;
typedef TmcTuple<std::string, double, double, double> TmcTupleStringDouble3;
typedef TmcTuple<std::string, int, int, int> TmcTupleStringInt3;
typedef TmcTuple<short, short, short, short> TmcTupleShort4;
typedef TmcTuple<bool, bool, bool> TmcTupleBool3;
typedef TmcTuple<int, double, double> TmcTupleIntDouble2;
typedef TmcTuple<int, bool> TmcTupleIntBool;
typedef TmcTuple<std::string, std::string> TmcTupleString2;

// class TmcTupleWrapper
// {
// public:
//    TmcTuple<int, int> a;
//
// #ifdef CAB_RCF
//    template<class Archive>
//    void SF_SERIALIZE(Archive & ar)
//    {
//       ar & a;
//    }
//    void doSomething()
//    {
//       std::cout<<val<1>(a)<<std::endl;
//
//       std::cout<<val<2>(a)<<std::endl;
//    }
//
// #endif
//
// };

#endif // TMCTUPLE_H
