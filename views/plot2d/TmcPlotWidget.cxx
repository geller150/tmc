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

#include "TmcPlotWidget.h"

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSplitter>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>

#include <vtkTable.h>
#include <vtkNew.h>
#include <vtkContextView.h>
#include <vtkChartXY.h>
#include <vtkChartLegend.h>
#include <vtkPlot.h>
#include <vtkRenderer.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxis.h>
#include <vtkPNGWriter.h>
#include <vtkImageData.h>
#include <vtkDelimitedTextReader.h>
#include <vtkDelimitedTextWriter.h>
#include <vtkDoubleArray.h>
#include <vtkWindowToImageFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <QVTKRenderWidget.h>
#include <vtkMergeTables.h>
#include <vtkColorSeries.h>
#include <vtkNamedColors.h>

#include <cmath>
#include <cstring>
#include <sstream>
#include <iostream>
#include <tuple>

struct TmcPlotWidgetPrivate
{
    TmcPlotWidgetPrivate() : model(0), disableUpdate(false) {}

    QGridLayout *gridLayout;
    QVTKRenderWidget *qvtkWidget;

    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonShowLogScale;
    QPushButton *pushButtonResize;
    QLabel *label;
    QComboBox *comboBox;
    int axisXComboBoxIndex;
    QLabel *label_2;

    QTreeView *treeView;

    vtkNew<vtkContextView> view;
    vtkNew<vtkChartXY> chart;
    std::vector<vtkSmartPointer<vtkTable>> tables;
    std::vector<vtkSmartPointer<vtkTable>> memberTables;

    QStandardItemModel *model;
    bool isLogScale;
    bool disableUpdate;
    bool keepZoom;
    vtkSmartPointer<vtkColorSeries> plotColors;

    QIcon iconLog;
    QIcon iconLin;
};

