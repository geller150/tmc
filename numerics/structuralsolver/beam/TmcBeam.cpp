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
     beam

\*---------------------------------------------------------------------------*/

#include "./TmcBeam.h"

#include <numerics/algebra/LaLinearEquation.h>
#include <numerics/algebra/LaSquareMatrix.h>
#include <numerics/algebra/LaVector.h>

#include <numerics/structuralsolver/beam/TmcBeamSystem.h>
#include <common/utilities/TmcFileInput.h>
#include <common/utilities/TmcFileOutput.h>

TmcBeam::TmcBeam()
{
}
/*============================================================*/
TmcBeam::TmcBeam(int knotenanzahl, double E, double I, double length, double m, double d) //, double deltaT)
{
    this->init(knotenanzahl, E, I, length, m, d); //, deltaT);
}
/*============================================================*/
TmcBeam::~TmcBeam()
{
    delete this->kmatrix;
    delete this->mmatrix;
    delete this->dmatrix;
}
/*============================================================*/
void TmcBeam::init(int knotenanzahl, double E, double I, double length, double m, double d) //, double deltaT)
{
    this->E = E;
    this->I = I;
    this->m = m;
    this->d = d;
    this->length = length;
    // this->dT = deltaT;
    this->linienlast = 0.0;

    this->knotenanzahl = knotenanzahl;
    this->elementanzahl = knotenanzahl - 1;
    this->degreeOfFreedom = 2 * knotenanzahl;
    double elementlength = length / elementanzahl;

    TmcBeamSystem system;

    this->kmatrix = system.getKMatrix(knotenanzahl, E, I, elementlength);
    this->mmatrix = system.getMMatrix(knotenanzahl, m, elementlength);
    this->dmatrix = system.getDMatrix(knotenanzahl, d);
}
/*=====================================================*/
/*=====================================================*/
void TmcBeam::read(UbFileInput *input)
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
void TmcBeam::write(UbFileOutput *output)
{
    // output->writeLine("TmcBeam");
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
string TmcBeam::toString()
{
    stringstream ss;
    ss << "TmcBeam[";
    ss << " E:" << this->E;
    ss << ", I:" << this->I;
    ss << ", length:" << this->length;
    ss << "]" << endl;
    return (ss.str());
}
