#include "calibrationwindow.h"
#include "ui_calibrationwindow.h"

CalibrationWindow::CalibrationWindow(QWidget *parent, std::vector<ColorFilter> * inFilters, QString filterPath) :
    QDialog(parent),
    ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);

    filters = inFilters;
    ocvf = new OcvFrame(filters);
    loadFilters(filterPath);
    //frame = new QGraphicsScene(0,0,321,581,this);
    frame.setSceneRect(0,0,300,533);
    frame.setItemIndexMethod(QGraphicsScene::NoIndex);
    frame.addItem(ocvf);

    ui->graphicsView->setScene(&frame);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::black,Qt::SolidPattern));
    ui->graphicsView->show();
    timer;
    QObject::connect(&timer, SIGNAL(timeout()), &frame, SLOT(advance()));
    timer.start(1000 / 24);

}

CalibrationWindow::CalibrationWindow(QWidget *parent, QString filterPath, QString videoPath, bool still) :
    QDialog(parent),
    ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);

    loadFilters(filterPath);
    //frame = new QGraphicsScene(0,0,321,581,this);
    frame.setSceneRect(0,0,300,533);
    frame.setItemIndexMethod(QGraphicsScene::NoIndex);
    ocvf = new OcvFrame(filters);
    frame.addItem(ocvf);

    ui->graphicsView->setScene(&frame);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::black,Qt::SolidPattern));
    ui->graphicsView->show();
    timer;
    QObject::connect(&timer, SIGNAL(timeout()), &frame, SLOT(advance()));
    timer.start(1000 / 24);

}

CalibrationWindow::CalibrationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);

    frame.setSceneRect(-160,-290,160,290);
    frame.setItemIndexMethod(QGraphicsScene::NoIndex);
    OcvFrame *ocvf = new OcvFrame(filters);
    frame.addItem(ocvf);

    ui->graphicsView->setScene(&frame);

    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &frame, SLOT(advance()));
    timer.start(1000 / 24);
}

CalibrationWindow::~CalibrationWindow()
{
    delete ui;
}

void CalibrationWindow::loadFilters(QString filePath){
    ui->FiltersList->clear();
    filters->clear();
    qDebug() << "List & Vector Cleared";
    QFile file(filePath);
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
    int eG,dG;
    bool endFilters = false;
    int readLine = 0;
    while(!in.atEnd() && !endFilters) {
        QString line = in.readLine();
        qDebug() << readLine << ": "<< line;
        if(!line.startsWith("#")){
            QStringList fields = line.split(":");
            qDebug() << "Field: "<< fields.at(0);
            if(!onItem){
                if(QString::compare(fields.at(0),"NAME",Qt::CaseInsensitive) == 0){
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
                    eG = cv::MORPH_CROSS;
                    dG = cv::MORPH_CROSS;
                    qDebug() << "FILTER STARTED";
                }
            }
            else if(QString::compare(fields.at(0),"NAME",Qt::CaseInsensitive) == 0){
                QMessageBox::information(0, "Format Error in File", file.errorString());
            }
            else{
                if(QString::compare(fields.at(0),"HUE",Qt::CaseInsensitive) == 0){
                    QStringList interval = fields.at(1).split(",");
                    minH = interval.at(0).toInt();
                    maxH = interval.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"SAT",Qt::CaseInsensitive) == 0){
                    QStringList interval = fields.at(1).split(",");
                    minS = interval.at(0).toInt();
                    maxS = interval.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"VAL",Qt::CaseInsensitive) == 0){
                    QStringList interval = fields.at(1).split(",");
                    minV = interval.at(0).toInt();
                    maxV = interval.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ES",Qt::CaseInsensitive) == 0){
                    eS = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ER",Qt::CaseInsensitive) == 0){
                    eR = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ED",Qt::CaseInsensitive) == 0){
                    eD = (QString::compare(fields.at(1),"TRUE")==0) ? true:false;
                    qDebug() << eD;
                }
                if(QString::compare(fields.at(0),"DS",Qt::CaseInsensitive) == 0){
                    dS = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"DR",Qt::CaseInsensitive) == 0){
                    dR = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"DD",Qt::CaseInsensitive) == 0){
                    dD = (QString::compare(fields.at(1),"TRUE")==0) ? true:false;
                    qDebug() << eD;
                }
                if(QString::compare(fields.at(0),"EG",Qt::CaseInsensitive) == 0){
                    eG = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"DG",Qt::CaseInsensitive) == 0){
                    dG = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ENDFILTER",Qt::CaseInsensitive) == 0){
                    ColorFilter cf(fName,minH,maxH,minS,maxS,minV,maxV,eS,eR,eD,dS,dR,dD,eG,dG);
                    _filters.push_back(cf);
                    onItem = false;
                }
                if(QString::compare(fields.at(0),"END",Qt::CaseInsensitive) == 0){
                    endFilters = true;
                }
            }
        }

        readLine++;
    }
    file.close();
    //filters = std::vector<ColorFilter>(_filters.begin(),_filters.end());

    // TODO: Setup Interface
    // Update List
    for(int i = 0; i < _filters.size(); i++){
        filters->push_back(_filters[i]);
        ui->FiltersList->addItem(_filters[i].get_name());
    }
    ui->FiltersList->setCurrentRow(0);
    // Update Controls
    //loadCurrentFilter(filters[0]);
}

