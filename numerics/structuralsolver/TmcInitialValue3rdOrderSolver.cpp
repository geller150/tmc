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
#include "./TmcInitialValue3rdOrderSolver.h"

#include <numerics/algebra/LaLinearEquation.h>
#include <numerics/algebra/LaSquareMatrix.h>
#include <numerics/algebra/LaVector.h>

#include <common/utilities/TmcFileInput.h>
#include <common/utilities/TmcFileOutput.h>
#include <common/math/TmcMath.h>

/*============================================================*/
TmcInitialValue3rdOrderSolver::TmcInitialValue3rdOrderSolver(int degreeOfFreedom, LaSquareMatrix *gMatrix, LaSquareMatrix *mMatrix, LaSquareMatrix *dMatrix, LaSquareMatrix *kMatrix, double deltaT)
{
    this->init(degreeOfFreedom, gMatrix, mMatrix, dMatrix, kMatrix, deltaT);
}
/*============================================================*/
void TmcInitialValue3rdOrderSolver::init(int degreeOfFreedom, LaSquareMatrix *gMatrix, LaSquareMatrix *mMatrix, LaSquareMatrix *dMatrix, LaSquareMatrix *kMatrix, double deltaT)
{
    this->dT = deltaT;

    this->degreeOfFreedom = degreeOfFreedom;

    this->gmatrix = gMatrix;
    this->mmatrix = mMatrix;
    this->dmatrix = dMatrix;
    this->kmatrix = kMatrix;

    this->xmatrix = new LaSquareMatrix(degreeOfFreedom);

    this->theta = 1.0;      // 1.37;		//Wilson
    this->alpha = 0.5;      // 0.5;
    this->beta = 1.0 / 6.0; // 0.25//1.0/6.0;
    this->gamma = 1.0 / 24.0;

    q = new double[degreeOfFreedom];
    qn = new double[degreeOfFreedom];
    ut = new double[degreeOfFreedom];
    u0 = new double[degreeOfFreedom];
    u1 = new double[degreeOfFreedom];
    u2 = new double[degreeOfFreedom];
    u3 = new double[degreeOfFreedom];
    u0n = new double[degreeOfFreedom];
    u1n = new double[degreeOfFreedom];
    u2n = new double[degreeOfFreedom];
    u3n = new double[degreeOfFreedom];
    for (int i = 0; i < degreeOfFreedom; i++)
    {
        u0[i] = 0.0;
        u1[i] = 0.0;
        u2[i] = 0.0;
        u3[i] = 0.0;
        u0n[i] = 0.0;
        u1n[i] = 0.0;
        u2n[i] = 0.0;
        u3n[i] = 0.0;
        q[i] = 0.0;
        qn[i] = 0.0;
        ut[i] = 0.0;
    }
    this->pvector = new LaVector(degreeOfFreedom, "P-Vector");
    this->gvector = new LaVector(degreeOfFreedom, "G-Vector");
    this->mvector = new LaVector(degreeOfFreedom, "M-Vector");
    this->dvector = new LaVector(degreeOfFreedom, "D-Vector");
    this->kvector = new LaVector(degreeOfFreedom, "K-Vector");
}
/*=====================================================*/
void TmcInitialValue3rdOrderSolver::setDisplacement(int index, double value)
{
    u0[index] = value;
}
/*=====================================================*/
void TmcInitialValue3rdOrderSolver::setVelocity(int index, double value)
{
    u1[index] = value;
}
/*=====================================================*/
void TmcInitialValue3rdOrderSolver::setAcceleration(int index, double value)
{
    u2[index] = value;
}
/*=====================================================*/
void TmcInitialValue3rdOrderSolver::setJerk(int index, double value)
{
    u3[index] = value;
}

/*=====================================================*/
// 2nd order Newmark: theta = 1.0;  alpha = 0.5; beta  = 0.25;
// 2nd order Wilson:  theta = 1.37; alpha = 0.5; beta  = 1.0/6.0;

