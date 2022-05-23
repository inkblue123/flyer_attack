#ifndef ENEMY_H
#define ENEMY_H

#include <QPixmap>
#include <cmath>
#include "plane.h"
#include "bullet.h"
#include "config.h"
#define pi 3.1415926

class Enemy : public plane
{
    Q_OBJECT//这个类使用信号和槽的前提
    typedef void (Enemy::*INIT_ENEMY)(int);
public:
    Enemy(int *game_W,int *game_H);
    void init_enemy(enemy_massage msg);
    void move_track1(int i);
    void move_track2(int i);
    void move_track3(int i);
    void move_track4(int i);
    void move_track5(int i);
    double level_get_value(int level);
    void activation_enemy(int x0,int y0);
    void updata_position();
    void updata_bullet();

    int buller_cd;//子弹发射间隔
    int buller_time;//记录子弹发射间隔
    bool flag_live;
    int live_time;
    int enemy_type;
    int move[10][10];

    std::map <int,INIT_ENEMY> move_track_func;
    QRect enemy_rect;//敌机的碰撞体积
    QPixmap enemy_picture;//敌机的贴图
//信号函数
signals:
    void aput_bullet(int num,bull_massage *bull_msg);
};

#endif // ENEMY_H
