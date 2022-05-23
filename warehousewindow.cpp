#include "warehousewindow.h"

WarehouseWindow::WarehouseWindow()
{
//    this->setMinimumWidth(0);

    settings = new QSettings("Option.ini",QSettings::IniFormat);
    settings->beginGroup("player");
    player_main_weapon_type = settings->value("main_weapon_type").toInt();
    player_secon_weapon_type = settings->value("secon_weapon_type").toInt();
    player_wingman_type = settings->value("wingman_type").toInt();
    player_armor_type = settings->value("armor_type").toInt();
    settings->endGroup();// 结束创建

    settings->beginGroup("game_mod");
    int max_level = settings->value("max_level_mode").toInt();
    settings->endGroup();
    if(max_level==0)
    {
        main_weapon_total=2;//主武器动态创建按钮的个数
//        secon_weapon_total=1;//副武器动态创建按钮的个数
        wingman_total=1;//僚机动态创建按钮的个数
        armor_total=2;//装甲动态创建按钮的个数
    }
    else if(max_level==1)
    {
        main_weapon_total=3;//主武器动态创建按钮的个数
//        secon_weapon_total=1;//副武器动态创建按钮的个数
        wingman_total=2;//僚机动态创建按钮的个数
        armor_total=3;//装甲动态创建按钮的个数
    }
    else if(max_level==2)
    {
        main_weapon_total=3;//主武器动态创建按钮的个数
//        secon_weapon_total=1;//副武器动态创建按钮的个数
        wingman_total=3;//僚机动态创建按钮的个数
        armor_total=4;//装甲动态创建按钮的个数
    }
    settings->beginGroup("money");
    money1 = settings->value("money1").toInt();
    money2 = settings->value("money2").toInt();
    settings->endGroup();

//    QString level = QString("min_weapon%1").arg(player_main_weapon_type);
//    settings->beginGroup("weapon");//创建所属组，并开始创建
//    player_main_weapon_level = settings->value(level).toInt();
//    settings->endGroup();// 结束创建


    VBoxLayout = new QVBoxLayout(this); // 在主界面创建一个布局
    VBoxLayout->setMargin(0);//布局和所在窗口之间的间隙大小
    VBoxLayout->setSpacing(0);//布局内控件之间的间隙大小

    creat_up_lable();//初始化上部的“仓库”文本框
    creat_on_scrollarea();//初始化中间的滚动条窗口
    creat_money_widget();//初始化中间的滚动条窗口
    creat_down_widget();//初始化下部的按钮窗口，目前预留，没有功能

    VBoxLayout->addWidget(up_lable);
    VBoxLayout->addWidget(money_widget);
    VBoxLayout->addWidget(on_scrollarea); // 将滚动显示区域 添加到布局
    VBoxLayout->addWidget(down_widget);
}

void WarehouseWindow::creat_up_lable()//初始化上部的“仓库”文本框
{
    up_lable = new QLabel("仓库",this);
    up_lable->setStyleSheet("QLabel{background:#cccccc;font:40px;}");
    up_lable->setGeometry(100,100,200,100);
    up_lable->setAlignment(Qt::AlignCenter);//设置文本内容居中显示
    up_lable->setMinimumHeight(50);//这个控件最小的高度
}

