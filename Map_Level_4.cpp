#include "Map_Level_4.h"

Map_Level_4::Map_Level_4(): QObject(nullptr), Level_Map_Impl()
{
    //初始化地图元素
    level_4_Scene = new QGraphicsScene;
    filePath = "../Map/level_4.txt";
    map_4 = InitByFile(filePath);

    //添加背景
    QPixmap backgroundImage(":/back/img/background/level_4_back.jpg");
    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(backgroundImage);
    backgroundItem->setPos(0, 0);
    level_4_Scene->addItem(backgroundItem);

    int tileSize = 50;
    for(int row = 0;row< map_4.size() ;++row)
    {
        for(int col = 0;col<map_4[static_cast<std::vector<int>::size_type>(row)].size();++col)
        {
            QGraphicsRectItem *tile = new QGraphicsRectItem(col * tileSize, row * tileSize, tileSize, tileSize);
            switch (map_4[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)])
            {
            case Magma:
            {
                QPixmap pixmap(":/level_4/img/block/Magma.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_4_Scene->addItem(pixmapItem);
                break;
            }
            case Magma_dont_allow :
            {
                QPixmap pixmap(":/level_4/img/block/Magma.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_4_Scene->addItem(pixmapItem);
                break;
            }
            case Weird_Fungus_Rock:
            {
                QPixmap pixmap(":/level_4/img/block/Weird_Fungus_Rock.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_4_Scene->addItem(pixmapItem);
                break;
            }
            case TransmitBlockBack:
            {
                QPixmap pixmap(":/level_4/img/block/TransmitBlock.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_4_Scene->addItem(pixmapItem);
                break;
            }
            case Netherrack:
            {
                QPixmap pixmap(":/level_4/img/block/Netherrack.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_4_Scene->addItem(pixmapItem);
                break;
            }
            }
        }
    }

    //Steve在地图三的初始化
    m_Steve.setX(100);
    m_Steve.setY(800);
    level_4_Scene->addItem(&m_Steve);
    m_Steve.setMap(this);  // 设置Steve的地图指针

    // 添加血量UI到场景
    if (m_Steve.getHealthUI())
    {
        level_4_Scene->addItem(m_Steve.getHealthUI());
        m_Steve.getHealthUI()->setPos(20, 20); // 设置血量UI的位置
    }

    //Enderman初始化
    m_Enderman.setX(1700);
    m_Enderman.setY(700);
    level_4_Scene->addItem(&m_Enderman);
    m_Enderman.setSteve(&m_Steve);  // 设置Steve指针
    m_Enderman.setScene(level_4_Scene);  // 设置场景，用于添加血条UI



}

Map_Level_4::~Map_Level_4()
{
    delete level_4_Scene;
}

void Map_Level_4::checkMagmaDamage(int x, int y)
{
    const int col = x / 50;
    const int row = y / 50;
    
    // 检查是否在Magma方块上
    if (row >= 0 && row < map_4.size() && col >= 0 && col < map_4[row].size()) 
    {
        if (map_4[row][col] == Magma || map_4[row][col] == Magma_dont_allow) 
        {
            // 如果玩家站在Magma方块上，每0.5秒扣除3点生命值
            m_Steve.takeDamage(3);
        }
    }
}

bool Map_Level_4::isRoad(int x, int y) const
{
    const int col = x / 50;
    const int row = y / 50;
    
    if (row < 0 || row >= map_4.size() || col < 0 || col >= map_4[row].size())
    {
        return false;
    }
    
    const int blockType = map_4[row][col];
    
    // 只在Magma方块上时进行伤害检测
    if (blockType == Magma || blockType == Magma_dont_allow)
    {
        const_cast<Map_Level_4*>(this)->checkMagmaDamage(x, y);
    }
    
    return (blockType == 1 || blockType == 15 || blockType == 29);
}


bool Map_Level_4::isLadder(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_4.size() && col >= 0 && col < map_4[static_cast<std::vector<int>::size_type>(row)].size())
    {
        return map_4[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)] ==22;  // 修改为检查 map_1[row][col] 是否为 梯子
    }
    return false;
}

bool Map_Level_4::isTransmit(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_4.size() && col >= 0 && col < map_4[row].size()) {
        return map_4[row][col] == 9;  // 修改为检查 map_4[row][col] 是否为 9
    }
    return false;
}
bool Map_Level_4::isTransmitBack(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_4.size() && col >= 0 && col < map_4[static_cast<std::vector<int>::size_type>(row)].size())
    {
        return map_4[static_cast<std::vector<int>::size_type>(row)][col] == 15;  // 修改为检查 map_1[row][col] 是否为 15
    }
    return false;
}

bool Map_Level_4::isInteractive(int x, int y) const
{
    //空实现
}

bool Map_Level_4::isTrigger(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_4.size() && col >= 0 && col < map_4[static_cast<std::vector<int>::size_type>(row)].size())
    {
        return map_4[static_cast<std::vector<int>::size_type>(row)][col] == Trigger;
    }
    return false;
}

void Map_Level_4::removeTrigger(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_4.size() && col >= 0 && col < map_4[static_cast<std::vector<int>::size_type>(row)].size())
    {
        if (map_4[static_cast<std::vector<int>::size_type>(row)][col] == Trigger) {
            map_4[static_cast<std::vector<int>::size_type>(row)][col] = Road;
            QGraphicsPixmapItem* roadItem = new QGraphicsPixmapItem(QPixmap(":/level_4/img/block/Road.png"));
            roadItem->setPos(col * 50, row * 50);
            level_4_Scene->addItem(roadItem);
        }
    }
}

void Map_Level_4::setTrigger(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_4.size() && col >= 0 && col < map_4[row].size())
    {
        // 先检查该位置是否已经有Trigger
        if (map_4[row][col] != Trigger)
        {
            map_4[row][col] = Trigger;
            // 添加Trigger图片
            QPixmap pixmap(":/level_4/img/block/Trigger.png");
            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
            pixmapItem->setPos(col * 50, row * 50);
            pixmapItem->setData(0, "trigger"); // 设置标识符
            level_4_Scene->addItem(pixmapItem);
        }
    }
}


