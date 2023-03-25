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
     beam system matrix

\*---------------------------------------------------------------------------*/

#include "./TmcBeamSystem.h"

#include <numerics/algebra/LaSquareMatrix.h>

LaSquareMatrix *TmcBeamSystem::getKMatrix(int pointnumber, double E, double I, double elementlength)
{
    int anzahl = pointnumber * 2;
    LaSquareMatrix *kmatrix = new LaSquareMatrix(anzahl, "Stiffnessmatrix");
    double k = E * I / elementlength;
    for (int i = 0; i < anzahl - 3; i += 2)
    {
        kmatrix->addValue(i, i, k * 12.0 / elementlength / elementlength);
        kmatrix->addValue(i + 1, i, -k * 6.0 / elementlength);
        kmatrix->addValue(i + 2, i, -k * 12.0 / elementlength / elementlength);
        kmatrix->addValue(i + 3, i, -k * 6.0 / elementlength);

        kmatrix->addValue(i, i + 1, -k * 6.0 / elementlength);
        kmatrix->addValue(i + 1, i + 1, k * 4.0);
        kmatrix->addValue(i + 2, i + 1, k * 6.0 / elementlength);
        kmatrix->addValue(i + 3, i + 1, k * 2.0);

        kmatrix->addValue(i, i + 2, -k * 12.0 / elementlength / elementlength);
        kmatrix->addValue(i + 1, i + 2, k * 6.0 / elementlength);
        kmatrix->addValue(i + 2, i + 2, k * 12.0 / elementlength / elementlength);
        kmatrix->addValue(i + 3, i + 2, k * 6.0 / elementlength);

        kmatrix->addValue(i, i + 3, -k * 6.0 / elementlength);
        kmatrix->addValue(i + 1, i + 3, k * 2.0);
        kmatrix->addValue(i + 2, i + 3, k * 6.0 / elementlength);
        kmatrix->addValue(i + 3, i + 3, k * 4.0);
    }
    return (kmatrix);
}
/*=================================================*/
LaSquareMatrix *TmcBeamSystem::getMMatrix(int pointnumber, double m, double length)
{
    int anzahl = pointnumber * 2;
    LaSquareMatrix *mmatrix = new LaSquareMatrix(anzahl, "Massmatrix");
    double me = m * length; // m ...mas per length
    for (int i = 0; i < anzahl - 3; i += 2)
    {
        mmatrix->addValue(i, i, me * 13.0 / 35.0);
        mmatrix->addValue(i + 1, i, me * 11.0 / 210.0 * length);
        mmatrix->addValue(i + 2, i, me * 9.0 / 70.0);
        mmatrix->addValue(i + 3, i, -me * 13.0 / 420.0 * length);

        mmatrix->addValue(i, i + 1, me * 11.0 / 210.0 * length);
        mmatrix->addValue(i + 1, i + 1, me * 1.0 / 105.0 * length * length);
        mmatrix->addValue(i + 2, i + 1, me * 13.0 / 420.0 * length);
        mmatrix->addValue(i + 3, i + 1, -me * 1.0 / 140.0 * length * length);

        mmatrix->addValue(i, i + 2, me * 9.0 / 70.0);
        mmatrix->addValue(i + 1, i + 2, me * 13.0 / 420.0 * length);
        mmatrix->addValue(i + 2, i + 2, me * 13.0 / 35.0);
        mmatrix->addValue(i + 3, i + 2, -me * 11.0 / 210.0 * length);

        mmatrix->addValue(i, i + 3, -me * 13.0 / 420.0 * length);
        mmatrix->addValue(i + 1, i + 3, -me * 1.0 / 140.0 * length * length);
        mmatrix->addValue(i + 2, i + 3, -me * 11.0 / 210.0 * length);
        mmatrix->addValue(i + 3, i + 3, me * 1.0 / 105.0 * length * length);
    }
    return (mmatrix);
}
/*=================================================*/
LaSquareMatrix *TmcBeamSystem::getDMatrix(int pointnumber, double d)
{
    int anzahl = pointnumber * 2;
    LaSquareMatrix *dmatrix = new LaSquareMatrix(anzahl, "Dampingmatrix");
    for (int i = 0; i < anzahl - 3; i += 2)
    {
        dmatrix->addValue(i, i, 0.0);
        dmatrix->addValue(i + 1, i, 0.0);
        dmatrix->addValue(i + 2, i, 0.0);
        dmatrix->addValue(i + 3, i, 0.0);

        dmatrix->addValue(i, i + 1, 0.0);
        dmatrix->addValue(i + 1, i + 1, 0.0);
        dmatrix->addValue(i + 2, i + 1, 0.0);
        dmatrix->addValue(i + 3, i + 1, 0.0);

        dmatrix->addValue(i, i + 2, 0.0);
        dmatrix->addValue(i + 1, i + 2, 0.0);
        dmatrix->addValue(i + 2, i + 2, 0.0);
        dmatrix->addValue(i + 3, i + 2, 0.0);

        dmatrix->addValue(i, i + 3, 0.0);
        dmatrix->addValue(i + 1, i + 3, 0.0);
        dmatrix->addValue(i + 2, i + 3, 0.0);
        dmatrix->addValue(i + 3, i + 3, 0.0);
    }
    return (dmatrix);
}
/*=================================================*/
