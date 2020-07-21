#include "rotatecover.h"
 /*如何使用 旋转封面
  *1.将QLabel提升至RotateCover
  *2.在QWidget的cpp中setRotatePixmap
  */
RotateCover::RotateCover(QWidget *parent):QLabel(parent)
{
    setRotatePixmap(PixmapToRound(QPixmap(":/image/icon.png").scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation),50));
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&RotateCover::updateRotate);
    timer->start(10);
}


void RotateCover::updateRotate()
{
    //每36000转十度 每3600转1度 每600转1分 每100转1/6分
    if(startRotate){
        update();
        imageRotate+=0.25;//0.25 和上面的 10 联合起来控制转速
    }
}

void RotateCover::setRotatePixmap(QPixmap pic)
{
    cover = new QPixmap(pic);
}

QPixmap RotateCover::PixmapToRound(QPixmap src, int radius)
{
    if (src.isNull()) {
            return QPixmap();
        }

        QSize size(2*radius, 2*radius);
        QBitmap mask(size);
        QPainter painter(&mask);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
        painter.setBrush(QColor(0, 0, 0));
        painter.drawRoundedRect(0, 0, size.width(), size.height(), radius, radius); //画圆角
        QPixmap image = src.scaled(size);
        image.setMask(mask);
        return image;
}

void RotateCover::paintEvent(QPaintEvent *ev){ //下面的数字要根据具体的情况修改 第一个 translate 参数为圆心相对坐标 下一个translate 恢复成原来的中心点
    QPainter painter(this);
//    QPixmap disc(":/image/icon.png");
    /* 碟机转动 */
    if(imageRotate>=360)
         imageRotate = 0;
    /* 设定旋转中心点 */
//    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);//消锯齿 很有用
    painter.setRenderHint(QPainter::HighQualityAntialiasing);//这个效果是最好的！！
    painter.translate(this->width()/2,this->height()/2);
    /* 旋转的角度 */
    painter.rotate(imageRotate);
    /* 恢复中心点 */
    painter.translate(-63,-63);
    /* 画图操作 */
    painter.drawPixmap(13,13,100,100, *cover); //后两个是直径 前两个是大圆盘半径与小圆盘半径的差值
}

