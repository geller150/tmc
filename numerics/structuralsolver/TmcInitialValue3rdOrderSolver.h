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
 Copyright (C) 2022-2023 Sebastian Geller

 This software is distributed WITHOUT ANY WARRANTY.

 License

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
     3rd order initial value solver
     The implementation is based on the idea of an additional force based on the jerk, F = m*t*j [Jan/Feb 2021]

\*---------------------------------------------------------------------------*/

#ifndef TMCINITIALVALUE3RDORDERSOLVER_H
#define TMCINITIALVALUE3RDORDERSOLVER_H

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

class TMC_DLL_EXPORT TmcInitialValue3rdOrderSolver
{
public:
    TmcInitialValue3rdOrderSolver(int degreeOfFreedom, LaSquareMatrix *gMatrix, LaSquareMatrix *mMatrix, LaSquareMatrix *dMatrix, LaSquareMatrix *kMatrix, double deltaT);
    ~TmcInitialValue3rdOrderSolver(){};

    void init(int degreeOfFreedom, LaSquareMatrix *gMatrix, LaSquareMatrix *mMatrix, LaSquareMatrix *dMatrix, LaSquareMatrix *kMatrix, double deltaT);

    void setFixedIndex(int index);
    void setDisplacement(int index, double value);
    void setVelocity(int index, double value);
    void setAcceleration(int index, double value);
    void setJerk(int index, double value);

    std::vector<double *> getCalculatedStartSolution(LaVector *lastvector);
    std::vector<double *> getCalculatedNextTimeStepSolution(LaVector *lastvector, bool okForNextTimeStep);

    int getDegreeOfFreedom() { return this->degreeOfFreedom; }
    double getDeltaT() { return this->dT; }

    void setAlphaBetaGammaThetaDeltaT(double alpha, double beta, double gamma, double theta, double deltaT);

    std::string toString();
    void read(TmcFileInput *input);
    void write(TmcFileOutput *output);

    LaSquareMatrix *getKMatrix() { return this->kmatrix; }
    LaSquareMatrix *getMMatrix() { return this->mmatrix; }

private:
    std::vector<int> fixedIndices;
    LaSquareMatrix *kmatrix;
    LaSquareMatrix *dmatrix;
    LaSquareMatrix *mmatrix;
    LaSquareMatrix *gmatrix;
    LaSquareMatrix *xmatrix;
    double *u0, *u0n;
    double *u1, *u1n;
    double *u2, *u2n;
    double *u3, *u3n;
    double *ut;
    double *q, *qn;
    LaVector *pvector;
    LaVector *gvector;
    LaVector *mvector;
    LaVector *dvector;
    LaVector *kvector;
    double dT;
    double theta;
    double alpha;
    double beta;
    double gamma;

    int degreeOfFreedom; // degree of freedom
};
#endif
