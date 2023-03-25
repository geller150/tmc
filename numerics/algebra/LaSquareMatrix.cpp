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

#include "./LaSquareMatrix.h"
#include <common/utilities/TmcFileInput.h>
#include <common/utilities/TmcFileOutput.h>

using namespace std;

/**
  Creates a square matrix with no rows and columns.
*/
LaSquareMatrix::LaSquareMatrix() : LaObject("LaSquareMatrix")
{
    this->Init(0);
}
/**
  Creates a square matrix with the specified dimension and initial values 0.0.
  @param dimension the number of rows and columns
*/
LaSquareMatrix::LaSquareMatrix(int dimension) : LaObject("LaSquareMatrix")
{
    this->Init(dimension);
    this->value = new double *[dimension];
    for (int i = 0; i < dimension; i++)
        this->value[i] = new double[dimension];
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            this->value[i][j] = 0.0;
}
/**
  Creates a square matrix from the specified twodimensional double-array.
  @param doublearray[][] double-array to put in the matrix
  @exception TiInvalidObjectException if doublearray is no quadratic array
*/
LaSquareMatrix::LaSquareMatrix(double **doublearray, int dimension) : LaObject("LaSquareMatrix")
{
    // cout<<"Dimension:"<<dimension;
    this->Init(dimension);
    this->value = new double *[dimension];
    for (int i = 0; i < dimension; i++)
        this->value[i] = new double[dimension];
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            this->value[i][j] = doublearray[i][j];
}
/**
  Creates a square matrix as clone of the specified square matrix.
  @param matrix matrix to clone
*/
LaSquareMatrix::LaSquareMatrix(LaSquareMatrix *matrix) : LaObject(matrix->name)
{
    int dimension = matrix->getRowNumber();
    this->Init(dimension);
    this->value = new double *[dimension];
    for (int i = 0; i < dimension; i++)
        this->value[i] = new double[dimension];

    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            this->value[i][j] = matrix->value[i][j];
}
LaSquareMatrix::LaSquareMatrix(LaSquareMatrix *matrix, string name) : LaObject(name)
{
    int dimension = matrix->getRowNumber();
    this->Init(dimension);
    this->value = new double *[dimension];
    for (int i = 0; i < dimension; i++)
        this->value[i] = new double[dimension];

    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            this->value[i][j] = matrix->value[i][j];
}

/**
  Creates a square matrix as clone of the specified general matrix.
  @param matrix matrix to clone
  @exception TiInvalidObjectException if the specified matrix is not quadratic
*/
// public LaSquareMatrix(LaMatrix matrix) throws TiInvalidObjectException
//{
//    this();
//    int m = matrix.value.length;
//    for(int i=0; i<m; i++) if(matrix.value[i].length != m) throw new TiInvalidObjectException(this.name+": no square matrix");

//   this.value = new double[m][m];
//   for(int i=0; i<m; i++) System.arraycopy(matrix.value[i], 0, this.value[i], 0, m);
//}

/**
  Creates a square matrix with no rows and columns and the specified name.
  @param name name of the matrix
*/
LaSquareMatrix::LaSquareMatrix(string name) : LaObject(name)
{
    this->Init(0);
}
/**
  Creates a square matrix with the specified dimension, initial values 0.0 and the specified name.
  @param dimension the number of rows and columns
  @param name name of the matrix
*/
LaSquareMatrix::LaSquareMatrix(int dimension, string name) : LaObject(name)
{
    this->Init(dimension);
    this->value = new double *[dimension];
    for (int i = 0; i < dimension; i++)
        this->value[i] = new double[dimension];
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            this->value[i][j] = 0.0;
}
LaSquareMatrix::~LaSquareMatrix()
{
    if (this->value)
    {
        for (int i = 0; i < rows; i++)
            delete[] value[i];
        delete[] value;
        this->value = NULL;
    }
}
/*======================================================================*/
void LaSquareMatrix::Init(int dimension)
{
    this->rows = dimension;
    this->columns = dimension;

    isDiagonal = false;
    diagonalChecked = false;
    isTriDiagonal = false;
    triDiagonalChecked = false;
    isIdentity = false;
    identityChecked = false;
    isSymmetric = false;
    symmetricChecked = false;
    isAntisymmetric = false;
    antisymmetricChecked = false;
    isOrthogonal = false;
    orthogonalChecked = false;
    lufactorization = NULL;
    permutations = NULL;
    rowinterchanges = 1;
    LUconsistent = false;
    isNearlySingular = false;
    decompositionBehaviour = false;
    singularEpsilon = 1.0e-10;
    lufactorization2 = NULL;
    permutations2 = NULL;
    leftunknownsize = -1;
    rightunknownsize = -1;
    leftunknown = NULL;
    rightunknown = NULL;
    LUconsistent2 = false;
    isNearlySingular2 = false;
    eigenvectors = NULL;
    eigenvalues = NULL;
    eigenvaluesI = NULL;
    // ceigenvalues           = NULL;
    solvedEigensystem = false;
}

int LaSquareMatrix::getRowNumber() { return (this->rows); }
int LaSquareMatrix::getColumnNumber() { return (this->columns); }

/**
  Sets the element specified by a rownumber and columnnumber
  @param row the row to set the element
  @param column the column to set the element
  @param a the doublevalue to set the specified element with
  @exception ArrayIndexOutOfBoundsException if row or column is out of range
  @see LaMatrix#getValue
*/
double LaSquareMatrix::getValue(int row, int column)
{
    return (this->value[row][column]);
}

void LaSquareMatrix::setValue(int row, int column, double a)
{
    if (row >= this->rows)
        throw TmcException("LaSquareMatrix.setValue() - row out of range ");
    if (column >= this->columns)
        throw TmcException("LaSquareMatrix.setValue() - column out of range ");
    this->value[row][column] = a;
    this->setInconsistent();
}
/**
  Returns the number of rows/colums
  @return the number of rows/colums
  @see LaMatrix#getRowNumber
  @see LaRectangularMatrix#getColumnNumber
  @see LaMatrix#getColumnNumber
*/
int LaSquareMatrix::getDimension()
{
    return (this->getRowNumber());
}

