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
     vector

\*---------------------------------------------------------------------------*/

#include "./LaVector.h"

#include <common/utilities/TmcFileInput.h>
#include <common/utilities/TmcFileOutput.h>

using namespace std;

/*=========================================================================*/
LaVector::LaVector() : LaObject("LaVector")
{
    this->value = NULL;
}
/*=========================================================================*/
LaVector::LaVector(int dimension) : LaObject("LaVector")
{
    this->value = new vector<double>;
    this->value->resize(dimension, 0.0);
}
/*=========================================================================*/
LaVector::LaVector(vector<double> *array) : LaObject("LaVector")
{
    this->value = new vector<double>;
    this->value->resize((int)array->size(), 0.0);
    for (int u = 0; u < (int)this->value->size(); u++)
    {
        (*this->value)[u] = (*array)[u];
    }
}
/*=========================================================================*/
LaVector::LaVector(double *array, int size) : LaObject("LaVector")
{
    this->value = new vector<double>;
    this->value->resize(size, 0.0);
    for (int u = 0; u < size; u++)
    {
        (*this->value)[u] = array[u];
    }
}
/*=========================================================================*/
LaVector::LaVector(LaVector *vektor) : LaObject(vektor->name)
{
    this->value = new vector<double>;
    this->value->resize((int)vektor->value->size(), 0.0);
    for (int u = 0; u < (int)this->value->size(); u++)
    {
        (*this->value)[u] = (*vektor->value)[u];
    }
}
/*=========================================================================*/
LaVector::LaVector(string name) : LaObject(name)
{
}
/*=========================================================================*/
LaVector::LaVector(int dimension, string name) : LaObject(name)
{
    this->value = new vector<double>;
    this->value->resize(dimension, 0.0);
}
/*=========================================================================*/
LaVector::~LaVector()
{
    delete this->value;
}
/*=========================================================================*/

/*======================================================================*/
/**
  Returns a simple String-representation of the vector
  @return the String-representation of the vector
*/
string LaVector::toString()
{
    stringstream ss;
    ss << "LaVector[";
    ss << this->name << "]" << endl;
    for (int i = 0; i < (int)value->size(); i++)
        ss << " " << (*value)[i];
    ss << endl;
    return ((ss.str()).c_str());
}

/**
     Sets all values less than 1e-<I>base</I> to 0.0!
     @param base the base (must be positive)
     @exception IllegalArgumentException if base is less than 1
   */
void LaVector::cleanSmallNumbers(int base)
{
    throw TmcException("not yet implemented");
}
int LaVector::getDimension() { return ((int)this->value->size()); }

void LaVector::setValue(int row, double a)
{
    (*this->value)[row] = a;
}
void LaVector::setValues(double a)
{
    for (int i = 0; i < (int)this->value->size(); i++)
        (*this->value)[i] *= a;
}

/**
     Sets the values of this vector from the specified double array.
     @param values the values to set this vector with
     @exception ArrayIndexOutOfBoundsException if the specified array does not match
   */
double LaVector::getValue(int row)
{
    return ((*this->value)[row]);
}
vector<double> *LaVector::getValues()
{
    vector<double> *values = new vector<double>;
    values->resize((int)this->value->size(), 0.0);
    for (int u = 0; u < (int)this->value->size(); u++)
    {
        (*values)[u] = (*this->value)[u];
    }
    return (values);
}
void LaVector::addValue(int row, double a)
{
    (*this->value)[row] += a;
}
void LaVector::subtractValue(int row, double a)
{
    (*this->value)[row] -= a;
}
void LaVector::multiplyValue(int row, double a)
{
    (*this->value)[row] *= a;
}
void LaVector::divideByValue(int row, double a)
{
    (*this->value)[row] /= a;
}
void LaVector::addValue(double a)
{
    for (int i = 0; i < (int)this->value->size(); i++)
        (*this->value)[i] += a;
}
void LaVector::subtractValue(double a)
{
    for (int i = 0; i < (int)this->value->size(); i++)
        (*this->value)[i] -= a;
}
void LaVector::multiplyValue(double a)
{
    for (int i = 0; i < (int)this->value->size(); i++)
        (*this->value)[i] *= a;
}
void LaVector::divideByValue(double a)
{
    for (int i = 0; i < (int)this->value->size(); i++)
        (*this->value)[i] /= a;
};
/*======================================================================*/