void CalibrationWindow::loadCurrentFilter(ColorFilter filter){
    // Update Sliders
    ui->minVS_H->setValue(filter.get_minVals()[0]);
    ui->minVS_S->setValue(filter.get_minVals()[1]);
    ui->minVS_V->setValue(filter.get_minVals()[2]);

    ui->maxVS_H->setValue(filter.get_maxVals()[0]);
    ui->maxVS_S->setValue(filter.get_maxVals()[1]);
    ui->maxVS_V->setValue(filter.get_maxVals()[2]);

    ui->minSB_H->setValue(filter.get_minVals()[0]);
    ui->minSB_S->setValue(filter.get_minVals()[1]);
    ui->minSB_V->setValue(filter.get_minVals()[2]);

    ui->maxSB_H->setValue(filter.get_maxVals()[0]);
    ui->maxSB_S->setValue(filter.get_maxVals()[1]);
    ui->maxSB_V->setValue(filter.get_maxVals()[2]);
    // Update Erode & Dilate
    if((filter.get_erode()[0]-filter.get_erode()[filter.get_erode().size()-1])!= 0)   ui->DecreaseCHB_E->setChecked(true);
    else    ui->DecreaseCHB_E->setChecked(false);
    if((filter.get_dilate()[0]-filter.get_dilate()[filter.get_dilate().size()-1])!= 0) ui->DecreaseCHB_D->setChecked(true);
    else    ui->DecreaseCHB_D->setChecked(false);

    ui->SizeSB_E->setValue(filter.get_erode()[0]);
    ui->SizeSB_D->setValue(filter.get_dilate()[0]);
    ui->RepsSB_E->setValue(filter.get_erode().size());
    ui->RepsSB_D->setValue(filter.get_dilate().size());

    qDebug() << "----Eroding Sizes----";
    qDebug() << "Iterations: " << filter.get_erode().size();
    for(int i = 0; i < filter.get_erode().size(); i++)
        qDebug() << filter.get_erode()[i];

    ui->GeometryCB_E->setCurrentIndex(filter.get_erode_geometry());
    ui->GeometryCB_D->setCurrentIndex(filter.get_dilate_geometry());
}

