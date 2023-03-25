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

#ifndef LASCALAR_H
#define LASCALAR_H

#include <cmath>
#include <iostream>

#include "./LaObject.h"
#include <TmcMacroFile.h>

class TMC_DLL_EXPORT LaScalar : public LaObject
{
private:
    double value;

public:
    LaScalar();
    LaScalar(double value);
    LaScalar(LaScalar *scalar);
    LaScalar(std::string name);
    LaScalar(double value, std::string name);

    static const double PI;

    std::string toString();
    std::string toString(std::string format);
    void cleanSmallNumbers(int base);
    void setValue(double value);
    double getValue();
    void addValue(double a);
    void subtractValue(double a);
    void multiplyValue(double a);
    void divideByValue(double a);

    LaScalar *multiply(LaScalar *scalar);
    LaScalar *divide(LaScalar *scalar);
    LaScalar *add(LaScalar *scalar);
    LaScalar *subtract(LaScalar *scalar);

    LaScalar *sqrt();
    LaScalar *pow(double base);
    LaScalar *sin();
    LaScalar *cos();
    LaScalar *tan();
    LaScalar *cot();
    LaScalar *asin();
    LaScalar *acos();
    LaScalar *atan();
    LaScalar *acot();
    LaScalar *sinh();
    LaScalar *cosh();
    LaScalar *tanh();
    LaScalar *coth();
    LaScalar *arsinh();
    LaScalar *arcosh();
    LaScalar *artanh();
    LaScalar *arcoth();
    LaScalar *exp();
    LaScalar *log();
    LaScalar *abs();

private:
    static double sinh(double v);
    static double cosh(double v);
    static double tanh(double v);
    static double coth(double v);

    static double arsinh(double v);
    static double arcosh(double v);
    static double artanh(double v);
    static double arcoth(double v);
};
/*=========================================================================*/
#endif
