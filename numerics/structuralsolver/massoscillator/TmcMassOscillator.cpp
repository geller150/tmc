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

#include "./TmcMassOscillator.h"

#include <numerics/algebra/LaLinearEquation.h>
#include <numerics/algebra/LaSquareMatrix.h>
#include <numerics/algebra/LaVector.h>
#include <numerics/structuralsolver/TmcInitialValueSolver.h>
#include <numerics/structuralsolver/TmcInitialValue3rdOrderSolver.h>

/*============================================================*/
TmcMassOscillator::TmcMassOscillator()
{
    this->init(1.0, 1.0, 1.0, 1.0, 1.0, 0.1, 100.0);
}
/*============================================================*/
TmcMassOscillator::TmcMassOscillator(double B, double M, double D, double K, double load, double deltaT, double time)
{
    this->init(B, M, D, K, load, deltaT, time);
}
/*============================================================*/
void TmcMassOscillator::init(double B, double M, double D, double K, double load, double deltaT, double time)
{
    int degreeOfFreedom = 1;
    this->bMatrix = new LaSquareMatrix(1);
    this->mMatrix = new LaSquareMatrix(1);
    this->dMatrix = new LaSquareMatrix(1);
    this->kMatrix = new LaSquareMatrix(1);
    this->loadVector = new LaVector(1);
    this->bMatrix->setValue(0, 0, B);
    this->mMatrix->setValue(0, 0, M);
    this->dMatrix->setValue(0, 0, D);
    this->kMatrix->setValue(0, 0, K);
    this->loadVector->setValue(0, load);
    double theta = 1.0;
    double alpha = 1.0;
    double beta = 1.0;
    double gamma = 1.0;

    // theta = 1.0;  alpha = 0.5; beta  = 0.25;   //Newmark / 2nd order approach
    // theta = 1.37; alpha = 0.5; beta  = 1.0/6.0; //Wilson / 2nd order approach
    this->solverStandard = new TmcInitialValueSolver(degreeOfFreedom, kMatrix, mMatrix, dMatrix, deltaT);
    this->solverNew = new TmcInitialValue3rdOrderSolver(degreeOfFreedom, bMatrix, mMatrix, dMatrix, kMatrix, deltaT);

    this->setValues(B, M, D, K, load, alpha, beta, gamma, theta, deltaT, time);
}
/*============================================================*/
void TmcMassOscillator::setValues(double B, double M, double D, double K, double load, double alpha, double beta, double gamma, double theta, double deltaT, double time)
{
    this->bMatrix->setValue(0, 0, B);
    this->mMatrix->setValue(0, 0, M);
    this->dMatrix->setValue(0, 0, D);
    this->kMatrix->setValue(0, 0, K);
    this->loadVector->setValue(0, load);
    this->deltaT = deltaT;
    this->time = time;
    this->solverStandard->setAlphaBetaThetaDeltaT(alpha, beta, theta, deltaT);
    this->solverNew->setAlphaBetaGammaThetaDeltaT(alpha, beta, gamma, theta, deltaT);
}
/*============================================================*/
void TmcMassOscillator::compute(std::vector<std::tuple<double, double, double, double, double>> &data, bool standard)
{
    double valueU = 0.0;
    double valueV = 0.0;
    double valueA = 0.0;
    double valueJ = 0.0;

    std::vector<double *> result;
    if (standard)
        result = solverStandard->getCalculatedStartSolution(this->loadVector);
    else
        result = solverNew->getCalculatedStartSolution(this->loadVector);
    valueU = result[0][0];
    valueV = result[1][0];
    valueA = result[2][0];
    if (standard)
        valueJ = 0.0;
    else
        valueJ = result[3][0];

    std::cout << "START - u,v,a,j:" << valueU << " " << valueV << " " << valueA << " " << valueJ << std::endl;
    valueU = 0.0;
    if (standard)
        solverStandard->setDisplacement(0, 0.0);
    else
    {
        solverNew->setDisplacement(0, 0.0);
        solverNew->setJerk(0, 0.0);
    }
    data.push_back(std::make_tuple(0.0, valueU, valueV, valueA, valueJ));

    int iterations = int(this->time / this->deltaT) + 1;
    std::cout << "Iterations:" << iterations << std::endl;
    for (int i = 1; i < iterations; i++)
    {
        if (standard)
            result = solverStandard->getCalculatedNextTimeStepSolution(this->loadVector, true);
        else
            result = solverNew->getCalculatedNextTimeStepSolution(this->loadVector, true);
        valueU = result[0][0];
        valueV = result[1][0];
        valueA = result[2][0];
        if (standard)
            valueJ = 0.0;
        else
            valueJ = result[3][0];
        data.push_back(std::make_tuple((double)i * this->deltaT, valueU, valueV, valueA, valueJ));
        // std::cout << "u,v,a,j:" << valueU << " " << valueV << " " << valueA << " " << valueJ << std::endl;
    }
    std::cout << "END time:" << time << " - u,v,a,j:" << valueU << " " << valueV << " " << valueA << " " << valueJ << std::endl;
}
/*============================================================*/
std::string TmcMassOscillator::toString()
{
    std::string result = "mass oscillator:";
    result += this->solverNew->toString();
    result += " B: " + std::to_string(this->bMatrix->getValue(0, 0));
    result += ", M: " + std::to_string(this->mMatrix->getValue(0, 0));
    result += ", D: " + std::to_string(this->dMatrix->getValue(0, 0));
    result += ", K: " + std::to_string(this->kMatrix->getValue(0, 0));
    result += ", load: " + std::to_string(this->loadVector->getValue(0));
    result += ", deltaT: " + std::to_string(this->deltaT);
    result += ", time: " + std::to_string(this->time);
    return result;
}