void CalibrationWindow::on_FiltersList_currentRowChanged(int currentRow)
{
    qDebug() << "Row Changed to:" << currentRow;
    if(currentRow < 0){
        ocvf->activateFilter(false);
        ocvf->setActiveFilter(-1);
        ui->ClearButton->setEnabled(false);
        ui->DeleteButton->setEnabled(false);
        ui->minSB_H->setEnabled(false);
        ui->minSB_S->setEnabled(false);
        ui->minSB_V->setEnabled(false);
        ui->maxSB_H->setEnabled(false);
        ui->maxSB_S->setEnabled(false);
        ui->maxSB_V->setEnabled(false);
        ui->minVS_H->setEnabled(false);
        ui->minVS_S->setEnabled(false);
        ui->minVS_V->setEnabled(false);
        ui->maxVS_H->setEnabled(false);
        ui->maxVS_S->setEnabled(false);
        ui->maxVS_V->setEnabled(false);
        ui->SizeSB_D->setEnabled(false);
        ui->SizeSB_E->setEnabled(false);
        ui->RepsSB_D->setEnabled(false);
        ui->RepsSB_E->setEnabled(false);
        ui->GeometryCB_D->setEnabled(false);
        ui->GeometryCB_E->setEnabled(false);
        ui->DecreaseCHB_D->setEnabled(false);
        ui->DecreaseCHB_E->setEnabled(false);
    }
    else{
        loadCurrentFilter(filters->at(currentRow));
        ocvf->activateFilter(ui->applyCB->isChecked());
        int filt = (ui->comboBox->currentIndex()==0) ? -1:currentRow;
        ocvf->setActiveFilter(filt);
        ui->ClearButton->setEnabled(true);
        ui->DeleteButton->setEnabled(true);
        ui->minSB_H->setEnabled(true);
        ui->minSB_S->setEnabled(true);
        ui->minSB_V->setEnabled(true);
        ui->maxSB_H->setEnabled(true);
        ui->maxSB_S->setEnabled(true);
        ui->maxSB_V->setEnabled(true);
        ui->minVS_H->setEnabled(true);
        ui->minVS_S->setEnabled(true);
        ui->minVS_V->setEnabled(true);
        ui->maxVS_H->setEnabled(true);
        ui->maxVS_S->setEnabled(true);
        ui->maxVS_V->setEnabled(true);
        ui->SizeSB_D->setEnabled(true);
        ui->SizeSB_E->setEnabled(true);
        ui->RepsSB_D->setEnabled(true);
        ui->RepsSB_E->setEnabled(true);
        ui->GeometryCB_D->setEnabled(true);
        ui->GeometryCB_E->setEnabled(true);
        ui->DecreaseCHB_D->setEnabled(true);
        ui->DecreaseCHB_E->setEnabled(true);
    }
}

void CalibrationWindow::on_minVS_H_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->minSB_H->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_minVals(value,ui->minVS_S->value(),ui->minVS_V->value());
}

void CalibrationWindow::on_minVS_S_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->minSB_S->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_minVals(ui->minVS_H->value(),value,ui->minVS_V->value());
}

void CalibrationWindow::on_minVS_V_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->minSB_V->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_minVals(ui->minVS_H->value(),ui->minVS_S->value(),value);
}

void CalibrationWindow::on_maxVS_H_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_H->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_maxVals(value,ui->maxVS_S->value(),ui->maxVS_V->value());
}

void CalibrationWindow::on_maxVS_S_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_S->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_maxVals(ui->maxVS_H->value(),value,ui->maxVS_V->value());
}

void CalibrationWindow::on_maxVS_V_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_V->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_maxVals(ui->maxVS_H->value(),ui->maxVS_S->value(),value);
}


void CalibrationWindow::on_minSB_H_valueChanged(int arg1)
{
    ui->minVS_H->setValue(arg1);
}

void CalibrationWindow::on_minSB_S_valueChanged(int arg1)
{
    ui->minVS_S->setValue(arg1);
}

void CalibrationWindow::on_minSB_V_valueChanged(int arg1)
{
    ui->minVS_V->setValue(arg1);
}

void CalibrationWindow::on_maxSB_H_valueChanged(int arg1)
{
    ui->maxVS_H->setValue(arg1);
}

void CalibrationWindow::on_maxSB_S_valueChanged(int arg1)
{
    ui->maxVS_S->setValue(arg1);
}

void CalibrationWindow::on_maxSB_V_valueChanged(int arg1)
{
    ui->maxVS_V->setValue(arg1);
}

void CalibrationWindow::on_LoadFileButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open Filter File"),"./Data/","All Files (*.*);;Text File (*.txt)");
    qDebug() << filepath;
    loadFilters(filepath);
}

std::vector<ColorFilter> *CalibrationWindow::getFilters(){
    return filters;
}

void CalibrationWindow::on_buttonBox_accepted()
{
    emit validate();
}



void CalibrationWindow::on_SizeSB_E_valueChanged(int arg1)
{
    filters->at(ui->FiltersList->currentRow()).recalc_erode(arg1,ui->RepsSB_E->value(),ui->DecreaseCHB_E->isChecked());
}

