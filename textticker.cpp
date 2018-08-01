#include "textticker.h"
#include <QPainter>
#include <QTimer>


TextTicker::TextTicker(QWidget *parent)
    : QLabel(parent)
{
    setMinimumWidth(200);
    setMinimumHeight(40);

    m_curIndex = 0;//当前角码
    m_showText = "歌曲名";//显示的文字
    m_charWidth = fontMetrics().width("a");//每个字符的宽度



    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TextTicker::updateIndex);
    timer->start(100);
}


TextTicker::~TextTicker()
{


}

void TextTicker::setShowText(QString t){
    m_showText = t;
    update();
}

void TextTicker::paintEvent(QPaintEvent *event)
{
    if(fontMetrics().width(m_showText)>width()&&fontMetrics().width(m_showText)<500){
        QPainter painter(this);
        painter.drawText(0, 40, m_showText.mid(m_curIndex));
        painter.drawText(width() - m_charWidth*m_curIndex, 40, m_showText.left(m_curIndex));
    }else{
        QString str = fontMetrics().elidedText(m_showText, Qt::ElideRight, 500);//返回一个带有省略号的字符串
        setShowText(str);
        QLabel::paintEvent(event);
    }
}


void TextTicker::updateIndex()
{
    update();
    m_curIndex++;
    if (m_curIndex*m_charWidth > width())
        m_curIndex = 0;
}
