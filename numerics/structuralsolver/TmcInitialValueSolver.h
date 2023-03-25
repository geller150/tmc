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
     standard initial value solver

\*---------------------------------------------------------------------------*/

#ifndef TMCINITIALVALUESOLVER_H
#define TMCINITIALVALUESOLVER_H

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

#include <TmcMacroFile.h>

class LaLinearEquation;
class LaSquareMatrix;
class LaVector;
class TmcFileInput;
class TmcFileOutput;

class TMC_DLL_EXPORT TmcInitialValueSolver
{
public:
    TmcInitialValueSolver(int degreeOfFreedom, LaSquareMatrix *kMatrix, LaSquareMatrix *mMatrix, LaSquareMatrix *dMatrix, double deltaT);
    ~TmcInitialValueSolver(){};

    void init(int degreeOfFreedom, LaSquareMatrix *kMatrix, LaSquareMatrix *mMatrix, LaSquareMatrix *dMatrix, double deltaT);

    void setFixedIndex(int index);
    void setDisplacement(int index, double value);

    std::vector<double *> getCalculatedStartSolution(LaVector *lastvector);
    std::vector<double *> getCalculatedNextTimeStepSolution(LaVector *lastvector, bool okForNextTimeStep);

    int getDegreeOfFreedom() { return this->degreeOfFreedom; }
    double getDeltaT() { return this->dT; }

    void setAlphaBetaThetaDeltaT(double alpha, double beta, double theta, double deltaT);

    std::string toString();
    void read(TmcFileInput *input);
    void write(TmcFileOutput *output);

    LaSquareMatrix *getKMatrix() { return this->kmatrix; }
    LaSquareMatrix *getMMatrix() { return this->mmatrix; }

private:
    std::vector<int> fixedIndices;
    LaSquareMatrix *kmatrix;
    LaSquareMatrix *mmatrix;
    LaSquareMatrix *dmatrix;
    LaSquareMatrix *amatrix;
    double *u;
    double *u0, *u0n;
    double *u1, *u1n;
    double *u2, *u2n;
    double *p, *ut;
    double *q, *qn;
    LaVector *pvector;
    LaVector *mvector;
    LaVector *dvector;
    LaVector *kvector;
    LaVector *hvector;
    double dT;
    double theta;
    double alpha;
    double beta;

    int degreeOfFreedom; // degree of freedom
};
#endif
