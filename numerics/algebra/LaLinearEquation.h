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
     equation system

\*---------------------------------------------------------------------------*/

#ifndef LALINEAREQUATION_H
#define LALINEAREQUATION_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "./LaSquareMatrix.h"
#include "./LaVector.h"

#include <TmcMacroFile.h>

class TMC_DLL_EXPORT LaLinearEquation : public LaObject
{
private:
    LaSquareMatrix *matrix;
    LaVector *vektor;
    LaVector *left;
    std::vector<bool> *index;
    bool solved;
    bool solved2;
    /**
      Creates a linear equation with no matrix or vector.
    */
public:
    LaLinearEquation();
    LaLinearEquation(LaSquareMatrix *matrix, LaVector *vektor);
    LaLinearEquation(std::string name);
    LaLinearEquation(LaSquareMatrix *matrix, LaVector *vektor, std::string name);
    LaLinearEquation(LaSquareMatrix *matrix, LaVector *left, LaVector *right);
    LaLinearEquation(LaSquareMatrix *matrix, LaVector *left, LaVector *right, std::string name);
    LaLinearEquation(LaSquareMatrix *matrix, LaVector *left, LaVector *right, std::vector<bool> *index);
    LaLinearEquation(LaSquareMatrix *matrix, LaVector *left, LaVector *right, std::vector<bool> *index, std::string name);
    LaLinearEquation(LaLinearEquation *eqn);

private:
    void init();

public:
    std::string toString();
    std::string toString(std::string format);
    void cleanSmallNumbers(int base);

    void setMatrix(LaSquareMatrix *matrix);
    void setVector(LaVector *vektor);
    void setLeftHandVector(LaVector *vektor);
    void setIndexVector(std::vector<bool> *index);
    std::vector<bool> *getIndexVector();
    LaVector *getLeftHandVector();
    LaVector *getVector();
    LaSquareMatrix *getMatrix();

    LaVector *solve();
    LaVector *solveSeparated();
};
#endif