LaSquareMatrix *LaSquareMatrix::getLUMatrix()
{
    if (lufactorization == NULL)
        return NULL;
    else
        return new LaSquareMatrix(lufactorization, this->getRowNumber());
}
/********************************************/
LaSquareMatrix *LaSquareMatrix::getUntereDreiecksMatrix()
{
    throw TmcException("das ist nicht die untere !!!");
    cout << "Ist auf alle Fälle im Testmodus !!!!!" << endl;
    if (lufactorization == NULL)
        return NULL;

    LaSquareMatrix *back = this->getLUMatrix();
    int anzahl = this->getRowNumber();
    for (int i = anzahl - 1; i >= 0; i--)
        for (int j = anzahl - 1; j > i; j--)
            back->setValue(i, j, 0.0);

    return back;
}
/**
  Sets the decomposition behaviour of this matrix. <BR>
  If set to true, an ArithmetiTmcException is thrown during decomposition, if this matrix
  (or partial matrix in case of separated decomposition) is nearly singular. <BR>
  If set to false, an ArithmetiTmcException is thrown during solving, if this matrix
  (or partial matrix in case of separated decomposition) is exactly singular.
  @param decompositionBehaviour the decomposition behaviour (defaults to false)
*/
void LaSquareMatrix::setDecompositionBehaviour(bool decompositionBehaviour)
{
    this->decompositionBehaviour = decompositionBehaviour;
    this->setInconsistent();
}
/**
  Sets the singularity epsilon of this matrix. Default value is 1e-10.
      @param epsilon the singularity epsilon
      @see #getSingularityEpsilon
    */
void LaSquareMatrix::setSingularityEpsilon(double epsilon)
{
    this->singularEpsilon = epsilon;
    this->setInconsistent();
}
/**
  Returns the singularity epsilon of this matrix.
  @param epsilon the singularity epsilon
  @see #setSingularityEpsilon
*/
double LaSquareMatrix::getSingularityEpsilon()
{
    return (this->singularEpsilon);
}

/**
  Adds the specified value to the element specified by a rownumber and columnnumber
  @param row the row of the element
  @param column the column of the element
  @param a the doublevalue to add to the specified element
  @exception ArrayIndexOutOfBoundsException if row or column is out of range
*/
void LaSquareMatrix::addValue(int row, int column, double a)
{
    this->value[row][column] += a;
    this->setInconsistent();
}
/**
  Subtracts the specified value from the element specified by a rownumber and columnnumber
  @param row the row of the element
  @param column the column of the element
  @param a the doublevalue to be subtracted from the specified element
  @exception ArrayIndexOutOfBoundsException if row or column is out of range
*/
void LaSquareMatrix::subtractValue(int row, int column, double a)
{
    this->value[row][column] -= a;
    this->setInconsistent();
}
/**
  Multiplies the specified value to the element specified by a rownumber
  @param row the row of the element
  @param column the column of the element
  @param a the doublevalue to multiply the specified element with
  @exception ArrayIndexOutOfBoundsException if row or column is out of range
*/
void LaSquareMatrix::multiplyValue(int row, int column, double a)
{
    this->value[row][column] *= a;
    this->setInconsistent();
}
/**
  Divides the element specified by a rownumber by the specified value
  @param row the row of the element
  @param column the column of the element
  @param a the doublevalue to divide the specified element by
  @exception ArrayIndexOutOfBoundsException if row or column is out of range
*/
void LaSquareMatrix::divideByValue(int row, int column, double a)
{
    this->value[row][column] /= a;
    this->setInconsistent();
}
/**
  Subtracts the specified value from all elements
  @param a the doublevalue to be subtracted from the specified element
*/
/**
  Multiplies the specified value to all elements
  @param a the doublevalue to multiply the specified element with
*/
void LaSquareMatrix::multiplyValue(double a)
{
    for (int i = 0; i < this->rows; i++)
        for (int j = 0; j < this->columns; j++)
            this->value[i][j] *= a;
    this->setInconsistent();
}

void LaSquareMatrix::setInconsistent()
{
    this->diagonalChecked = false;
    this->triDiagonalChecked = false;
    this->identityChecked = false;
    this->symmetricChecked = false;
    this->antisymmetricChecked = false;
    this->orthogonalChecked = false;
    this->LUconsistent = false;
    this->LUconsistent2 = false;
    this->solvedEigensystem = false;
}
/*======================================================================*/

LaVector *LaSquareMatrix::multiply(LaVector *vector)
{
    int n = this->rows;
    int m = this->columns;
    int p = (int)vector->value->size();
    if (p != m)
        throw TmcException(".multiply(): incompatible sizes");

    LaVector *back = new LaVector(n);
    for (int i = 0; i < n; i++)
        for (int k = 0; k < m; k++)
            (*back->value)[i] += value[i][k] * (*vector->value)[k];
    return (back);
}

/**
  Multiplies the square matrix with another square matrix.
  @param matrix matrix to multiply with
  @return the result-matrix
  @exception ArrayIndexOutOfBoundsException if sizes are incompatible
*/
LaSquareMatrix *LaSquareMatrix::multiply(LaSquareMatrix *matrix)
{
    int n = this->rows;
    int p = matrix->getRowNumber();
    if (p != n)
        throw TmcException("LaSquareMatrix.multiply()");
    LaSquareMatrix *back = new LaSquareMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                back->value[i][j] += this->value[i][k] * matrix->value[k][j];
    return (back);
}

