#include "player.h"

Player::Player(int *game_W,int *game_H)
{
//    player_picture.load(":/images/player2.png");
    x=0;
    y=0;
    x0=0;
    y0=0;
    HP=1000;
    ATK_level=1;
    main_weapon_cd = 0;
    main_weapon_time = 0;
    wingman_cd = 0;
    wingman_time = 0;

    game_width=game_W;
    game_height=game_H;

//    player_main_weapon_type=1;
    player_main_weapon_level=1;//我方飞机的主武器等级
//    player_secon_weapon_type=1;
    player_secon_weapon_level=1;//我方飞机的副武器等级
//    player_wingman_type=1;
    player_wingman_level=1;//我方飞机的僚机等级
//    player_armor_type=1;
    player_armor_level=1;//我方飞机的护甲等级

    settings = new QSettings("Option.ini",QSettings::IniFormat);


    main_weapon_func[0] = &Player::weapom_func0;
    main_weapon_func[1] = &Player::main_weapon_func1;
    main_weapon_func[2] = &Player::main_weapon_func2;

    wingman_func[0] = &Player::weapom_func0;
    wingman_func[1] = &Player::wingman_func1;
    wingman_func[2] = &Player::wingman_func2;

    init_equip();
    player_rect.setWidth(PLAYER_RECT_WIDTH);
    player_rect.setHeight(PLAYER_RECT_HEIGHT);
    player_rect.moveTo(x-PLAYER_RECT_WIDTH*0.5,
                       y-PLAYER_RECT_HEIGHT*0.5);
}

void Player::init_equip()//从配置中读出对应的数值
{
//    if(settings->value("flag").toInt()==0)
//    {
//        settings->setValue("flag", 1);
//        settings->beginGroup("player");//创建所属组，并开始创建
//        settings->setValue("main_weapon_type",1);//添加组成员，并设置它的值
//        settings->setValue("secon_weapon_type", 1);
//        settings->setValue("wingman_type",1);
//        settings->setValue("armor_type",0);
//        settings->endGroup();// 结束创建
//    }
    settings->beginGroup("player");
    player_main_weapon_type = settings->value("main_weapon_type").toInt();
    player_secon_weapon_type = settings->value("secon_weapon_type").toInt();
    player_wingman_type = settings->value("wingman_type").toInt();
    player_armor_type = settings->value("armor_type").toInt();
    settings->endGroup();// 结束创建

    QString level = QString("min_weapon%1").arg(player_main_weapon_type);
    settings->beginGroup("weapon");//创建所属组，并开始创建
    player_main_weapon_level = settings->value(level).toInt();
    settings->endGroup();// 结束创建

    updata_bullet();
}

void Player::updata_bullet()//更新子弹
{
    main_weapon_time++;
    wingman_time++;
    //根据装甲类别切换不同的装甲
    QString image = QString(":/images/player%1.png").arg(player_armor_type+1);
    player_picture.load(image);
    //根据装备的武器种类发射对应的子弹
    (this ->* main_weapon_func[player_main_weapon_type])();
    (this ->* wingman_func[player_wingman_type])();
}

void Player::weapom_func0()//不发射子弹，重置对应武器的cd
{
    if(player_main_weapon_type==0)
    {
        main_weapon_time=0;
    }
    if(player_wingman_type==0)
    {
        wingman_time=0;
    }
}

void Player::main_weapon_func1()//第一种主武器，向前发射子弹
{
    if(main_weapon_cd!=PLAYER_BULLETS_TIME)
    {
        main_weapon_cd = PLAYER_BULLETS_TIME;
    }
    if(main_weapon_time>=main_weapon_cd)
    {//发射子弹cd好了
        bull_massage *msg=new bull_massage;
        msg->camp=FRIENDLY;
        msg->weapon_type=WEAPON_M;//只会向前发射子弹
        msg->weapon_level=player_main_weapon_level;//
        msg->mode_type=BULLET_MOVE_1;//这类弹幕同时只发射一发子弹，不需要这个变量
        msg->ATK_level=ATK_level;
        msg->x=x;
        msg->y=y;
        msg->speed_x=0;
        msg->speed_y=-5;
        emit put_bullet(1,msg);
        main_weapon_time=0;
    }
}

void Player::main_weapon_func2()//第二种主武器，发射散弹
{
    if(main_weapon_cd!=100)
    {//调整武器的子弹发射间隔
        main_weapon_cd = 100;
    }
    if(main_weapon_time>=main_weapon_cd)
    {//发射子弹cd好了
        int N=0;

        if(main_weapon_time-main_weapon_cd==0)//cd好了之后会发射两轮弹药
        {
            N=(player_main_weapon_level+1)/2+2;
        }
        else if(main_weapon_time-main_weapon_cd==20)
        {
            N=player_main_weapon_level/2+2;
            main_weapon_time=0;
        }
        else
        {
            return ;
        }
        bull_massage msg[N];
        for(int i=0;i<N;i++)
        {
            msg[i].camp=FRIENDLY;
            msg[i].weapon_type=WEAPON_M;
            msg[i].weapon_level=player_main_weapon_level;//
            msg[i].mode_type=BULLET_MOVE_2;//直线运动，依靠角度调整方向
            msg[i].ATK_level=ATK_level;
            msg[i].x=x;
            msg[i].y=y;
            msg[i].angle=(2*i-N+1)*7.5;
            msg[i].speed=3;
        }
        emit put_bullet(N,msg);
    }
}

void Player::wingman_func1()//第一种僚机，发射自狙击子弹
{
    if(wingman_cd!=200)
    {
        wingman_cd = 200;
    }
    if(wingman_time>=wingman_cd)
    {//发射子弹cd好了
        bull_massage msg[2];
        for(int i=0;i<2;i++)
        {
            msg[i].camp=FRIENDLY;
            msg[i].weapon_type=WEAPON_W;
            msg[i].weapon_level=player_wingman_level;//
            msg[i].mode_type=BULLET_MOVE_3;//发射自狙击子弹
            msg[i].ATK_level=ATK_level;
            msg[i].x=x+50*(i*2-1);
            msg[i].y=y;
            msg[i].speed=5;
        }
        emit put_bullet(2,msg);
        wingman_time=0;
    }
}

void Player::wingman_func2()//第二种僚机，发射直线运动子弹
{
    if(wingman_cd!=15)
    {
        wingman_cd = 15;
    }
    if(wingman_time>=wingman_cd)
    {//发射子弹cd好了
        bull_massage msg[2];
        for(int i=0;i<2;i++)
        {
            msg[i].camp=FRIENDLY;
            msg[i].weapon_type=WEAPON_W;//
            msg[i].weapon_level=player_wingman_level;//
            msg[i].mode_type=BULLET_MOVE_1;//发射直线运动子弹
            msg[i].ATK_level=ATK_level;
            msg[i].x=x+50*(i*2-1);
            msg[i].y=y;
            msg[i].speed_x=0;
            msg[i].speed_y=-4.5;
        }
        emit put_bullet(2,msg);
        wingman_time=0;
    }
}

void Player::setxy(int x,int y)
{
    this->x=x;
    this->y=y;
    this->player_rect.moveTo(this->x-PLAYER_RECT_WIDTH*0.5,
                             this->y-PLAYER_RECT_HEIGHT*0.5);
}