void TmcInitialValue3rdOrderSolver::setAlphaBetaGammaThetaDeltaT(double alpha, double beta, double gamma, double theta, double deltaT)
{
    this->theta = theta;
    this->alpha = alpha;
    this->beta = beta;
    this->gamma = gamma;
    this->dT = deltaT;
}
/*=====================================================*/
void TmcInitialValue3rdOrderSolver::setFixedIndex(int index) // from 0 to ...
{
    this->fixedIndices.push_back(index);
    for (int u = 0; u < this->degreeOfFreedom; u++)
    {
        kmatrix->setValue(index, u, 0.0);
        dmatrix->setValue(index, u, 0.0);
        mmatrix->setValue(index, u, 0.0);
        gmatrix->setValue(index, u, 0.0);
    }
    kmatrix->setValue(index, index, 1.0);
    dmatrix->setValue(index, index, 1.0);
    mmatrix->setValue(index, index, 1.0);
    gmatrix->setValue(index, index, 1.0);
}

/*=====================================================*/

std::vector<double *> TmcInitialValue3rdOrderSolver::getCalculatedStartSolution(LaVector *loadvector)
{
    // std::cout<<"gmatrix:"<<gmatrix->getValue(0, 0)<<std::endl;
    // std::cout<<"mmatrix:"<<mmatrix->getValue(0, 0)<<std::endl;
    // std::cout<<"dmatrix:"<<dmatrix->getValue(0, 0)<<std::endl;
    // std::cout<<"kmatrix:"<<kmatrix->getValue(0, 0)<<std::endl;

    bool debug = true;
    // k*u = f
    for (int j = 0; j < degreeOfFreedom; j++)
    {
        q[j] = loadvector->getValue(j);
        pvector->setValue(j, loadvector->getValue(j));
    };

    LaLinearEquation *equationsystem = new LaLinearEquation(kmatrix, pvector);
    LaVector *solutionDisplacementVector = equationsystem->solve();

    for (int j = 0; j < this->degreeOfFreedom; j++)
    {
        u0[j] = solutionDisplacementVector->getValue(j);
        u1[j] = 0.0;
        u2[j] = 0.0;
        u3[j] = 0.0;
        u0n[j] = u0[j];
        u1n[j] = u1[j];
        u2n[j] = u2[j];
        u3n[j] = u3[j];
        ut[j] = 0.0;
    }
    for (int j = 0; j < this->degreeOfFreedom; j++)
    {
        std::cout << "disp - Time0 - u:" << u0[j] << ", v:" << u1[j] << ", a:" << u2[j] << ", j:" << u3[j] << " ,p:" << q[j] << std::endl;
    }

    for (int j = 0; j < degreeOfFreedom; j++)
    {
        this->kvector->setValue(j, u0[j]);
        this->dvector->setValue(j, u1[j]);
        this->mvector->setValue(j, u2[j]);
        this->gvector->setValue(j, u3[j]);
    }

    this->gvector = this->gmatrix->multiply(this->gvector);
    this->mvector = this->mmatrix->multiply(this->mvector);
    this->dvector = this->dmatrix->multiply(this->dvector);
    this->kvector = this->kmatrix->multiply(this->kvector);

    double thetaDT = theta * dT;
    for (int i = 0; i < degreeOfFreedom; i++)
    {
        for (int j = 0; j < degreeOfFreedom; j++)
        {
            xmatrix->setValue(i, j,
                              (kmatrix->getValue(i, j) +
                               (beta / (gamma * thetaDT)) * dmatrix->getValue(i, j) +
                               (alpha / (gamma * thetaDT * thetaDT)) * mmatrix->getValue(i, j) +
                               (1.0 / (gamma * thetaDT * thetaDT * thetaDT)) * gmatrix->getValue(i, j)));

            std::cout << "xMatrix:" << i << ", " << j << " - " << xmatrix->getValue(i, j);
        }
    }
    for (int a = 0; a < (int)fixedIndices.size(); a++)
    {
        int index = fixedIndices[a];
        for (int u = 0; u < this->degreeOfFreedom; u++)
        {
            xmatrix->setValue(index, u, 0.0);
        }
        xmatrix->setValue(index, index, 1.0);
    }

    // compute acceleration
    // m*a = f - d*v - k*u
    // m*a = - k*u
    if (!TmcMath::zero(mmatrix->getValue(0, 0)))
    {
        for (int j = 0; j < degreeOfFreedom; j++)
        {
            // pvector->setValue(j, -loadvector->getValue(j)-kvector->getValue(j) -dvector->getValue(j));
            pvector->setValue(j, kvector->getValue(j));
        }
        equationsystem->setMatrix(mmatrix);
        equationsystem->setVector(pvector);
        LaVector *solutionAccelerationVector = equationsystem->solve();
        std::cout << "##############################################" << std::endl;
        std::cout << "pvector:" << pvector->getValue(0) << std::endl;
        std::cout << "mmatrix:" << mmatrix->getValue(0, 0) << std::endl;
        std::cout << "acceleration:" << solutionAccelerationVector->getValue(0) << std::endl;
        std::cout << "##############################################" << std::endl;
        for (int j = 0; j < degreeOfFreedom; j++)
        {
            u2[j] = solutionAccelerationVector->getValue(j);
            this->mvector->setValue(j, u2[j]);
        }
        delete solutionAccelerationVector;
        this->mvector = this->mmatrix->multiply(this->mvector);

        for (int j = 0; j < this->degreeOfFreedom; j++)
        {
            std::cout << "with acceleration Time0 - u:" << u0[j] << ", v:" << u1[j] << ", a:" << u2[j] << ", j:" << u3[j] << " ,p:" << q[j] << std::endl;
        }
    }

    // compute jerk
    // g*j = f - m*a - d*v - k*v
    if (!TmcMath::zero(gmatrix->getValue(0, 0)))
    {
        for (int j = 0; j < degreeOfFreedom; j++)
        {
            pvector->setValue(j, -loadvector->getValue(j) - dvector->getValue(j) - mvector->getValue(j) - gvector->getValue(j));
        }
        if (debug)
            std::cout << "gmatrix:" << gmatrix->toString() << std::endl;
        if (debug)
            std::cout << "pvector:" << pvector->toString() << std::endl;
        equationsystem->setMatrix(gmatrix);
        equationsystem->setVector(pvector);

        LaVector *solutionJerkVector = equationsystem->solve();
        for (int j = 0; j < degreeOfFreedom; j++)
        {
            u3[j] = solutionJerkVector->getValue(j);
            this->gvector->setValue(j, u3[j]);
        }
        delete solutionJerkVector;
    }

    delete equationsystem;

    for (int j = 0; j < this->degreeOfFreedom; j++)
    {
        std::cout << "with jerk Time 0 - u:" << u0[j] << ", v:" << u1[j] << ", a:" << u2[j] << ", j:" << u3[j] << " ,p:" << q[j] << std::endl;
    }

    std::vector<double *> result;
    result.push_back(u0);
    result.push_back(u1);
    result.push_back(u2);
    result.push_back(u3);
    return result;
}

