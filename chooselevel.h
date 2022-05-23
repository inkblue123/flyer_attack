#ifndef CHOOSELEVEL_H
#define CHOOSELEVEL_H

#include <QPainter>
#include <QBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include "config.h"
class Chooselevel : public QWidget
{

        Q_OBJECT

public:
    Chooselevel();
    void paintEvent(QPaintEvent *event);//绘图事件
    void quitwindow();//退出游戏窗口
    void up_widget();
    void down_widget();
    QVBoxLayout *VBoxLayout;
    QWidget *up;
    QWidget *down;
    QPushButton *level_1;
    QPushButton *level_2;
    QPushButton *endless_999;
    QPushButton *endless_998;
    QPushButton *quitbut;

signals:
    void quit();
};

#endif // CHOOSELEVEL_H
