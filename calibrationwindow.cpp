#include "calibrationwindow.h"
#include "ui_calibrationwindow.h"

CalibrationWindow::CalibrationWindow(QWidget *parent, std::vector<ColorFilter> * inFilters, QString filterPath) :
    QDialog(parent),
    ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);

    filters = inFilters;
    ocvf = new OcvFrame(filters);
    load_filters(filterPath);
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

    load_filters(filterPath);
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

void CalibrationWindow::load_filters(QString filePath){
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
    int p1,p2,mD,minR,maxR,sT;
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
                    p1 = 100;
                    p2 = 20;
                    mD = 5;
                    minR = 2;
                    maxR = 20;
                    sT = 0;
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
                if(QString::compare(fields.at(0),"P1",Qt::CaseInsensitive) == 0){
                    p1 = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"P2",Qt::CaseInsensitive) == 0){
                    p2 = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"MD",Qt::CaseInsensitive) == 0){
                    mD = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"MIN",Qt::CaseInsensitive) == 0){
                    minR = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"MAX",Qt::CaseInsensitive) == 0){
                    maxR = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ST",Qt::CaseInsensitive) == 0){
                    sT = fields.at(1).toInt();
                }
                if(QString::compare(fields.at(0),"ENDFILTER",Qt::CaseInsensitive) == 0){
                    ColorFilter cf(fName,minH,maxH,minS,maxS,minV,maxV,eS,eR,eD,dS,dR,dD,eG,dG);
                    cf.set_canny_threshold(p1);
                    cf.set_centers_threshold(p2);
                    cf.set_min_center_distance(mD);
                    cf.set_min_radius(minR);
                    cf.set_max_radius(maxR);
                    if(sT == 1) cf.set_hough_active(true);
                    if(sT == 2) cf.set_edge_active(true);
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
    //load_current_filter(filters[0]);
}

void CalibrationWindow::load_current_filter(ColorFilter filter){
    // Update Sliders
    ui->minVS_H->setValue(filter.get_min_vals()[0]);
    ui->minVS_S->setValue(filter.get_min_vals()[1]);
    ui->minVS_V->setValue(filter.get_min_vals()[2]);

    ui->maxVS_H->setValue(filter.get_max_vals()[0]);
    ui->maxVS_S->setValue(filter.get_max_vals()[1]);
    ui->maxVS_V->setValue(filter.get_max_vals()[2]);

    ui->minSB_H->setValue(filter.get_min_vals()[0]);
    ui->minSB_S->setValue(filter.get_min_vals()[1]);
    ui->minSB_V->setValue(filter.get_min_vals()[2]);

    ui->maxSB_H->setValue(filter.get_max_vals()[0]);
    ui->maxSB_S->setValue(filter.get_max_vals()[1]);
    ui->maxSB_V->setValue(filter.get_max_vals()[2]);
    // Update Erode & Dilate
    if((filter.get_erode()[0]-filter.get_erode()[filter.get_erode().size()-1])!= 0)   ui->DecreaseCHB_E->setChecked(true);
    else    ui->DecreaseCHB_E->setChecked(false);
    if((filter.get_dilate()[0]-filter.get_dilate()[filter.get_dilate().size()-1])!= 0) ui->DecreaseCHB_D->setChecked(true);
    else    ui->DecreaseCHB_D->setChecked(false);

    ui->SizeSB_E->setValue(filter.get_erode()[0]);
    ui->SizeSB_D->setValue(filter.get_dilate()[0]);
    ui->RepsSB_E->setValue(filter.get_erode().size());
    ui->RepsSB_D->setValue(filter.get_dilate().size());

    ui->GeometryCB_E->setCurrentIndex(filter.get_erode_geometry());
    ui->GeometryCB_D->setCurrentIndex(filter.get_dilate_geometry());

    // Object Detection and tracking
    ui->cannySlider->setValue(filter.get_canny_threshold());
    ui->cannySB->setValue(filter.get_canny_threshold());
    ui->centersSlider->setValue(filter.get_centers_threshold());
    ui->centersSB->setValue(filter.get_centers_threshold());
    ui->minDistSlider->setValue(filter.get_min_center_distance());
    ui->minDistSB->setValue(filter.get_min_center_distance());
    ui->minRadSB->setValue(filter.get_min_radius());
    ui->maxRadSB->setValue(filter.get_max_radius());
    if(filter.get_edge_active()) ui->edgeRB->setChecked(true);
    else if (filter.get_hough_active()) ui->circlesRB->setChecked(true);
    else ui->disableRB->setChecked(true);
}

void CalibrationWindow::on_FiltersList_currentRowChanged(int currentRow)
{
    qDebug() << "Row Changed to:" << currentRow;
    if(currentRow < 0){
        ocvf->activate_filter(false);
        ocvf->set_active_filter(-1);
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
        load_current_filter(filters->at(currentRow));
        ocvf->activate_filter(ui->applyCB->isChecked());
        int filt = (ui->comboBox->currentIndex()==0) ? -1:currentRow;
        ocvf->set_active_filter(filt);
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
    filters->at(ui->FiltersList->currentRow()).set_min_vals(value,ui->minVS_S->value(),ui->minVS_V->value());
}

void CalibrationWindow::on_minVS_S_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->minSB_S->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_min_vals(ui->minVS_H->value(),value,ui->minVS_V->value());
}

