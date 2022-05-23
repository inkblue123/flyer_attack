#include "gamewindow.h"

GameWindow::GameWindow(int level_num)
{
    this->setWindowTitle("飞机大战");
//    this->resize(GAME_WIDTH,GAME_HEIGH);//调整最外层窗口的大小
    this->timer.setInterval(10);//设置定时器时间10毫秒
//    this->setMouseTracking(true);//是否持续判断鼠标位置

    this->timer.start();//启动定时器

    srand(time(0));//随机种子

    flag_start_game=1;
    flag_end_game=0;
    flag_player_move=0;
    flag_player_fire=0;
    flag_next_wava=1;

    timer_time=0;
    timer_end_time=0;
    make_enemy_time=0;
    make_enemy_cd=0;
    no_enemy_cd=0;
    grade=0;
    enemys_num=0;
    wava_num=0;
    a_wava_num=0;
    a_wava=0;

    level=level_num;
    game_width=0;
    game_height=0;
    all_enemys_num=0;



    //初始化背景图片
    background1.load(":/images/background.png");
    background2.load(":/images/background.png");
    map_y1=0,map_y2=0;
    for(int i = 0 ;i < ENEMYS_NUM;i++)
    {//初始化敌机，控制敌机能出现的范围
        enemys[i]=new Enemy(&(this->game_width),&(this->game_height));
    }
//    for(int i = 0 ;i < 5;i++)
//    {//初始化道具，控制道具能出现的范围
//        item[i]=new Item();
//    }
    for (int i=0;i<GAME_BULLET_NUM;i++)
    {//初始化场地无主子弹，控制子弹能出现的范围
        bulls[i]=new Bullet(&(this->game_width),&(this->game_height),3);
    }
    //初始化我方飞机
    player=new Player(&(this->game_width),&(this->game_height));
    //初始化按钮
    quitbut = new QPushButton("退出关卡",this);
    suspend = new QPushButton("暂停",this);
    //初始化文本框
    endlab = new QLabel(this);
    endlab->setStyleSheet("QLabel{background:#cccccc;font:20px;}");
    endlab->setMinimumSize(QSize(100,100));//设置游戏窗口最小的大小
    endlab->setAlignment(Qt::AlignCenter);//设置文本内容居中显示
    endlab->hide();
    //从关卡文件中取出关卡信息
    init_level();
    //初始化配置信息
    settings = new QSettings("Option.ini",QSettings::IniFormat);
    //定时器超时，就执行对应的超时函数
    connect(&timer,&QTimer::timeout,this,&GameWindow::outtime);
    connect(this->quitbut,&QPushButton::clicked,this,&GameWindow::quitwindow);
    connect(this->suspend,&QPushButton::clicked,this,&GameWindow::suspend_window);
    connect(this->player,&Player::put_bullet,this,&GameWindow::put_bullet);
    for(int i=0;i<ENEMYS_NUM;i++)
    {
        connect(this -> enemys[i],&Enemy::aput_bullet,this,&GameWindow::put_bullet);
    }
}

void GameWindow::init_level()//根据不同的关卡，读取对应的关卡信息文件，调整部分参数
{
//几个敌机！初始位置！移动轨迹！生命！攻击力！子弹类别！贴图！
    QFile fp;
    if(level==0)
    {
        fp.setFileName(":/level/level_0.txt");
        quitbut->hide();//隐藏退出游戏按钮
    }
    else if(level==1)
    {
        fp.setFileName(":/level/level_1.txt");
    }
    else if(level==2)
    {
        fp.setFileName(":/level/level_2.txt");
    }
    else if(level==998)
    {
        fp.setFileName(":/level/level_998.txt");
    }
    else if(level==999)
    {
        fp.setFileName(":/level/level_999.txt");
    }
    fp.open(QIODevice::ReadOnly);
    QByteArray a=fp.readAll();
    fp.close();
    this->level_massage=(std::string)a;
    removeAll(this->level_massage,'\n');
    removeAll(this->level_massage,'\r');
    for(int i=0;level_massage[i]!='A';i++)
    {//读取本关的总敌人数
        all_enemys_num=all_enemys_num*10+level_massage[i]-'0';
    }
}

