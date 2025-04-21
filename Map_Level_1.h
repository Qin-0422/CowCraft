#ifndef MAP_LEVEL_1_H
#define MAP_LEVEL_1_H
#include<QWidget>
#include<QPainter>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "Level_Map_Impl.h" // 引入实现类
#include "Steve.h"
#include "Villager_Fake.h"//引入村民类
#include <QTimer>

class Map_Level_1: public Level_Map_Impl // 继承 Level_Map_Impl
{
public:
    Map_Level_1();
    ~Map_Level_1();

    bool isRoad(int x, int y) const override;
    bool isLadder(int x, int y) const override;
    bool isTransmit (int x, int y) const override;
    bool isTransmitBack (int x, int y) const override;
    bool isInteractive(int x, int y) const override;
    bool isTrigger(int x, int y) const override;
    void removeTrigger(int x, int y) override;
    void setTrigger(int x, int y) override;
    void updateMap();
    void updateBlock(int x, int y, int newBlockType); // 添加更新地图块的方法
    int getBlockType(int x, int y) const { return map_1[y][x]; } // 获取指定位置的方块类型
    QGraphicsScene* getScene() const override { return level_1_Scene; }
    bool isBox(int x, int y) const; // 检查指定位置是否为箱子
    bool isBoxOpened(int x, int y) const; // 检查箱子是否已经打开
    void changeBoxState(int x, int y); // 改变箱子状态
    int getBoxId(int x, int y) const; // 获取箱子ID
    void handlePlayerInteraction();

    QGraphicsScene* level_1_Scene;
    Steve m_Steve;
    QTimer* m_UpdateTimer;

private:
    QString filePath ;
    vector<vector<int>> map_1;
    Villager_F m_villager_fake;

};

#endif // MAP_LEVEL_1_H