/*======================================================================*/
/*  calculation methods                                                 */
/*                                                                      */
/**
  Multiply each element of the vector with a scalar
  @param scalar the scalar to multiply with
  @return the result-vector
*/
// LaVector* multiply(LaScalar *scalar)
//  {
//     LaVector *vector = new LaVector(this->value);
// for(int i=0; i<(int)vector->value->size(); i++) (*vector->value)[i] *= scalar->value;
//    return(vector);
// };
/**
  Divide each element of the vector by a scalar
  @param scalar the scalar to divide by
  @return the result-vector
*/
// LaVector* divide(LaScalar *scalar)
// {
//    LaVector *vector = new LaVector(this->value);
//    for(int i=0; i<(int)vector->value->size(); i++) (*vector->value)[i] /= scalar->value;
//    return(vector);
// };
/**
  Add two vectors
  @param vector the vector to add
  @return the result-vector
  @exception ArrayIndexOutOfBoundsException if vector sizes are incompatible
*/
LaVector *LaVector::add(LaVector *vector)
{
    int n = (int)this->value->size();
    int m = (int)vector->value->size();
    if (n != m)
        throw new string("LaVector.add"); //(this.name+".add("+vector.name+"): incompatible sizes");

    //		LaVector *back = new LaVector(this->value);
    //      for(int i=0; i<(int)back->value->size(); i++) (*back->value)[i] += (*vector->value)[i];
    //      return(back);
    return NULL;
}
/**
  Subtract two vectors
  @param vector the vector to subtract
  @return the result-vector
  @exception ArrayIndexOutOfBoundsException if vector sizes are incompatible
*/
LaVector *LaVector::subtract(LaVector *vector)
{
    int n = (int)this->value->size();
    int m = (int)vector->value->size();
    if (n != m)
        throw new string("LaVector.substract");

    // LaVector *back = new LaVector(this->value);
    //		for(int i=0; i<(int)back->value->size(); i++) (*back->value)[i] -= (*vector->value)[i];
    //      return(back);
    return NULL;
}

/**
  Calculates the norm of the vecor as root of the sclarproduct with itself.
  @return the norm
*/
// public LaScalar norm()
//{
//    return(new LaScalar(Math.sqrt((this.scalarProduct(this)).value)));
// }

/**
  Calculates the scalar product of the vector with another vector.
  @param vector vector to multiply with
  @return the result-scalar
  @exception ArrayIndexOutOfBoundsException if vector sizes are incompatible
*/
// public LaScalar scalarProduct(LaVector vector) throws ArrayIndexOutOfBoundsException
//{
//    int n = this.value.length;
//    int m = vector.value.length;
//    if(n != m) throw new ArrayIndexOutOfBoundsException(this.name+".scalarProduct("+vector.name+"): incompatible sizes");

//   LaScalar back = new LaScalar(0.0);
//   for(int i=0; i<n; i++) back.value += value[i]*vector.value[i];
//   return(back);
//}

/**
  Calculates the dyadic product of the vector with another vector.
  @param vector vector to multiply with
  @return the resulting square matrix
  @exception ArrayIndexOutOfBoundsException if vector sizes are incompatible
*/
// public LaSquareMatrix dyadicProduct(LaVector vector) throws ArrayIndexOutOfBoundsException
// {
//    int n = this.value.length;
//    int m = vector.value.length;
//    if(n != m) throw new ArrayIndexOutOfBoundsException(this.name+".dyadicProduct("+vector.name+"): incompatible sizes");

