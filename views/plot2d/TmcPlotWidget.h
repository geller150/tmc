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
 Original Copyright (C) 2013-2014 Paul Edwards
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
    plot widget

\*---------------------------------------------------------------------------*/

#ifndef TmcPlotWidget_H
#define TmcPlotWidget_H

#include <vector>
#include <tuple>

#include <QDockWidget>
#include <QScopedPointer>

#include <TmcMacroFile.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingContextOpenGL2)

class vtkTable;
struct TmcPlotWidgetPrivate;

class TMC_DLL_EXPORT TmcPlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TmcPlotWidget(QWidget *parent = 0);
    virtual ~TmcPlotWidget();

    void updateData(std::vector<std::tuple<double, double, double, double, double>>& data);
    void addPluginWidget(QWidget* widget);

    void writeCsv(const QString &filename);
    void refresh();

public slots:

    void on_viewLogScaleAction_toggled(bool b);

    void on_fileSaveAction_triggered();
    void on_fileExitAction_triggered();

protected slots:
    void update();
    void updateAndResize();

    void on_pushButtonResize_clicked();

    void updateTable();

private:
    Q_DISABLE_COPY(TmcPlotWidget)

    QScopedPointer<TmcPlotWidgetPrivate> d_ptr;

};

#endif //
