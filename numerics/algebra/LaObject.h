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
 Original Copyright (C) 2000 Frank Schley (BTU Cottbus, Germany, java bauinformatik package)
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
     linear algebra object

\*---------------------------------------------------------------------------*/

#ifndef LAOBJECT_H
#define LAOBJECT_H

#include <string>
#include <sstream>
#include <iostream>

#include <common/utilities/TmcException.h>
#include <TmcMacroFile.h>

class TMC_DLL_EXPORT LaObject
{
public:
    std::string name;
    LaObject()
    {
        this->name = "LaObject";
    }
    LaObject(LaObject *object)
    {
        this->name = object->name;
    }
    LaObject(std::string name)
    {
        this->name = name;
    }
    virtual ~LaObject()
    {
    }

    virtual std::string toString() = 0;
    void cleanSmallNumbers() { this->cleanSmallNumbers(10); };
    virtual void cleanSmallNumbers(int base) = 0;
    std::string getName() { return (this->name); };
    void setName(std::string name) { this->name = name; };
};
#endif
