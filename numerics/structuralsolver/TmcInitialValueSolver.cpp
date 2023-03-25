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

#include "./TmcInitialValueSolver.h"

#include <numerics/algebra/LaLinearEquation.h>
#include <numerics/algebra/LaSquareMatrix.h>
#include <numerics/algebra/LaVector.h>

#include <common/utilities/TmcFileInput.h>
#include <common/utilities/TmcFileOutput.h>

/*============================================================*/
TmcInitialValueSolver::TmcInitialValueSolver(int degreeOfFreedom, LaSquareMatrix *kMatrix, LaSquareMatrix *mMatrix, LaSquareMatrix *dMatrix, double deltaT)
{
    this->init(degreeOfFreedom, kMatrix, mMatrix, dMatrix, deltaT);
}
/*============================================================*/
void TmcInitialValueSolver::init(int degreeOfFreedom, LaSquareMatrix *kMatrix, LaSquareMatrix *mMatrix, LaSquareMatrix *dMatrix, double deltaT)
{
    this->dT = deltaT;

    this->degreeOfFreedom = degreeOfFreedom;

    this->kmatrix = kMatrix;
    this->mmatrix = mMatrix;
    this->dmatrix = dMatrix;

    // Newmark
    this->theta = 1.0; // 1.37;		//Wilson
    this->alpha = 0.5; // 0.5;
    this->beta = 0.25; // 1.0/6.0;

    // Wilson
    // this->theta = 1.37;
    // this->alpha = 0.5;
    // this->beta  = 1.0/6.0;

    u = new double[degreeOfFreedom];
    p = new double[degreeOfFreedom];
    q = new double[degreeOfFreedom];
    qn = new double[degreeOfFreedom];
    ut = new double[degreeOfFreedom];
    u0 = new double[degreeOfFreedom];
    u1 = new double[degreeOfFreedom];
    u2 = new double[degreeOfFreedom];
    u0n = new double[degreeOfFreedom];
    u1n = new double[degreeOfFreedom];
    u2n = new double[degreeOfFreedom];
    for (int i = 0; i < degreeOfFreedom; i++)
    {
        u[i] = 0.0;
        p[i] = 0.0;
        ut[i] = 0.0;
        u0[i] = 0.0;
        u1[i] = 0.0;
        u2[i] = 0.0;
        u0n[i] = 0.0;
        u1n[i] = 0.0;
        u2n[i] = 0.0;
    }
    /* vector                                                                  */
    this->hvector = new LaVector(degreeOfFreedom, "H-Vector");
    this->pvector = new LaVector(degreeOfFreedom, "P-Vector");
    this->kvector = new LaVector(degreeOfFreedom, "K-Vector");
    this->mvector = new LaVector(degreeOfFreedom, "M-Vector");
    this->dvector = new LaVector(degreeOfFreedom, "D-Vector");
}
/*=====================================================*/
void TmcInitialValueSolver::setDisplacement(int index, double value)
{
    u0[index] = value;
}

/*=====================================================*/
// Newmark: theta = 1.0;  alpha = 0.5; beta  = 0.25;
// Wilson:  theta = 1.37; alpha = 0.5; beta  = 1.0/6.0;
void TmcInitialValueSolver::setAlphaBetaThetaDeltaT(double alpha, double beta, double theta, double deltaT)
{
    this->theta = theta;
    this->alpha = alpha;
    this->beta = beta;
    this->dT = deltaT;
}
/*=====================================================*/
void TmcInitialValueSolver::setFixedIndex(int index)
{
    this->fixedIndices.push_back(index);
    for (int u = 0; u < this->degreeOfFreedom; u++)
    {
        kmatrix->setValue(index, u, 0.0);
        dmatrix->setValue(index, u, 0.0);
        mmatrix->setValue(index, u, 0.0);
    }
    kmatrix->setValue(index, index, 1.0);
    dmatrix->setValue(index, index, 1.0);
    mmatrix->setValue(index, index, 1.0);
}

