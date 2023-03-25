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
     vector

\*---------------------------------------------------------------------------*/

#ifndef LAVECTOR_H
#define LAVECTOR_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "./LaObject.h"
#include <TmcMacroFile.h>

class TmcFileInput;
class TmcFileOutput;

class TMC_DLL_EXPORT LaVector : public LaObject
{
public:
    std::vector<double> *value;

    LaVector();
    LaVector(int dimension);
    LaVector(std::vector<double> *array);
    LaVector(double *array, int size);
    LaVector(LaVector *vektor);
    LaVector(std::string name);
    LaVector(int dimension, std::string name);
    ~LaVector();

    /*======================================================================*/
    std::string toString();
    std::string toString(std::string format);
    void cleanSmallNumbers(int base);
    int getDimension();
    void setValue(int row, double a);
    void setValues(double a);
    double getValue(int row);
    std::vector<double> *getValues();
    void addValue(int row, double a);
    void subtractValue(int row, double a);
    void multiplyValue(int row, double a);
    void divideByValue(int row, double a);
    void addValue(double a);
    void subtractValue(double a);
    void multiplyValue(double a);
    void divideByValue(double a);

    LaVector *add(LaVector *vector);
    LaVector *subtract(LaVector *vector);

    void read(TmcFileInput *in);
    void write(TmcFileOutput *out);
};
#endif
