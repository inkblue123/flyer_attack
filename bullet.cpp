#include "bullet.h"

Bullet::Bullet(int *game_W,int *game_H,int camp)
{
    this->flag_camp=camp;//子弹属于哪个阵营，1友善2敌对(3中立)
    this->game_width=game_W;
    this->game_height=game_H;

    flag_live=false;

    init_bullet_map[1] = &Bullet::bullet_func1;
    init_bullet_map[2] = &Bullet::bullet_func2;
    init_bullet_map[3] = &Bullet::bullet_func3;

//    init_bullet_hostile[1] = &Bullet::enemy_bullet_func1;
//    init_bullet_hostile[2] = &Bullet::enemy_bullet_func2;
}

void Bullet::init_bullet(bull_massage bull_msg)//初始化一个子弹
{
    //初始化贴图相关属性
    bullet_type=bull_msg.weapon_type;
    bullet_level=bull_msg.weapon_level;
    bullet_mod_type=bull_msg.mode_type;
    flag_camp=bull_msg.camp;
    live_time=0;
    bullet_x0=bull_msg.x;
    bullet_y0=bull_msg.y;
    bullet_x=bullet_x0;
    bullet_y=bullet_y0;

    //根据这个子弹初始化贴图、攻击力、移动速度
    (this ->* init_bullet_map[bull_msg.mode_type])(bull_msg);

    if(flag_camp==HOSTILE)
    {
        bullet_rect.setWidth(bullet_picture.width()-5);
        bullet_rect.setHeight(bullet_picture.height()-5);
    }
    else
    {
        bullet_rect.setWidth(bullet_picture.width());
        bullet_rect.setHeight(bullet_picture.height());
    }

    bullet_rect.moveTo(bull_msg.x-bullet_rect.width()*0.5,
                       bull_msg.y-bullet_rect.height()*0.5);
    flag_live=true;
}

void Bullet::updata_position()
{
    //
    live_time++;
    //更新子弹的坐标
    if(bullet_mod_type==BULLET_MOVE_3)
    {
        if(live_time<BULLET_SPEEDUP_TIME)
        {
            bullet_x=bullet_x0+( bullet_speed_x1 * ((double)(live_time)/BULLET_SPEEDUP_TIME) * live_time )/2;
            bullet_y=bullet_y0+( bullet_speed_y1 * ((double)(live_time)/BULLET_SPEEDUP_TIME) * live_time )/2;
        }
        else
        {
            bullet_x=bullet_x0+(bullet_speed_x1*((live_time-BULLET_SPEEDUP_TIME)+live_time))/2;
            bullet_y=bullet_y0+(bullet_speed_y1*((live_time-BULLET_SPEEDUP_TIME)+live_time))/2;
        }
    }
    else
    {
        bullet_x=bullet_x0+bullet_speed_x1*live_time;
        bullet_y=bullet_y0+bullet_speed_y1*live_time;
    }

    //更新子弹的碰撞箱位置
    bullet_rect.moveTo(bullet_x-bullet_rect.width()*0.5,
                       bullet_y-bullet_rect.height()*0.5);
    //假如离开屏幕就不再活跃
    if(bullet_y < -(*game_height)*0.2 || bullet_y > (*game_height)*1.2)
    {
        flag_live=false;
    }
    else if(bullet_x <-(*game_width)*0.2 || bullet_x > (*game_width)*1.2)
    {
        flag_live=false;
    }
}

void Bullet::bullet_func1(bull_massage msg)//直线运行子弹，通过xy轴速度移动
{
    //子弹的攻击力
    if(msg.weapon_type==WEAPON_E)
    {
        bullet_ATK=ENEMY_BULLET_ATK+50*msg.ATK_level;
        bullet_picture.load(ENEMY_BULLET_PICTRE);
    }
    else if(msg.weapon_type==WEAPON_M)
    {
        bullet_ATK=PLAYER_BULLETS_ATK+50*msg.ATK_level;
        if(bullet_level>5)
        {
            bullet_level=5;
        }

        QString image = QString(":/images/player_bullet%1_%2.png").arg(msg.mode_type).arg(bullet_level);
        bullet_picture.load(image);
    }
    else if(msg.weapon_type==WEAPON_W)
    {
        bullet_ATK=10+5*msg.ATK_level;
        if(bullet_level>=4)
        {
            bullet_level=4;
        }
        QString image = QString(":/images/player_bullet%1_%2.png").arg(msg.mode_type).arg(bullet_level);
        bullet_picture.load(image);
    }

    bullet_speed_x1 = msg.speed_x;
    bullet_speed_y1 = msg.speed_y;
}