/*=====================================================*/
std::vector<double *> TmcInitialValueSolver::getCalculatedStartSolution(LaVector *lastvector)
{
    // create equation system and solve
    // k*u = f
    LaLinearEquation *equationsystem = new LaLinearEquation(kmatrix, lastvector);
    LaVector *lhvector = equationsystem->solve();

    for (int j = 0; j < this->degreeOfFreedom; j++)
    {
        u0[j] = lhvector->getValue(j);
        u1[j] = 0.;
    }
    for (int j = 0; j < degreeOfFreedom; j++)
        q[j] = lhvector->getValue(j);

    for (int j = 0; j < degreeOfFreedom; j++)
    {
        kvector->setValue(j, u0[j]);
        dvector->setValue(j, u1[j]);
    }
    dvector = dmatrix->multiply(dvector);
    kvector = kmatrix->multiply(kvector);
    for (int j = 0; j < degreeOfFreedom; j++)
    {
        lhvector->setValue(j, -kvector->getValue(j) - dvector->getValue(j));
    }

    equationsystem->setMatrix(mmatrix);
    equationsystem->setVector(lhvector);
    hvector = equationsystem->solve();
    delete lhvector;
    for (int j = 0; j < degreeOfFreedom; j++)
        u2[j] = hvector->getValue(j);

    this->amatrix = new LaSquareMatrix(this->degreeOfFreedom);
    for (int i = 0; i < degreeOfFreedom; i++)
    {
        for (int j = 0; j < degreeOfFreedom; j++)
        {
            amatrix->setValue(i, j,
                              kmatrix->getValue(i, j) +
                                  alpha / (beta * theta * dT) * dmatrix->getValue(i, j) +
                                  1. / (beta * (theta * dT) * (theta * dT)) * mmatrix->getValue(i, j));
        }
    }

    for (int a = 0; a < (int)fixedIndices.size(); a++)
    {
        int index = fixedIndices[a];
        for (int u = 0; u < this->degreeOfFreedom; u++)
        {
            amatrix->setValue(index, u, 0.0);
        }
        amatrix->setValue(index, index, 1.0);
    }

    delete equationsystem;

    std::vector<double *> result;
    result.push_back(u0);
    result.push_back(u1);
    result.push_back(u2);
    return result;
}

