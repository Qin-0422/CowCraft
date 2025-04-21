#ifndef MAP_LEVEL_2_H
#define MAP_LEVEL_2_H
#include<QWidget>
#include<QPainter>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QObject>
#include "Level_Map_Impl.h" // 引入实现类
#include "Steve.h"//引入人物类
#include "Zombie.h"//引入僵尸类
#include "Creeper.h"//引入爬行者类
#include "SpawnerBlock.h"//引入刷怪笼类

class Map_Level_2: public QObject, public Level_Map_Impl // 继承 QObject 和 Level_Map_Impl
{
    Q_OBJECT
public:
    Map_Level_2();
    ~Map_Level_2();
    
    // 更新视野范围函数
    void updateVisibility();

    bool isRoad(int x, int y) const override;
    bool isLadder(int x, int y) const override;
    bool isTransmit(int x, int y) const override;
    bool isTransmitBack(int x, int y) const override;
    bool isInteractive(int x, int y) const override;
    void setTrigger(int x, int y) override;
    void removeTrigger(int x, int y) override;
    bool Trigger_Arrow();
    QGraphicsScene* getScene() const override { return level_2_Scene; }
    int getBlockType(int x, int y) const { return map_2[y][x]; } // 获取指定位置的方块类型
    bool isBox(int x, int y) const;
    bool isTrigger(int x, int y) const override;
    bool isBoxOpened(int x, int y) const;
    void changeBoxState(int x, int y);
    int getBoxId(int x, int y) const; // 获取箱子ID
    void handlePlayerInteraction();
    bool isCraftingTable(int x, int y) const; // 检测指定位置是否为工作台
    void updateBlock(int x, int y, int newBlockType); // 添加更新地图块的方法，用于TNT爆炸

public:
    QGraphicsScene* level_2_Scene;
    Steve m_Steve;
    QTimer* m_ZombieTimer;
    QTimer* m_CreeperTimer;
private:
    QGraphicsItem* m_Arrow;
    QString filePath;
    vector<vector<int>> map_2;
    Zombie m_Zombie;
    Creeper m_Creeper;
    
    // 视野遮罩相关
    QGraphicsPathItem* m_visionMask; // 视野遮罩
    bool m_isInUpperArea; // 是否在上方区域(Y<250)
    QTimer* m_visibilityTimer; // 更新视野的定时器
};

#endif // MAP_LEVEL_2_H
