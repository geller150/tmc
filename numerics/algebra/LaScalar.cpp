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
 Original Copyright (C) 1997 Markus Brueggemann (BTU Cottbus, Germany, java bauinformatik package)
 Modifications Copyright (C) 2003 Sebastian Geller (VirtualFluids, TU Braunschweig, Germany, translated to C++)
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
     scalar

\*---------------------------------------------------------------------------*/

#include "./LaScalar.h"

using namespace std;

const double LaScalar::PI = 4.0 * std::atan(1.0);

LaScalar::LaScalar() : LaObject("LaScalar")
{
    this->value = 0.0;
}
LaScalar::LaScalar(double value) : LaObject("LaScalar")
{
    this->value = value;
}
LaScalar::LaScalar(LaScalar *scalar) : LaObject(scalar->name)
{
    this->value = scalar->value;
}
LaScalar::LaScalar(string name) : LaObject(name)
{
    this->value = 0.0;
}
LaScalar::LaScalar(double value, string name) : LaObject(name)
{
    this->value = value;
}
/*======================================================================*/

string LaScalar::toString()
{
    return NULL;
}
/**
  Returns a formatted  String-representation of the scalar
  @param format the format string
  @return the String-representation of the scalar
*/
string LaScalar::toString(string format)
{
    return NULL;
}

/**
  Sets all values less than 1e-<I>base</I> to 0.0!
  @param base the base (must be positive)
  @exception IllegalArgumentException if base is less than 1
*/
void LaScalar::cleanSmallNumbers(int base)
{
    if (base < 1)
        throw new string("LaScalar.cleanSmallNumbers");

    if (std::fabs(this->value) < std::pow(10.0, -base))
        this->value = 0.0;
}

/**
  Sets the value of this scalar.
  @param value of the scalar
  @see #getValue
*/
void LaScalar::setValue(double value)
{
    this->value = value;
}
/**
  Returns the value of this scalar.
  @return value of the scalar
  @see #setValue
*/
double LaScalar::getValue()
{
    return (this->value);
}

/**
  Adds the specified value to this scalar
  @param a the doublevalue to add to this scalar
*/
void LaScalar::addValue(double a)
{
    this->value += a;
}
/**
  Subtracts the specified value from this scalar
  @param a the doublevalue to be subtracted from this scalar
*/
void LaScalar::subtractValue(double a)
{
    this->value -= a;
}
/**
  Multiplies the specified value to this scalar
  @param a the doublevalue to multiply this scalar with
*/
void LaScalar::multiplyValue(double a)
{
    this->value *= a;
}
/**
  Multiplies this scalar by the specified value
  @param a the doublevalue to divide this scalar by
*/
void LaScalar::divideByValue(double a)
{
    this->value /= a;
}

/**
  Multiply two scalars
  @param scalar the scalar to multiply with
  @return the result-scalar
*/
LaScalar *LaScalar::multiply(LaScalar *scalar)
{
    return (new LaScalar(this->value * scalar->value));
}
/**
  Divide two scalars
  @param scalar the scalar to divide by
  @return the result-scalar
*/
LaScalar *LaScalar::divide(LaScalar *scalar)
{
    return (new LaScalar(this->value / scalar->value));
}
/**
  Add two scalars
  @param scalar the scalar to add with
  @return the result-scalar
*/
LaScalar *LaScalar::add(LaScalar *scalar)
{
    return (new LaScalar(this->value + scalar->value));
}
/**
  Subtract two scalars
  @param scalar the scalar to subtract
  @return the result-scalar
*/
LaScalar *LaScalar::subtract(LaScalar *scalar)
{
    return (new LaScalar(this->value - scalar->value));
}

/**
  Return the squareroot of a scalar (non negative root).
  If the value of the scalar is NaN or negative, the value of the result-scalar will be NaN.
  @return the result-scalar
*/
LaScalar *LaScalar::sqrt()
{
    return (new LaScalar(std::sqrt(this->value)));
}
/**
  Return a new scalar with this scalars value raised to the specified base.
  If the result will not be a real number, an Exception is thrown!.
  @param base the base to raise to the power of
  @return the result-scalar
  @exception ArithmetiUbException if the result will not be a real number
*/
LaScalar *LaScalar::pow(double base)
{
    return (new LaScalar(std::pow(this->value, base)));
}

