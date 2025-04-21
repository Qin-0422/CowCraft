#ifndef MAP_LEVEL_4_H
#define MAP_LEVEL_4_H
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
#include "Enderman.h"

class Map_Level_4: public QObject, public Level_Map_Impl
{
    Q_OBJECT
public:
    Map_Level_4();
    ~Map_Level_4();

    bool isRoad(int x, int y) const override;
    void checkMagmaDamage(int x, int y);
    bool isLadder(int x, int y) const override;
    bool isTransmit(int x, int y) const override;
    bool isTransmitBack(int x, int y) const override;
    bool isInteractive(int x, int y) const override;
    bool isTrigger(int x, int y) const override;
    void removeTrigger(int x, int y) override;
    void setTrigger(int x, int y) override;
    void updateBlock(int x, int y, int newBlockType){}
    QGraphicsScene* getScene() const override { return level_4_Scene; }

public:
    QGraphicsScene* level_4_Scene;
    Steve m_Steve;
    Enderman m_Enderman;
private:
    QString filePath;
    vector<vector<int>> map_4;
};

#endif // MAP_LEVEL_4_H
