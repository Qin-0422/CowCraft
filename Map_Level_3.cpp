#include "Map_Level_3.h"
#include <QTimer>
#include "TNT.h"
#include <QGraphicsPixmapItem>
#include <QSet>
#include <QPair>
Map_Level_3::Map_Level_3(): QObject(nullptr), Level_Map_Impl()
{
    //初始化地图元素
    level_3_Scene = new QGraphicsScene;
    filePath = "../Map/level_3.txt";
    map_3 = InitByFile(filePath);

    //添加背景
    QPixmap backgroundImage(":/back/img/background/level_3_back.jpg");
    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(backgroundImage);
    backgroundItem->setPos(0, 0);
    level_3_Scene->addItem(backgroundItem);

    QPixmap backgroundImage_2(":/back/img/background/level_3_back_2.jpg");
    QGraphicsPixmapItem* backgroundItem_2 = new QGraphicsPixmapItem(backgroundImage_2);
    backgroundItem_2->setPos(800, 900);
    level_3_Scene->addItem(backgroundItem_2);

    int tileSize = 50;
    for(int row = 0;row< map_3.size() ;++row)
    {
        for(int col = 0;col<map_3[static_cast<std::vector<int>::size_type>(row)].size();++col)
        {
            QGraphicsRectItem *tile = new QGraphicsRectItem(col * tileSize, row * tileSize, tileSize, tileSize);
            switch (map_3[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)])
            {
            case bedrock:
            {
                QPixmap pixmap(":/level_3/img/block/bedrock.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case Road:
            {
                QPixmap pixmap("");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case obsidian:
            {
                QPixmap pixmap(":/level_3/img/block/obsidian.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }

            case sand:
            {
                QPixmap pixmap(":/level_3/img/block/sand.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case sand_gravel:
            {
                QPixmap pixmap(":/level_3/img/block/sand_gravel.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case box:
            {
                QPixmap pixmap(":/level_3/img/block/box.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case TNT:
            {
                QPixmap pixmap(":/level_3/img/block/TNT.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case box_open:
            {
                QPixmap pixmap(":/level_3/img/block/box_open.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case TransmitBlockBack:
            {
                QPixmap pixmap(":/level_3/img/block/TransmitBlock.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case ladder_sand:
            {
                QPixmap pixmap(":/level_3/img/block/ladder_sand.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case Stone:
            {
                QPixmap pixmap(":/level_3/img/block/Stone.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            case Touch_pressure_plate:
            {
                QPixmap pixmap(":/level_3/img/block/Touch_pressure_plate.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_3_Scene->addItem(pixmapItem);
                break;
            }
            }
        }
    }
//Steve在地图三的初始化
    m_Steve.setX(150);
    m_Steve.setY(550);
    level_3_Scene->addItem(&m_Steve);

    m_Steve.setMap(this);  // 设置Steve的地图指针

//僵尸1
    m_Zombie_1.setX(450);
    m_Zombie_1.setY(550);
    level_3_Scene->addItem(&m_Zombie_1);
    // 创建定时器，每50毫秒更新一次僵尸状态
    m_ZombieTimer_1 = new QTimer(this);
    connect(m_ZombieTimer_1, &QTimer::timeout, this, [this]() {
        m_Zombie_1.update(&m_Steve);
    });
    m_ZombieTimer_1->start(50);
//僵尸2
    m_Zombie_2.setX(550);
    m_Zombie_2.setY(550);
    level_3_Scene->addItem(&m_Zombie_2);
    // 创建定时器，每50毫秒更新一次僵尸状态
    m_ZombieTimer_2 = new QTimer(this);
    connect(m_ZombieTimer_2, &QTimer::timeout, this, [this]() {
        m_Zombie_2.update(&m_Steve);
    });
    m_ZombieTimer_2->start(50);

    // 添加血量UI到场景
    if (m_Steve.getHealthUI())
    {
        level_3_Scene->addItem(m_Steve.getHealthUI());
        m_Steve.getHealthUI()->setPos(20, 20); // 设置血量UI的位置
    }
}
Map_Level_3::~Map_Level_3()
{
    delete level_3_Scene;
}

bool Map_Level_3::isRoad(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size())
    {
        return ( map_3[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)] == 1||map_3[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)] ==9||map_3[static_cast<std::vector<int>::size_type>(row)][col] ==22||map_3[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)] ==15||map_3[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)] ==20);  // 修改为检查 map_1[row][col] 是否为 路,传送门，梯子
    }
    return false;
}
bool Map_Level_3::isLadder(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size())
    {
        return map_3[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)] ==22;  // 修改为检查 map_1[row][col] 是否为 梯子
    }
    return false;
}
bool Map_Level_3::isTransmit(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size())
    {
        return map_3[static_cast<std::vector<int>::size_type>(row)][col] == 9;  // 修改为检查 map_1[row][col] 是否为 9
    }
    return false;
}
bool Map_Level_3::isTransmitBack(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size())
    {
        return map_3[static_cast<std::vector<int>::size_type>(row)][col] == TransmitBlockBack;  // 使用枚举值检查是否为传送门
    }
    return false;
}

bool Map_Level_3::isTouch_pressure_plate(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    
    // 创建当前触碰板位置的键值对
    QPair<int, int> platePosition(col, row);
    
    // 检查这个触碰板是否已经被触发过
    if (triggeredPressurePlates.contains(platePosition))
    {
        // 如果已经触发过，直接返回true但不执行任何操作
        return true;
    }
    
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && 
        col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size())
    {
        if (map_3[static_cast<std::vector<int>::size_type>(row)][col] == Touch_pressure_plate)
        {
            // 将此触碰板位置添加到已触发集合中
            triggeredPressurePlates.insert(platePosition);
            
            // 设置全局触发状态（保留以兼容现有代码）
            is_Trigger = true;
            
            // 检查附近是否有TNT
            const int checkRange = 2; // 检查周围2格范围
            for (int i = -checkRange; i <= checkRange; ++i)
            {
                for (int j = -checkRange; j <= checkRange; ++j)
                {
                    int checkRow = row + i;
                    int checkCol = col + j;
                    if (checkRow >= 0 && static_cast<std::vector<int>::size_type>(checkRow) < map_3.size() &&
                            checkCol >= 0 && checkCol < map_3[static_cast<std::vector<int>::size_type>(checkRow)].size())
                    {
                        if (map_3[static_cast<std::vector<int>::size_type>(checkRow)][checkCol] == TNT)
                        {
                            // 触发TNT爆炸
                            triggerTNTExplosion(checkCol * 50, checkRow * 50);
                        }
                    }
                }
            }
            return true;
        }
    }
    return false;
}

void Map_Level_3::triggerTNTExplosion(int x, int y)
{
    const int tileSize = 50;
    int col = x / tileSize;
    int row = y / tileSize;

    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() &&
        col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size()) {
        if (map_3[static_cast<std::vector<int>::size_type>(row)][col] == TNT) {
            // 创建TNT实例并设置位置
            class TNT* tnt = new class TNT;
            tnt->setPos(col * tileSize, row * tileSize);
            tnt->setMap(this);

            // 移除原有的TNT图片项
            QList<QGraphicsItem*> items = level_3_Scene->items(QRectF(x, y, tileSize, tileSize));
            for (QGraphicsItem* item : items) {
                if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item)) {
                    level_3_Scene->removeItem(pixmapItem);
                    delete pixmapItem;
                }
            }

            // 将TNT添加到场景
            level_3_Scene->addItem(tnt);

            // 开始爆炸准备
            tnt->startExplosionPreparation();
        }
    }
}



bool Map_Level_3::removeTNT(int x, int y)
{
    const int tileSize = 50;
    int col = x / tileSize;
    int row = y / tileSize;

    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() &&
        col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size()) {
        if (map_3[static_cast<std::vector<int>::size_type>(row)][col] == TNT) {
            // 将TNT方块替换为空路
            map_3[static_cast<std::vector<int>::size_type>(row)][col] = Road;

            // 移除TNT图形项
            QList<QGraphicsItem*> items = level_3_Scene->items(QRectF(col * tileSize, row * tileSize, tileSize, tileSize));
            for (QGraphicsItem* item : items) {
                if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item)) {
                    level_3_Scene->removeItem(pixmapItem);
                    delete pixmapItem;
                }
            }

            // 添加空路图形项
            QPixmap roadPixmap("");
            QGraphicsPixmapItem* roadItem = new QGraphicsPixmapItem(roadPixmap);
            roadItem->setPos(col * tileSize, row * tileSize);
            level_3_Scene->addItem(roadItem);

            return true;
        }
    }
    return false;
}
bool Map_Level_3::isInteractive(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size())
    {
        // 检查是否为可交互物品（未打开的箱子等）
        return map_3[static_cast<std::vector<int>::size_type>(row)][col] == box;
    }
    return false;
}
bool Map_Level_3::isBox(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size()) {
        return map_3[static_cast<std::vector<int>::size_type>(row)][col] == box;
    }
    return false;
}

bool Map_Level_3::isBoxOpened(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size()) {
        // 检查箱子是否已经打开
        return map_3[static_cast<std::vector<int>::size_type>(row)][col] == box_open;
    }
    return false;
}

bool Map_Level_3::isTrigger(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size()) {
        return map_3[static_cast<std::vector<int>::size_type>(row)][col] == Trigger;
    }
    return false;
}

void Map_Level_3::removeTrigger(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size()) {
        if (map_3[static_cast<std::vector<int>::size_type>(row)][col] == Trigger) {
            map_3[static_cast<std::vector<int>::size_type>(row)][col] = Road;
            QGraphicsPixmapItem* roadItem = new QGraphicsPixmapItem(QPixmap(":/level_3/img/block/Road.png"));
            roadItem->setPos(col * 50, row * 50);
            level_3_Scene->addItem(roadItem);
        }
    }
}