void GameWindow::outtime()//计时器事件
{
    timer_time++;
    if(flag_start_game==1)
    {
        start_game();
    }
    else if(flag_end_game==1)
    {
        end_game();
    }
    else
    {//正常游戏进行
        make_enemy();//产生新的敌机

        collision();//碰撞检测
    }
    updata_position();//更新游戏资源

    update();//qt函数，刷新窗口
}

void GameWindow::start_game()//游戏开始时的动画展示
{
    int x0,y0;
    x0=width()/2;
    y0=height()+player->player_picture.height();
    y0 = y0-timer_time*(3+(3-0.03*timer_time))/2;
    player->setxy(x0,y0);
    flag_player_fire=0;//登场动画，不发射子弹
    if(timer_time>=100)
    {//登场动画播放完毕，恢复正常战斗设置
        flag_start_game=0;
        flag_player_fire=1;//我方飞机允许发射子弹
        if(level!=0)//第0关展示我方飞机装备，不允许移动
        {
            flag_player_move=1;//可以操控飞机位置
        }
    }
}

void GameWindow::end_game()//游戏结束时的动画展示
{

    if(timer_end_time==0)
    {
        flag_player_move=0;//结束动画，不允许移动
        flag_player_fire=0;//不发射子弹
        timer_end_time=timer_time;
        player->x0=player->x;
        player->y0=player->y;
        suspend->hide();//关闭暂停按钮
    }

    if(player->HP>0)
    {//我方胜利
        int S=0;
        int T=timer_time-timer_end_time;
        if(T<=200)
        {
            S=T*(T*3.0/200)/2;
        }
        else
        {
            S=(T-100)*3;
        }
        player->setxy(player->x0,player->y0-S);


        if(player->y+player->player_picture.height()+100<0)
        {
            suspend_window();//暂停游戏
            endlab->move(width()/2-50,height()/2-50);
            endlab->setText("游戏胜利");

            //记录通过的最大关卡
            settings->beginGroup("game_mod");
            int max_level = settings->value("max_level_mode").toInt();
            if(max_level<level)
            {
                settings->setValue("max_level_mode",level);
                endlab->setText("游戏胜利，解锁了新装备");
            }

            settings->endGroup();// 结束创建

            endlab->show();
        }
    }
    else
    {//我方死亡
        int t=timer_time-timer_end_time;
        player->speed_y1=t*3.0/200;
        if(player->speed_y1>=3)
        {
            player->x=player->x0;
            player->y=player->y0+((t+t-200)*3)/2;
        }
        else
        {
            player->x=player->x0;
            player->y=player->y0+(t*player->speed_y1)/2;
        }
        if(player->y>width()+player->player_picture.height()+100)
        {
            suspend_window();//暂停游戏
            endlab->move(width()/2-50,height()/2-50);
            endlab->setText("游戏结束");
            endlab->show();
        }
    }
}