/**
  Returns true if this real matrix is diagonal. To be a diagonal matrix, the non diagonal elements have to be |a<SUB>ij</SUB>| < 10<SUP>-10</SUP>.
  @return true if this real matrix is diagonal
*/
bool LaSquareMatrix::isDiagonalMatrix()
{
    if (!this->diagonalChecked)
    {
        this->isDiagonal = false;
        if (this->isSymmetricMatrix())
        {
            this->isDiagonal = true;
            for (int i = 0; i < this->rows - 1; i++)
            {
                if (!this->isDiagonal)
                    break;
                for (int j = i + 1; j < columns; j++)
                    if (std::fabs(value[i][j]) > 1e-10)
                    {
                        this->isDiagonal = false;
                        break;
                    }
            }
            this->diagonalChecked = true;
        }
    }
    return (this->isDiagonal);
}
/**
  Returns true if this real matrix is tridiagonal. To be a tridiagonal matrix, the non -1,0,1 diagonal elements have to be |a<SUB>ij</SUB>| < 10<SUP>-10</SUP>.
  @return true if this real matrix is tridiagonal
*/
bool LaSquareMatrix::isTriDiagonalMatrix()
{
    if (!this->triDiagonalChecked)
    {
        this->isTriDiagonal = false;
        if (this->isSymmetricMatrix())
        {
            this->isTriDiagonal = true;
            for (int i = 0; i < rows - 2; i++)
            {
                if (!this->isTriDiagonal)
                    break;
                for (int j = i + 2; j < rows; j++)
                    if (std::fabs(this->value[i][j]) > 1e-10)
                    {
                        this->isTriDiagonal = false;
                        break;
                    }
            }
            this->triDiagonalChecked = true;
        }
    }
    return (this->isTriDiagonal);
}
/**
  Returns true if this real matrix is an identity matrix. To be an identity matrix, the diagonal elements have to be |a<SUB>ii</SUB>-1| < 10<SUP>-10</SUP>.
  @return true if this real matrix is an identity matrix
*/
bool LaSquareMatrix::isIdentityMatrix()
{
    if (!this->identityChecked)
    {
        this->isIdentity = false;
        if (this->isDiagonalMatrix())
        {
            this->isIdentity = true;
            for (int i = 0; i < rows; i++)
                if (std::fabs(this->value[i][i] - 1.0) > 1e-10)
                {
                    this->isIdentity = false;
                    break;
                }
            this->identityChecked = true;
        }
    }
    return (this->isIdentity);
}
/**
  Returns true if this real matrix is symmetric (<B>A<SUP>T</SUP></B>=<B>A</B>). To be a symmetric matrix, the corresponding elements difference has to be less 10<SUP>-10</SUP>.
  Real symmetric matrixes are hermitian (a<SUB>ij</SUB> = a<SUB>ji</SUB><SUP>*</SUP>).
  @return true if this real matrix is symmetric
*/
bool LaSquareMatrix::isSymmetricMatrix()
{
    if (!this->symmetricChecked)
    {
        this->isSymmetric = true;
        for (int i = 0; i < rows - 1; i++)
        {
            if (!this->isSymmetric)
                break;
            for (int j = i + 1; j < rows; j++)
                if (std::fabs(this->value[i][j] - this->value[j][i]) > 1e-10)
                {
                    this->isSymmetric = false;
                    break;
                }
        }
        this->symmetricChecked = true;
    }
    return (this->isSymmetric);
}
/**
  Returns true if this real matrix is antisymmetric (<B>-A<SUP>T</SUP></B>=<B>A</B>).
  Real antisymmetric matrixes are antihermitian (a<SUB>ij</SUB> = -a<SUB>ji</SUB><SUP>*</SUP>).
  @return true if this real matrix is antisymmetric
*/
bool LaSquareMatrix::isAntisymmetricMatrix()
{
    throw TmcException("not yet implemented");
}
/**
  Returns true if this real matrix is hermitian (<B>A</B>=<B>(A*)<SUP>T</SUP></B>).
  @return true if this real matrix is hermitian
  @see #isSymmetric
*/
bool LaSquareMatrix::isHermitian()
{
    return (this->isSymmetricMatrix());
}
/**
  Returns true if this real matrix is antihermitian (<B>A</B>=<B>-(A*)<SUP>T</SUP></B>).
  @return true if this real matrix is antihermitian
  @see #isAntisymmetric
*/
bool LaSquareMatrix::isAntihermitian()
{
    return (this->isAntisymmetricMatrix());
}
/**
  Returns true if this real matrix is orthogonal (<B>A<SUP>T</SUP></B>*<B>A</B> = <B>A</B>*<B>A<SUP>T</SUP></B> = <B>I</B>).
  Real orthogonal matrixes are unitary (inverse equals hermitian conjugate).
  @return true if this real matrix is orthogonal
*/
bool LaSquareMatrix::isOrthogonalMatrix()
{
    throw TmcException("not yet implemented");
}
/**
  Returns true if this real matrix is unitary (<B>(A*)<SUP>T</SUP></B>=<B>A<SUP>-1</SUP></B>).
  @return true if this real matrix is unitary
  @see #isOrthogonal
*/
bool LaSquareMatrix::isUnitary()
{
    return (this->isOrthogonalMatrix());
}

/**
  Calculates the inverse matrix if it exists using the LU-factorization.
  @return the inverse matrix
  @exception ArithmetiTmcException if this matrix is singular
*/
LaSquareMatrix *LaSquareMatrix::inverse()
{
    try
    {
        int length = this->getColumnNumber();
        vector<double> *vektor = new vector<double>;
        vektor->resize(length, 0.0);
        LaSquareMatrix *back = new LaSquareMatrix(length);

        decomposeLU();
        for (int i = 0; i < length; i++)
        {
            for (int j = 0; j < length; j++)
                (*vektor)[j] = 0.0;
            (*vektor)[i] = 1.0;

            vector<double> *result = substituteLUback(vektor);
            for (int j = 0; j < length; j++)
                back->value[j][i] = (*result)[j];
        }
        return (back);
    }
    catch (TmcException &e)
    {
        e.addInfo("Matrix - inverse some problems ...");
        throw;
    }
}

LaSquareMatrix *LaSquareMatrix::transpose()
{
    try
    {
        int length = this->getColumnNumber();
        LaSquareMatrix *back = new LaSquareMatrix(length);

        for (int i = 0; i < length; i++)
        {
            for (int j = 0; j < length; j++)
                back->value[j][i] = this->value[i][j];
        }
        return (back);
    }
    catch (TmcException &e)
    {
        e.addInfo("Matrix - inverse some problems ...");
        throw;
    }
}

/*======================================================================*/

