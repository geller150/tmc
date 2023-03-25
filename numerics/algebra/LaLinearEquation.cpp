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
     equation system

\*---------------------------------------------------------------------------*/

#include "./LaLinearEquation.h"

using namespace std;

/**
  Creates a linear equation with no matrix or vector.
*/
LaLinearEquation::LaLinearEquation() : LaObject("LaLinearEquation")
{
    this->init();
}
/**
  Creates a linear equation with the specified matrix and right hand vector.
  @param matrix the square matrix <B>A</B> of the linear equation
  @param vector the right hand vector <B>b</B> of the linear equation
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaLinearEquation::LaLinearEquation(LaSquareMatrix *matrix, LaVector *vektor) : LaObject("LaLinearEquation")
{
    this->init();
    int n = matrix->getRowNumber();
    int m = (int)vektor->value->size();

    if (n != m)
        throw TmcException("LaLE: incompatible vector and matrix sizes");

    this->matrix = matrix;
    this->vektor = vektor;
}
/**
  Creates a linear equation with no matrix or vector and with the specified name.
  @param name name of the matrix
*/
LaLinearEquation::LaLinearEquation(string name) : LaObject(name)
{
    this->init();
}
/**
  Creates a linear equation with the specified matrix and right hand vector and with the specified name.
  @param matrix the square matrix <B>A</B> of the linear equation
  @param vector the right hand vector <B>b</B> of the linear equation
  @param name name of the matrix
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaLinearEquation::LaLinearEquation(LaSquareMatrix *matrix, LaVector *vektor, string name) : LaObject(name)
{
    this->init();
    int n = matrix->getRowNumber();
    int m = (int)vektor->value->size();
    if (n != m)
        throw new string("LaLinearEquation: incompatible vector and matrix sizes");

    this->matrix = matrix;
    this->vektor = vektor;
}
/**
  Creates a linear equation with the specified matrix, left hand vector and right hand vector.
  Assumes a fully true index vector (equal to fully unknown left hand vector).
  @param matrix the square matrix <B>A</B> of the linear equation
  @param left  the right hand vector <B>x</B> of the linear equation
  @param right the right hand vector <B>b</B> of the linear equation
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaLinearEquation::LaLinearEquation(LaSquareMatrix *matrix, LaVector *left, LaVector *right) : LaObject("LaLinearEquation")
{
    this->init();
    int n = matrix->getRowNumber();
    int m = (int)left->value->size();
    int o = (int)right->value->size();
    if (n != m || n != o)
        throw new string("LaLE : incompatible vector and matrix sizes");

    this->matrix = matrix;
    this->left = left;
    this->vektor = right;
    this->index = new vector<bool>;
    this->index->resize(n, true);
}
/**
  Creates a linear equation with the specified matrix, left hand vector, right hand vector and with the specified name.
  Assumes a fully true index vector (equal to fully unknown left hand vector).
  @param matrix the square matrix <B>A</B> of the linear equation
  @param left  the right hand vector <B>x</B> of the linear equation
  @param right the right hand vector <B>b</B> of the linear equation
  @param name name of the matrix
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaLinearEquation::LaLinearEquation(LaSquareMatrix *matrix, LaVector *left, LaVector *right, string name) : LaObject(name)
{
    this->init();
    int n = matrix->getRowNumber();
    int m = (int)left->value->size();
    int o = (int)right->value->size();

    if (n != m || n != o)
        throw new string("LaLE : incompatible vector and matrix sizes");

    this->matrix = matrix;
    this->left = left;
    this->vektor = right;
    this->index = new vector<bool>;
    this->index->resize(n, true);
}
/**
  Creates a linear equation with the specified matrix, left hand vector and right hand vector.
  @param matrix the square matrix <B>A</B> of the linear equation
  @param left  the right hand vector <B>x</B> of the linear equation
  @param right the right hand vector <B>b</B> of the linear equation
  @param index the index vector of the linear equation
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaLinearEquation::LaLinearEquation(LaSquareMatrix *matrix, LaVector *left, LaVector *right, vector<bool> *index) : LaObject("LaLinearEquation")
{
    this->init();
    int n = matrix->getRowNumber();
    int m = (int)left->value->size();
    int o = (int)right->value->size();
    int p = (int)index->size();
    if (n != m || n != o || n != p)
        throw new string("LaLE : incompatible vector and matrix sizes");

    this->matrix = matrix;
    this->left = left;
    this->vektor = right;
    this->index = index;
}
/**
  Creates a linear equation with the specified matrix, left hand vector, right hand vector and with the specified name.
  @param matrix the square matrix <B>A</B> of the linear equation
  @param left  the right hand vector <B>x</B> of the linear equation
  @param right the right hand vector <B>b</B> of the linear equation
  @param index the index vector of the linear equation, a true entry marks an unknown left-hand side entry
  @param name name of the matrix
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaLinearEquation::LaLinearEquation(LaSquareMatrix *matrix, LaVector *left, LaVector *right, vector<bool> *index, string name) : LaObject(name)
{
    this->init();
    int n = matrix->getRowNumber();
    int m = (int)left->value->size();
    int o = (int)right->value->size();
    int p = (int)index->size();
    if (n != m || n != o || n != p)
        throw new string("LaLE : incompatible vector and matrix sizes");

    this->matrix = matrix;
    this->left = left;
    this->vektor = right;
    this->index = index;
}
/**
  Creates a linear equation as a copy of another equation.
*/
LaLinearEquation::LaLinearEquation(LaLinearEquation *eqn) : LaObject("LaLinearEquation")
{
    this->init();
    if (eqn->matrix != NULL)
        this->matrix = new LaSquareMatrix(eqn->matrix);
    if (eqn->vektor != NULL)
        this->vektor = new LaVector(eqn->vektor);
    if (eqn->left != NULL)
        this->left = new LaVector(eqn->left);
    if (eqn->index != NULL)
    {
        this->index = new vector<bool>;
        this->index->resize((int)eqn->index->size(), true);
        for (int i = 0; i < (int)eqn->index->size(); i++)
            (*this->index)[i] = (*eqn->index)[i];
    }
}
/*======================================================================*/
void LaLinearEquation::init()
{
    this->matrix = NULL;
    this->vektor = NULL;
    this->left = NULL;
    this->index = NULL;
    this->solved = false;
    this->solved2 = false;
}

