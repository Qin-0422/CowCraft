#ifndef CREEPER_H
#define CREEPER_H

#include <QObject>
#include "Character.h"
#include "Steve.h"
#include <cstdlib>// 用于随机数生成
#include <cmath>// 用于计算距离
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QGraphicsPixmapItem>

class Creeper : public QObject, public Character, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Creeper();
    
    void chase(Steve* target);// 追逐目标函数
    void respawn();// 复活函数
    void takeDamage(int damage, Steve* attacker); // 被攻击时的处理函数
    
    // 设置活动范围
    void setHomeAreaX1(int x1) { m_HomeAreaX1 = x1; }
    void setHomeAreaX2(int x2) { m_HomeAreaX2 = x2; }
    void setHomeAreaY1(int y1) { m_HomeAreaY1 = y1; }
    void setHomeAreaY2(int y2) { m_HomeAreaY2 = y2; }
    
private:
    // Creeper活动范围
    int m_HomeAreaX1;
    int m_HomeAreaY1;
    int m_HomeAreaX2;
    int m_HomeAreaY2;
    int m_MoveDirction;
    int m_moveSpeed; // 移动速度
    
    bool isNear(Steve* target); // 判断目标是否在附近的函数
    bool isVeryClose(Steve* target); // 判断目标是否非常接近（50像素内）
    bool isFarEnough(Steve* target); // 判断目标是否足够远（100像素外）
    void knockBack(Steve* attacker); // 击退效果的处理函数
    void startExplosion(); // 开始爆炸准备
    void cancelExplosion(); // 取消爆炸
    void explode(); // 执行爆炸
    
    Steve* m_TargetSteve;
    QTimer* m_directionTimer; // 方向持续时间计时器
    QTimer* m_randomMoveTimer; // 定时器用于随机移动
    QTimer* m_respawnTimer; // 定时器用于复活
    QTimer* m_explosionTimer; // 爆炸倒计时定时器
    QTimer* m_KnockBackTimer; // 定时器用于击退效果
    
    int m_KnockBackSteps; // 击退效果的步数
    int m_KnockBackX; // 击退效果的X方向移动量
    int m_KnockBackY; // 击退效果的Y方向移动量
    int m_KnockBackInitialY; // 初始Y方向移动量
    
    bool m_isExploding; // 是否处于爆炸准备状态
    QString m_normalImagePath; // 正常状态图片路径
    QString m_explosionImagePathLeft; // 向左爆炸状态图片路径
    QString m_explosionImagePathRight; // 向右爆炸状态图片路径
    bool m_facingRight; // 是否面向右侧
    
public slots:
    void updateKnockBack(); // 更新击退效果的槽函数
    void randomMove(); // 随机移动函数
    void updatePosition(); // 更新移动位置的槽函数
    void update(Steve* target); // 更新Creeper状态的函数
    void onExplosionTimerFinished(); // 爆炸倒计时结束
};

#endif // CREEPER_H