#include "chooselevel.h"

Chooselevel::Chooselevel()
{
    VBoxLayout = new QVBoxLayout(this);
//    HBoxLayout->setMargin(0);//布局和所在窗口之间的间隙大小
//    HBoxLayout->setSpacing(0);//布局内控件之间的间隙大小
    up_widget();

    down_widget();

    VBoxLayout->addWidget(up,1);
    VBoxLayout->addWidget(down,1);

    quitbut = new QPushButton("返回主菜单",this);
//    level_1 = new QPushButton("第一关",this);
//    level_2 = new QPushButton("第二关",this);
    //按下右下角的退出按钮，就关闭游戏界面
    connect(quitbut,&QPushButton::clicked,this,&Chooselevel::quitwindow);
}

void Chooselevel::up_widget()//初始化下部的按钮窗口，目前预留，没有功能
{
    up = new QWidget();
#ifdef DEBUG
    up->setObjectName("up");
    up->setStyleSheet("QWidget#up{border: 1px solid #000000;}");
#endif
    QVBoxLayout *V = new QVBoxLayout(up);

    QLabel *lable = new QLabel("关卡模式");
    lable->setStyleSheet("QLabel{background:#ffffff;font:40px;}");//设置文本框的背景颜色，字体大小

    lable->setAlignment(Qt::AlignCenter);//设置文本内容居中显示
    lable->setMinimumHeight(50);//这个控件最小的高度

    QHBoxLayout *H = new QHBoxLayout();
    level_1 = new QPushButton("第一关",this);
    level_1->setMinimumSize(QSize(80,50));
    level_2 = new QPushButton("第二关",this);
    level_2->setMinimumSize(QSize(80,50));
    H->addWidget(level_1);
    H->addWidget(level_2);
    H->addStretch();

    V->addWidget(lable);
    V->addLayout(H);
    V->addStretch();

}

void Chooselevel::down_widget()
{
    down = new QWidget();
#ifdef DEBUG
    down->setObjectName("down");
    down->setStyleSheet("QWidget#down{border: 1px solid #000000;}");
#endif
    QVBoxLayout *V = new QVBoxLayout(down);

    QLabel *lable = new QLabel("无尽模式");
#ifdef DEBUG
    lable->setObjectName("lable1");
    lable->setStyleSheet("QLabel{background:#ffffff;font:40px;}"//设置文本框的背景颜色，字体大小
                        "QWidget#lable1{border: 1px solid #000000;}");//显示文本框的边框
#else
    lable->setStyleSheet("QLabel{background:#ffffff;font:40px;}");//设置文本框的背景颜色，字体大小
#endif
    lable->setAlignment(Qt::AlignCenter);//设置文本内容居中显示
    lable->setMinimumHeight(50);//这个控件最小的高度

    QHBoxLayout *H = new QHBoxLayout();
    endless_998 = new QPushButton("普通",this);
    endless_998->setMinimumSize(QSize(80,50));
    endless_999 = new QPushButton("困难",this);
    endless_999->setMinimumSize(QSize(80,50));
    H->addWidget(endless_998);
    H->addWidget(endless_999);
    H->addStretch();

    V->addWidget(lable);
    V->addLayout(H);
    V->addStretch();
}


void Chooselevel::paintEvent(QPaintEvent *event)//绘图事件
{
    QPainter painter(this);
    painter.setPen(QColor("black"));
    painter.drawRect(0,0,width()-1,height()-1);
    quitbut->setGeometry(width()-100,height()-50,100,50);
//    level_1->setGeometry(width()/3-100/2,250,80,50);
//    level_2->setGeometry(width()*2/3-100/2,250,80,50);
}

void Chooselevel::quitwindow()//退出游戏窗口
{
    emit this->quit();
}