void Bullet::bullet_func2(bull_massage msg)//直线运行子弹，通过角度和速度移动
{

    //子弹的攻击力
    if(msg.weapon_type==WEAPON_E)
    {

    }
    else if(msg.weapon_type==WEAPON_M)
    {
        bullet_ATK=510;
        for(int i=1;i<=msg.ATK_level;i++)
        {
            int A=i*i-11*i+40;
            bullet_ATK=bullet_ATK+A;
        }
//        bullet_ATK=PLAYER_BULLETS_ATK+50*msg.ATK_level;
        if(bullet_level>=5)
        {
            bullet_level=5;
        }
        QString image = QString(":/images/player_bullet%1_%2.png").arg(msg.mode_type).arg(bullet_level);
        bullet_picture.load(image);
        QMatrix matrix;
        matrix.rotate( msg.angle );
        bullet_picture=bullet_picture.transformed(matrix,Qt::SmoothTransformation);//将图片倾斜一定角度

    }
    else if(msg.weapon_type==WEAPON_W)
    {

    }

    bullet_speed_x1 = cos( (msg.angle-90)*PI/180)*msg.speed;//-90是为了同步角度
    bullet_speed_y1 = sin( (msg.angle-90)*PI/180)*msg.speed;
}

void Bullet::bullet_func3(bull_massage msg)//自狙击子弹，通过目标坐标与当前坐标定位
{
    //子弹的攻击力
    if(msg.weapon_type==WEAPON_E)
    {

    }
    else if(msg.weapon_type==WEAPON_M)
    {

    }
    else if(msg.weapon_type==WEAPON_W)
    {
        bullet_ATK=PLAYER_BULLETS_ATK+50*msg.ATK_level;
        QString image = QString(":/images/wingman_1.png");
        bullet_picture.load(image);

        double x1=msg.target_x-msg.x;
        double y1=msg.y-msg.target_y;//窗口的坐标系与三角函数坐标系不一致

        QMatrix matrix;
    //    //atan2的正确用法应该是atan2(y,x)，为了简化代码选择了反过来用，可能会有bug
    //    matrix.rotate(90-(180*atan2(y1,x1)/PI));
        matrix.rotate(180*atan2(x1,y1)/PI);
        bullet_picture=bullet_picture.transformed(matrix,Qt::SmoothTransformation);//将图片倾斜一定角度
    }
    target_x=msg.target_x;
    target_y=msg.target_y;
    double x1=target_x-bullet_x0;
    double y1=target_y-bullet_y0;
    double x2=x1/sqrt(x1*x1+y1*y1);
    double y2=y1/sqrt(x1*x1+y1*y1);

    bullet_speed=msg.speed;
    bullet_speed_x1 = x2*bullet_speed;
    bullet_speed_y1 = y2*bullet_speed;
}

void Bullet::deep_copy_bull(Bullet *bull)//深拷贝一个子弹，废案
{
    this->bullet_type=bull->bullet_type;
    this->bullet_x0=bull->bullet_x0;
    this->bullet_y0=bull->bullet_y0;
    this->bullet_x=bull->bullet_x;
    this->bullet_y=bull->bullet_y;
    this->bullet_speed_x1=bull->bullet_speed_x1;
    this->bullet_speed_y1=bull->bullet_speed_y1;
    this->bullet_ATK=bull->bullet_ATK;
    this->flag_live=bull->flag_live;
    this->game_width=bull->game_width;
    this->game_height=bull->game_height;
    this->live_time=bull->live_time;
    this->bullet_picture=bull->bullet_picture;
    this->bullet_rect=bull->bullet_rect;
}