TmcPlotWidget::TmcPlotWidget(QWidget *parent) : QWidget(parent),
                                                d_ptr(new TmcPlotWidgetPrivate)
{
    QSurfaceFormat::setDefaultFormat(QVTKRenderWidget::defaultFormat());
    d_ptr->plotColors = vtkSmartPointer<vtkColorSeries>::New();
    d_ptr->plotColors->SetColorSchemeByName("VTKBlueColors");
    vtkSmartPointer<vtkNamedColors> nc = vtkSmartPointer<vtkNamedColors>::New();
    if (true)
    {
        // white theme colors
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#b8860b"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#9400D3"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#556B2f"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#b03060"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#0000ff"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#556B2f"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#ff4500"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#191970"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#cd5555"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#191970"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#698b69"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#8b864e"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#cd9B9B"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#00008b"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#ee2c2c"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#b8860b"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#2f4f4f"));
        d_ptr->plotColors->AddColor(nc->HTMLColorToRGB("#4A708b"));
    }

    this->setWindowTitle("Plot");

    d_ptr->gridLayout = new QGridLayout();
    d_ptr->gridLayout->setObjectName(QStringLiteral("gridLayout"));
    d_ptr->qvtkWidget = new QVTKRenderWidget(this);
    d_ptr->qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));

    d_ptr->dockWidgetContents = new QWidget(this);
    d_ptr->dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
    d_ptr->dockWidgetContents->setWindowTitle(QApplication::translate("TmcPlotWidget", "Options", Q_NULLPTR));
    d_ptr->dockWidgetContents->setMinimumWidth(50);
    d_ptr->verticalLayout = new QVBoxLayout(d_ptr->dockWidgetContents);
    d_ptr->verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    d_ptr->horizontalLayout = new QHBoxLayout();
    d_ptr->horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

    d_ptr->pushButtonShowLogScale = new QPushButton(d_ptr->dockWidgetContents);
    d_ptr->pushButtonShowLogScale->setObjectName(QStringLiteral("pushButtonShowLogScale"));
    d_ptr->pushButtonShowLogScale->setCheckable(true);
    d_ptr->pushButtonShowLogScale->setChecked(false);
    d_ptr->pushButtonShowLogScale->setText(QString("log(y)"));
    d_ptr->pushButtonShowLogScale->setToolTip(QApplication::translate("TmcPlotWidget", "Switch between normal and log axis view", Q_NULLPTR));

    d_ptr->horizontalLayout->addWidget(d_ptr->pushButtonShowLogScale);

    d_ptr->pushButtonResize = new QPushButton(d_ptr->dockWidgetContents);
    d_ptr->pushButtonResize->setText(QApplication::translate("TmcPlotWidget", "Resize", Q_NULLPTR));
    d_ptr->pushButtonResize->setToolTip(QApplication::translate("TmcPlotWidget", "Press \"r\" key inside the plot view to resize", Q_NULLPTR));

    d_ptr->horizontalLayout->addWidget(d_ptr->pushButtonResize);

    d_ptr->verticalLayout->addLayout(d_ptr->horizontalLayout);

    d_ptr->label = new QLabel(d_ptr->dockWidgetContents);
    d_ptr->label->setObjectName(QStringLiteral("label"));
    d_ptr->label->setText(QApplication::translate("TmcPlotWidget", "X-Axis", Q_NULLPTR));

    d_ptr->verticalLayout->addWidget(d_ptr->label);

    d_ptr->comboBox = new QComboBox(d_ptr->dockWidgetContents);
    d_ptr->comboBox->setObjectName(QStringLiteral("comboBox"));

    d_ptr->verticalLayout->addWidget(d_ptr->comboBox);

    d_ptr->label_2 = new QLabel(d_ptr->dockWidgetContents);
    d_ptr->label_2->setObjectName(QStringLiteral("label_2"));
    d_ptr->label_2->setText(QApplication::translate("TmcPlotWidget", "Y-Axis", Q_NULLPTR));

    d_ptr->verticalLayout->addWidget(d_ptr->label_2);

    d_ptr->treeView = new QTreeView(d_ptr->dockWidgetContents);
    d_ptr->treeView->setObjectName(QStringLiteral("treeView"));
    d_ptr->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    d_ptr->treeView->setRootIsDecorated(false);
    d_ptr->treeView->setHeaderHidden(true);

    d_ptr->verticalLayout->addWidget(d_ptr->treeView);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(d_ptr->dockWidgetContents);
    splitter->addWidget(d_ptr->qvtkWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 10);
    d_ptr->gridLayout->addWidget(splitter);

    this->setLayout(d_ptr->gridLayout);

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    d_ptr->qvtkWidget->setRenderWindow(renWin);

    d_ptr->view->SetRenderWindow(d_ptr->qvtkWidget->renderWindow());
    d_ptr->view->SetInteractor(d_ptr->qvtkWidget->interactor());

    d_ptr->qvtkWidget->setRenderWindow(d_ptr->view->GetRenderWindow());

    d_ptr->view->GetScene()->AddItem(d_ptr->chart.GetPointer());
    d_ptr->chart->SetShowLegend(true);
    d_ptr->chart->GetAxis(0)->SetTitle("");
    d_ptr->chart->GetAxis(0)->SetLogScale(false);
    d_ptr->isLogScale = false;
    d_ptr->keepZoom = false;

    d_ptr->view->GetRenderer()->SetBackground(0.8, 0.8, 0.8);

    QObject::connect(d_ptr->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
    QObject::connect(d_ptr->pushButtonResize, SIGNAL(clicked()), this, SLOT(on_pushButtonResize_clicked()));
    QObject::connect(d_ptr->pushButtonShowLogScale, SIGNAL(toggled(bool)), this, SLOT(on_viewLogScaleAction_toggled(bool)));
}

TmcPlotWidget::~TmcPlotWidget()
{
}

/*===========================================================*/
void TmcPlotWidget::addPluginWidget(QWidget *widget)
{
    d_ptr->verticalLayout->addWidget(widget);
}
/*===========================================================*/
void TmcPlotWidget::refresh()
{
    d_ptr->view->GetRenderWindow()->GetInteractor()->Render();
}
/*===========================================================*/

