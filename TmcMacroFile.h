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
     macro file to use shared libs on windows

\*---------------------------------------------------------------------------*/
#ifndef TmcMacroFile
#define TmcMacroFile

//======================================================
// Windows-specific definitions
//======================================================

#if (defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__) || defined(__MINGW64__)) && !defined(WNT)
#define WNT
#endif

#if defined(_WIN32) && !defined(HAVE_NO_DLL)

#ifndef TMC_DLL_EXPORT
#define TMC_DLL_EXPORT __declspec( dllexport )
#endif  /* Standard_EXPORT */

#else  /* WNT */

//======================================================
// UNIX definitions
//======================================================

#ifndef TMC_DLL_EXPORT
#define TMC_DLL_EXPORT
#endif  /* Standard_EXPORT */

#endif  /* WNT */

//======================================================
// Type definitions
//======================================================
typedef int    iType;
typedef double  rType;

#endif // TmcMacroFile