void Map_Level_3::setTrigger(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_3.size() && col >= 0 && col < map_3[row].size())
    {
        // 先检查该位置是否已经有Trigger
        if (map_3[row][col] != Trigger) {
            map_3[row][col] = Trigger;
            // 添加Trigger图片
            QPixmap pixmap(":/level_3/img/block/Trigger.png");
            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
            pixmapItem->setPos(col * 50, row * 50);
            pixmapItem->setData(0, "trigger"); // 设置标识符
            level_3_Scene->addItem(pixmapItem);
        }
    }
}

void Map_Level_3::changeBoxState(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_3.size() && col >= 0 && col < map_3[static_cast<std::vector<int>::size_type>(row)].size())
    {
        if (map_3[static_cast<std::vector<int>::size_type>(row)][col] == box)
        {
            // 将箱子状态改为打开
            map_3[static_cast<std::vector<int>::size_type>(row)][col] = box_open;

            // 移除旧的箱子图形项
            QList<QGraphicsItem*> items = level_3_Scene->items(QRectF(col * 50, row * 50, 50, 50));
            for (QGraphicsItem* item : items)
            {
                if (QGraphicsPixmapItem* pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item))
                {
                    level_3_Scene->removeItem(item);
                    delete item;
                    break;
                }
            }

            // 添加新的打开状态图形项
            QPixmap pixmap(":/level_3/img/block/box_open.png");
            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
            pixmapItem->setPos(col * 50, row * 50);
            level_3_Scene->addItem(pixmapItem);
        }
    }
}