/*=====================================================*/
std::vector<double *> TmcInitialValueSolver::getCalculatedNextTimeStepSolution(LaVector *lastvector, bool okForNextTimeStep)
{
    for (int j = 0; j < degreeOfFreedom; j++)
        qn[j] = lastvector->getValue(j);

    /* equation system                                                         */
    /* matrix                                                                  */

    /* right vector                                                            */
    for (int j = 0; j < degreeOfFreedom; j++)
    {
        pvector->setValue(j, (q[j] * (1. - theta) + qn[j] * theta));
        mvector->setValue(j, ((1. - 1. / (2. * beta)) * u2[j] -
                              1. / (beta * (theta * dT)) * u1[j] -
                              1. / (beta * (theta * dT) * (theta * dT)) * u0[j]));
        dvector->setValue(j, (((1. - alpha / (2. * beta)) * (theta * dT)) * u2[j] +
                              (1. - alpha / beta) * u1[j] -
                              alpha / (beta * (theta * dT)) * u0[j]));
    }
    LaVector *dummy1 = mmatrix->multiply(mvector);
    for (int j = 0; j < degreeOfFreedom; j++)
        mvector->setValue(j, dummy1->getValue(j));
    delete dummy1;

    LaVector *dummy2 = dmatrix->multiply(dvector);
    for (int j = 0; j < degreeOfFreedom; j++)
        dvector->setValue(j, dummy2->getValue(j));
    delete dummy2;

    for (int j = 0; j < degreeOfFreedom; j++)
    {
        pvector->setValue(j, pvector->getValue(j) - dvector->getValue(j) - mvector->getValue(j));
    }
    /* boundary condition */
    for (int u = 0; u < (int)fixedIndices.size(); u++)
    {
        pvector->setValue(fixedIndices[u], 0.);
    }

    /* solution                                                                */
    LaLinearEquation *system = new LaLinearEquation(amatrix, pvector);
    LaVector *dummy = system->solve();
    for (int j = 0; j < degreeOfFreedom; j++)
    {
        ut[j] = dummy->getValue(j);
        pvector->setValue(j, ut[j]);
    }
    delete dummy;
    delete system;

    /* new state variables                                                     */
    for (int j = 0; j < degreeOfFreedom; j++)
    {
        u0n[j] = u0[j] + 1. / (theta * theta * theta) * (ut[j] - u0[j]) + (1. - 1. / (theta * theta)) * dT * u1[j] + 0.5 * (1. - 1. / (theta)) * dT * dT * u2[j];
        u1n[j] = alpha / (beta * theta * theta * theta * dT) * (ut[j] - u0[j]) + (1. - alpha / (beta * theta * theta)) * u1[j] + (1. - alpha / (2. * beta * theta)) * dT * u2[j];
        u2n[j] = 1. / (beta * theta * theta * theta * dT * dT) * (ut[j] - u0[j]) - (1. / (beta * theta * theta * dT)) * u1[j] + (1. - 1. / (2. * beta * theta)) * u2[j];
    }
    /* prepare for next time step	*/

    if (okForNextTimeStep)
    {
        for (int j = 0; j < degreeOfFreedom; j++)
        {
            q[j] = qn[j];
            u0[j] = u0n[j];
            u1[j] = u1n[j];
            u2[j] = u2n[j];
        }
    }

    std::vector<double *> result;
    result.push_back(u0);
    result.push_back(u1);
    result.push_back(u2);
    return result;
}
/*=====================================================*/
void TmcInitialValueSolver::read(TmcFileInput *input)
{
    // cout<<input->readString()<<endl;
    // input->readLine();
    // double Einput = input->readDouble();
    // double Iinput = input->readDouble();
    // double lengthinput = input->readDouble();
    // double minput = input->readDouble();
    // double dinput = input->readDouble();
    // double dTinput = input->readDouble();
    // int knotenanzahlinput = input->readInteger();
    // this->init(knotenanzahlinput, Einput, Iinput, lengthinput, minput, dinput, dTinput);
    // double alpha = input->readDouble();
    // double beta = input->readDouble();
    // double theta = input->readDouble();
    // this->setAlphaBetaTheta(alpha, beta, theta);
    // this->linienlast = input->readDouble();

    // input->readLine();
    // this->mvector = new LaVector();
    // mvector->read(input);   input->readLine();
    // this->dvector = new LaVector();
    // dvector->read(input);   input->readLine();
    // this->kvector = new LaVector();
    // kvector->read(input);   input->readLine();
    // this->pvector = new LaVector();
    // pvector->read(input);    input->readLine();
    // this->amatrix = new LaSquareMatrix();
    // amatrix->read(input);    input->readLine();
    // this->mmatrix = new LaSquareMatrix();
    // mmatrix->read(input);    input->readLine();
    // this->dmatrix = new LaSquareMatrix();
    // dmatrix->read(input);    input->readLine();
    // this->kmatrix = new LaSquareMatrix();
    // kmatrix->read(input);   input->readLine();
    // cout<<" "<<degreeOfFreedom<<endl;

    // input->readString(); for(int u=0;u<degreeOfFreedom;u++) q[u] = input->readDouble(); input->readLine();
    // input->readString(); for(int u=0;u<degreeOfFreedom;u++) u1[u] = input->readDouble();input->readLine();
    // input->readString(); for(int u=0;u<degreeOfFreedom;u++) u2[u] = input->readDouble();input->readLine();
    // input->readString(); for(int u=0;u<degreeOfFreedom;u++) u0[u] = input->readDouble();input->readLine();
    // input->readString(); for(int u=0;u<degreeOfFreedom;u++) ut[u] = input->readDouble();
}
/*=====================================================*/
void TmcInitialValueSolver::write(TmcFileOutput *output)
{
    // output->writeLine("TmcInitialValueSolver");
    // output->writeDouble(this->E);
    // output->writeDouble(this->I);
    // output->writeDouble(this->length);
    // output->writeDouble(this->m);
    // output->writeDouble(this->d);
    // output->writeDouble(this->dT);
    // output->writeInteger(this->knotenanzahl);
    // output->writeDouble(this->alpha);
    // output->writeDouble(this->beta);
    // output->writeDouble(this->theta);
    // output->writeDouble(this->linienlast);
    // output->writeLine();
    // this->mvector->write(output);
    // this->dvector->write(output);
    // this->kvector->write(output);
    // this->pvector->write(output);
    // this->amatrix->write(output);
    // this->mmatrix->write(output);
    // this->dmatrix->write(output);
    // this->kmatrix->write(output);

    // output->writeString("q[i]");
    // for(int u=0;u<degreeOfFreedom;u++) output->writeDouble(q[u]);
    // output->writeLine();
    // output->writeString("u1[i]");
    // for(int u=0;u<degreeOfFreedom;u++) output->writeDouble(u1[u]);
    // output->writeLine();
    // output->writeString("u2[i]");
    // for(int u=0;u<degreeOfFreedom;u++) output->writeDouble(u2[u]);
    // output->writeLine();
    // output->writeString("u0[i]");
    // for(int u=0;u<degreeOfFreedom;u++) output->writeDouble(u0[u]);
    // output->writeLine();
    // output->writeString("ut[i]");
    // for(int u=0;u<degreeOfFreedom;u++) output->writeDouble(ut[u]);
    // output->writeLine();
}
/*=====================================================*/
std::string TmcInitialValueSolver::toString()
{
    std::stringstream ss;
    ss << "TmcInitialValueSolver[";
    ss << "]" << std::endl;
    return (ss.str());
}