//    LaSquareMatrix back = new LaSquareMatrix(n);
//    for(int i=0; i<n; i++)
// for(int j=0; j<n; j++)
//   back.value[i][j] = value[i]*vector.value[j];

//    return(back);
// }

/**
  Calculates the vectorial product of the vector with another vector in 3D space.
  @param vector vector to multiply with
  @return the resulting vectorial product
  @exception ArrayIndexOutOfBoundsException if vector sizes are incompatible
  @exception ArrayIndexOutOfBoundsException if vectors are non-3D
*/
// public LaVector vectorialProduct3D(LaVector vector) throws ArrayIndexOutOfBoundsException
// {
//    int j,k;
//    int n = this.value.length;
//    int m = vector.value.length;
//    if(n != m) throw new ArrayIndexOutOfBoundsException(this.name+".vectorialProduct3D("+vector.name+"): incompatible sizes");

//    if(n != 3) throw new ArrayIndexOutOfBoundsException(this.name+".vectorialProduct3D("+vector.name+"): only valid in 3D space");

//    /* use index cycling */
//    LaVector back = new LaVector(n);
//    for(int i=0; i<n; i++)
//    {
// j=i+1;
// if (i==n-1) j=0;
// k=j+1;
// if (j==n-1) k=0;

// back.value[i] = this.value[j]*vector.value[k] - this.value[k]*vector.value[j];
//    }
//    return(back);
// }
/**
  Calculates the vectorial product of the vector with another vector in 3D space.
  @param vector vector to multiply with
  @return the resulting vectorial product
  @exception ArrayIndexOutOfBoundsException if vector sizes are incompatible
  @exception ArrayIndexOutOfBoundsException if vectors are non-3D
*/
// public LaComplexVector vectorialProduct3D(LaComplexVector vector) throws ArrayIndexOutOfBoundsException
// {
//    int j,k;
//    int n = this.value.length;
//    int m = vector.rvalue.length;
//    if(n != m) throw new ArrayIndexOutOfBoundsException(this.name+".vectorialProduct3D("+vector.name+"): incompatible sizes");

//    if(n != 3) throw new ArrayIndexOutOfBoundsException(this.name+".vectorialProduct3D("+vector.name+"): only valid in 3D space");

//    /* use index cycling */
//    LaComplexVector back = new LaComplexVector(n);
//    for(int i=0; i<n; i++)
//    {
// j=i+1;
// if (i==n-1) j=0;
// k=j+1;
// if (j==n-1) k=0;

// back.rvalue[i] = value[j]*vector.rvalue[k] - value[k]*vector.rvalue[j];
// back.ivalue[i] = value[j]*vector.ivalue[k] - value[k]*vector.ivalue[j];
//    }
//    back.preparePhaseValues();
//    return(back);
// }

/*======================================================================*/
/*  Datei-Ein/Ausgabe                                                   */
/*                                                                      */
void LaVector::read(TmcFileInput *in)
{
    if (in == NULL)
        throw TmcException(this->name + ".readFromFile(): null input-stream");
    try
    {
        this->name = in->readString();
        int dimension = in->readInteger();
        this->value = new vector<double>;
        this->value->resize(dimension, 0.0);

        in->readLine();
        for (int i = 0; i < (int)value->size(); i++)
            (*value)[i] = in->readDouble();
    }
    catch (...)
    {
        throw TmcException(this->name + ".writeToFile(): error while writing");
    }
}

void LaVector::write(TmcFileOutput *out)
{
    try
    {
        out->writeString(this->name);
        out->writeInteger(this->getDimension());
        out->writeLine();
        stringstream ss;
        for (int i = 0; i < (int)value->size(); i++)
            ss << " " << (*value)[i];
        ss << endl;
        out->writeString(ss.str());
    }
    catch (...)
    {
        throw TmcException(this->name + ".writeToFile(): error while writing");
    }
}

/*=========================================================================*/