int Map_Level_3::getBoxId(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    QPointF boxPos(col * 50, row * 50);

    // 检查是否为箱子5的位置
    if (QLineF(boxPos, QPointF(800, 1050)).length() < 25)
    {
        return 5;
    }
    // 检查是否为箱子6的位置
    else if (QLineF(boxPos, QPointF(1400, 1050)).length() < 25)
    {
        return 6;
    }
    return 0; // 不是已知的箱子位置
}


void Map_Level_3::updateBlock(int x, int y, int newBlockType)
{
    if (x >= 0 && x < map_3[0].size() && y >= 0 && y < map_3.size())
    {
        map_3[y][x] = newBlockType;

        // 保存当前位置的所有图形项
        QList<QGraphicsItem*> itemsAtPos = level_3_Scene->items(QRectF(x * 50, y * 50, 50, 50));

        // 移除当前位置的旧方块
        for (QGraphicsItem* item : itemsAtPos)
        {
            if (dynamic_cast<QGraphicsPixmapItem*>(item) && item->pos() == QPointF(x * 50, y * 50))
            {
                level_3_Scene->removeItem(item);
                delete item;
            }
        }

        // 创建新的方块图形项
        QGraphicsPixmapItem* newItem = nullptr;
        if (newBlockType == TransmitBlock)
        {
            QPixmap pixmap(":/level_3/img/block/TransmitBlock.png");
            newItem = new QGraphicsPixmapItem(pixmap);
            newItem->setZValue(1); // 确保传送门显示在背景之上
        }

        if (newItem)
        {
            newItem->setPos(x * 50, y * 50);
            level_3_Scene->addItem(newItem);
        }
    }
}