void WarehouseWindow::creat_on_scrollarea()//初始化中间的滚动条窗口
{
    QWidget * arWidget = new QWidget; // 创建滚动显示区域幕布
    QVBoxLayout * l = new QVBoxLayout(arWidget); // 创建布局, 用于将控件嵌入幕布中
    l->setMargin(0);//布局和所在窗口之间的间隙大小
    l->setSpacing(0);//布局内控件之间的间隙大小
//    arWidget->setObjectName("arWidget");
//    arWidget->setStyleSheet("QWidget#arWidget{border: 2px solid #66ccff;}");

    QWidget *main_weapon_widget = new QWidget; // 创建主武器选项框
    main_weapon_widget->setObjectName("main_weapon_widget");
    main_weapon_widget->setStyleSheet("QWidget#main_weapon_widget{border: 1px solid #000000;}");
    QHBoxLayout * main_weapon_lay = new QHBoxLayout(main_weapon_widget); // 创建布局, 用于将控件嵌入幕布中
    main_weapon_lay->setMargin(0);//布局和所在窗口之间的间隙大小
    main_weapon_lay->setSpacing(0);//布局内控件之间的间隙大小
    int i=0;
    for(int flag=i;i-flag<main_weapon_total;i++)
    {
        btnPush=new QPushButton();
        //设置按钮在主对话框的位置
        btnPush->setMinimumHeight(40);//这个控件最小的高度
        btnPush->setMinimumWidth(40);//这个控件最小的宽度
        //设置按钮对象名字
//        btnPush->setObjectName("main_weapon"+QString::number(i));
        btnPush->setObjectName(QString::number(i-flag));
        //设置按钮显示文本
        if(i-flag==0)
        {
            btnPush->setText("无主武器");
        }
        else if(i-flag==1)
        {
            btnPush->setText("连射弹");
        }
        else if(i-flag==2)
        {
            btnPush->setText("散弹");
        }
        //把按钮放在对应的窗口内
        main_weapon_lay->addWidget(btnPush);
        //向QList添加成员项
        btnPushlist.append(btnPush);
        //关联信号槽函数
        connect(btnPushlist.at(i),SIGNAL(clicked()),this,SLOT(main_weapon_func()));
    }
    l->addWidget(main_weapon_widget);

    QWidget *secon_weapon_widget = new QWidget; // 创建副武器选项
    secon_weapon_widget->setObjectName("secon_weapon_widget");
    secon_weapon_widget->setStyleSheet("QWidget#secon_weapon_widget{border: 1px solid #000000;}");//显示widget的边框
    QHBoxLayout * secon_weapon_lay = new QHBoxLayout(secon_weapon_widget); // 创建布局, 用于将控件嵌入幕布中
    secon_weapon_lay->setMargin(0);//布局和所在窗口之间的间隙大小
    secon_weapon_lay->setSpacing(0);//布局内控件之间的间隙大小
//    for(int flag=i;i-flag<total;i++)
//    {
//        btnPush=new QPushButton();
//        //设置按钮在主对话框的位置
//        btnPush->setMinimumHeight(40);//这个控件最小的高度
//        btnPush->setMinimumWidth(40);//这个控件最小的宽度
//        //设置按钮对象名字
//        btnPush->setObjectName("secon_weapon"+QString::number(i));
//        //设置按钮显示文本
//        btnPush->setText("副武器"+QString::number(i-flag+1));
//        //把按钮放在对应的窗口内
//        secon_weapon_lay->addWidget(btnPush);
//        //向QList添加成员项
//        btnPushlist.append(btnPush);
//        //关联信号槽函数
//        connect(btnPushlist.at(i),SIGNAL(clicked()),this,SLOT(secon_weapon_func()));
//    }
    l->addWidget(secon_weapon_widget);

    QWidget *wingman_widget = new QWidget; // 创建僚机
    wingman_widget->setObjectName("wingman_widget");
    wingman_widget->setStyleSheet("QWidget#wingman_widget{border: 1px solid #000000;}");//显示widget的边框
    QHBoxLayout * wingman_lay = new QHBoxLayout(wingman_widget); // 创建布局, 用于将控件嵌入幕布中
    wingman_lay->setMargin(0);//布局和所在窗口之间的间隙大小
    wingman_lay->setSpacing(0);//布局内控件之间的间隙大小
    for(int flag=i;i-flag<wingman_total;i++)
    {
        btnPush=new QPushButton();
        //设置按钮在主对话框的位置
        btnPush->setMinimumHeight(40);//这个控件最小的高度
        btnPush->setMinimumWidth(40);//这个控件最小的宽度
        //设置按钮对象名字
//        btnPush->setObjectName("wingman"+QString::number(i));
        btnPush->setObjectName(QString::number(i-flag));
        //设置按钮显示文本
        if(i-flag==0)
        {
            btnPush->setText("无僚机");
        }
        else if(i-flag==1)
        {
            btnPush->setText("狙击导弹");
        }
        else if(i-flag==2)
        {
            btnPush->setText("连射弹");
        }
        //把按钮放在对应的窗口内
        wingman_lay->addWidget(btnPush);
        //向QList添加成员项
        btnPushlist.append(btnPush);
        //关联信号槽函数
        connect(btnPushlist.at(i),SIGNAL(clicked()),this,SLOT(wingman_func()));
    }
    l->addWidget(wingman_widget);

    QWidget *armor_widget = new QWidget; // 创建装甲
    armor_widget->setObjectName("armor_widget");
    armor_widget->setStyleSheet("QWidget#armor_widget{border: 1px solid #000000;}");//显示widget的边框
    QHBoxLayout * armor_lay = new QHBoxLayout(armor_widget); // 创建布局, 用于将控件嵌入幕布中
    armor_lay->setMargin(0);//布局和所在窗口之间的间隙大小
    armor_lay->setSpacing(0);//布局内控件之间的间隙大小
    for(int flag=i;i-flag<armor_total;i++)
    {
        btnPush=new QPushButton();
        //设置按钮在主对话框的位置
        btnPush->setMinimumHeight(40);//这个控件最小的高度
        btnPush->setMinimumWidth(40);//这个控件最小的宽度
        //设置按钮对象名字
//        btnPush->setObjectName("armor"+QString::number(i));
        btnPush->setObjectName(QString::number(i-flag));
        //设置按钮显示文本
        if(i-flag==0)
        {
            btnPush->setText("白绿装甲");
        }
        else if(i-flag==1)
        {
            btnPush->setText("白红装甲");
        }
        else if(i-flag==2)
        {
            btnPush->setText("绿装甲");
        }
        else if(i-flag==3)
        {
            btnPush->setText("黄装甲");
        }
        //把按钮放在对应的窗口内
        armor_lay->addWidget(btnPush);
        //向QList添加成员项
        btnPushlist.append(btnPush);
        //关联信号槽函数
        connect(btnPushlist.at(i),SIGNAL(clicked()),this,SLOT(armor_func()));
    }
    l->addWidget(armor_widget);


    on_scrollarea = new QScrollArea(); // 创建滚动显示区域
//    testar->setAlignment(Qt::AlignRight); // 设置内部控件居右
    on_scrollarea->setWidget(arWidget); // 将设置好的幕布嵌入到滚动显示区域
    on_scrollarea->setWidgetResizable(true);//滚动窗内的widget会填充整个窗口
//    on_scrollarea->setMinimumWidth(0);
}

