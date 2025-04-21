#ifndef ENDERMAN_H
#define ENDERMAN_H

#include <QObject>
#include "Character.h"
#include "Steve.h"
#include <cstdlib>
#include <cmath>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QGraphicsPixmapItem>
#include "HealthUI.h"

class Enderman : public QObject, public Character, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Enderman();
    ~Enderman();
    
    // 设置Steve指针，用于追踪和瞬移
    void setSteve(Steve* steve) { m_Steve = steve; }
    
    // 受到伤害处理函数
    void takeDamage(int damage, Steve* attacker);
    
    // 获取血条UI
    QGraphicsLineItem* getHealthUI() const { return m_healthBar; }
    
    // 设置地图四场景，用于添加血条UI
    void setScene(QGraphicsScene* scene);
    void knockBack(Steve* attacker);
private:
    // 状态枚举
    enum State {
        NORMAL,     // 正常状态（未被激怒）
        ANGRY,      // 愤怒状态（被激怒）
        HURT,       // 受伤状态
        TELEPORTING // 瞬移状态
    };
    
    // 当前状态
    State m_currentState;
    
    // 移动方向和速度
    int m_moveDirection;
    int m_moveSpeed;
    int m_angryMoveSpeed; // 愤怒状态下的移动速度
    
    // 目标Steve
    Steve* m_Steve;
    
    // 计时器
    QTimer* m_moveTimer;       // 移动计时器
    QTimer* m_randomMoveTimer; // 随机移动方向计时器
    QTimer* m_hurtTimer;       // 受伤状态计时器
    QTimer* m_attackTimer;     // 攻击冷却计时器
    QTimer* m_teleportTimer;   // 瞬移计时器
    
    // 血量相关
    int m_maxHealth;
    QGraphicsLineItem* m_healthBar; // 血条
    QGraphicsLineItem* m_healthBarBackground; // 血条背景
    
    // 瞬移相关
    int m_teleportThreshold;   // 瞬移触发的血量阈值
    int m_lastTeleportHealth;  // 上次瞬移时的血量
    
    // 图片路径
    QString m_normal_Left_ImagePath;
    QString m_normal_Right_ImagePath;
    QString m_angry_Left_0_ImagePath;
    QString m_angry_Left_1_ImagePath;
    QString m_angry_Left_2_ImagePath;
    QString m_angry_Right_0_ImagePath;
    QString m_angry_Right_1_ImagePath;
    QString m_angry_Right_2_ImagePath;
    QString m_hurt_Left_ImagePath;
    QString m_hurt_Right_ImagePath;
    QString m_attack_Left_ImagePath;
    QString m_attack_Right_ImagePath;

    
    // 动画相关
    int m_animationFrame;     // 当前动画帧
    QTimer* m_animationTimer; // 动画更新计时器
    
    // 私有方法
    void randomMove();         // 随机移动
    void chaseSteve();         // 追逐Steve
    void attackSteve();        // 攻击Steve
    void teleportBehindSteve(); // 瞬移到Steve身后
    void updateState();        // 更新状态
    void updateImage();        // 更新图像
    
public slots:
    void update();             // 更新Enderman状态
    void onHurtTimerFinished(); // 受伤状态结束
    void onRandomMoveTimeout(); // 随机移动方向更新
};

#endif // ENDERMAN_H
