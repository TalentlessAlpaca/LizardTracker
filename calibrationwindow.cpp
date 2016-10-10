#include "calibrationwindow.h"
#include "ui_calibrationwindow.h"

CalibrationWindow::CalibrationWindow(QWidget *parent, QString filePath) :
    QDialog(parent),
    ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);
}

CalibrationWindow::CalibrationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);
}

CalibrationWindow::~CalibrationWindow()
{
    delete ui;
}

void CalibrationWindow::loadFilters(QString filePath){
    QFile file("/home/hamad/lesson11.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error Opening File", file.errorString());
    }

    QTextStream in(&file);

    bool onItem = false;
    std::vector<ColorFilter> _filters;
    QString fName;
    int minH,minS,minV;
    int maxH,maxS,maxV;
    int eS,eR,dS,dR;
    bool eD,dD;
    bool endFilters = false;
    while(!in.atEnd() && !endFilters) {
        QString line = in.readLine();
        if(!line.startsWith("#")){
            QStringList fields = line.split(":");
            if(!onItem){
                if(QString::compare(fields.at(0),"NAME",Qt::CaseInsensitive)){
                    fName = fields.at(1);
                    onItem = true;
                    minH = 0;
                    minS = 0;
                    minV = 0;
                    maxH = 255;
                    maxS = 255;
                    maxV = 255;
                    eS = 1;
                    eR = 1;
                    dS = 1;
                    dR = 1;
                    eD = false;
                    dD = false;
                }
            }
            else if(QString::compare(fields.at(0),"NAME",Qt::CaseInsensitive))
                QMessageBox::information(0, "Format Error in File", file.errorString());
            else{
                if(QString::compare(fields.at(0),"HUE",Qt::CaseInsensitive)){
                    QStringList interval = fields.at(1).split(",");
                    minH = interval.at(0).toInt();
                    maxH = interval.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"SAT",Qt::CaseInsensitive)){
                    QStringList interval = fields.at(1).split(",");
                    minS = interval.at(0).toInt();
                    maxS = interval.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"VAL",Qt::CaseInsensitive)){
                    QStringList interval = fields.at(1).split(",");
                    minV = interval.at(0).toInt();
                    maxV = interval.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ES",Qt::CaseInsensitive)){
                    eS = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ER",Qt::CaseInsensitive)){
                    eR = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ED",Qt::CaseInsensitive)){
                    eD = QString::compare(fields.at(1),"TRUE");
                }
                if(QString::compare(fields.at(0),"DS",Qt::CaseInsensitive)){
                    dS = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"DR",Qt::CaseInsensitive)){
                    dR = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"DD",Qt::CaseInsensitive)){
                    dD = QString::compare(fields.at(1),"TRUE");
                }
                if(QString::compare(fields.at(0),"ENDFILTER",Qt::CaseInsensitive)){
                    ColorFilter cf(fName,minH,maxH,minS,maxS,minV,maxV,eS,eR,eD,dS,dR,dD);
                    _filters.push_back(cf);
                }
                if(QString::compare(fields.at(0),"END",Qt::CaseInsensitive)){
                    endFilters = true;
                }
            }
        }
    }
    file.close();
    filters = std::vector<ColorFilter>(_filters.begin(),_filters.end());
    // TODO: Setup Interface
    // Update List
    for(int i = 0; i < filters.size(); i++)
        ui->FiltersList->addItem(filters[i].get_name());
    ui->FiltersList->setCurrentRow(0);
    // Update Controls
    loadCurrentFilter(filters[0]);
}

void CalibrationWindow::loadCurrentFilter(ColorFilter filter){
    // Update Sliders
    ui->minVS_H->setValue(filter.get_minVals()[0]);
    ui->minVS_S->setValue(filter.get_minVals()[1]);
    ui->minVS_V->setValue(filter.get_minVals()[2]);
    ui->minSB_H->setValue(filter.get_minVals()[0]);
    ui->minSB_S->setValue(filter.get_minVals()[1]);
    ui->minSB_V->setValue(filter.get_minVals()[2]);

    ui->maxVS_H->setValue(filter.get_maxVals()[0]);
    ui->maxVS_S->setValue(filter.get_maxVals()[1]);
    ui->maxVS_V->setValue(filter.get_maxVals()[2]);
    ui->maxSB_H->setValue(filter.get_maxVals()[0]);
    ui->maxSB_S->setValue(filter.get_maxVals()[1]);
    ui->maxSB_V->setValue(filter.get_maxVals()[2]);
    // Update Erode & Dilate
    ui->SizeSB_E->setValue(filter.get_erode()[0]);
    ui->SizeSB_D->setValue(filter.get_dilate()[0]);
    ui->RepsSB_E->setValue(filter.get_erode().size());
    ui->RepsSB_D->setValue(filter.get_dilate().size());
    if((filter.get_erode()[0]-filter.get_erode()[1])!= 0)   ui->DecreaseCHB_E->setChecked(true);
    else                                                    ui->DecreaseCHB_E->setChecked(false);
    if((filter.get_dilate()[0]-filter.get_dilate()[1])!= 0) ui->DecreaseCHB_D->setChecked(true);
    else                                                    ui->DecreaseCHB_D->setChecked(false);
}

void CalibrationWindow::on_FiltersList_currentRowChanged(int currentRow)
{
    loadCurrentFilter(filters[0]);
}

void CalibrationWindow::on_minVS_H_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->minSB_H->setValue(value);
    // Change Value in filter
    filters[ui->FiltersList->currentRow()].get_minVals()[0];
}

void CalibrationWindow::on_minVS_S_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->minSB_S->setValue(value);
    // Change Value in filter
    filters[ui->FiltersList->currentRow()].get_minVals()[1];
}

void CalibrationWindow::on_minVS_V_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->minSB_V->setValue(value);
    // Change Value in filter
    filters[ui->FiltersList->currentRow()].get_minVals()[2];
}

void CalibrationWindow::on_maxVS_H_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_H->setValue(value);
    // Change Value in filter
    filters[ui->FiltersList->currentRow()].get_maxVals()[0];
}

void CalibrationWindow::on_maxVS_S_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_S->setValue(value);
    // Change Value in filter
    filters[ui->FiltersList->currentRow()].get_maxVals()[1];
}

void CalibrationWindow::on_maxVS_V_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_V->setValue(value);
    // Change Value in filter
    filters[ui->FiltersList->currentRow()].get_maxVals()[2];
}


void CalibrationWindow::on_LoadFileButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open Filter File"),"../Data/","All Files (*.*);;Text File (*.txt)");
    loadFilters(filepath);
}
