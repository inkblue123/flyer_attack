#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <vector>
#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QGroupBox>
#include <QString>
#include <QMouseEvent>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QBrush>
#include <QFont>
#include <QSettings>


#include "player.h"
#include "enemy.h"
#include "config.h"
#include "bullet.h"
#include "item.h"

class GameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameWindow(int level_num);
    void init_level();
    void mousePressEvent(QMouseEvent * event);//鼠标按下事件
    void make_enemy();//产生敌机
    void get_enemy_massage(int num);//获得本关第num个飞机的信息
    void updata_position();//更新游戏资源
    void collision();//碰撞检测
    void start_game();
    void end_game();
    bool is_enemylive();

    void mouseMoveEvent(QMouseEvent * event);//鼠标移动事件
    void paintEvent(QPaintEvent *event);//绘图事件
private:
    int flag_start_game;//是否启动游戏开始时的动画展示
    int flag_end_game;//是否启动游戏结束时的动画展示
    int flag_player_move;//是否允许我方飞机移动
    int flag_player_fire;//是否允许我方飞机开火
    int flag_next_wava;//是否读取下一波的信息并制造飞机

    int timer_time;//计时器运转次数
    int timer_end_time;//游戏结束的时间

    int grade;//记分
    int enemys_num;//记录本关出现了多少敌人

    int all_enemys_num;//本关敌人总数
    int make_enemy_time;//敌机产生的cd
    int a_wava_num;//这一波有几个敌人
    int wava_type;//这一波的类型
    int no_enemy_cd;//一波敌人被消灭之后下一波出现的时间
    int make_enemy_cd;//两波敌人出现的时间间隔
    int wava_num;//本关的波次计数
    int a_wava;//本关真正出现了多少波的计数，用于无尽模式
//    int ever_wava_num[10];//每一波敌人的标记，可能也是废案

    int game_width;//游戏窗口宽度，主要供应其他类使用
    int game_height;//游戏窗口高度，主要供应其他类使用
    int map_y1;//背景图片的x坐标
    int map_y2;//背景图片的y坐标

    QPushButton *quitbut;//退出按钮
    QPushButton *suspend;//暂停按钮
    QPixmap background1;//背景图片1
    QPixmap background2;//背景图片2
    QLabel *endlab;
    QTimer timer;//计时器
    QSettings *settings;//读写配置相关
    Player *player;//我方飞机
    Enemy *enemys[10];//敌机们
    Item *item[5];//道具
    int level;//关卡计数
    std::string level_massage;//这次游戏界面的关卡信息

    Bullet *bulls[100];//场地遗留的无主子弹

//信号函数
signals:
    void quit();

//槽函数
public slots:
    void quitwindow();
    void suspend_window();
    void outtime();//超时事件
    void update_equip(int,int,int,int);//接收仓库窗口的信号，更新游戏界面的武器装备
    void level_up(int,int);
    void write_config();
    void put_bullet(int num,bull_massage *msg);

};

#endif // GAMEWINDOW_H
