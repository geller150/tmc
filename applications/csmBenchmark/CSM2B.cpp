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
     CSM benchmark test case 2

\*---------------------------------------------------------------------------*/

#include <applications/csmBenchmark/CSMTestCases.h>

#include <common/utilities/TmcFileOutputASCII.h>
#include <numerics/structuralsolver/beam/TmcBeam.h>
#include <numerics/structuralsolver/TmcInitialValueSolver.h>
#include <numerics/algebra/LaVector.h>
#include <numerics/algebra/LaSquareMatrix.h>

/*=====================================================================*/
void CSMTestCases::createCSM2BBenchmark()
{
    string pathname = "c:/temp/outputFSW/";
    TmcFileOutputASCII *logfile = createFoldersAndGetLogfile(pathname);

    logfile->writeLine("CSM-2B-Benchmark for FE-Code !!!");

    double dTstructure = 0.0;

    int knotenanzahl = 26;
    int elementanzahl = knotenanzahl - 1;
    int degreeOfFreedom = 2 * knotenanzahl;
    double length = 0.6 - 0.24898; // 0.35//

    double rhoS = 1000.0;
    double hBalken = 0.02;
    double E = 5600000.;
    double I = 0.0000006666666667;
    E = E / (1.0 - 0.4 * 0.4);
    double m = hBalken * rhoS; // rho*b*h
    double d = 0.0;
    double elementlength = length / elementanzahl;

    // load vector
    LaVector lastvector(degreeOfFreedom, "LastVektor");
    double qLast = -2.0 * m; // 40.;//-0.5;// -1000000.;
    cout << "degreeOfFreedom:" << degreeOfFreedom << endl;
    for (int i = 0; i < elementanzahl; i++)
    {
        double x = ((double)i + 0.5) / (double)elementanzahl;
        double p = qLast;
        double Q = 0.5 * p * elementlength;
        double M = 1. / 12. * p * elementlength * elementlength;
        lastvector.addValue(i * 2, Q);
        lastvector.addValue(i * 2 + 1, -M);
        lastvector.addValue(i * 2 + 2, Q);
        lastvector.addValue(i * 2 + 3, M);
    }
    // cout<<lastvector->toString()<<endl;
    lastvector.setValue(0, 0.0);
    lastvector.setValue(1, 0.0);

    TmcBeam *beam = new TmcBeam(knotenanzahl, E, I, length, m, d);
    beam->setLinienlast(qLast);

    TmcInitialValueSolver solver(knotenanzahl * 2, beam->getKMatrix(), beam->getMMatrix(), beam->getDMatrix(), dTstructure);
    solver.setFixedIndex(0);
    solver.setFixedIndex(1);
    std::vector<double *> result = solver.getCalculatedStartSolution(&lastvector);
    for (int u = 0; u < degreeOfFreedom; u += 2)
    {
        cout << "u[" << u << "]:" << result[0][u] << endl;
    }
}
/*=====================================================*/