void TmcPlotWidget::writeCsv(const QString &filename)
{
    for (int t = 0; t < (int)d_ptr->tables.size(); ++t)
    {
        vtkSmartPointer<vtkTable> table = d_ptr->tables[t];
        vtkSmartPointer<vtkDelimitedTextWriter> writer = vtkSmartPointer<vtkDelimitedTextWriter>::New();
        std::string filenameStr = filename.toStdString() + "_" + std::to_string(t) + ".csv";
        writer->SetFileName(filenameStr.c_str());
        writer->SetInputData(table.GetPointer());
        writer->Write();
    }
}

void TmcPlotWidget::updateTable()
{
    if (d_ptr->isLogScale)
    {
        for (int t = 0; t < (int)d_ptr->tables.size(); ++t)
        {
            vtkSmartPointer<vtkTable> table = d_ptr->tables[t];
            vtkSmartPointer<vtkTable> memberTable = d_ptr->memberTables[t];
            memberTable->DeepCopy(table);

            const std::size_t nr = table->GetNumberOfRows();
            const std::size_t nc = table->GetNumberOfColumns();
            for (std::size_t r = 0; r < nr; ++r)
            {
                for (std::size_t c = 0; c < nc; ++c)
                {
                    double val = table->GetValue(r, c).ToDouble();
                    if (val < 1e-17)
                    {
                        table->SetValue(r, c, nan(""));
                    }
                }
            }
        }
    }
    else
    {
        for (int t = 0; t < (int)d_ptr->tables.size(); ++t)
        {
            vtkSmartPointer<vtkTable> table = d_ptr->tables[t];
            vtkSmartPointer<vtkTable> memberTable = d_ptr->memberTables[t];

            table->DeepCopy(memberTable);
        }
    }
    // need the max here as the combobox returns -1 if no items are present
    int lastXAxis = std::max(0, d_ptr->comboBox->currentIndex());

    QSet<int> lastChecked;
    if (d_ptr->model)
    {
        int nr = d_ptr->model->rowCount();
        for (int r = 0; r < nr; ++r)
        {
            if (d_ptr->model->item(r)->checkState() == Qt::Checked)
            {
                lastChecked.insert(r);
            }
        }
    }
    if (lastChecked.isEmpty())
    {
        lastChecked.insert(1);
    }
    d_ptr->disableUpdate = true;

    d_ptr->comboBox->clear();

    if (d_ptr->model != NULL)
        disconnect(d_ptr->model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(update()));
    d_ptr->model = new QStandardItemModel(this);
    connect(d_ptr->model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(update()));
    QStandardItem *parentItem = d_ptr->model->invisibleRootItem();
    int count = 0;

    for (int t = 0; t < (int)d_ptr->tables.size(); ++t)
    {
        vtkSmartPointer<vtkTable> table = d_ptr->tables[t];
        const int nc = table->GetNumberOfColumns();
        for (int c = 0; c < nc; ++c)
        {
            QString colName = table->GetColumnName(c);
            d_ptr->comboBox->addItem(colName);
            QStandardItem *item = new QStandardItem(colName);
            item->setCheckable(true);
            if (lastChecked.contains(count))
            {
                item->setCheckState(Qt::Checked);
            }
            count++;
            parentItem->appendRow(item);
        }
    }
    d_ptr->treeView->setModel(d_ptr->model);

    if (lastXAxis < d_ptr->comboBox->count())
    {
        d_ptr->comboBox->setCurrentIndex(lastXAxis);
    }

    d_ptr->disableUpdate = false;

    this->update();
}

// can be applied if enabling/disabling a field
void TmcPlotWidget::updateAndResize()
{
    this->update();
    this->on_pushButtonResize_clicked();
}