/*======================================================================*/
/*  eigen system                                                        */
/*                                                                      */
/**
  Returns the eigenvalues of this matrix (ordered by size), if it is symmetric.
  Returns the real parts of the eigenvalues of this matrix, if it is non-symmetric.
  <BR>
  Calculation is only done if necessary. Eigenvalues for symmetric matrixes are solved using
  tri-diagonal form, eigenvalues for non-symmetric matrixes are found using Hessenberg form.
  <BR>
  For symmetric matrixes eigenvalues and eigenvectors are calculated together. For
  non-symmetric matrixes, only eigenvalues are calculated.
  <BR><BR>
  For details about calculation, see <A HREF="http://www.nr.com">Numerical recipies</A>.
  @return the eigenvalues (real part, in case of non-symmetrix matrixes)
  @exception ArithmetiTmcException if calculation fails (e.g. too many iterations needed)
  @see #getEigenvectors()
  @see #getImaginaryEigenvalues()
  @see #getComplexEigenvalues()
*/
LaVector *LaSquareMatrix::getEigenvalues()
{
    this->calculateEigenvaluesAndEigenvectors();
    return (this->eigenvalues);
}
/**
  Returns the imaginary part of the eigenvalues of this matrix, if it is non-symmetric.
  In case of symmetric matrixes, returns null.
  <BR>
  Calculation is only done if necessary. Eigenvalues for non-symmetric matrixes are found using Hessenberg form.
  For non-symmetric matrixes, only eigenvalues are calculated.
  <BR><BR>
  For details about calculation, see <A HREF="http://www.nr.com">Numerical recipies</A>.
  @return the imaginary part of the eigenvalues (null for symmetric matrixes)
  @exception ArithmetiTmcException if calculation fails (e.g. too many iterations needed)
  @see #getEigenvalues()
  @see #getComplexEigenvalues()
*/
LaVector *LaSquareMatrix::getImaginaryEigenvalues()
{
    throw TmcException("not yet implemented");
}
/**
  Returns the eigenvectors of this matrix (ordered corresponding to the eigenvalues), if it is symmetric. Returns null for non-symmetric matrixes.
  <BR>
  Calculation is only done if necessary.
  <BR><BR>
  For details about calculation, see <A HREF="http://www.nr.com">Numerical recipies</A>.
  @return the eigenvectors
  @exception ArithmetiTmcException if calculation fails (e.g. too many iterations needed)
  @see #getEigenvalues()
*/
LaSquareMatrix *LaSquareMatrix::getEigenvectors()
{
    this->calculateEigenvaluesAndEigenvectors();
    return (this->eigenvectors);
}
/*======================================================================*/

/*======================================================================*/
/*   solving linear equations                                           */
/*                                                                      */
/**
  Solves the linear equation Ax=b using the LU-factorization.
  @param vector the right-hand vector
  @return the result-vector
  @exception ArithmetiTmcException if matrix is singular.
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaVector *LaSquareMatrix::solveLinearEquation(LaVector *vektor)
{
    if (rows != (int)vektor->value->size())
        throw TmcException("LaSquareMatrix.solveLinearEquation");

    try
    {
        decomposeLU();
        vector<double> *substituteVec = substituteLUback(vektor->value);
        LaVector *back = new LaVector(substituteVec);
        delete substituteVec;
        return (back);
    }
    catch (string &s)
    {
        cout << s << endl;
        throw TmcException(__FILE__, __LINE__, "LaSquareMatrix.solveLinearEquation()");
    }
    catch (string *s)
    {
        cout << *s << endl;
        throw TmcException(__FILE__, __LINE__, "LaSquareMatrix.solveLinearEquation()");
    }
    catch (...)
    {
        throw TmcException("LaSquareMatrix.solveLinearEquation()");
    }
}

/**
  Solves the linear equation Ax=b (unknown in x and b!!) using the LU-factorization.
  @param left  the left-hand vector
  @param right the right-hand vector
  @param index the index vector, a true entry marks an unknown left-hand side entry
  @return the result-vector
  @exception ArithmetiTmcException if matrix is singular.
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaVector *LaSquareMatrix::solveLinearEquation(LaVector *left, LaVector *right, vector<bool> *index)
{
    if (rows != (int)left->value->size())
        throw TmcException("LaSquareMatrix.solveLinearEquation()");
    if (rows != (int)right->value->size())
        throw TmcException("LaSquareMatrix.solveLinearEquation()");

    try
    {
        prepareSeparationVectors(index);
        decomposeLU2();
        LaVector *result = new LaVector(substituteLUback2(left, right));
        result->setName("Result");
        return (result);
    }
    catch (string &s)
    {
        cout << s << endl;
        throw TmcException("LaSquareMatrix.solveLinearEquation()");
    }
    catch (string *s)
    {
        cout << *s << endl;
        throw TmcException("LaSquareMatrix.solveLinearEquation()");
    }
    catch (...)
    {
        throw TmcException("LaSquareMatrix.solveLinearEquation()");
    }
}
/*======================================================================*/

/*======================================================================*/
/*  complete matrix                                                     */
/*                                                                      */
void LaSquareMatrix::decomposeLU()
{
    if (LUconsistent)
    {
        return;
    }

    int o = rows;
    int imax = 0;
    vector<double> vektor;
    vektor.resize(o, 0.0);
    if (lufactorization == NULL)
    {
        lufactorization = new double *[o];
        for (int i = 0; i < o; i++)
        {
            lufactorization[i] = new double[o];
        }
    }
    if (permutations == NULL)
        permutations = new int[o];
    rowinterchanges = 1;
    isNearlySingular = false;

    for (int i = 0; i < o; i++)
        for (int j = 0; j < o; j++)
            lufactorization[i][j] = value[i][j];
    for (int i = 0; i < o; i++)
    {
        double big = 0.0;
        for (int j = 0; j < o; j++)
        {
            if (std::fabs(lufactorization[i][j]) > big)
                big = std::fabs(lufactorization[i][j]);
        }
        if (big == 0.0)
            throw TmcException("LaMatrix.decomposeLU(): Matrix is singular");
        if (big < singularEpsilon)
            isNearlySingular = true;
        (vektor)[i] = 1.0 / big;
    }

    for (int j = 0; j < o; j++)
    {
        for (int i = 0; i < j; i++)
        {
            double sum = lufactorization[i][j];
            for (int k = 0; k < i; k++)
                sum -= lufactorization[i][k] * lufactorization[k][j];
            lufactorization[i][j] = sum;
        }
        double big = 0.0;
        for (int i = j; i < o; i++)
        {
            double sum = lufactorization[i][j];
            for (int k = 0; k < j; k++)
                sum -= lufactorization[i][k] * lufactorization[k][j];
            lufactorization[i][j] = sum;
            if ((vektor)[i] * std::fabs(sum) >= big)
            {
                big = (vektor)[i] * std::fabs(sum);
                imax = i;
            }
        }
        if (j != imax)
        {
            for (int k = 0; k < o; k++)
            {
                double dum = lufactorization[imax][k];
                lufactorization[imax][k] = lufactorization[j][k];
                lufactorization[j][k] = dum;
            }
            rowinterchanges *= -1;
            (vektor)[imax] = (vektor)[j];
        }
        permutations[j] = imax;
        if (lufactorization[j][j] == 0.0)
            throw TmcException("LaMatrix..decomposeLU(): Matrix is singular");
        if (std::fabs(lufactorization[j][j]) < singularEpsilon)
            isNearlySingular = true;
        if (j != o)
        {
            double dum = 1.0 / lufactorization[j][j];
            for (int i = j + 1; i < o; i++)
                lufactorization[i][j] *= dum;
        }
    }
    if (decompositionBehaviour && isNearlySingular)
        throw TmcException("LaMatrix.decomposeLU(): Matrix is nearly singular");

    LUconsistent = true;
}