void GameWindow::make_enemy()//制造一波敌机
{
    if(flag_next_wava==1)//初始状态，读取当前波次的信息
    {
        int flag=0;
        for(flag=0;level_massage[flag]!='A';flag++);
        for(int i=0;i<=wava_num;i++)
        {
            a_wava_num=0;
            for(flag++;level_massage[flag]!='@';flag++)
            {//这一波有多少敌人
                a_wava_num=a_wava_num*10+level_massage[flag]-'0';
            }
            wava_type=0;
            for(flag++;level_massage[flag]!='@';flag++)
            {//这一波的类别，1是时间到了出下一波，2是全部死亡出下一波
                wava_type=wava_type*10+level_massage[flag]-'0';
            }
            no_enemy_cd=0;
            for(flag++;level_massage[flag]!='@';flag++)
            {//如果这一波内没有敌人了，多久出下一波
                no_enemy_cd=no_enemy_cd*10+level_massage[flag]-'0';
            }
            make_enemy_cd=0;
            for(flag++;level_massage[flag]!='!';flag++)
            {//这一波产生完毕之后，下一波出现的时间间隔
                make_enemy_cd=make_enemy_cd*10+level_massage[flag]-'0';
            }
        }
        //制造这一波的敌人
        for(int j=0;j<a_wava_num;j++)
        {
            for(int i = 0 ;i < ENEMYS_NUM;i++)
            {//遍历每一个敌机
                if(enemys[i]->flag_live==false)
                {//如果敌机不活跃
                    //获取第enemys_num个敌机的信息，完成敌机的初始化
                    get_enemy_massage(i);
                    enemys_num++;
                    break;
                }
            }
        }
        flag_next_wava=0;//读取一波信息之后需要满足条件才能读取下一波
        make_enemy_time=0;//开始计数
    }

//    if(!is_enemylive())
//    {//全部敌机死亡
//        if(wava_type==0)
//        {//已经是最后一波，这一关结束，进入结束动画
//            flag_end_game=1;
//        }
//        else
//        {
//            make_enemy_time++;//等待时间
//            if(make_enemy_time >= no_enemy_cd)
//            {
//                flag_next_wava=1;//这一波敌人处理完毕，读取下一波的信息
//                if(wava_type==998)
//                {
//                    enemys_num-=a_wava_num;
//                }
//                else
//                {
//                    wava_num++;
//                }
//            }
//        }
//    }
//    else
//    {
//        if(wava_type==2||wava_type==0)
//        {//等待所有敌机死亡产生下一波
//            return ;
//        }
//        make_enemy_time++;//等待时间
//        if(make_enemy_time >= make_enemy_cd)
//        {
//            flag_next_wava=1;//这一波敌人处理完毕，读取下一波的信息
//            if(wava_type==999)
//            {//特殊波次，一直定时产生这一波敌人
//                enemys_num-=a_wava_num;
//            }
//            if(wava_type==1)
//            {
//                wava_num++;
//            }
//        }
//    }


    if(wava_type==0)
    {//最后一波已经出现，全部敌机死亡，这一关结束
        if(!is_enemylive())
        {//所有敌人死亡，游戏结束，进入结束动画
            flag_end_game=1;
        }
    }
    else if(wava_type==999)
    {//特殊波次，一直定时产生这一波敌人
        make_enemy_time++;
        if(make_enemy_time >= make_enemy_cd)
        {
            flag_next_wava=1;//这一波敌人等待完成，归零
            enemys_num-=a_wava_num;
            a_wava++;
        }
    }
    else if(wava_type==998)
    {//特殊波次，敌人全部死亡之后重新产生这一波
        if(!is_enemylive())
        {
            make_enemy_time++;//等待时间
            if(make_enemy_time >= no_enemy_cd)
            {
                flag_next_wava=1;//这一波敌人等待完成，归零
                enemys_num-=a_wava_num;
                a_wava++;
            }
        }
    }
    else
    {
        //普通波次
        if(!is_enemylive())
        {//所有敌机死亡
            make_enemy_time++;//等待时间
            if(make_enemy_time >= no_enemy_cd)
            {
                flag_next_wava=1;//这一波敌人等待完成，读取下一波的信息
                wava_num++;
            }
        }
        else
        {//还有活着的敌机
            if(wava_type==1)
            {//等待时间生产下一波
                make_enemy_time++;//等待时间
                if(make_enemy_time >= make_enemy_cd)
                {
                    flag_next_wava=1;//这一波敌人等待完成，读取下一波的信息
                    wava_num++;
                }
            }
            else if(wava_type==2)
            {//等待所有敌机死亡产生下一波
                return ;
            }
        }
    }
}

bool GameWindow::is_enemylive()//判断所有敌机的活跃情况，如果有活跃的返回true，全部死亡返回false
{
    int flag=0;
    for(int i=0;i<ENEMYS_NUM;i++)
    {//遍历所有敌机，处理敌机全部死亡的情况
        if(enemys[i]->flag_live==true)
        {
            flag=1;
            break;
        }
    }
    return flag;
}