/**
  Returns a simple String-representation of the linear equation
  Includes an outputstring for the matrix and the vector
  @return the String-representation of the linear equation
*/
string LaLinearEquation::toString()
{
    return 0;
}
/**
  Returns a formatted  String-representation of the linear equation
  Includes an outputstring for the matrix and the vector
  @param format the format string
  @return the String-representation of the linear equation
*/
string LaLinearEquation::toString(string format)
{
    return 0;
}

/**
  Sets all values less than 1e-<I>base</I> to 0.0!
  @param base the base (must be positive)
  @exception IllegalArgumentException if base is less than 1
*/
void LaLinearEquation::cleanSmallNumbers(int base)
{
    if (this->matrix != NULL)
        this->matrix->cleanSmallNumbers(base);
    if (this->left != NULL)
        this->left->cleanSmallNumbers(base);
    if (this->vektor != NULL)
        this->vektor->cleanSmallNumbers(base);
}

/**
  Sets the square matrix <B>A</B> of the linear equation
  @param matrix the square matrix of the linear equation
  @exception ArrayIndexOutOfBoundsException if matrix size is incompatible
*/
void LaLinearEquation::setMatrix(LaSquareMatrix *matrix)
{
    if (this->vektor != NULL)
    {
        int n = matrix->getRowNumber();
        int m = (int)this->vektor->value->size();
        if (n != m)
            throw new string("LaLE.setMatrix(): incompatible matrix size");
    }
    this->matrix = matrix;
    // super.notifyObservers();
}
/**
  Sets the right hand vector <B>b</B> of the linear equation
  @param vector the right hand vector of the linear equation
  @exception ArrayIndexOutOfBoundsException if vector size is incompatible
*/
void LaLinearEquation::setVector(LaVector *vektor)
{
    if (this->matrix != NULL)
    {
        int n = matrix->getRowNumber();
        int m = (int)this->vektor->value->size();
        if (n != m)
            throw new string("LaLE.setVector(): incompatible vector size");
    }
    this->vektor = vektor;
}
/**
  Sets the left hand vector <B>x</B> of the linear equation
  @param vector the left hand vector of the linear equation
  @exception ArrayIndexOutOfBoundsException if vector size is incompatible
*/
void LaLinearEquation::setLeftHandVector(LaVector *vektor)
{
    if (this->matrix != NULL)
    {
        int n = matrix->getRowNumber();
        int m = (int)this->vektor->value->size();
        if (n != m)
            throw new string("LaLE.setLeftHandVector(): incompatible vector size");
    }
    this->left = vektor;
}
/**
  Sets the index vector of the linear equation
  @param index the index vector, a true entry marks an unknown left-hand side entry
  @exception ArrayIndexOutOfBoundsException if vector size is incompatible
*/
void LaLinearEquation::setIndexVector(vector<bool> *index)
{
    if (this->matrix != NULL)
    {
        int n = matrix->getRowNumber();
        int m = (int)index->size();
        if (n != m)
            throw new string("LaLE.setIndexVector(): incompatible vector size");
    }
    this->index = index;
}
/**
  Returns the index vector of the linear equation
  @return index vector of the linear equation
*/
vector<bool> *LaLinearEquation::getIndexVector() { return (this->index); }
/**
  Returns the left hand vector <B>x</B> of the linear equation
  @return left hand vector of the linear equation
*/
LaVector *LaLinearEquation::getLeftHandVector() { return (this->left); }
/**
  Returns the right hand vector <B>b</B> of the linear equation
  @return right hand vector of the linear equation
*/
LaVector *LaLinearEquation::getVector() { return (this->vektor); }
/**
  Returns the square matrix <B>A</B> of the linear equation
  @return square matrix of the linear equation
*/
LaSquareMatrix *LaLinearEquation::getMatrix() { return (this->matrix); }