vector<double> *LaSquareMatrix::substituteLUback(vector<double> *vektor)
{

    int o = (int)vektor->size();
    int flag = -1;
    vector<double> *back = new vector<double>;
    back->resize(o, 0.0);

    for (int i = 0; i < o; i++)
        (*back)[i] = (*vektor)[i];

    /*-------------------------------------------------------------------*/
    /*  Substituting back - Calculating unknown left hand parts          */
    /*                                                                   */
    for (int i = 0; i < o; i++)
    {
        double sum = (*back)[permutations[i]];
        (*back)[permutations[i]] = (*back)[i];

        if (flag >= 0)
            for (int j = flag; j < i; j++)
                sum -= lufactorization[i][j] * (*back)[j];
        else if (sum != 0.0)
            flag = i;

        (*back)[i] = sum;
    }
    for (int i = o - 1; i >= 0; i--)
    {
        double sum = (*back)[i];
        for (int j = i + 1; j < o; j++)
            sum -= lufactorization[i][j] * (*back)[j];
        (*back)[i] = sum / lufactorization[i][i];
    }
    return (back);
}
/*======================================================================*/

/*======================================================================*/
/*   Eigensystem                                                        */
/*                                                                      */
double LaSquareMatrix::i_PHYTAG(double a, double b)
{
    double absa, absb;
    absa = std::fabs(a);
    absb = std::fabs(b);
    if (absa > absb)
        return (absa * std::sqrt(1.0 + i_SQR(absb / absa)));
    else
        return (absb == 0.0 ? 0.0 : absb * std::sqrt(1.0 + i_SQR(absa / absb)));
}
double LaSquareMatrix::i_SIGN(double a, double b)
{
    return (b >= 0.0 ? std::fabs(a) : -std::fabs(a));
}
double LaSquareMatrix::i_SQR(double a)
{
    return (a == 0.0 ? 0.0 : a * a);
}

