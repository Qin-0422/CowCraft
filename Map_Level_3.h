#ifndef MAP_LEVEL_3_H
#define MAP_LEVEL_3_H
#include<QWidget>
#include<QPainter>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>
#include "Level_Map_Impl.h" // 引入实现类
#include "Steve.h"//引入人物类
#include "Zombie.h"//引入僵尸类



class Map_Level_3: public QObject, public Level_Map_Impl // 继承 QObject 和 Level_Map_Impl
{
    Q_OBJECT
public:
    virtual int getWidth() const override { return 40; }  // 返回固定宽度40格（2000像素）
    virtual int getHeight() const override { return 26; }  // 返回固定高度26格（1300像素）
    Map_Level_3();
    ~Map_Level_3();

    bool isRoad(int x, int y) const override;
    bool isLadder(int x, int y) const override;
    bool isTransmit(int x, int y) const override;
    bool isTransmitBack(int x, int y) const override;
    bool isInteractive(int x, int y) const override;
    bool isTrigger(int x, int y) const override;
    void removeTrigger(int x, int y) override;
    void setTrigger(int x, int y) override;

    QGraphicsScene* getScene() const override { return level_3_Scene; }
    bool isBox(int x, int y) const;
    bool isBoxOpened(int x, int y) const;
    void changeBoxState(int x, int y);    
    int getBoxId(int x, int y) const;
    bool isTouch_pressure_plate(int x, int y) ;
    void triggerTNTExplosion(int x, int y);
    bool removeTNT(int x, int y);
    void updateBlock(int x, int y, int newBlockType);
public:
    QGraphicsScene* level_3_Scene;
    Steve m_Steve;
    QTimer* m_ZombieTimer_1;
    QTimer* m_ZombieTimer_2;
    bool is_Trigger = false; // 全局触发状态（保留以兼容现有代码）
private:
    QString filePath;
    vector<vector<int>> map_3;
    Zombie m_Zombie_1;
    Zombie m_Zombie_2;
    // 存储已触发的触碰板位置
    QSet<QPair<int, int>> triggeredPressurePlates;

};

#endif // MAP_LEVEL_3_H