/**
  Solves the linear equation <B>Ax=b</B> using the LU-factorization.
  @return the result-vector
  @exception ArithmetiTmcException is thrown, if matrix is singular (or nearly singular in case of solve behaviour set to true)
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaVector *LaLinearEquation::solve()
{
    int n = matrix->getRowNumber();
    int m = (int)this->vektor->value->size();
    if (n != m)
        throw TmcException("LaLE.solve(): incompatible matrix and vector dimensions");

    try
    {
        return (this->matrix->solveLinearEquation(this->vektor));
    }
    catch (string &s)
    {
        cout << s << endl;
        throw TmcException(__FILE__, __LINE__, "LaLE.solve(): ");
    }
    catch (string *s)
    {
        cout << *s << endl;
        throw TmcException(__FILE__, __LINE__, "LaLE.solve(): ");
    }
    catch (...)
    {
        throw TmcException(__FILE__, __LINE__, "LaLE.solve(): ");
    }
}

/**
  Solves the linear equation <B>Ax=b</B> in one single step (unknown values in <B>x</B> and <B>b</B>!!) using the LU-factorization.
  <BR><BR>
  Separates the linear equation (using the index vector of this linear equation system) in
  <NOBR>
    <B>A<SUB>11</SUB>x<SUB>u</SUB> = b<SUB>k</SUB> - A<SUB>12</SUB>x<SUB>k</SUB></B> and
    <B>b<SUB>u</SUB> = A<SUB>21</SUB>x<SUB>u</SUB> + A<SUB>22</SUB>x<SUB>k</SUB></B>,<BR>
  </NOBR>
  where index u is unknown and k is known.
  <BR><BR>
  The return vector contains both unknown left AND right hand values in order. If right hand values No 2 and 5 are unknown,
  right hand results are found at index 2 and 5, left hand results are found at the other indexes.
  @return the result-vector (containing both unknown left AND right hand values!)
  @exception ArithmetiTmcException if matrix is singular (or nearly singular in case of solve behaviour set to true)
  @exception ArrayIndexOutOfBoundsException if matrix and vector sizes are inconsistent
*/
LaVector *LaLinearEquation::solveSeparated()
{
    int n = this->matrix->getRowNumber();
    int m = (int)this->vektor->value->size();
    int o = (int)this->left->value->size();
    int p = (int)this->index->size();

    if (n != m)
        throw TmcException("LaLE.solveSeparated(): incompatible matrix and left hand vector dimensions");
    if (n != o)
        throw TmcException("LaLE.solveSeparated(): incompatible matrix and right hand vector dimensions");
    if (n != p)
        throw TmcException("LaLE.solveSeparated(): incompatible matrix and index vector dimensions");

    try
    {
        return (this->matrix->solveLinearEquation(this->left, this->vektor, this->index));
    }
    catch (...)
    {
        throw TmcException("LaLE.solveSeparated(): ");
    }
}