void LaSquareMatrix::calculateEigenvaluesAndEigenvectors()
{
    if (this->solvedEigensystem)
        return;

    try
    {
        int i, j, k, l, m, n, nn, mmin, its, iter, last;
        double s, r, p, h, g, f, c, b, x, y, z, q, t, u, v, w, dd, hh, scale, sqrdx, anorm;
        // double a[][], V[][], d[], C[], E[], wr[], wi[];
        double **a, **V, *d, *C, *E, *wr, *wi;

        n = this->rows;
        C = new double[n];
        d = new double[n];
        a = new double *[n];
        for (int uu = 0; uu < n; uu++)
            a[uu] = new double[n];
        V = new double *[n];
        for (int uu = 0; uu < n; uu++)
            V[uu] = new double[n];
        wr = NULL;
        wi = NULL;

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                a[i][j] = this->value[i][j];
                V[i][j] = 0.0;
            }
            C[i] = 0.0;
            d[i] = a[i][i];
            V[i][i] = 1.0;
        }

        if (!this->isDiagonalMatrix() && this->isSymmetricMatrix())
        {
            E = new double[n];

            if (this->isTriDiagonalMatrix())
            {
                for (i = 0; i < n; i++)
                {
                    d[i] = a[i][i];
                    if (i > 0)
                        E[i] = a[i - 1][i];
                    else
                        E[i] = 0.0;
                }
            }
            else // ....Transforming into  TriDiagonal form...
            {
                for (i = n - 1; i > 0; i--)
                {
                    l = i - 1;
                    h = scale = 0.0;
                    if (l > 0)
                    {
                        for (k = 0; k <= l; k++)
                            scale += std::fabs(a[i][k]);
                        if (scale == 0.0)
                            E[i] = a[i][l];
                        else
                        {
                            for (k = 0; k <= l; k++)
                            {
                                a[i][k] /= scale;
                                h += a[i][k] * a[i][k];
                            }
                            f = a[i][l];
                            g = (f >= 0.0 ? -std::sqrt(h) : std::sqrt(h));
                            E[i] = scale * g;
                            h -= f * g;
                            a[i][l] = f - g;
                            f = 0.0;
                            for (j = 0; j <= l; j++)
                            {
                                a[j][i] = a[i][j] / h;
                                g = 0.0;
                                for (k = 0; k <= j; k++)
                                    g += a[j][k] * a[i][k];
                                for (k = j + 1; k <= l; k++)
                                    g += a[k][j] * a[i][k];
                                E[j] = g / h;
                                f += E[j] * a[i][j];
                            }
                            hh = f / (h + h);
                            for (j = 0; j <= l; j++)
                            {
                                f = a[i][j];
                                E[j] = g = E[j] - hh * f;
                                for (k = 0; k <= j; k++)
                                    a[j][k] -= (f * E[k] + g * a[i][k]);
                            }
                        }
                    }
                    else
                        E[i] = a[i][l];

                    d[i] = h;
                }
                d[0] = 0.0;
                E[0] = 0.0;

                for (i = 0; i < n; i++)
                {
                    l = i - 1;
                    if (d[i] != 0.0)
                    {
                        for (j = 0; j <= l; j++)
                        {
                            g = 0.0;
                            for (k = 0; k <= l; k++)
                                g += a[i][k] * a[k][j];
                            for (k = 0; k <= l; k++)
                                a[k][j] -= g * a[k][i];
                        }
                    }
                    d[i] = a[i][i];
                    a[i][i] = 1.0;
                    for (j = 0; j <= l; j++)
                        a[j][i] = a[i][j] = 0.0;
                }
                V = a;
            }

            for (i = 1; i < n; i++)
                E[i - 1] = E[i];
            E[n - 1] = 0.0;

            for (l = 0; l < n; l++) // ....Calculating Eigensystem for TriDiagonal Matrix...
            {
                iter = 0;
                do
                {
                    for (m = l; m < n - 1; m++)
                    {
                        dd = std::fabs(d[m]) + std::fabs(d[m + 1]);
                        if ((std::fabs(E[m]) + dd) == dd)
                            break;
                    }
                    if (m != l)
                    {
                        if (iter++ > 50)
                            throw TmcException("new ArithmetiTmcException()");

                        g = (d[l + 1] - d[l]) / (2.0 * E[l]);
                        r = i_PHYTAG(g, 1.0);
                        g = d[m] - d[l] + E[l] / (g + i_SIGN(r, g));
                        s = c = 1.0;
                        p = 0.0;

                        for (i = m - 1; i >= l; i--)
                        {
                            f = s * E[i];
                            b = c * E[i];
                            E[i + 1] = (r = i_PHYTAG(f, g));
                            if (r == 0.0)
                            {
                                d[i + 1] -= p;
                                E[m] = 0.0;
                                break;
                            }
                            s = f / r;
                            c = g / r;
                            g = d[i + 1] - p;
                            r = (d[i] - g) * s + 2.0 * c * b;
                            d[i + 1] = g + (p = s * r);
                            g = c * r - b;
                            for (k = 0; k < n; k++)
                            {
                                f = V[k][i + 1];
                                V[k][i + 1] = s * V[k][i] + c * f;
                                V[k][i] = c * V[k][i] - s * f;
                            }
                        }
                        if (r == 0.0 && i >= l)
                            continue;
                        d[l] -= p;
                        E[l] = g;
                        E[m] = 0.0;
                    }
                } while (m != l);
            }
        }
        else if (!this->isDiagonalMatrix()) // ...NON SYMMETRIC CASE...
        {
            int RADIX = 2;

            sqrdx = (double)RADIX * RADIX;
            last = 0;
            while (last == 0) // ...Balancing...
            {
                last = 1;
                for (i = 0; i < n; i++)
                {
                    r = c = 0.0;
                    for (j = 0; j < n; j++)
                    {
                        if (j != i)
                        {
                            c += std::fabs(a[j][i]);
                            r += std::fabs(a[i][j]);
                        }
                    }
                    if (c != 0.0 && r != 0.0)
                    {
                        g = r / RADIX;
                        f = 1.0;
                        s = c + r;
                        while (c < g)
                        {
                            f *= RADIX;
                            c *= sqrdx;
                        }
                        g = r * RADIX;
                        while (c > g)
                        {
                            f /= RADIX;
                            c /= sqrdx;
                        }
                        if ((c + r) / f < 0.95 * s)
                        {
                            last = 0;
                            g = 1.0 / f;
                            for (j = 0; j < n; j++)
                                a[i][j] *= g;
                            for (j = 0; j < n; j++)
                                a[j][i] *= f;
                        }
                    }
                }
            }
            for (m = 1; m < n - 1; m++) // ...Transforming into Hessenberg form...
            {
                x = 0.0;
                i = m;
                for (j = m; j < n; j++)
                {
                    if (std::fabs(a[j][m - 1]) > std::fabs(x))
                    {
                        x = a[j][m - 1];
                        i = j;
                    }
                }
                if (i != m)
                {
                    for (j = m - 1; j < n; j++)
                    {
                        dd = a[i][j];
                        a[i][j] = a[m][j];
                        a[m][j] = dd;
                    }
                    for (j = 0; j < n; j++)
                    {
                        dd = a[j][i];
                        a[j][i] = a[j][m];
                        a[j][m] = dd;
                    }
                }
                if (x != 0.0)
                {
                    for (i = m + 1; i < n; i++)
                    {
                        if ((y = a[i][m - 1]) != 0.0)
                        {
                            y /= x;
                            a[i][m - 1] = y;
                            for (j = m; j < n; j++)
                                a[i][j] -= y * a[m][j];
                            for (j = 1; j < n; j++)
                                a[j][m] += y * a[j][i];
                        }
                    }
                }
            }

            wr = new double[n];
            wi = new double[n];

            anorm = 0.0;
            for (i = 0; i < n; i++)
                for (j = std::max(i - 1, 0); j < n; j++)
                    anorm += std::fabs(a[i][j]);
            nn = n - 1;
            p = 0.0;
            q = 0.0;
            r = 0.0;
            t = 0.0;
            while (nn >= 0) // ...QR-Algorithm for Hessenberg form...
            {
                its = 0;
                do
                {
                    for (l = nn; l > 0; l--)
                    {
                        s = std::fabs(a[l - 1][l - 1]) + std::fabs(a[l][l]);
                        if (s == 0.0)
                            s = anorm;
                        if ((std::fabs(a[l][l - 1]) + s) == s)
                            break;
                    }
                    x = a[nn][nn];
                    if (l == nn)
                    {
                        wr[nn] = x + t;
                        wi[nn--] = 0.0;
                    }
                    else
                    {
                        y = a[nn - 1][nn - 1];
                        w = a[nn][nn - 1] * a[nn - 1][nn];
                        if (l == (nn - 1))
                        {
                            p = 0.5 * (y - x);
                            q = p * p + w;
                            z = std::sqrt(std::fabs(q));
                            x += t;
                            if (q >= 0.0)
                            {
                                z = p + i_SIGN(z, p);
                                wr[nn - 1] = wr[nn] = x + z;
                                if (z != 0.0)
                                    wr[nn] = x - w / z;
                                wi[nn - 1] = wi[nn] = 0.0;
                            }
                            else
                            {
                                wr[nn - 1] = wr[nn] = x + p;
                                wi[nn - 1] = -(wi[nn] = z);
                            }
                            nn -= 2;
                        }
                        else
                        {
                            if (its > 29)
                                throw TmcException(" ArithmeticException()");
                            if (its == 10 || its == 20)
                            {
                                t += x;
                                for (i = 0; i <= nn; i++)
                                    a[i][i] -= x;
                                s = std::fabs(a[nn][nn - 1]) + std::fabs(a[nn - 1][nn - 2]);
                                y = x = 0.75 * s;
                                w = -0.4375 * s * s;
                            }
                            ++its;
                            for (m = (nn - 2); m >= l; m--)
                            {
                                z = a[m][m];
                                r = x - z;
                                s = y - z;
                                p = (r * s - w) / a[m + 1][m] + a[m][m + 1];
                                q = a[m + 1][m + 1] - z - r - s;
                                r = a[m + 2][m + 1];
                                s = std::fabs(p) + std::fabs(q) + std::fabs(r);
                                p /= s;
                                q /= s;
                                r /= s;
                                if (m == l)
                                    break;
                                u = std::fabs(a[m][m - 1]) * (std::fabs(q) + std::fabs(r));
                                v = std::fabs(p) * (std::fabs(a[m - 1][m - 1]) + std::fabs(z) + std::fabs(a[m + 1][m + 1]));
                                if ((u + v) == v)
                                    break;
                            }
                            for (i = m + 2; i <= nn; i++)
                            {
                                a[i][i - 2] = 0.0;
                                if (i != (m + 2))
                                    a[i][i - 3] = 0.0;
                            }
                            for (k = m; k < nn; k++)
                            {
                                if (k != m)
                                {
                                    p = a[k][k - 1];
                                    q = a[k + 1][k - 1];
                                    r = 0.0;
                                    if (k != (nn - 1))
                                        r = a[k + 2][k - 1];
                                    if ((x = std::fabs(p) + std::fabs(q) + std::fabs(r)) != 0.0)
                                    {
                                        p /= x;
                                        q /= x;
                                        r /= x;
                                    }
                                }
                                if ((s = i_SIGN(std::sqrt(p * p + q * q + r * r), p)) != 0.0)
                                {
                                    if (k == m)
                                    {
                                        if (l != m)
                                            a[k][k - 1] = -a[k][k - 1];
                                    }
                                    else
                                        a[k][k - 1] = -s * x;
                                    p += s;
                                    x = p / s;
                                    y = q / s;
                                    z = r / s;
                                    q /= p;
                                    r /= p;
                                    for (j = k; j <= nn; j++)
                                    {
                                        p = a[k][j] + q * a[k + 1][j];
                                        if (k != (nn - 1))
                                        {
                                            p += r * a[k + 2][j];
                                            a[k + 2][j] -= p * z;
                                        }
                                        a[k + 1][j] -= p * y;
                                        a[k][j] -= p * x;
                                    }
                                    mmin = nn < k + 3 ? nn : k + 3;
                                    for (i = l; i <= mmin; i++)
                                    {
                                        p = x * a[i][k] + y * a[i][k + 1];
                                        if (k != (nn - 1))
                                        {
                                            p += z * a[i][k + 2];
                                            a[i][k + 2] -= p * r;
                                        }
                                        a[i][k + 1] -= p * q;
                                        a[i][k] -= p;
                                    }
                                }
                            }
                        }
                    }
                } while (l < nn - 1);
            }
            d = NULL;
            V = NULL;
        }

        if (d != NULL && V != NULL) // ...Sorting for symmetric case...
        {
            for (i = 0; i < n; i++)
            {
                p = d[k = i];
                for (j = i; j < n; j++)
                    if (d[j] < p)
                        p = d[k = j];
                if (k != i)
                {
                    d[k] = d[i];
                    d[i] = p;
                    for (j = 0; j < n; j++)
                    {
                        p = V[j][i];
                        V[j][i] = V[j][k];
                        V[j][k] = p;
                    }
                }
            }
        }
        if (d != NULL) // ...Symmetric case...
        {
            this->eigenvalues = new LaVector(d, rows);
            this->eigenvaluesI = NULL;

            if (V != NULL)
                this->eigenvectors = new LaSquareMatrix(V, rows);
        }
        else if (wr != NULL && wi != NULL) // ...Non-symmetric case...
        {
            this->eigenvalues = new LaVector(wr, rows);
            this->eigenvaluesI = new LaVector(wi, rows);
        }
        this->solvedEigensystem = true;
    }
    catch (...)
    {
        this->eigenvectors = NULL;
        this->eigenvalues = NULL;
        this->eigenvaluesI = NULL;
        throw TmcException(".calculateEigenvalues/-vectors(): calculation failed");
    }
}
/*======================================================================*/

