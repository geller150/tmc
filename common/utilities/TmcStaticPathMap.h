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
 Original Copyright (C) 2007-2011 Soeren Textor (VirtualFluids, TU Braunschweig, Germany)
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
    static path map
    stores pathnames for pathIDs (e.g. on different processes different paths with same pathID)
    adding an path autom. changes "\" to "/" and removed last "/" if exists

\*---------------------------------------------------------------------------*/

#ifndef TMCSTATICPATHMAP_H
#define TMCSTATICPATHMAP_H

#include <iostream>
#include <string>
#include <map>

#include "TmcSystem.h"

class TmcStaticPathMap
{
    typedef std::map<std::string, std::string> PathMap;

public:
    static const std::string GLOBAL;

public:
    static std::string addAndMakePath(const std::string &id, const std::string &path)
    {
        std::string tmpPath = TmcStaticPathMap::addPath(id, path);
        if (!tmpPath.empty())
        {
            if (!TmcSystem::makeDirectory(tmpPath, 20))
            {
                throw TmcException(UB_EXARGS, "could not create " + tmpPath);
            }
        }
        return tmpPath;
    }
    static std::string addPath(const std::string &id, const std::string &path)
    {
        std::string tmpPath = TmcSystem::replaceInString(path, "\\", "/");
        if (tmpPath.rfind("/") == tmpPath.size() - 1)
            tmpPath.resize(tmpPath.size() - 1);
        pathMap[id] = tmpPath;
        return tmpPath;
    }
    static std::string getPath(const std::string &id)
    {
        PathMap::iterator it = pathMap.find(id);
        if (it == pathMap.end())
            return "";
        return it->second;
    }
    static void removePath(const std::string &id)
    {
        pathMap.erase(id);
    }

protected:
    static PathMap pathMap;

private:
    TmcStaticPathMap() {}
    TmcStaticPathMap(const TmcStaticPathMap &) {}
};

#endif // TMCSTATICPATHMAP_H
