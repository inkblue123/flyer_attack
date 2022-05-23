#ifndef CONFIG_H
#define CONFIG_H

#include <QDebug>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QDir>

//#define DEBUG//测试模式

enum CAMP
{//阵营的枚举
    FRIENDLY=1,//友善
    HOSTILE,//敌对
    NEUTRAL,//中立
};
enum WEAPON_TYPE
{//武器对象的枚举
    WEAPON_E=1,//敌机
    WEAPON_M,//主武器
    WEAPON_S,//副武器
    WEAPON_W,//僚机
};

//menuwindow类
//#define MENU_WIDTH 1000//菜单窗口的宽度
//#define MENU_HEIGH 600//菜单窗口的高度

//gamewindow类
#define GAME_BULLET_NUM 100//场地内无主子弹的数量上限
//bullet类
#define BULLET_PICTRE ":/images/player_bullet1_1.png"//子弹的贴图
#define BULLET_SPEEDUP_TIME 50//第三种子弹，自狙击子弹的加速时间
#define BULLET_MOVE_1 1//直线运动，依靠xy轴速度
#define BULLET_MOVE_2 2//直线运动，依靠角度和速度
#define BULLET_MOVE_3 3//自狙击，依靠目标坐标和速度

//player类
#define PLAYER_PICTRE //我方飞机的贴图

#define PLAYER_FIRST_X 175//我方飞机初始x坐标
#define PLAYER_FIRST_Y 500//我方飞机初始y坐标
#define PLAYER_RECT_WIDTH 10//我方飞机的碰撞箱宽度
#define PLAYER_RECT_HEIGHT 10//我方飞机的碰撞箱高度
#define PLAYER_BULLETS_NUM 100//我方飞机的弹夹数量
#define PLAYER_BULLETS_TIME 10//我方飞机的射速
#define PLAYER_BULLETS_ATK 100//我方子弹的攻击力
#define PLAYER_BULLET_SPEED 3//我方子弹移动速度

//enemy类
#define ENEMY_PICTRE ":/images/2enemy.png"//敌机的贴图
#define ENEMY_SPEED1 0.1//敌机的移动速度
#define ENEMY_SPEED2 0.3//敌机的移动速度
#define ENEMY_SPEED3 0.5//敌机的移动速度
#define ENEMY_SPEED4 1//敌机的移动速度
#define ENEMY_BULLET_PICTRE ":/images/enemy_bullet1.png"//敌机子弹的贴图
#define ENEMY_BULLET_SPEED1 0.2//敌机子弹移动速度
#define ENEMY_BULLET_SPEED2 0.5//敌机子弹移动速度
#define ENEMY_BULLET_SPEED3 1//敌机子弹移动速度
#define ENEMY_BULLET_SPEED4 2//敌机子弹移动速度
#define ENEMY_BULLET_SPEED5 3//敌机子弹移动速度
#define ENEMY_BULLET_ATK 100//敌机的子弹攻击力
#define ENEMY_BULLETS_NUM 30//敌机的弹夹数量
#define ENEMY_BULLETS_TIME1 100//敌方子弹的射速
#define ENEMY_BULLETS_TIME2 300//敌方子弹的射速

#define ENEMYS_NUM 10//敌机数量上限
#define ENEMY_HP1 1000//敌机的生命值
#define ENEMY_HP2 3000//敌机的生命值

#define ENEMY_MAKE_TIME 200//敌机的制造时间间隔

void removeAll(std::string &str,char c);

struct bull_massage
{//初始化一个子弹所需的信息
    int camp=0;//子弹阵营：友善，敌对，中立
    int weapon_type=0;//子弹由哪种武器发射：敌机，主武器，副武器，僚机
    int weapon_level=0;
    int mode_type=0;//子弹属于哪种移动方式
    int ATK_level=0;
    int x=0;//发射点的中心坐标
    int y=0;
    //两个方向的速度矢量和
    double speed_x=0;//x轴移动速度
    double speed_y=0;//y轴移动速度
    //方向和速度的组合
    double angle=0;//相对于常规坐标系正x轴，向第二象限增长的角度
    double speed=0;//速度绝对值
    //目标的中心坐标，自狙击子弹需要
    int target_x=0;
    int target_y=0;
};

struct enemy_massage
{//初始化一个敌机所需的信息
    int x=0,y=0;
    int HP=0;
    int ATK_level=0;
    int bull=0;
    int picture=0;
    int move[10][10]={};
};

#endif // CONFIG_H
