#ifndef LEVEL_MAP_IMPL_H
#define LEVEL_MAP_IMPL_H
#include "Level_Map.h"
#include <QGraphicsScene>

class Level_Map_Impl : public Level_Map
{
public:
    Level_Map_Impl();
    virtual ~Level_Map_Impl(); // 添加虚析构函数
    virtual bool isRoad(int x, int y) const override = 0;
    virtual bool isLadder(int x, int y) const override = 0;
    virtual bool isTransmit(int x, int y) const override = 0;
    virtual bool isTransmitBack(int x, int y) const override = 0;
    virtual bool isInteractive(int x, int y) const override = 0;
    virtual bool isTrigger(int x, int y) const override { return false; }
    virtual void removeTrigger(int x, int y) override = 0;
    virtual void setTrigger(int x, int y) override = 0;
    virtual QGraphicsScene* getScene() const override = 0;
    virtual int getWidth() const override { return map_data.empty() ? 0 : map_data[0].size(); }
    virtual int getHeight() const override { return map_data.size(); }
    virtual void updateBlock(int x, int y, int blockType) override = 0;
protected:
    vector<vector<int>> map_data;
    QGraphicsScene* scene;
};

#endif // LEVEL_MAP_IMPL_H
