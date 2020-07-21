#include "mini.h"
#include "ui_mini.h"

Mini::Mini(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mini)
{
    ui->setupUi(this);
    if(!QPixmap("cover.jpg").isNull()){
        ui->cover->setRotatePixmap(RotateCover::PixmapToRound(QPixmap("cover.jpg").scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation),50));
    }else if(!QPixmap("cover.jpeg").isNull()){
        ui->cover->setRotatePixmap(RotateCover::PixmapToRound(QPixmap("cover.jpeg").scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation),50));
    }else if(!QPixmap("cover.png").isNull()){
        ui->cover->setRotatePixmap(RotateCover::PixmapToRound(QPixmap("cover.png").scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation),50));
    }else if(!QPixmap("cover.gif").isNull()){
        ui->cover->setRotatePixmap(RotateCover::PixmapToRound(QPixmap("cover.gif").scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation),50));
    }else if(!QPixmap("cover.bmp").isNull()){
        ui->cover->setRotatePixmap(RotateCover::PixmapToRound(QPixmap("cover.bmp").scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation),50));
    }


    connect(ui->pushButton_switch,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_toggled(bool)));
    connect(ui->pushButton_play,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_play_toggled(bool)));
    connect(ui->pushButton_pause,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_pause_toggled(bool)));

}

Mini::~Mini()
{
    delete ui;
}

void Mini::on_pushButton_toggled(bool checked)
{
    standard->show();
    this->hide();
}

void Mini::createStandard(QWidget *st){
    standard = st;


}

void Mini::on_pushButton_play_toggled(bool checked)
{
    ui->cover->startRotate = true;
}

void Mini::on_pushButton_pause_toggled(bool checked)
{
    ui->cover->startRotate = false;
}