/*======================================================================*/
/*  indexed part of matrix                                              */
/*                                                                      */
void LaSquareMatrix::prepareSeparationVectors(vector<bool> *index)
{
    if (leftunknownsize + rightunknownsize != (int)index->size())
        LUconsistent2 = false;
    else if (leftunknownsize == -1)
        LUconsistent2 = false;
    else if (rightunknownsize == -1)
        LUconsistent2 = false;
    else
    {
        for (int i = 0; i < leftunknownsize; i++)
            if (!(*index)[leftunknown[i]])
            {
                LUconsistent2 = false;
                break;
            }
        for (int i = 0; i < rightunknownsize; i++)
            if ((*index)[rightunknown[i]])
            {
                LUconsistent2 = false;
                break;
            }
    }
    if (!LUconsistent2)
    {
        leftunknownsize = 0;
        int j = 0;
        int k = 0;

        for (int i = 0; i < (int)index->size(); i++)
            if ((*index)[i])
                leftunknownsize++;
        rightunknownsize = (int)index->size() - leftunknownsize;

        leftunknown = new int[leftunknownsize];
        rightunknown = new int[rightunknownsize];

        for (int i = 0; i < (int)index->size(); i++)
        {
            if ((*index)[i])
            {
                leftunknown[j] = i;
                j++;
            }
            else
            {
                rightunknown[k] = i;
                k++;
            }
        }
    }
}
void LaSquareMatrix::decomposeLU2()
{
    if (LUconsistent2)
        return;

    int imax = 0;
    vector<double> *vektor = new vector<double>;
    vektor->resize(leftunknownsize, 0.0);

    lufactorization2 = new double *[leftunknownsize];
    for (int i = 0; i < leftunknownsize; i++)
        lufactorization2[i] = new double[leftunknownsize];

    permutations2 = new int[leftunknownsize];
    isNearlySingular2 = false;

    for (int i = 0; i < leftunknownsize; i++)
        for (int j = 0; j < leftunknownsize; j++)
            lufactorization2[i][j] = value[leftunknown[i]][leftunknown[j]];

    for (int i = 0; i < leftunknownsize; i++)
    {
        double big = 0.0;
        for (int j = 0; j < leftunknownsize; j++)
            if (std::fabs(lufactorization2[i][j]) > big)
                big = std::fabs(lufactorization2[i][j]);
        if (big == 0.0)
            throw TmcException("LaMatrix.decomposeLU(): Partial Matrix is singular");
        if (big < singularEpsilon)
            isNearlySingular2 = true;
        (*vektor)[i] = 1.0 / big;
    }
    for (int j = 0; j < leftunknownsize; j++)
    {
        for (int i = 0; i < j; i++)
        {
            double sum = lufactorization2[i][j];
            for (int k = 0; k < i; k++)
                sum -= lufactorization2[i][k] * lufactorization2[k][j];
            lufactorization2[i][j] = sum;
        }
        double big = 0.0;
        for (int i = j; i < leftunknownsize; i++)
        {
            double sum = lufactorization2[i][j];
            for (int k = 0; k < j; k++)
                sum -= lufactorization2[i][k] * lufactorization2[k][j];
            lufactorization2[i][j] = sum;
            if ((*vektor)[i] * std::fabs(sum) >= big)
            {
                big = (*vektor)[i] * std::fabs(sum);
                imax = i;
            }
        }
        if (j != imax)
        {
            for (int k = 0; k < leftunknownsize; k++)
            {
                double dum = lufactorization2[imax][k];
                lufactorization2[imax][k] = lufactorization2[j][k];
                lufactorization2[j][k] = dum;
            }
            (*vektor)[imax] = (*vektor)[j];
        }
        permutations2[j] = imax;
        if (lufactorization2[j][j] == 0.0)
            throw TmcException("LaMatrix.decomposeLU(): Partial Matrix is singular");
        if (std::fabs(lufactorization2[j][j]) < singularEpsilon)
            isNearlySingular2 = true;
        if (j != leftunknownsize)
        {
            double dum = 1.0 / lufactorization2[j][j];
            for (int i = j + 1; i < leftunknownsize; i++)
                lufactorization2[i][j] *= dum;
        }
    }
    if (decompositionBehaviour && isNearlySingular2)
        throw TmcException("LaSquareMatrix.decomposeLU(): Partial Matrix is nearly singular");
    LUconsistent2 = true;
};

