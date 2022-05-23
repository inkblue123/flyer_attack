#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QBoxLayout>
#include <QSettings>
#include "gamewindow.h"
#include "warehousewindow.h"
#include "chooselevel.h"
#include "config.h"

class MenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWindow();
    void quit_GameWindow();
    void level1();
    void level2();
    void endless_999();
    void endless_998();
    QSettings *settings;//读写配置相关
    GameWindow *slow_on_menu;
    GameWindow *active_on_menu;
    Chooselevel *left_menu;
    WarehouseWindow *right_menu;
    WarehouseWindow *new_right_menu;
    QHBoxLayout *HBoxLayout;
//signals:
//    void quit();
};

#endif // MENUWINDOW_H
