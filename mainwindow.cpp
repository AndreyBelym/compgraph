#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <cmath>
#include <QDebug>
#define MOVE MovM(new_x,new_y,new_z,1,1,1,1,l,m,n)
#define SCALE MovM(0,0,0,hreflect*scl_x,vreflect*scl_y,freflect*scl_z,scl_gen)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timer_timeout()));

    update=true;
    angle_OX=start_angle_OX=ui->spinBox_2->value();
    angle_OY=start_angle_OY=ui->spinBox_3->value();
    angle_OZ=start_angle_OZ=ui->spinBox_4->value();

    new_x=start_x=ui->doubleSpinBox->value();
    new_y=start_y=ui->doubleSpinBox_2->value();
    new_z=start_z=ui->doubleSpinBox_6->value();

    scl_x=start_scl_x=ui->doubleSpinBox_3->value();
    scl_y=start_scl_y=ui->doubleSpinBox_4->value();
    scl_z=start_scl_z=1;
    scl_gen=start_scl_gen=1/ui->doubleSpinBox_5->value();
    hreflect=ui->checkBox_5->isChecked()?-1:1;
    vreflect=ui->checkBox_6->isChecked()?-1:1;
    freflect=ui->checkBox_7->isChecked()?-1:1;

    if(ui->checkBox_4->isChecked()){
        timer.setInterval(ui->spinBox->value());
        timer.start();
    }

    l=0;//./ui->doubleSpinBox_10->value();
    m=0;//1./ui->doubleSpinBox_7->value();
    n=0;//-0.004;//1./ui->doubleSpinBox_9->value();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_dial_valueChanged(int value)
{
    QString t;
    ui->spinBox_2->setValue(value);
    start_angle_OX=value;
    if(!ui->checkBox_4->isChecked()||!ui->checkBox->isChecked()){
        angle_OX=value;
        Matrix *&RotOXMatrix=ui->graphicsView->RotOXMatrix;
        *RotOXMatrix=RotM(value*M_PI/180,OX);
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    if(arg1==Qt::Checked){
        timer.setInterval(ui->spinBox->value());
        timer.start();
    }else
        timer.stop();
}

void MainWindow::timer_timeout(){
    if(ui->checkBox->isChecked()){
        Matrix *RotMatrix=ui->graphicsView->RotOXMatrix;
        *RotMatrix=RotM(angle_OX*M_PI/180,OX);
        RotMatrix=ui->graphicsView->RotOYMatrix;
        *RotMatrix=RotM(angle_OY*M_PI/180,OY);
        RotMatrix=ui->graphicsView->RotOZMatrix;
        *RotMatrix=RotM(angle_OZ*M_PI/180,OZ);
    }
    if(ui->checkBox_2->isChecked()){
        Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
        *MoveMatrix=MovM(new_x,new_y,new_z,1,1,1,1,l,m,n);
    }
    if(ui->checkBox_3->isChecked()){
        Matrix *ScaleMatrix=ui->graphicsView->ScaleMatrix;
        *ScaleMatrix=SCALE;
    }
    ui->graphicsView->viewport()->repaint();
    if(ui->checkBox->isChecked()){
        angle_OX=(angle_OX+start_angle_OX)%360;
        angle_OY=(angle_OY+start_angle_OY)%360;
        angle_OZ=(angle_OZ+start_angle_OZ)%360;
    }
    if(ui->checkBox_2->isChecked()){
        new_x+=start_x;
        new_y+=start_y;
    }
    if(ui->checkBox_3->isChecked()){
        scl_x*=start_scl_x;
        scl_y*=start_scl_y;
        scl_gen*=start_scl_gen;
    }
}

void MainWindow::on_spinBox_2_valueChanged(int value)
{
    ui->dial->setValue(value);
}

void MainWindow::on_spinBox_valueChanged(int value)
{
    timer.setInterval(value);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    start_x=arg1;
    ui->horizontalSlider->setValue(arg1);
    if(!ui->checkBox_4->isChecked()||!ui->checkBox_2->isChecked()){
        new_x=arg1;
        Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
        *MoveMatrix=MOVE;
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    start_y=arg1;
    ui->horizontalSlider_2->setValue(arg1);
    if(!ui->checkBox_4->isChecked()||!ui->checkBox_2->isChecked()){
        new_y=arg1;
        Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
        *MoveMatrix=MOVE;
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    start_scl_x=arg1;
    update=false;
    ui->horizontalSlider_3->setValue(10*arg1);
    update=true;
    if(!ui->checkBox_4->isChecked()||!ui->checkBox_3->isChecked()){
        scl_x=arg1;
        Matrix *&ScaleMatrix=ui->graphicsView->ScaleMatrix;
        *ScaleMatrix=SCALE;
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
    start_scl_y=arg1;
    update=false;
    ui->horizontalSlider_4->setValue(10*arg1);
    update=true;
    if(!ui->checkBox_4->isChecked()||!ui->checkBox_3->isChecked()){
        scl_y=arg1;
        Matrix *&ScaleMatrix=ui->graphicsView->ScaleMatrix;
        *ScaleMatrix=SCALE;
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    start_scl_gen=1/arg1;
    update=false;
    ui->horizontalSlider_5->setValue(10*arg1);
    update=true;
    if(!ui->checkBox_4->isChecked()||!ui->checkBox_3->isChecked()){
        scl_gen=start_scl_gen;
        Matrix *&ScaleMatrix=ui->graphicsView->ScaleMatrix;
        *ScaleMatrix=SCALE;
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_pushButton_clicked()
{
    new_x=start_x;
    new_y=start_y;
    angle_OX=start_angle_OX;
    angle_OY=start_angle_OY;
    angle_OZ=start_angle_OZ;
    scl_x=start_scl_x;
    scl_y=start_scl_y;
    scl_gen=start_scl_gen;

    Matrix *RotMatrix=ui->graphicsView->RotOXMatrix;
    *RotMatrix=RotM(angle_OX*M_PI/180,OX);
    RotMatrix=ui->graphicsView->RotOYMatrix;
    *RotMatrix=RotM(angle_OY*M_PI/180,OY);
    RotMatrix=ui->graphicsView->RotOZMatrix;
    *RotMatrix=RotM(angle_OZ*M_PI/180,OZ);

    Matrix *&ScaleMatrix=ui->graphicsView->ScaleMatrix;
    *ScaleMatrix=MovM(0,0,0,hreflect*scl_x,vreflect*scl_y,scl_z,scl_gen);

    Matrix *MoveMatrix=ui->graphicsView->MoveMatrix;
    qDebug()<<n;
    *MoveMatrix=MovM(new_x,new_y,new_z,1,1,1,1,l,m,n);

    ui->graphicsView->viewport()->repaint();
}

void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    hreflect=ui->checkBox_5->isChecked()?-1.:1.;
    Matrix *&ScaleMatrix=ui->graphicsView->ScaleMatrix;
    *ScaleMatrix=SCALE;
    ui->graphicsView->viewport()->repaint();
}

void MainWindow::on_checkBox_6_stateChanged(int arg1)
{
    vreflect=ui->checkBox_6->isChecked()?-1.:1.;
    Matrix *&ScaleMatrix=ui->graphicsView->ScaleMatrix;
    *ScaleMatrix=SCALE;
    ui->graphicsView->viewport()->repaint();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->doubleSpinBox->setValue(value);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->doubleSpinBox_2->setValue(value);
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    if(update)
      ui->doubleSpinBox_3->setValue(value/10.);
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    if(update)
      ui->doubleSpinBox_4->setValue(value/10.);
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    if(update)
      ui->doubleSpinBox_5->setValue(value/10.);
}

void MainWindow::on_dial_2_valueChanged(int value)
{
    QString t;
    ui->spinBox_3->setValue(value);
    start_angle_OY=value;
    if(!ui->checkBox_4->isChecked()||!ui->checkBox->isChecked()){
        angle_OY=value;
        Matrix *&RotOYMatrix=ui->graphicsView->RotOYMatrix;
        *RotOYMatrix=RotM(value*M_PI/180,OY);
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_dial_3_valueChanged(int value)
{
    QString t;
    ui->spinBox_4->setValue(value);
    start_angle_OZ=value;
    if(!ui->checkBox_4->isChecked()||!ui->checkBox->isChecked()){
        angle_OZ=value;
        Matrix *&RotOZMatrix=ui->graphicsView->RotOZMatrix;
        *RotOZMatrix=RotM(value*M_PI/180,OZ);
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    ui->dial_2->setValue(arg1);
}

void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    ui->dial_3->setValue(arg1);
}

void MainWindow::on_doubleSpinBox_6_valueChanged(double arg1)
{
    start_z=arg1;
    ui->horizontalSlider_6->setValue(arg1);
    if(!ui->checkBox_4->isChecked()||!ui->checkBox_2->isChecked()){
        new_z=arg1;
        Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
        *MoveMatrix=MOVE;
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_doubleSpinBox_8_valueChanged(double arg1)
{
    start_scl_z=arg1;
    update=false;
    ui->horizontalSlider_7->setValue(10*arg1);
    update=true;
    if(!ui->checkBox_4->isChecked()||!ui->checkBox_3->isChecked()){
        scl_z=arg1;
        Matrix *&ScaleMatrix=ui->graphicsView->ScaleMatrix;
        *ScaleMatrix=SCALE;
        ui->graphicsView->viewport()->repaint();
    }
}

void MainWindow::on_horizontalSlider_7_valueChanged(int value)
{
    if(update)
      ui->doubleSpinBox_8->setValue(value/10.);
}

void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    ui->doubleSpinBox_6->setValue(value);

}

void MainWindow::on_checkBox_7_stateChanged(int arg1)
{
    freflect=ui->checkBox_7->isChecked()?-1.:1.;
    Matrix *&ScaleMatrix=ui->graphicsView->ScaleMatrix;
    *ScaleMatrix=SCALE;
    ui->graphicsView->viewport()->repaint();
}

void MainWindow::on_checkBox_10_stateChanged(int arg1)
{
    if(arg1)
        n=-1/ui->doubleSpinBox_10->value();
    else
        n=0;
    Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
    *MoveMatrix=MOVE;
}

void MainWindow::on_checkBox_9_stateChanged(int arg1)
{
    if(arg1)
        m=-1/ui->doubleSpinBox_7->value();
    else
        m=0;
    Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
    *MoveMatrix=MOVE;
}

void MainWindow::on_checkBox_8_stateChanged(int arg1)
{
    if(arg1)
        l=-1/ui->doubleSpinBox_9->value();
    else
        l=0;
    Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
    *MoveMatrix=MOVE;
}

void MainWindow::on_doubleSpinBox_9_valueChanged(double arg1)
{
    if(ui->checkBox_8->isChecked()){
        l=-1/arg1;
        Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
        *MoveMatrix=MOVE;
    }
}

void MainWindow::on_doubleSpinBox_7_valueChanged(double arg1)
{
    if(ui->checkBox_9->isChecked()){
        m=-1/arg1;
        Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
        *MoveMatrix=MOVE;
    }
}

void MainWindow::on_doubleSpinBox_10_valueChanged(double arg1)
{
    if(ui->checkBox_10->isChecked()){
        n=-1/arg1;
        Matrix *&MoveMatrix=ui->graphicsView->MoveMatrix;
        *MoveMatrix=MOVE;
    }
}

void MainWindow::on_horizontalSlider_9_valueChanged(int value)
{
    ui->doubleSpinBox_9->setValue(value);
}

void MainWindow::on_horizontalSlider_8_valueChanged(int value)
{
    ui->doubleSpinBox_7->setValue(value);
}

void MainWindow::on_horizontalSlider_10_valueChanged(int value)
{
    ui->doubleSpinBox_10->setValue(value);
}
