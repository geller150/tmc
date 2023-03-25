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
     mass oscillator
     The implementation is based on the idea of an additional force based on the jerk, F = m*t*j = B*j [Jan/Feb 2021]

\*---------------------------------------------------------------------------*/

#ifndef TMCMASSOSCILLATOR_H
#define TMCMASSOSCILLATOR_H

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <tuple>

#include <TmcMacroFile.h>

class LaLinearEquation;
class LaSquareMatrix;
class LaVector;
class TmcInitialValue3rdOrderSolver;
class TmcInitialValueSolver;

class TMC_DLL_EXPORT TmcMassOscillator
{
public:
    TmcMassOscillator();
    TmcMassOscillator(double B, double M, double D, double K, double load, double dT, double time);
    ~TmcMassOscillator(){};

    void init(double B, double M, double D, double K, double load, double dT, double time);

    void setValues(double B, double M, double D, double K, double load, double alpha, double beta, double gamma, double theta, double deltaT, double time);

    void compute(std::vector<std::tuple<double, double, double, double, double>> &data, bool standard);

    std::string toString();

private:
    LaSquareMatrix *bMatrix;
    LaSquareMatrix *mMatrix;
    LaSquareMatrix *dMatrix;
    LaSquareMatrix *kMatrix;
    LaVector *loadVector;
    double deltaT;
    double time;
    TmcInitialValue3rdOrderSolver *solverNew;
    TmcInitialValueSolver *solverStandard;
};
#endif