void TmcPlotWidget::update()
{
    if (d_ptr->disableUpdate)
    {
        return;
    }
    double axisOneRange[2];
    double axisTwoRange[2];
    d_ptr->chart->GetAxis(0)->GetRange(axisOneRange);
    d_ptr->chart->GetAxis(1)->GetRange(axisTwoRange);
    if (d_ptr->keepZoom == true)
    {
        d_ptr->chart->GetAxis(0)->SetBehavior(vtkAxis::FIXED);
        d_ptr->chart->GetAxis(1)->SetBehavior(vtkAxis::FIXED);
    }
    else
    {
        d_ptr->chart->GetAxis(0)->SetBehavior(vtkAxis::AUTO);
        d_ptr->chart->GetAxis(1)->SetBehavior(vtkAxis::AUTO);
    }
    d_ptr->chart->ClearPlots();

    int xAxis = d_ptr->comboBox->currentIndex();

    bool xAxisChanged = false;
    if (d_ptr->axisXComboBoxIndex != xAxis)
        xAxisChanged = true;
    d_ptr->axisXComboBoxIndex = xAxis;

    d_ptr->chart->GetAxis(1)->SetTitle(d_ptr->comboBox->currentText().toStdString().c_str());

    int count = 0;

    xAxis = 0;
    for (int t = 0; t < (int)d_ptr->tables.size(); t++)
    {
        vtkSmartPointer<vtkTable> table = d_ptr->tables[t];
        int nr = table->GetNumberOfColumns();
        for (int yAxis = 0; yAxis < nr; ++yAxis)
        {
            QStandardItem *item = d_ptr->model->item(count);
            vtkColor3ub color = d_ptr->plotColors->GetColorRepeating(count);
            count++;

            if (yAxis > 0 && item->checkState() == Qt::Checked)
            {
                vtkPlot *line = d_ptr->chart->AddPlot(vtkChart::LINE);
                line->SetColorF(color.GetRed() / 255., color.GetGreen() / 255., color.GetBlue() / 255.);
                line->SetInputData(table.GetPointer(), xAxis, yAxis);
            }
        }
    }

    if (xAxisChanged)
    {
        on_pushButtonResize_clicked();
    }

    if (d_ptr->keepZoom == true)
    {
        int xAxisIndex = d_ptr->comboBox->currentIndex();
        vtkVariant newRangeX; // = d_ptr->table->GetValue(d_ptr->table->GetNumberOfRows()-1,xAxisIndex);
        bool found = false;

        for (int t = 0; t < (int)d_ptr->tables.size(); t++)
        {
            vtkSmartPointer<vtkTable> table = d_ptr->tables[t];
            int nr = table->GetNumberOfColumns();
            for (int yAxis = 0; yAxis < nr; ++yAxis)
            {
                if (count == xAxisIndex)
                {
                    newRangeX = table->GetValue(table->GetNumberOfRows() - 1, yAxis);
                    found = true;
                    break;
                }
                count++;
            }
            if (found)
                break;
        }
        if (found)
        {
            axisTwoRange[1] = newRangeX.ToDouble();
            d_ptr->chart->GetAxis(1)->SetRange(axisTwoRange);
        }
        d_ptr->chart->GetAxis(0)->SetRange(axisOneRange);
        d_ptr->chart->GetAxis(0)->AutoScale();
        d_ptr->chart->GetAxis(1)->AutoScale();
        d_ptr->chart->Modified();
    }
    else
    {
        d_ptr->chart->Modified();
    }
}

