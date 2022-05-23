#include "mainwindow.h"

MainWindow::MainWindow()
{
    this->setWindowTitle("飞机大战");
    this->resize(400,600);//调整最外层窗口的大小
    this->setWindowIcon(QIcon(":/images/logo.ico"));//设置左上角程序图标
//    this->setMouseTracking(true);//是否持续判断鼠标位置
    lab = new QLabel("飞机大战",this);
    lab->setStyleSheet("QLabel{background:#cccccc;font:20px;}");
    lab->setGeometry(100,100,200,100);
    lab->setAlignment(Qt::AlignCenter);//设置文本内容居中显示
    QPushButton *but = new QPushButton("开始",this);
    but->setGeometry(150,450,100,50);
//    MP3 = new QMediaPlayer;//音乐播放器
//    MP3->setMedia(QUrl::fromEncoded("qrc:/music/starwars.mp3"));
//    MP3->setVolume(30);//设置音量
//    MP3->play();




    connect(but,&QPushButton::clicked,this,&MainWindow::open_GameWindow);

}

void MainWindow::open_GameWindow()
{
    Menu = new MenuWindow;
    this->hide();
    Menu->show();
    //监听Game类，假如有quit信号发出，把游戏界面关掉，回到主菜单
    connect(Menu->left_menu,&Chooselevel::quit,this,&MainWindow::quit_GameWindow);
}

void MainWindow::quit_GameWindow()
{
    this->show();
    delete Menu;
    Menu = NULL;
}

//void MainWindow::mousePressEvent(QMouseEvent *e)//实例一个鼠标事件的对象e
//{//点击
//    //单击鼠标事件函数，当点击鼠标时，触发该函数执行
//    int x = e->globalX();
//    int y = e->globalY();
//    qDebug().nospace()<<"绝对点击坐标：x=["<<x<<"],y=["<<y<<"]";

//    x=e->pos().x();
//    y=e->pos().y();
//    qDebug().nospace()<<"相对点击坐标：x=["<<x<<"],y=["<<y<<"]";
//}

//void MainWindow::mouseMoveEvent(QMouseEvent *event)//实例一个鼠标事件的对象e
//{//移动
//    QPoint p_ab = event->globalPos();//绝对坐标
//    QPoint p_re = event->pos();//相对坐标
//    int x = p_re.x();
//    int y = p_re.y();
//    QString str = QString("相对移动坐标:x=[%1],y=[%2]").arg(x).arg(y);
//    qDebug() <<str;
////    lab->setGeometry(x,y,100,100);
//}

MainWindow::~MainWindow()
{

}

