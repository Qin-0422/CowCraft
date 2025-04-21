#ifndef LEVEL_1_MAP_H
#define LEVEL_1_MAP_H
#include <QObject>
#include <QFile>
#include <QDebug>
#include <vector>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
using namespace  std;
enum Map_Element
{
    bedrock=0,
    Road=1,
    soil=2,
    grass=3,
    Cobblestone=4,//原石
    Log=5,
    leaf,
    Plank,
    Stone,
    TransmitBlock,
    craftingTable=10,
    MossyStone,//苔藓石块
    Ladder,
    LauncherBlock,
    box,
    TransmitBlockBack=15,//传送回去
    box_open,
    Monster_Spawner,//刷怪笼
    sand,
    sand_gravel,
    Touch_pressure_plate=20,
    obsidian,
    ladder_sand,
    Trigger,
    TNT,
    TNT_willing=25,
    Weird_Fungus_Rock,//诡异菌岩
    Netherrack,//地狱岩
    Magma_dont_allow,//不可通过的岩浆区域
    Magma,//岩浆
};

class Level_Map
{
public:
    Level_Map();
    virtual ~Level_Map() = default;
    vector<vector<int>> InitByFile(const QString &filePath);
    virtual bool isRoad(int x, int y) const = 0;
    virtual bool isLadder(int x, int y) const = 0;
    virtual bool isTransmit(int x, int y) const = 0;
    virtual bool isTransmitBack(int x, int y) const = 0;
    virtual bool isInteractive(int x, int y) const = 0;
    virtual bool isTrigger(int x, int y) const = 0;
    virtual void removeTrigger(int x, int y) = 0;
    virtual void setTrigger(int x, int y) = 0;
    virtual QGraphicsScene* getScene() const = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual void updateBlock(int x, int y, int blockType) = 0;
};
#endif // LEVEL_1_MAP_H