/*=====================================================*/
std::vector<double *> TmcInitialValue3rdOrderSolver::getCalculatedNextTimeStepSolution(LaVector *loadvector, bool okForNextTimeStep)
{

    for (int j = 0; j < degreeOfFreedom; j++)
        qn[j] = loadvector->getValue(j);

    double thetaDT = theta * dT;
    for (int j = 0; j < degreeOfFreedom; j++)
    {

        pvector->setValue(j, (q[j] * (1. - theta) + qn[j] * theta));

        gvector->setValue(j, ((1.0 - 1.0 / (6.0 * gamma)) * u3[j] -
                              (1.0 / (2.0 * gamma * thetaDT)) * u2[j] -
                              (1.0 / (gamma * thetaDT * thetaDT)) * u1[j] -
                              (1.0 / (gamma * thetaDT * thetaDT * thetaDT)) * u0[j]));

        mvector->setValue(j, ((1.0 - alpha / (6.0 * gamma)) * thetaDT * u3[j] +
                              (1. - alpha / (2.0 * gamma)) * u2[j] -
                              (alpha / (gamma * thetaDT)) * u1[j] -
                              (alpha / (gamma * thetaDT * thetaDT)) * u0[j]));

        dvector->setValue(j, (((0.5 - beta / (6. * gamma)) * thetaDT * thetaDT) * u3[j] +
                              ((1.0 - beta / (2.0 * gamma)) * thetaDT) * u2[j] +
                              (1.0 - beta / gamma) * u1[j] -
                              beta / (gamma * thetaDT) * u0[j]));
    }

    LaVector *dummy3 = gmatrix->multiply(gvector);
    for (int j = 0; j < degreeOfFreedom; j++)
        gvector->setValue(j, dummy3->getValue(j));
    delete dummy3;

    LaVector *dummy2 = mmatrix->multiply(mvector);
    for (int j = 0; j < degreeOfFreedom; j++)
        mvector->setValue(j, dummy2->getValue(j));
    delete dummy2;

    LaVector *dummy1 = dmatrix->multiply(dvector);
    for (int j = 0; j < degreeOfFreedom; j++)
        dvector->setValue(j, dummy1->getValue(j));
    delete dummy1;

    for (int j = 0; j < degreeOfFreedom; j++)
    {
        pvector->setValue(j, pvector->getValue(j) - dvector->getValue(j) - mvector->getValue(j) - gvector->getValue(j));
    }
    /* boundary condition */
    for (int u = 0; u < (int)fixedIndices.size(); u++)
    {
        pvector->setValue(fixedIndices[u], 0.);
    }

    // std::cout<<"xMatrix:"<<xmatrix->getValue(0,0)<<std::endl;
    // std::cout<<"pVector:"<<pvector->getValue(0)<<std::endl;
    // std::cout<<"gVector:"<<gvector->getValue(0)<<std::endl;
    // std::cout<<"mVector:"<<mvector->getValue(0)<<std::endl;
    // std::cout<<"dVector:"<<dvector->getValue(0)<<std::endl;
    /* solution                                                                */
    LaLinearEquation *system = new LaLinearEquation(xmatrix, pvector);
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
        u3n[j] = (1.0 - 1.0 / (6.0 * gamma * theta)) * u3[j] - (1.0 / (2.0 * gamma * theta * thetaDT)) * u2[j] - (1.0 / (gamma * theta * thetaDT * thetaDT)) * u1[j] + (1.0 / (gamma * theta * thetaDT * thetaDT * thetaDT)) * (ut[j] - u0[j]);
        u2n[j] = (1.0 - 1.0 / (6.0 * gamma * theta)) * dT * u3[j] + (1.0 - alpha / (2.0 * gamma * theta * theta)) * u2[j] - (alpha / (gamma * theta * theta * thetaDT)) * u1[j] + (alpha / (gamma * theta * theta * thetaDT * thetaDT)) * (ut[j] - u0[j]);
        u1n[j] = (1.0 - beta / (3.0 * gamma * theta)) * 0.5 * dT * dT * u3[j] + (1.0 - beta / (2.0 * gamma * theta * theta)) * dT * u2[j] + (1.0 - beta / (gamma * theta * theta * theta)) * u1[j] + (beta / (gamma * theta * theta * theta * thetaDT)) * (ut[j] - u0[j]);
        u0n[j] = (1.0 - 1.0 / (theta)) * 1.0 / 6.0 * dT * dT * dT * u3[j] + (1.0 - 1.0 / (theta * theta)) * 0.5 * dT * dT * u2[j] + (1.0 - 1.0 / (theta * theta * theta)) * dT * u1[j] + u0[j] + (1.0 / (theta * theta * theta * theta)) * (ut[j] - u0[j]);
    }

    if (okForNextTimeStep)
    {
        for (int j = 0; j < degreeOfFreedom; j++)
        {
            q[j] = qn[j];
            u0[j] = u0n[j];
            u1[j] = u1n[j];
            u2[j] = u2n[j];
            u3[j] = u3n[j];
        }
    }
    std::vector<double *> result;
    result.push_back(u0);
    result.push_back(u1);
    result.push_back(u2);
    result.push_back(u3);
    return result;
}
/*=====================================================*/
void TmcInitialValue3rdOrderSolver::read(TmcFileInput *input)
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
void TmcInitialValue3rdOrderSolver::write(TmcFileOutput *output)
{
    // output->writeLine("TmcInitialValue3rdOrderSolver");
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
std::string TmcInitialValue3rdOrderSolver::toString()
{
    std::stringstream ss;
    ss << "TmcInitialValue3rdOrderSolver[";
    ss << "alpha=" << this->alpha << ",";
    ss << "beta=" << this->beta << ",";
    ss << "gamm=" << this->gamma << ",";
    ss << "theta=" << this->theta << ",";

    ss << "]" << std::endl;
    return (ss.str());
}
