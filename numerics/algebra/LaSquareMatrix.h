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
 Original Copyright (C) 1997 Markus Brueggemann (BTU Cottbus, Germany, java bauinformatik package)
 Modifications Copyright (C) 2003 Sebastian Geller (VirtualFluids, TU Braunschweig, Germany, translated to C++)
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
     square matrix

\*---------------------------------------------------------------------------*/

#ifndef LASQUAREMATRIX_H
#define LASQUAREMATRIX_H

#include <vector>
#include <cmath>
#include <iostream>

#include "./LaObject.h"
#include "./LaVector.h"
#include <TmcMacroFile.h>
class TmcFileInput;
class TmcFileOutput;

class TMC_DLL_EXPORT LaSquareMatrix : public LaObject
{
    /*======================================================================*/
    /*  Property Attributes                                                 */
    /*                                                                      */
private:
    bool isDiagonal;
    bool diagonalChecked;
    bool isTriDiagonal;
    bool triDiagonalChecked;
    bool isIdentity;
    bool identityChecked;
    bool isSymmetric;
    bool symmetricChecked;
    bool isAntisymmetric;
    bool antisymmetricChecked;
    bool isOrthogonal;
    bool orthogonalChecked;

    double **value;
    int rows;
    int columns;

    /*......................................................................*/
    /*  Left hand unknown equation system                                   */
    /*                                                                      */
    double **lufactorization;
    int *permutations;
    int rowinterchanges;
    bool LUconsistent;
    bool isNearlySingular;
    bool decompositionBehaviour;
    double singularEpsilon;

    /*......................................................................*/
    /*  Mixed unknown equation system                                       */
    /*                                                                      */
    double **lufactorization2;
    int *permutations2;
    int leftunknownsize;
    int rightunknownsize;
    int *leftunknown;
    int *rightunknown;
    bool LUconsistent2;
    bool isNearlySingular2;

    /*......................................................................*/
    /*  Eigensystem properties                                              */
    /*                                                                      */
    LaSquareMatrix *eigenvectors;
    LaVector *eigenvalues;
    LaVector *eigenvaluesI;
    // LaComplexVector ceigenvalues           = null;
    bool solvedEigensystem;
    /*  Konstruktoren                                                       */
    /*                                                                      */
public:
    LaSquareMatrix();
    LaSquareMatrix(int dimension);
    LaSquareMatrix(double **doublearray, int dimension);
    LaSquareMatrix(LaSquareMatrix *matrix);
    LaSquareMatrix(LaSquareMatrix *matrix, std::string name);
    LaSquareMatrix(std::string name);
    LaSquareMatrix(int dimension, std::string name);
    ~LaSquareMatrix();

    int getRowNumber();
    int getColumnNumber();

    LaSquareMatrix *getLUMatrix();
    LaSquareMatrix *getUntereDreiecksMatrix();

    double getValue(int row, int column);
    void setValue(int row, int column, double a);
    int getDimension();
    void setDecompositionBehaviour(bool decompositionBehaviour);
    void setSingularityEpsilon(double epsilon);
    double getSingularityEpsilon();
    void addValue(int row, int column, double a);
    void subtractValue(int row, int column, double a);
    void multiplyValue(int row, int column, double a);
    void divideByValue(int row, int column, double a);
    void multiplyValue(double a);

private:
    void Init(int dimension);
    void setInconsistent();

public:
    LaVector *multiply(LaVector *vector);
    LaSquareMatrix *multiply(LaSquareMatrix *matrix);
    bool isDiagonalMatrix();
    bool isTriDiagonalMatrix();
    bool isIdentityMatrix();
    bool isSymmetricMatrix();
    bool isAntisymmetricMatrix();
    bool isHermitian();
    bool isAntihermitian();
    bool isOrthogonalMatrix();
    bool isUnitary();
    LaSquareMatrix *inverse();
    LaSquareMatrix *transpose();

    LaVector *getEigenvalues();
    LaVector *getImaginaryEigenvalues();
    LaSquareMatrix *getEigenvectors();
    LaVector *solveLinearEquation(LaVector *vektor);
    LaVector *solveLinearEquation(LaVector *left, LaVector *right, std::vector<bool> *index);

    void decomposeLU();

private:
    std::vector<double> *substituteLUback(std::vector<double> *vektor);

    double i_PHYTAG(double a, double b);
    double i_SIGN(double a, double b);
    double i_SQR(double a);
    void calculateEigenvaluesAndEigenvectors();

    void prepareSeparationVectors(std::vector<bool> *index);
    void decomposeLU2();
    std::vector<double> *substituteLUback2(LaVector *left, LaVector *right);

public:
    void cleanSmallNumbers(int base);
    std::string toString();
    std::string toString(std::string format);

    void read(TmcFileInput *in);
    void write(TmcFileOutput *out);
};
#endif