void CalibrationWindow::on_RepsSB_E_valueChanged(int arg1)
{
    filters->at(ui->FiltersList->currentRow()).recalc_erode(ui->SizeSB_E->value(),arg1,ui->DecreaseCHB_E->isChecked());
}



void CalibrationWindow::on_SizeSB_D_valueChanged(int arg1)
{
    filters->at(ui->FiltersList->currentRow()).recalc_dilate(arg1,ui->RepsSB_D->value(),ui->DecreaseCHB_D->isChecked());
}

void CalibrationWindow::on_RepsSB_D_valueChanged(int arg1)
{
    filters->at(ui->FiltersList->currentRow()).recalc_dilate(ui->SizeSB_D->value(),arg1,ui->DecreaseCHB_D->isChecked());
}

void CalibrationWindow::on_AddButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("New Filter"),
                                         tr("User name:"), QLineEdit::Normal,
                                         "Wakar", &ok);
    ColorFilter newFilter = ColorFilter(name,0,255,0,255,0,255,1,1,false,1,1,false,0,0);
    filters->push_back(newFilter);
    ui->FiltersList->addItem(name);
    if(ui->FiltersList->count() > 1) ui->DeleteButton->setEnabled(true);
    qDebug() << name;
}

void CalibrationWindow::on_DeleteButton_clicked()
{
    int erased = ui->FiltersList->currentRow();
    if(erased != 0)ui->FiltersList->setCurrentRow(erased-1);
    ui->FiltersList->takeItem(erased);
    filters->erase(filters->begin()+erased);
    if(ui->FiltersList->count() == 1) ui->DeleteButton->setEnabled(false);
}

void CalibrationWindow::on_SaveButton_clicked()
{
    QString Pfile = QFileDialog::getSaveFileName(this,tr("Save Filters File"),"./Data",tr("Text Files (*.txt)"));
    if (Pfile.compare("",Qt::CaseInsensitive) == 0) return;
    qDebug() << Pfile;

    QDateTime dt = QDateTime::currentDateTime();
    QFile file(Pfile);
    if(file.open(QIODevice::ReadWrite)){
        QTextStream stream(&file);
        stream << "#" <<dt.toString() << "\n";
        for(int i=0; i<filters->size();i++){
            stream << "NAME:"   << filters->at(i).get_name() << "\n";
            stream << "HUE:"    << filters->at(i).get_minVals()[0] << "," << filters->at(i).get_maxVals()[0] <<"\n";
            stream << "SAT:"    << filters->at(i).get_minVals()[1] << "," << filters->at(i).get_maxVals()[1] <<"\n";
            stream << "VAL:"    << filters->at(i).get_minVals()[2] << "," << filters->at(i).get_maxVals()[2] <<"\n";
            stream << "ES:"     << filters->at(i).get_erode()[0] << "\n";
            stream << "ER:"     << filters->at(i).get_erode().size() << "\n";
            stream << "ED:"     << (((filters->at(i).get_erode()[0]-filters->at(i).get_erode()[filters->at(i).get_erode().size()])!=0)?QString::fromUtf8("TRUE"):QString::fromUtf8("FALSE"));
            stream << "\n";
            stream << "EG:"     << filters->at(i).get_erode_geometry() << "\n";
            stream << "DS:"     << filters->at(i).get_dilate()[0] << "\n";
            stream << "DR:"     << filters->at(i).get_dilate().size() << "\n";
            stream << "DD:"     << (((filters->at(i).get_dilate()[0]-filters->at(i).get_dilate()[filters->at(i).get_dilate().size()])!=0)?QString::fromUtf8("TRUE"):QString::fromUtf8("FALSE"));
            stream <<"\n";
            stream << "DG:"     << filters->at(i).get_dilate_geometry() << "\n";
            stream <<  "ENDFILTER" << "\n";
        }
        stream <<  "END" << "\n";

    }
    file.close();
}

void CalibrationWindow::on_ClearButton_clicked()
{
    ui->FiltersList->clear();
    filters->clear();
}

void CalibrationWindow::onExit(){
    ocvf->closeStream();
    delete ocvf;
}

void CalibrationWindow::on_applyCB_stateChanged(int arg1)
{
    ocvf->activateFilter(arg1);
}

void CalibrationWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index>0) ocvf->setActiveFilter(ui->FiltersList->currentRow());
    else ocvf->setActiveFilter(-1);
}
