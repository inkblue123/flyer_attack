#include "enemy.h"

Enemy::Enemy(int *game_W,int *game_H)
{
    x=0;
    y=0;
    x0=0;
    y0=0;
    speed_x1=0;
    speed_y1=0;
    HP=0;
    ATK_level=0;//飞机对象的攻击力等级
    game_width=game_W;//飞机对象活跃的区域大小
    game_height=game_H;//飞机对象活跃的区域大小
    buller_cd=0;//子弹发射间隔
    buller_time=0;//记录子弹发射间隔

    this->live_time=-1;
    this->enemy_picture.load(ENEMY_PICTRE);
    this->flag_live=false;
    this->enemy_type=0;

    move_track_func[1] = &Enemy::move_track1;
    move_track_func[2] = &Enemy::move_track2;
    move_track_func[3] = &Enemy::move_track3;
    move_track_func[4] = &Enemy::move_track4;
    move_track_func[5] = &Enemy::move_track5;
}

void Enemy::init_enemy(enemy_massage msg)//初始化飞机
{
    if(msg.picture==1)
    {
        this->enemy_picture.load(":/images/1enemy.png");
    }
    else if(msg.picture==2)
    {
        this->enemy_picture.load(":/images/2enemy.png");
    }
    enemy_type=msg.bull;

    this->x0=msg.x;
    this->y0=msg.y;
    this->x=this->x0;
    this->y=this->y0;
    this->ATK_level=msg.ATK_level;
    this->HP=msg.HP;

    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            this->move[i][j]=msg.move[i][j];
        }
    }
    flag_live=true;
    live_time=0;//飞机存活时间归零
    buller_time=0;//子弹发射cd归零
    enemy_rect.setWidth(enemy_picture.width());
    enemy_rect.setHeight(enemy_picture.height());
    enemy_rect.moveTo(x0-enemy_rect.width()*0.5,
                      y0-enemy_rect.height()*0.5);
}

void Enemy::updata_position()//更新敌机的位置
{
    int i=0;
    live_time++;
    int T=live_time;
    while(1)
    {
        if(move[i][2]==1||T<move[i][1])
        {
            (this ->* move_track_func[move[i][0]])(i);//移动方式的类型//根据移动方式前进
            break;
        }
        else
        {
            T=T-move[i][1];
            i++;
        }
    }
    //移动碰撞体积
    enemy_rect.moveTo(x-enemy_rect.width()*0.5,y-enemy_rect.height()*0.5);
    //假如离开了游戏窗口，就销毁
    if(y > (*game_height)*1.2||y < -(*game_height)*0.2)
    {
        flag_live=false;
    }
    else if(x > (*game_width)*1.2||x < -(*game_width)*0.2)
    {
        flag_live=false;
    }
}

double Enemy::level_get_value(int level)//根据等级获得数值
{
    switch (level)
    {
        case -5:return -1.5;
        case -4:return -1;
        case -3:return -0.5;
        case -2:return -0.3;
        case -1:return -0.1;
        case 0:return 0;
        case 1:return 0.1;
        case 2:return 0.3;
        case 3:return 0.5;
        case 4:return 1;
        case 5:return 1.5;
        default :return 0;
    }
}

void Enemy::move_track1(int i)//直线移动，3是x轴速度等级，4是y轴速度等级
{
    int t=0;
    for(int n=0;n<i;n++)
    {
        t+=move[n][1];
    }
    t=live_time-t;
    if(t==0)
    {
        x0=x;
        y0=y;
    }
    speed_x1=level_get_value(move[i][3]);
    speed_y1=level_get_value(move[i][4]);

    x=x0+speed_x1*t;
    y=y0+speed_y1*t;

}