void GameWindow::get_enemy_massage(int num)//获取这一关的第num个飞机的信息，生成敌机
{
    //几个敌机！初始位置！移动轨迹！生命！攻击力！子弹类别！贴图！
    int flag=0;
    int flag_xy_except=0,flag_xy_num=0;

    for(flag=0;level_massage[flag]!='B';flag++);
    for(int i=0;;flag++)
    {
        if(level_massage[flag]=='C')
        {
            i++;
        }
        if(i>=enemys_num)
        {
            break;
        }
    }
    enemy_massage msg;
//    获取第num个敌机的初始位置
    flag_xy_except=0;
    for(flag++;level_massage[flag]!='@';flag++)
    {
        flag_xy_except=flag_xy_except*10+level_massage[flag]-'0';
    }
    flag_xy_num=0;
    for(flag++;level_massage[flag]!='!';flag++)
    {
        flag_xy_num=flag_xy_num*10+level_massage[flag]-'0';
    }
    msg.x=( width() / (flag_xy_except-1) )*(flag_xy_num%flag_xy_except);
    msg.y=( height() / (flag_xy_except-1) )*(flag_xy_num/flag_xy_except)-50;
    //读取移动轨迹
    int k=0,l=0,loss=1;
    for(flag++;level_massage[flag]!='!';flag++)
    {
        if(level_massage[flag]=='@')
        {
            l++,loss=1;
            continue;
        }
        else if(level_massage[flag]=='#')
        {
            k++,l=0,loss=1;
            continue;
        }
        else if(level_massage[flag]=='-')
        {
            loss=-1;
            continue;
        }
        msg.move[k][l]=msg.move[k][l]*10+(level_massage[flag]-'0')*loss;
    }
    //获取第num个敌机的生命
    for(flag++;level_massage[flag]!='!';flag++)
    {
        msg.HP=msg.HP*10+level_massage[flag]-'0';
    }
    //获取第num个敌机的攻击力
    for(flag++;level_massage[flag]!='!';flag++)
    {
        msg.ATK_level=msg.ATK_level*10+level_massage[flag]-'0';
    }
    //获取第num个敌机的子弹类型
    for(flag++;level_massage[flag]!='!';flag++)
    {
        msg.bull=msg.bull*10+level_massage[flag]-'0';
    }
    //获取第num个敌机的贴图编号
    for(flag++;level_massage[flag]!='!';flag++)
    {
        msg.picture=msg.picture*10+level_massage[flag]-'0';
    }

    if(level!=0&&(wava_type==999||wava_type==998))
    {
        msg.ATK_level+=a_wava*2;
        msg.HP+=a_wava*2000;
        msg.bull=1+rand()%2;
        msg.picture=rand()%2;
        msg.move[0][1]=100*(1+rand()%4);
        msg.move[0][4]=1+rand()%5;
        msg.move[1][1]=50*(1+rand()%4);
        msg.move[1][3]=rand()%5;
        if(msg.move[1][3]==0)
        {
            msg.move[1][4]=0;
        }
        else
        {
            msg.move[1][4]=rand()%msg.move[1][3];
        }
        msg.move[1][5]=rand()%5;
        if(msg.move[1][5]==0)
        {
            msg.move[1][6]=0;
        }
        else
        {
            msg.move[1][6]=rand()%msg.move[1][5];
        }
        msg.move[2][0]=3+rand()%2;
        if(msg.move[2][0]==3)//圆周运动，3是半径，4是秒单位周期
        {
            msg.move[2][3]=10*(1+rand()%10);
            msg.move[2][4]=1+rand()%10;
        }
        else//往复运动，3是周期，4是x轴速度，5是y轴速度
        {
            msg.move[2][3]=50*(1+rand()%10);
            msg.move[2][4]=rand()%6;
            msg.move[2][5]=rand()%6;
        }
    }
    enemys[num]->init_enemy(msg);//初始化敌机
}

void GameWindow::updata_position()//更新游戏资源
{
    game_width=width();
    game_height=height();

    map_y1=(map_y1+1)%background1.height();
    map_y2=map_y1-background1.height();

    //右下的退出按钮
    quitbut->setGeometry(width()-100,height()-50,100,50);
    suspend->setGeometry(width()-50,0,50,50);
    //更新我方飞机的子弹
    if(level==0)
    {//第0关展示战斗场面，不允许移动，系统自动搭配窗口移动

    }
    if(flag_player_fire==1)
    {
        player->updata_bullet();
    }
    //更新敌方飞机
    for(int i=0;i<ENEMYS_NUM;i++)
    {//遍历每一个敌机
        if(enemys[i]->flag_live==true)
        {//敌机活跃
            enemys[i]->updata_position();//更新自己的位置
            enemys[i]->updata_bullet();//发射子弹
        }
    }
    //更新场地无主子弹
    for(int i=0;i<GAME_BULLET_NUM;i++)
    {//遍历每一个场地内的无主子弹
        if(bulls[i]->flag_live==true)
        {
            bulls[i]->updata_position();
        }
    }
}