/**
  Return the sin of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::sin()
{
    return (new LaScalar(std::sin(this->value)));
}
/**
  Return the cos of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::cos()
{
    return (new LaScalar(std::cos(this->value)));
}
/**
  Return the tan of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::tan()
{
    return (new LaScalar(std::tan(this->value)));
}
/**
  Return the cot of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::cot()
{
    return (new LaScalar(1.0 / std::tan(this->value)));
}
/**
  Return the arcsin of a scalar.
  The value of the result-scalar will be in the range of [-pi/2,pi/2].
  If the value of the scalar is not in the range [-1,1], the value of the result-scalar will be NaN.
  @return the result-scalar
*/
LaScalar *LaScalar::asin()
{
    return (new LaScalar(std::asin(this->value)));
}
/**
  Return the arccos of a scalar.
  The value of the result-scalar will be in the range of [0,pi].
  If the value of the scalar is not in the range [-1,1], the value of the result-scalar will be NaN.
  @return the result-scalar
*/
LaScalar *LaScalar::acos()
{
    return (new LaScalar(std::acos(this->value)));
}
/**
  Return the arctan of a scalar.
  The value of the result-scalar will be in the range of [-pi/2,pi/2].
  @return the result-scalar
*/
LaScalar *LaScalar::atan()
{
    return (new LaScalar(std::atan(this->value)));
}
/**
  Return the arccot of a scalar.
  The value of the result-scalar will be in the range of [0,pi].
  @return the result-scalar
*/
LaScalar *LaScalar::acot()
{
    // return(new LaScalar(PI*0.5-std::atan(this->value)));
    return (new LaScalar(LaScalar::PI * 0.5 - std::atan(this->value)));
}

/**
  Return the sinh of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::sinh()
{
    return (new LaScalar(sinh(this->value)));
}
/**
  Return the cosh of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::cosh()
{
    return (new LaScalar(cosh(this->value)));
}
/**
  Return the tanh of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::tanh()
{
    return (new LaScalar(tanh(this->value)));
}
/**
  Return the coth of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::coth()
{
    return (new LaScalar(coth(this->value)));
}
/**
  Return the arsinh of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::arsinh()
{
    return (new LaScalar(arsinh(this->value)));
}
/**
  Return the arcosh of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::arcosh()
{
    return (new LaScalar(arcosh(this->value)));
}
/**
  Return the artanh of a scalar.
  The value of the result-scalar will be in the range of [-pi/2,pi/2].
  @return the result-scalar
*/
LaScalar *LaScalar::artanh()
{
    return (new LaScalar(artanh(this->value)));
}
/**
  Return the arcoth of a scalar.
  The value of the result-scalar will be in the range of [0,pi].
  @return the result-scalar
*/
LaScalar *LaScalar::arcoth()
{
    return (new LaScalar(arcoth(this->value)));
}

/**
  Return the exponential number raised by the value of the scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::exp()
{
    return (new LaScalar(std::exp(this->value)));
}
/**
  Return the natural logarithm of a scalar.
  If the value of the scalar is negative, the value of the result-scalar will be NaN.
  If the value of the scalar is 0, the value of the result-scalar will be -infinity.
  @return the result-scalar
*/
LaScalar *LaScalar::log()
{
    return (new LaScalar(std::log(this->value)));
}

/**
  Returns the absolut value of a scalar.
  @return the result-scalar
*/
LaScalar *LaScalar::abs()
{
    return (new LaScalar(std::fabs(this->value)));
}

double LaScalar::sinh(double v) { return ((std::exp(v) - std::exp(-v)) * 0.5); };
double LaScalar::cosh(double v) { return ((std::exp(v) + std::exp(-v)) * 0.5); };
double LaScalar::tanh(double v) { return ((std::exp(v) - std::exp(-v)) / (std::exp(v) + std::exp(-v))); };
double LaScalar::coth(double v) { return ((std::exp(v) + std::exp(-v)) / (std::exp(v) - std::exp(-v))); };

double LaScalar::arsinh(double v) { return (std::log(v + std::sqrt(v * v + 1.0))); };
double LaScalar::arcosh(double v) { return (std::log(v + std::sqrt(v * v - 1.0))); };
double LaScalar::artanh(double v) { return (0.5 * std::log((1.0 + v) / (1.0 - v))); };
double LaScalar::arcoth(double v) { return (0.5 * std::log((v + 1.0) / (v - 1.0))); };