void Enemy::move_track2(int i)//匀变速运动，3是x轴速度初始等级，4是x轴速度最后等级，5是y轴速度初始等级，6是y轴速度最后等级
{
    int t=0;
    for(int n=0;n<i;n++)
    {
        t+=move[n][1];
    }
    t=live_time-t;
    if(t==0)
    {
        x0=x;
        y0=y;
    }
    speed_x1=level_get_value(move[i][3]);
    double end_speed_x=level_get_value(move[i][4]);
    speed_y1=level_get_value(move[i][5]);
    double end_speed_y=level_get_value(move[i][6]);
    //匀变速运动的计算过程
//    double true_x=speed_x1-((speed_x1-end_speed_x)*t)/move[i][1];
//    double true_y=speed_y1-((speed_y1-end_speed_y)*t)/move[i][1];
//    double X=((speed_x1+true_x)*t)/2;
//    double Y=((speed_y1+true_y)*t)/2;
//    enemy_x = enemy_x0+X;
//    enemy_y = enemy_y0+Y;
    //化简
    x = x0+speed_x1*t-((speed_x1-end_speed_x)*t*t)/(2*move[i][1]);
    y = y0+speed_y1*t-((speed_y1-end_speed_y)*t*t)/(2*move[i][1]);
}

void Enemy::move_track3(int i)//圆周运动，3是半径，4是秒单位周期
{
    int t=0;
    int R=move[i][3];
    int zhouqi=move[i][4]*100;
    for(int n=0;n<i;n++)
    {
        t+=move[n][1];
    }
    t=live_time-t;
    if(t==0)
    {
        x0=x;
        y0=y+R;

    }
    x=x0+cos( ( t *360/zhouqi-90) *pi/180)*R;
    y=y0+sin( ( t *360/zhouqi-90) *pi/180)*R;
}

void Enemy::move_track4(int i)//往复运动，3是周期，4是x轴速度，5是y轴速度
{
    int t=0;
    int T=move[i][3];
    for(int n=0;n<i;n++)
    {
        t+=move[n][1];
    }
    t=live_time-t;
    if(t==0)
    {
        x0=x;
        y0=y;
    }
    speed_x1=level_get_value(move[i][4]);
    speed_y1=level_get_value(move[i][5]);
    //开始左右循环运动
    if( t%(T/4)==0)
    {//循环运动的周期是4s，兼容第一次进入循环，以1s为单位重置x0y0
        x0=x;
        y0=y;
    }
    if( t%T >= T/4 && t%T < (T*3)/4 )
    {//2s向右走
        speed_x1=-speed_x1;
        speed_y1=-speed_y1;
    }
    x=x0+speed_x1*(t%(T/4)+1);
    y=y0+speed_y1*(t%(T/4)+1);
}

void Enemy::move_track5(int i)//停止在原地，不运动
{
    int t=0;
    for(int n=0;n<i;n++)
    {
        t+=move[n][1];
    }
    t=live_time-t;
    if(t==0)
    {
        x0=x;
        y0=y;
    }
    x=x0;
    y=y0;
}

void Enemy::updata_bullet()//更新子弹
{
    buller_time++;
    if(enemy_type==0)
    {//特殊类别，不发射子弹
        return ;
    }
    else if(enemy_type==1)
    {//判断这个飞机的种类，ENEMY_TYPE1代表1型机，只会向前发射子弹
        buller_cd=100;
        if(buller_time>buller_cd)
        {//发射子弹cd没好
            bull_massage *msg=new bull_massage;
            msg->camp=HOSTILE;
            msg->weapon_type=WEAPON_E;
            msg->weapon_level=0;//
            msg->mode_type=BULLET_MOVE_1;//发射直线运动子弹
            msg->ATK_level=ATK_level;
            msg->x=x;
            msg->y=y;
            msg->speed_x=0;
            msg->speed_y=ENEMY_BULLET_SPEED4;
            emit aput_bullet(1,msg);
            buller_time=0;
        }
    }
    else if(enemy_type==2)
    {//ENEMY_TYPE2是2型机，会向前发射3发子弹
        buller_cd=300;
        if(buller_time>buller_cd)
        {//发射子弹cd没好
            bull_massage msg[3];
            for(int i=0;i<3;i++)
            {
                msg[i].camp=HOSTILE;
                msg[i].weapon_type=WEAPON_E;
                msg[i].weapon_level=0;//
                msg[i].mode_type=BULLET_MOVE_1;//发射一个直线运动子弹
                msg[i].ATK_level=ATK_level;
                msg[i].x=x;
                msg[i].y=y;
                msg[i].speed_x=(i-1)*ENEMY_BULLET_SPEED1;//调整xy轴的速度实现散弹效果
                msg[i].speed_y=ENEMY_BULLET_SPEED3;
            }
            emit aput_bullet(3,msg);
            buller_time=0;
        }
    }
}