void GameWindow::paintEvent(QPaintEvent *event)//绘图事件
{
    QPainter painter(this);
    painter.setPen(QColor("black"));
    painter.drawRect(0,0,width()-1,height()-1);    
    painter.setPen(QColor("green"));

    painter.drawPixmap(0,map_y1,width(),background1.height(),background1);
    painter.drawPixmap(0,map_y2,width(),background2.height(),background2);

    //绘制场地无主子弹
    for(int i=0;i<GAME_BULLET_NUM;i++)
    {
        if(bulls[i]->flag_live==true)
        {//如果场地无主子弹状态是活跃的，绘制出来
            painter.drawPixmap(bulls[i]->bullet_x - bulls[i]->bullet_picture.width()*0.5,
                               bulls[i]->bullet_y - bulls[i]->bullet_picture.height()*0.5,
                               bulls[i]->bullet_picture);
#ifdef DEBUG//显示场地无主子弹的碰撞体积
            painter.drawRect(bulls[i]->bullet_rect.x(),
                             bulls[i]->bullet_rect.y(),
                             bulls[i]->bullet_rect.width(),
                             bulls[i]->bullet_rect.height());
#endif
        }
    }

    //绘制敌机
    for(int i = 0 ;i < ENEMYS_NUM;i++)
    {//遍历每一个敌机
        if(enemys[i]->flag_live==true)
        {//如果这个敌机是活跃的，绘制出来，因为对象中的xy是中心坐标，所以绘制的时候需要调整到左上角
            painter.drawPixmap(enemys[i]->x - enemys[i]->enemy_picture.width()*0.5,
                               enemys[i]->y - enemys[i]->enemy_picture.height()*0.5,
                               enemys[i]->enemy_picture);

#ifdef DEBUG//显示敌机的碰撞体积
            painter.drawRect(enemys[i]->enemy_rect.x(),
                             enemys[i]->enemy_rect.y(),
                             enemys[i]->enemy_rect.width(),
                             enemys[i]->enemy_rect.height());
#endif
        }
    }
    //绘制我方飞机
    painter.drawPixmap(player->x - player->player_picture.width()*0.5,
                       player->y - player->player_picture.height()*0.5,
                       player->player_picture);
#ifdef DEBUG//显示我方飞机的碰撞体积
    painter.drawRect(player->player_rect.x(),
                     player->player_rect.y(),
                     player->player_rect.width(),
                     player->player_rect.height());
#endif
}

void GameWindow::collision()//碰撞检测
{
    //我方飞机和场地无主子弹的碰撞检测
    for(int i=0;i<GAME_BULLET_NUM;i++)
    {
        if(bulls[i]->flag_live==false)
        {
            continue;
        }
        if(bulls[i]->flag_camp==1)
        {//如果是友方子弹，遍历所有敌机
            for(int j = 0; j < ENEMYS_NUM; j++ )
            {
                if(enemys[j]->flag_live==false)
                {//如果这个敌机不是活跃状态，就判断下一个敌机
                    continue;
                }
                if(bulls[i]->bullet_rect.intersects(enemys[j]->enemy_rect))
                {
                    bulls[i]->flag_live=0;
                    enemys[j]->HP-=bulls[i]->bullet_ATK;
                    if(enemys[j]->HP<=0)
                    {//击败敌方敌机
                        enemys[j]->flag_live=false;
                        if(player->player_main_weapon_level<5)
                        {
                            player->player_main_weapon_level++;
                            player->ATK_level++;
                        }

                    }
                }
            }
        }
        else if(bulls[i]->flag_camp==2)
        {//如果是敌方子弹，与我方单位进行碰撞检测
            if(player->player_rect.intersects(bulls[i]->bullet_rect))
            {
                player->HP-=bulls[i]->bullet_ATK;
                qDebug() <<"被击中"<<grade++;
                bulls[i]->flag_live=false;
                if(player->HP<=0)
                {
                    flag_end_game=1;
                }
                return ;
            }
        }
    }
}

void GameWindow::quitwindow()//退出游戏窗口
{
    emit this->quit();
}

