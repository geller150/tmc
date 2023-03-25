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

#ifndef TmcBeam_H
#define TmcBeam_H

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

#include <TmcMacroFile.h>

using namespace std;

class LaLinearEquation;
class LaSquareMatrix;
class LaVector;
class GbPolygon2D;
class UbFileInput;
class UbFileOutput;

class TMC_DLL_EXPORT TmcBeam
{
public:
    TmcBeam();
    TmcBeam(int knotenanzahl, double E, double I, double length, double m, double d);
    ~TmcBeam();

    void init(int knotenanzahl, double E, double I, double length, double m, double d);

    int getDegreeOfFreedom() { return this->degreeOfFreedom; }
    int getElementAnzahl() { return this->elementanzahl; }
    int getKnotenAnzahl() { return this->knotenanzahl; }
    double getElementLength() { return length / elementanzahl; }
    double getMass() { return this->m; }

    void setLinienlast(double last) { linienlast = last; }
    double getLinienlast() { return linienlast; }

    string toString();
    void read(UbFileInput *input);
    void write(UbFileOutput *output);

    LaSquareMatrix *getKMatrix() { return this->kmatrix; }
    LaSquareMatrix *getMMatrix() { return this->mmatrix; }
    LaSquareMatrix *getDMatrix() { return this->dmatrix; }

private:
    double linienlast;

    LaSquareMatrix *kmatrix;
    LaSquareMatrix *mmatrix;
    LaSquareMatrix *dmatrix;

    double E; // E-Modul
    double I; // Traegheitsmoment
    double m; // laengenbezogene masse
    double d;
    double length;
    int knotenanzahl;
    int elementanzahl;
    int degreeOfFreedom;
};
#endif
