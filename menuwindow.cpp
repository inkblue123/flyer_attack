#include "menuwindow.h"

MenuWindow::MenuWindow()
{
    //初始化配置信息
    settings = new QSettings("Option.ini",QSettings::IniFormat);
    if(settings->value("flag").toInt()==0)
    {
        settings->setValue("flag", 1);
        settings->beginGroup("player");//创建所属组，并开始创建
        settings->setValue("main_weapon_type",1);//添加组成员，并设置它的值
        settings->setValue("secon_weapon_type", 0);
        settings->setValue("wingman_type",0);
        settings->setValue("armor_type",0);
        settings->endGroup();// 结束创建
        settings->beginGroup("game_mod");//创建所属组，并开始创建
        settings->setValue("max_level_mode",0);
        settings->setValue("max_endless_mode",0);
        settings->endGroup();// 结束创建
        settings->beginGroup("weapon");//创建所属组，并开始创建
        settings->setValue("min_weapon1",1);
        settings->setValue("min_weapon2",1);
        settings->endGroup();// 结束创建
        settings->beginGroup("money");//创建所属组，并开始创建
        settings->setValue("money1",0);
        settings->setValue("money2",0);
        settings->endGroup();// 结束创建
    }

    setWindowTitle("飞机大战");
    this->setMinimumSize(QSize(1000,600));//设置游戏窗口最小的大小
    this->setWindowIcon(QIcon(":/images/logo.ico"));//设置左上角程序图标
//    this->resize(800,600);
    HBoxLayout = new QHBoxLayout(this);
//    HBoxLayout->setMargin(0);//布局和所在窗口之间的间隙大小
//    HBoxLayout->setSpacing(0);//布局内控件之间的间隙大小

    left_menu = new Chooselevel();
    slow_on_menu = new GameWindow(0);
    right_menu = new WarehouseWindow();

    HBoxLayout->addWidget(left_menu,1);
    HBoxLayout->addWidget(slow_on_menu,1);
    HBoxLayout->addWidget(right_menu,1);

    //按下第一关的按钮，打开第一关的游戏界面
    connect(left_menu->level_1,&QPushButton::clicked,this,&MenuWindow::level1);
    connect(left_menu->level_2,&QPushButton::clicked,this,&MenuWindow::level2);
    connect(left_menu->endless_999,&QPushButton::clicked,this,&MenuWindow::endless_999);
    connect(left_menu->endless_998,&QPushButton::clicked,this,&MenuWindow::endless_998);
    connect(right_menu,&WarehouseWindow::update_equip,slow_on_menu,&GameWindow::update_equip);
    connect(right_menu,&WarehouseWindow::level_up,slow_on_menu,&GameWindow::level_up);
}

void MenuWindow::level1()
{
    //将选关和仓库界面隐藏
    left_menu->setVisible(false);
    right_menu->setVisible(false);
    //将我方飞机的装备信息写入配置
    slow_on_menu->write_config();
    //创建第一关
    active_on_menu = new GameWindow(1);
    //代替原本的关卡
    HBoxLayout->replaceWidget(slow_on_menu,active_on_menu);
    //监控信号
    connect(this->active_on_menu,&GameWindow::quit,this,&MenuWindow::quit_GameWindow);
    delete slow_on_menu;
    slow_on_menu=nullptr;
}

void MenuWindow::level2()
{
    //将选关和仓库界面隐藏
    left_menu->setVisible(false);
    right_menu->setVisible(false);
    //将我方飞机的装备信息写入配置
    slow_on_menu->write_config();
    //创建第一关
    active_on_menu = new GameWindow(2);
    //代替原本的关卡
    HBoxLayout->replaceWidget(slow_on_menu,active_on_menu);
    //监控信号
    connect(this->active_on_menu,&GameWindow::quit,this,&MenuWindow::quit_GameWindow);
    delete slow_on_menu;
    slow_on_menu=nullptr;
}

void MenuWindow::endless_999()
{
    //将选关和仓库界面隐藏
    left_menu->setVisible(false);
    right_menu->setVisible(false);
    //将我方飞机的装备信息写入配置
    slow_on_menu->write_config();
    //创建第一关
    active_on_menu = new GameWindow(999);
    //代替原本的关卡
    HBoxLayout->replaceWidget(slow_on_menu,active_on_menu);
    //监控信号
    connect(this->active_on_menu,&GameWindow::quit,this,&MenuWindow::quit_GameWindow);
    delete slow_on_menu;
    slow_on_menu=nullptr;
}

void MenuWindow::endless_998()
{
    //将选关和仓库界面隐藏
    left_menu->setVisible(false);
    right_menu->setVisible(false);
    //将我方飞机的装备信息写入配置
    slow_on_menu->write_config();
    //创建第一关
    active_on_menu = new GameWindow(998);
    //代替原本的关卡
    HBoxLayout->replaceWidget(slow_on_menu,active_on_menu);
    //监控信号
    connect(this->active_on_menu,&GameWindow::quit,this,&MenuWindow::quit_GameWindow);
    delete slow_on_menu;
    slow_on_menu=nullptr;
}

void MenuWindow::quit_GameWindow()
{
    //将选关和仓库界面重新展示
    left_menu->setVisible(true);
    right_menu->setVisible(true);
    //重新创建出第0关
    slow_on_menu = new GameWindow(0);
    //代替原本的关卡
    HBoxLayout->replaceWidget(active_on_menu,slow_on_menu);
    //监控信号
//    connect(right_menu,&WarehouseWindow::update_equip,slow_on_menu,&GameWindow::update_equip);
    delete active_on_menu;
    active_on_menu=nullptr;


    delete right_menu;//销毁右侧窗口
    right_menu = new WarehouseWindow();//创建新的右侧窗口
    HBoxLayout->addWidget(right_menu,1);
    connect(right_menu,&WarehouseWindow::update_equip,slow_on_menu,&GameWindow::update_equip);


}