vector<double> *LaSquareMatrix::substituteLUback2(LaVector *left, LaVector *right)
{
    int flag = -1;

    vector<double> *back = new vector<double>;
    back->resize(leftunknownsize + rightunknownsize, 0.0);

    /*-------------------------------------------------------------------*/
    /*  Initializing return vector                                       */
    /*                                                                   */
    for (int i = 0; i < leftunknownsize; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < rightunknownsize; j++)
            sum += this->value[leftunknown[i]][rightunknown[j]] * (*left->value)[rightunknown[j]];
        (*back)[leftunknown[i]] = (*right->value)[leftunknown[i]] - sum;
    }

    /*-------------------------------------------------------------------*/
    /*  Substituting back - Calculating unknown left hand parts          */
    /*                                                                   */
    for (int i = 0; i < leftunknownsize; i++)
    {
        double sum = (*back)[leftunknown[permutations2[i]]];
        (*back)[leftunknown[permutations2[i]]] = (*back)[leftunknown[i]];

        if (flag >= 0)
            for (int j = flag; j < i; j++)
                sum -= lufactorization2[i][j] * (*back)[leftunknown[j]];
        else if (sum != 0.0)
            flag = i;

        (*back)[leftunknown[i]] = sum;
    }
    for (int i = leftunknownsize - 1; i >= 0; i--)
    {
        double sum = (*back)[leftunknown[i]];
        for (int j = i + 1; j < leftunknownsize; j++)
            sum -= lufactorization2[i][j] * (*back)[leftunknown[j]];
        (*back)[leftunknown[i]] = sum / lufactorization2[i][i];
    }

    /*-------------------------------------------------------------------*/
    /*  Calculating unknown right hand parts                             */
    /*                                                                   */
    for (int i = 0; i < rightunknownsize; i++)
    {
        double sum = 0.0;

        for (int j = 0; j < leftunknownsize; j++)
            sum += this->value[rightunknown[i]][leftunknown[j]] * (*back)[leftunknown[j]];
        for (int j = 0; j < rightunknownsize; j++)
            sum += this->value[rightunknown[i]][rightunknown[j]] * (*left->value)[rightunknown[j]];

        (*back)[rightunknown[i]] = sum;
    }
    return (back);
}
/*======================================================================*/

void LaSquareMatrix::cleanSmallNumbers(int base)
{
    if (base < 1)
        throw TmcException("LaMatrix.cleanSmallNumbers");

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->columns; j++)
        {
            if (std::fabs(this->value[i][j]) < std::pow(10.0, -base))
                this->value[i][j] = 0.0;
        }
    }
}

string LaSquareMatrix::toString()
{
    stringstream ss;
    ss << "LaMatrix[";
    ss << this->name << "]" << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            ss << " " << value[i][j];
        ss << endl;
    }
    ss << endl;
    return ((ss.str()).c_str());
}
/**
  Returns a formatted String-representation of the matrix
  @param format the format string
  @return the String-representation of the matrix
*/
string LaSquareMatrix::toString(string format)
{
    return NULL;
}

/**
  Reads a square matrix.
  @param in stream to read from
  @exception IOException if an IO-error occurs
  @exception TiInvalidObjectException if the object could not be converted
*/
void LaSquareMatrix::read(TmcFileInput *in)
{
    if (in == NULL)
        throw TmcException(this->name + ".readFromFile(): null input-stream");
    try
    {
        this->name = in->readString();
        int dimension = in->readInteger();

        this->value = new double *[dimension];
        for (int i = 0; i < dimension; i++)
            this->value[i] = new double[dimension];
        for (int i = 0; i < dimension; i++)
            for (int j = 0; j < dimension; j++)
                this->value[i][j] = 0.0;
        this->Init(dimension);

        in->readLine();
        for (int i = 0; i < this->rows; i++)
            for (int j = 0; j < this->columns; j++)
                value[i][j] = in->readDouble();
    }
    catch (...)
    {
        throw TmcException(this->name + ".writeToFile(): error while writing");
    }
}

void LaSquareMatrix::write(TmcFileOutput *out)
{
    try
    {
        out->writeString(this->name);
        out->writeInteger(this->getDimension());
        out->writeLine();
        stringstream ss;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                ss << " " << value[i][j];
            ss << endl;
        }
        ss << endl;
        out->writeString(ss.str());
    }
    catch (...)
    {
        throw TmcException(this->name + ".writeToFile(): error while writing");
    }
}
