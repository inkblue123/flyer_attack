#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>
#include <QDebug>
#include <cmath>
#include "config.h"
#define PI 3.1415926

class Bullet
{
    typedef void (Bullet::*INIT_BULLET)(bull_massage);
public:
    Bullet(int *game_W,int *game_H,int camp);

    void updata_position();
    void init_bullet(int type,int level,int ATK,int x0,int y0);
    void init_bullet(bull_massage bull_msg);

    void bullet_func1(bull_massage msg);
    void bullet_func2(bull_massage msg);
    void bullet_func3(bull_massage msg);
//    void enemy_bullet_func1(bull_massage msg);
//    void enemy_bullet_func2(bull_massage msg);

//    void init_bullet_func5(int ATK);

    void deep_copy_bull(Bullet *bull);

    int flag_camp;//子弹的阵营，取决于发射这个子弹的对象
    int bullet_type;//子弹的类型，取决于发射这个子弹的对象
    int bullet_level;//子弹类型之下的等级，取决于发射这个子弹的对象
    int bullet_mod_type;//子弹移动轨迹的区分，例如同为3发散弹类型，左中右依靠这个参数区分
    double bullet_x0;
    double bullet_y0;
    double bullet_x;
    double bullet_y;
    double target_x;
    double target_y;
    double bullet_speed_x1;
    double bullet_speed_y1;
    double bullet_speed;
    int bullet_ATK;
    bool flag_live;
    int *game_width;
    int *game_height;
    std::map <int,INIT_BULLET> init_bullet_map;
    std::map <int,INIT_BULLET> init_bullet_hostile;//敌对子弹类型
    int live_time;//子弹活跃时间
    QPixmap bullet_picture;//子弹的贴图
    QRect bullet_rect;//子弹的碰撞体积
};

#endif // BULLET_H