void GameWindow::put_bullet(int num,bull_massage *msg)//发射子弹，由我方飞机和敌方飞机的信号激活
{
    int target_x=0,target_y=0,flag=0;
    if(msg[0].camp==FRIENDLY)
    {//这次子弹由友方发射
        for(int k=0;k<ENEMYS_NUM;k++)
        {//备用信息，一个敌方单位的坐标
            if(enemys[k]->flag_live==true)
            {
                target_x=enemys[k]->x;
                target_y=enemys[k]->y;
                flag=1;
                break;
            }
        }
    }
    else if(msg[0].camp==HOSTILE)
    {
        target_x=player->x;
        target_y=player->y;
        flag=1;
    }

    int j=0;
    for(int i=0;i<GAME_BULLET_NUM;i++)
    {//遍历所有子弹
        if(bulls[i]->flag_live==false)
        {//找到空闲子弹
            if(msg[j].mode_type==3)
            {//如果是我方自狙击弹药，填入备用信息再发射子弹
                if(flag==0)
                {//如果没有目标，索敌正前方直到离开屏幕
                    msg[j].target_x=msg[j].x;
                    msg[j].target_y=-height()*0.5;
                }
                else
                {
                    msg[j].target_x=target_x;
                    msg[j].target_y=target_y;
                }
            }
            this->bulls[i]->init_bullet(msg[j]);//发射num个子弹
            j++;
        }
        if(j>=num)
        {
            break;
        }
    }
}

void GameWindow::update_equip(int a,int b,int c,int d)//即时更新装备
{
    player->player_main_weapon_type=a;
    player->player_secon_weapon_type=b;
    player->player_wingman_type=c;
    player->player_armor_type=d;

    player->main_weapon_time=0;//更新装备之后重新计数，所以连续切换子弹会“卡顿”
    player->wingman_time=0;//更新装备之后重新计数，所以连续切换子弹会“卡顿”

    QString level = QString("min_weapon%1").arg(player->player_main_weapon_type);
    settings->beginGroup("weapon");//创建所属组，并开始创建
    player->player_main_weapon_level = settings->value(level).toInt();
    settings->endGroup();// 结束创建
}

void GameWindow::level_up(int weapon_type,int weapon_num)//即时升级装备
{
    if(weapon_type==1)
    {
        if(weapon_num!=0)
        {
            if(player->player_main_weapon_level<5)
            {
                player->player_main_weapon_level++;
                player->ATK_level++;
            }
        }
    }

}

void GameWindow::suspend_window()//暂停游戏
{
    if(timer.isActive())
    {
        //游戏暂停时不允许飞机移动，虽然时间停止，但程序依旧可以读取
        flag_player_move=0;
        timer.stop();
    }
    else
    {
        flag_player_move=1;
        timer.start();
    }
}

void GameWindow::write_config()//将我方飞机的装备信息写入配置
{
    QSettings *settings;//读写配置相关
    settings = new QSettings("Option.ini",QSettings::IniFormat);
    settings->beginGroup("player");//创建所属组，并开始创建
    settings->setValue("main_weapon_type",player->player_main_weapon_type);//添加组成员，并设置它的值
    settings->setValue("secon_weapon_type", player->player_secon_weapon_type);
    settings->setValue("wingman_type", player->player_wingman_type);
    settings->setValue("armor_type", player->player_armor_type);
    settings->endGroup();// 结束创建

    QString level = QString("min_weapon%1").arg(player->player_main_weapon_type);
    settings->beginGroup("weapon");//创建所属组，并开始创建
    settings->setValue(level, player->player_main_weapon_level);
    settings->endGroup();// 结束创建
}

void GameWindow::mousePressEvent(QMouseEvent *event)//重写点击鼠标事件
{
    if(flag_player_move==1)
    {
        QPoint p_re = event->pos();//相对坐标
        int x = p_re.x();
        int y = p_re.y();
    //    QString str = QString("相对移动坐标:x=[%1],y=[%2]").arg(x).arg(y);
    //    qDebug() <<str;
        if(x>=0&&x<=width()&&y>=0&&y<=height())
        {
            player->setxy(x,y);
        }
    }
}

void GameWindow::mouseMoveEvent(QMouseEvent *event)//重写拖动鼠标事件
{//移动
    if(flag_player_move==1)
    {
        //    QPoint p_ab = event->globalPos();//绝对坐标
        QPoint p_re = event->pos();//相对坐标
        int x = p_re.x();
        int y = p_re.y();
        //    QString str = QString("相对移动坐标:x=[%1],y=[%2]").arg(x).arg(y);
        //    qDebug() <<str;
        if(x>=0&&x<=width()&&y>=0&&y<=height())//不允许离开屏幕
        {
            player->setxy(x,y);
        }
    }
}