void CalibrationWindow::on_minVS_V_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->minSB_V->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_min_vals(ui->minVS_H->value(),ui->minVS_S->value(),value);
}

void CalibrationWindow::on_maxVS_H_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_H->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_max_vals(value,ui->maxVS_S->value(),ui->maxVS_V->value());
}

void CalibrationWindow::on_maxVS_S_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_S->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_max_vals(ui->maxVS_H->value(),value,ui->maxVS_V->value());
}

void CalibrationWindow::on_maxVS_V_valueChanged(int value)
{
    // Change Value in SpinBox
    ui->maxSB_V->setValue(value);
    // Change Value in filter
    filters->at(ui->FiltersList->currentRow()).set_max_vals(ui->maxVS_H->value(),ui->maxVS_S->value(),value);
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
    load_filters(filepath);
}

std::vector<ColorFilter> *CalibrationWindow::get_filters(){
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
            stream << "HUE:"    << filters->at(i).get_min_vals()[0] << "," << filters->at(i).get_max_vals()[0] <<"\n";
            stream << "SAT:"    << filters->at(i).get_min_vals()[1] << "," << filters->at(i).get_max_vals()[1] <<"\n";
            stream << "VAL:"    << filters->at(i).get_min_vals()[2] << "," << filters->at(i).get_max_vals()[2] <<"\n";
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
            stream << "P1:"      << filters->at(i).get_canny_threshold() << "\n";
            stream << "P2:"      << filters->at(i).get_centers_threshold() << "\n";
            stream << "MD:"      << filters->at(i).get_min_center_distance() << "\n";
            stream << "MIN:"     << filters->at(i).get_min_radius() << "\n";
            stream << "MAX:"     << filters->at(i).get_max_radius() << "\n";
            stream << "ST:"      << ((filters->at(i).get_hough_active()) ? 1 : ((filters->at(i).get_edge_active()) ? 2 : 0));
            stream <<"\n";
            stream << "ENDFILTER" << "\n";
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
    ocvf->close_stream();
    delete ocvf;
}


void CalibrationWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index>0) ocvf->set_active_filter(ui->FiltersList->currentRow());
    else ocvf->set_active_filter(-1);
}

void CalibrationWindow::on_DecreaseCHB_D_toggled(bool checked)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->recalc_dilate(ui->SizeSB_D->value(),ui->RepsSB_D->value(),checked);
}

void CalibrationWindow::on_DecreaseCHB_E_toggled(bool checked)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->recalc_erode(ui->SizeSB_E->value(),ui->RepsSB_E->value(),checked);
}

void CalibrationWindow::on_GeometryCB_E_currentIndexChanged(int index)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_erode_geometry(index);
}

void CalibrationWindow::on_GeometryCB_D_currentIndexChanged(int index)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_dilate_geometry(index);
}

void CalibrationWindow::on_applyCB_toggled(bool checked)
{
    ocvf->activate_filter(checked);
    ui->comboBox->setEnabled(checked);
}

void CalibrationWindow::on_cannySlider_valueChanged(int value)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_canny_threshold(value);
    ui->cannySB->setValue(value);
}

void CalibrationWindow::on_centersSlider_valueChanged(int value)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_centers_threshold(value);
    ui->centersSB->setValue(value);
}

void CalibrationWindow::on_minDistSlider_valueChanged(int value)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_min_center_distance(value);
    ui->minDistSB->setValue(value);
}

void CalibrationWindow::on_cannySB_valueChanged(int arg1)
{
    ui->cannySlider->setValue(arg1);
}

void CalibrationWindow::on_centersSB_valueChanged(int arg1)
{
    ui->centersSlider->setValue(arg1);
}

void CalibrationWindow::on_minDistSB_valueChanged(int arg1)
{
    ui->minDistSlider->setValue(arg1);
}

void CalibrationWindow::on_minRadSB_valueChanged(int arg1)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_min_radius(arg1);
}

void CalibrationWindow::on_maxRadSB_valueChanged(int arg1)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_max_radius(arg1);
}

void CalibrationWindow::on_edgeRB_toggled(bool checked)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_edge_active(checked);
}

void CalibrationWindow::on_circlesRB_toggled(bool checked)
{
    ColorFilter *cf = &filters->at(ui->FiltersList->currentRow());
    cf->set_hough_active(checked);
}

void CalibrationWindow::on_disableRB_toggled(bool checked)
{
    return;
}

void CalibrationWindow::on_playButton_clicked()
{
    ocvf->play_video(true);
}

void CalibrationWindow::on_pauseButton_clicked()
{
    ocvf->play_video(false);
}

void CalibrationWindow::on_next_frameButton_clicked()
{
    ocvf->advance_frame(true);
}