void TmcPlotWidget::updateData(std::vector<std::tuple<double, double, double, double, double>> &data)
{
    d_ptr->disableUpdate = true;

    vtkSmartPointer<vtkTable> newTable = vtkSmartPointer<vtkTable>::New();
    vtkSmartPointer<vtkDoubleArray> colTime = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkDoubleArray> colU = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkDoubleArray> colV = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkDoubleArray> colA = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkDoubleArray> colJ = vtkSmartPointer<vtkDoubleArray>::New();
    colTime->SetName("time");
    colU->SetName("Displacement");
    colV->SetName("Velocity");
    colA->SetName("Acceleration");
    colJ->SetName("Jerk");
    newTable->AddColumn(colTime);
    newTable->AddColumn(colU);
    newTable->AddColumn(colV);
    newTable->AddColumn(colA);
    newTable->AddColumn(colJ);

    for (int i = 0; i < data.size(); ++i)
    {
        colTime->InsertNextValue(std::get<0>(data[i]));
        colU->InsertNextValue(std::get<1>(data[i]));
        colV->InsertNextValue(std::get<2>(data[i]));
        colA->InsertNextValue(std::get<3>(data[i]));
        colJ->InsertNextValue(std::get<4>(data[i]));
    }

    QString prefix;

    if (d_ptr->tables.size() > 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Clean Plot", "Do you want to clean the plot before adding data?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            d_ptr->tables.clear();
        }
        else
        {
            bool ok;
            prefix = QString("Case");
            QInputDialog *inputDialog = new QInputDialog(this);
            inputDialog->setOptions(QInputDialog::NoButtons);
            QString text = inputDialog->getText(NULL, "Set Prefix", "prefix:", QLineEdit::Normal, prefix, &ok);

            if (ok && !text.isEmpty())
            {
                prefix = text;
            }
        }
    }

    // int setPrefix
    const int nc = newTable->GetNumberOfColumns();
    for (int c = 0; c < nc; ++c)
    {
        QString colName = newTable->GetColumnName(c);
        if (prefix.size() > 0)
        {
            colName.prepend(" ");
            colName.prepend(prefix);
        }
        newTable->GetColumn(c)->SetName(colName.toStdString().c_str());
    }

    vtkSmartPointer<vtkTable> newTable2 = vtkSmartPointer<vtkTable>::New();
    newTable2->DeepCopy(newTable);
    d_ptr->tables.push_back(newTable);
    d_ptr->memberTables.push_back(newTable2);

    this->updateTable();

    vtkChartLegend *legend = d_ptr->chart->GetLegend();
    legend->SetVisible(true);
    legend->SetHorizontalAlignment(vtkChartLegend::RIGHT);
    legend->SetVerticalAlignment(vtkChartLegend::TOP);
    this->refresh();
}

void TmcPlotWidget::on_fileSaveAction_triggered()
{
    QString currentDirectory;
    QString filename = QFileDialog::getSaveFileName(this, tr("Save plot"), currentDirectory, "*.png");
    if (!filename.isEmpty())
    {
        if (!filename.endsWith(".png"))
        {
            filename += ".png";
        }

        vtkSmartPointer<vtkRenderWindow> renderWindow = d_ptr->qvtkWidget->renderWindow();

        vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        windowToImageFilter->SetInput(renderWindow);
        windowToImageFilter->Modified();

        vtkNew<vtkPNGWriter> writer;
        writer->SetInputConnection(windowToImageFilter->GetOutputPort());
        writer->SetFileName(filename.toStdString().c_str());
        writer->Write();
    }
}

void TmcPlotWidget::on_fileExitAction_triggered()
{
    this->close();
}

void TmcPlotWidget::on_pushButtonResize_clicked()
{
    d_ptr->chart->GetAxis(0)->SetBehavior(vtkAxis::AUTO);
    d_ptr->chart->GetAxis(1)->SetBehavior(vtkAxis::AUTO);

    d_ptr->chart->RecalculateBounds();
    d_ptr->chart->Modified();
}

void TmcPlotWidget::on_viewLogScaleAction_toggled(bool b)
{
    if (this->d_ptr->tables.size() < 1)
        return;

    d_ptr->keepZoom = false;
    d_ptr->isLogScale = b;
    d_ptr->chart->GetAxis(0)->SetLogScale(b);
    this->updateTable();
    d_ptr->keepZoom = true;

    if (b)
        d_ptr->pushButtonShowLogScale->setIcon(d_ptr->iconLin);
    else
        d_ptr->pushButtonShowLogScale->setIcon(d_ptr->iconLog);

    this->on_pushButtonResize_clicked();
    d_ptr->view->GetInteractor()->Render();

    QObject::disconnect(d_ptr->pushButtonShowLogScale, SIGNAL(toggled(bool)), this, SLOT(on_viewLogScaleAction_toggled(bool)));
    d_ptr->pushButtonShowLogScale->setChecked(b);
    QObject::connect(d_ptr->pushButtonShowLogScale, SIGNAL(toggled(bool)), this, SLOT(on_viewLogScaleAction_toggled(bool)));
}
