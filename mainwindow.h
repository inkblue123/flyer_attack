#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QPropertyAnimation>
#include <QFile>
#include <QString>

#include "gamewindow.h"
#include "menuwindow.h"
#include "config.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public slots:
    void open_GameWindow();
    void quit_GameWindow();

public:
    MainWindow();
//    void mousePressEvent(QMouseEvent *);//鼠标按下事件
//    void mouseMoveEvent(QMouseEvent *);

    ~MainWindow();

private:
    QLabel *lab;
    QPushButton *button;
    QMediaPlayer *MP3;
    GameWindow *Game;//游戏界面
    MenuWindow *Menu;//游戏界面
};
#endif // MAINWINDOW_H
