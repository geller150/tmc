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
 Original Copyright (C) 2003-2011 Sebastian Geller (VirtualFluids, TU Braunschweig, Germany)
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
     execuatable - example solver equation system

\*---------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <numerics/algebra/LaVector.h>
#include <numerics/algebra/LaScalar.h>
#include <numerics/algebra/LaSquareMatrix.h>
#include <numerics/algebra/LaLinearEquation.h>

int main(int argc, char **argv)
{
    try
    {
        std::cout << "/*======Example 1====================================*/" << std::endl
                  << std::endl;
        LaSquareMatrix *mat = new LaSquareMatrix(2, "TMC-Matrix");
        mat->setValue(0, 0, 1.1);
        mat->setValue(0, 1, 2.0);
        mat->setValue(1, 0, 3.0);
        mat->setValue(1, 1, 1.6);
        std::cout << mat->toString();
        LaVector *vek = new LaVector(2, "TMC-Vektor");
        vek->setValue(0, 2.1);
        vek->setValue(1, 4.333);
        std::cout << vek->toString();
        LaLinearEquation *equation = new LaLinearEquation(mat, vek);
        LaVector *solution = equation->solve();
        std::cout << solution->toString();
        std::cout << "/*====================================================*/" << std::endl;
        std::cout << "/*======Example 2 with Indexvector(PrimalVector)======*/" << std::endl;
        std::cout << "/*====================================================*/" << std::endl;
        LaSquareMatrix *mat2 = new LaSquareMatrix(2, "TMC-Matrix2");
        mat2->setValue(0, 0, 1.1);
        mat2->setValue(0, 1, 2.0);
        mat2->setValue(1, 0, 3.0);
        mat2->setValue(1, 1, 1.6);
        std::cout << mat2->toString();
        LaVector *vek2 = new LaVector(2, "TMC-Vektor2");
        vek2->setValue(0, 2.1);
        std::cout << vek2->toString();
        LaVector *left = new LaVector(2, "TMC-LeftHandVector");
        left->setValue(1, 4.333);
        std::cout << left->toString();
        std::vector<bool> *index = new std::vector<bool>;
        index->resize(2, true);
        (*index)[1] = false;
        LaLinearEquation *equation2 = new LaLinearEquation(mat, left, vek, index);
        LaVector *solution2 = equation2->solveSeparated();
        std::cout << solution2->toString();
        std::cout << "/*====================================================*/" << std::endl;
    }
    catch (std::string &s)
    {
        std::cout << s << std::endl;
    }
    catch (std::string *s)
    {
        std::cout << *s << std::endl;
    }
    catch (...)
    {
        std::cout << "CRASHED for some unknown reason !" << std::endl;
    };
    return 0;
}
