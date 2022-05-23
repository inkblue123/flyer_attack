#ifndef PLANE_H
#define PLANE_H

#include <QObject>

class plane : public QObject
{
public:
    plane();
    int x;
    int y;
    int x0;
    int y0;
    double speed_x1;
    double speed_y1;
    int HP;
    int ATK_level;//飞机对象的攻击力等级
    int *game_width;//飞机对象活跃的区域大小
    int *game_height;//飞机对象活跃的区域大小
};

#endif // PLANE_H

