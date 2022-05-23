#ifndef WAREHOUSEWINDOW_H
#define WAREHOUSEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include "config.h"

class WarehouseWindow : public QWidget
{
    Q_OBJECT

public:

    explicit WarehouseWindow();
    void paintEvent(QPaintEvent *event);//绘图事件
    void creat_up_lable();
    void creat_on_scrollarea();
    void creat_down_widget();
    void creat_money_widget();
    void write_config();

    int player_main_weapon_type;
    int player_main_weapon_level;//我方飞机的主武器等级
    int player_secon_weapon_type;
    int player_secon_weapon_level;//我方飞机的副武器等级
    int player_wingman_type;
    int player_wingman_level;//我方飞机的僚机等级
    int player_armor_type;
    int player_armor_level;//我方飞机的护甲等级

    QList<QPushButton*> btnPushlist;//动态创建按钮的列表
    QPushButton *btnPush;//动态创建按钮指针
    int main_weapon_total=3;//主武器动态创建按钮的个数
    int secon_weapon_total=2;//副武器动态创建按钮的个数
    int wingman_total=3;//僚机动态创建按钮的个数
    int armor_total=4;//装甲动态创建按钮的个数

    int money1=0;
    int money2=0;
    QLabel *money_Label1;
    QLabel *money_Label2;
    QSettings *settings;//读写配置相关

    QLabel *up_lable;
    QScrollArea *on_scrollarea;
    QWidget *money_widget;
    QWidget *down_widget;
    QVBoxLayout *VBoxLayout;

//信号函数
signals:
    void update_equip(int,int,int,int);
    void level_up(int,int);

//槽函数
private slots:
    void main_weapon_func();
    void secon_weapon_func();
    void wingman_func();
    void armor_func();
    void main_weapon();
    void wingman();

};

#endif // WAREHOUSEWINDOW_H
