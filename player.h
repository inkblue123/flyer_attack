#ifndef PLAYER_H
#define PLAYER_H

#include <QPixmap>
#include <QSettings>
#include "plane.h"
#include "bullet.h"
#include "config.h"

class Player : public plane
{
    Q_OBJECT//这个类使用信号和槽的前提

    typedef void (Player::*INIT_EQUIP)();
public:
    Player(int *game_W,int *game_H);
    void init_equip();
    void updata_bullet();
    void setxy(int x,int y);
    void weapom_func0();
    void main_weapon_func1();
    void main_weapon_func2();
    void wingman_func1();
    void wingman_func2();

    int main_weapon_cd;//子弹发射间隔
    int main_weapon_time;//记录子弹发射间隔
    int wingman_cd;//子弹发射间隔
    int wingman_time;//记录子弹发射间隔

    int player_main_weapon_type;
    int player_main_weapon_level;//我方飞机的主武器等级
    int player_secon_weapon_type;
    int player_secon_weapon_level;//我方飞机的副武器等级
    int player_wingman_type;
    int player_wingman_level;//我方飞机的僚机等级
    int player_armor_type;
    int player_armor_level;//我方飞机的护甲等级

//    int flag_Invincible;//无敌

    QSettings *settings;//读写配置相关

    std::map <int,INIT_EQUIP> main_weapon_func;
    std::map <int,INIT_EQUIP> secon_weapon_func;
    std::map <int,INIT_EQUIP> wingman_func;
    std::map <int,INIT_EQUIP> armor_func;
    QPixmap player_picture;
    QRect player_rect;
//信号函数
signals:
    void put_bullet(int num,bull_massage *bull_msg);
};

#endif // PLAYER_H