void WarehouseWindow::creat_money_widget()//初始化下部的按钮窗口，目前预留，没有功能
{
    money_widget = new QWidget();
    QHBoxLayout *H = new QHBoxLayout(money_widget);

//    QString Lab = QString("金币：%1").arg(money1);
    money_Label1 = new QLabel(QString("金币：%1").arg(money1));
//    p1->setMinimumHeight(10);
    money_Label2 = new QLabel(QString("矿石：%1").arg(money2));
    H->addStretch();
    H->addWidget(money_Label1);
    H->addStretch();
    H->addWidget(money_Label2);
    H->addStretch();
//    H->addWidget(p3);
}


void WarehouseWindow::creat_down_widget()//初始化下部的按钮窗口，目前预留，没有功能
{
    down_widget = new QWidget();
    QHBoxLayout *H = new QHBoxLayout(down_widget);
    QPushButton *p1 = new QPushButton("主武器进阶");
//    p1->setMinimumHeight(10);
    QPushButton *p2 = new QPushButton("僚机升级");
//    QPushButton *p3 = new QPushButton("分解");
    H->addWidget(p1);
    H->addWidget(p2);
//    H->addWidget(p3);
    connect(p1,&QPushButton::clicked,this,&WarehouseWindow::main_weapon);
    connect(p2,&QPushButton::clicked,this,&WarehouseWindow::wingman);
}

void WarehouseWindow::main_weapon_func()//主武器列表内的按钮
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    //获取按钮显示文本
    QString text = btn->text();
    //获取按钮对象名称
    QString name = sender()->objectName();
//    QSettings *settings;//读写配置相关
    player_main_weapon_type = name.toInt();

    emit update_equip(player_main_weapon_type,player_secon_weapon_type,
                      player_wingman_type,player_armor_type);
}

void WarehouseWindow::secon_weapon_func()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    //获取按钮显示文本
    QString text = btn->text();
    //获取按钮对象名称
    QString name = sender()->objectName();
    //do something在这里进行其他事项处理
    qDebug()<<text;
    qDebug()<<name;
}

void WarehouseWindow::wingman_func()//僚机列表内的按钮
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    //获取按钮显示文本
    QString text = btn->text();
    //获取按钮对象名称
    QString name = sender()->objectName();
    player_wingman_type=name.toInt();

    emit update_equip(player_main_weapon_type,player_secon_weapon_type,
                      player_wingman_type,player_armor_type);
}

void WarehouseWindow::armor_func()//装甲列表内的按钮
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    //获取按钮显示文本
    QString text = btn->text();
    //获取按钮对象名称
    QString name = sender()->objectName();
    player_armor_type=name.toInt();

    emit update_equip(player_main_weapon_type,player_secon_weapon_type,
                      player_wingman_type,player_armor_type);
}

void WarehouseWindow::main_weapon()
{
    if(money1>=50)
    {
        money1-=50;
        money_Label1->setText(QString("金币：%1").arg(money1));
        emit level_up(1,player_main_weapon_type);
    }
}
void WarehouseWindow::wingman()
{

}

void WarehouseWindow::write_config()
{
    settings->beginGroup("money");//创建所属组，并开始创建
    settings->setValue("money1",money1);
    settings->endGroup();// 结束创建
}

void WarehouseWindow::paintEvent(QPaintEvent *event)//绘图事件
{

    QPainter painter(this);
    painter.setPen(QColor("black"));
    painter.drawRect(0,0,width()-1,height()-1);

//    QPainter painters(one);
//    painters.setPen(QColor("black"));
//    painters.drawRect(0,0,one->width()-1,one->height()-1);
}

