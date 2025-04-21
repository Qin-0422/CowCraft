#include "Map_Level_1.h"

Map_Level_1::Map_Level_1()
{
    //初始化地图元素
    level_1_Scene = new QGraphicsScene;
    filePath = "../Map/level_1.txt";
    map_1 = InitByFile(filePath);
    map_data = map_1; // 将地图数据同时存储到基类的map_data中
    level_1_Scene->addPixmap(QPixmap(":/back/img/background/leve_1_back.jpg"));
    int tileSize = 50;
    for(int row = 0;row<map_1.size();++row)
    {
        for(int col = 0;col<map_1[row].size();++col)
        {
            QGraphicsRectItem *tile = new QGraphicsRectItem(col * tileSize, row * tileSize, tileSize, tileSize);
            switch (map_1[row][col])
            {
            case bedrock:
            {
                QPixmap pixmap(":/level_1/img/block/bedrock.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case Road:
            {
                QPixmap pixmap(" ");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case soil:
            {
                QPixmap pixmap(":/level_1/img/block/soil.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case grass:
            {
                QPixmap pixmap(":/level_1/img/block/grass.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case Cobblestone:
            {
                QPixmap pixmap(":/level_1/img/block/Cobblestone.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case Log:
            {
                QPixmap pixmap(":/level_1/img/block/Log.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case leaf:
            {
                QPixmap pixmap(":/level_1/img/block/leaf.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case Plank:
            {
                QPixmap pixmap(":/level_1/img/block/Planks.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case TransmitBlock:
            {
                QPixmap pixmap(":/level_1/img/block/TransmitBlock.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            case box:
            {
                QPixmap pixmap(":/level_1/img/block/box.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_1_Scene->addItem(pixmapItem);
                break;
            }
            }
        }
    }

    m_Steve.setX(300);
    m_Steve.setY(950);
    level_1_Scene->addItem(&m_Steve);
    m_Steve.setMap(this);  // 设置Steve的地图指针

    m_villager_fake.setX(800);
    m_villager_fake.setY(950);
    m_villager_fake.setPosition(800,950);
    m_villager_fake.setSteve(&m_Steve);
    level_1_Scene->addItem(&m_villager_fake);
    // 添加血量UI到场景
    if (m_Steve.getHealthUI())
    {
        level_1_Scene->addItem(m_Steve.getHealthUI());
        m_Steve.getHealthUI()->setPos(20, 20); // 设置血量UI的位置
    }
}


Map_Level_1::~Map_Level_1()
{
    delete level_1_Scene;
}

bool Map_Level_1::isRoad(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size())
    {
        return ( map_1[row][col] == 1||map_1[row][col] ==9||map_1[row][col] ==12||map_1[row][col] ==23);  // 修改为检查 map_1[row][col] 是否为 路,传送门，梯子
    }
    return false;
}
bool Map_Level_1::isLadder(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size())
    {
        return  map_1[row][col] == 12;  // 修改为检查 map_1[row][col] 是否为 梯子
    }
    return false;
}
bool Map_Level_1::isTransmit(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size()) {
        return map_1[row][col] == 9;  // 修改为检查 map_1[row][col] 是否为 9
    }
    return false;
}
bool Map_Level_1::isTransmitBack(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size()) {
        return map_1[row][col] == 15;  // 修改为检查 map_1[row][col] 是否为 15
    }
    return false;
}

bool Map_Level_1::isTrigger(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size()) {
        return map_1[row][col] == Trigger;
    }
    return false;
}

void Map_Level_1::setTrigger(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size())
    {
        // 先检查该位置是否已经有Trigger
        if (map_1[row][col] != Trigger) {
            map_1[row][col] = Trigger;
            // 添加Trigger图片
            QPixmap pixmap(":/level_1/img/block/Trigger.png");
            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
            pixmapItem->setPos(col * 50, row * 50);
            pixmapItem->setData(0, "trigger"); // 设置标识符
            level_1_Scene->addItem(pixmapItem);
        }
    }
}

void Map_Level_1::removeTrigger(int x, int y)
{
    //空实现
}


bool Map_Level_1::isInteractive(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size()) {
        // 检查是否为可交互物品（箱子等）
        return map_1[row][col] == box;
    }
    return false;
}

bool Map_Level_1::isBox(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size()) {
        return map_1[row][col] == box;
    }
    return false;
}

bool Map_Level_1::isBoxOpened(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size()) {
        return map_1[row][col] == box_open;
    }
    return false;
}

void Map_Level_1::changeBoxState(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_1.size() && col >= 0 && col < map_1[row].size() && map_1[row][col] == box) {
        // 将箱子状态改为已打开
        map_1[row][col] = box_open;
        
        // 更新箱子图片为打开状态
        QPixmap openedBoxPixmap(":/level_1/img/block/box_open.png");
        QGraphicsPixmapItem* openedBoxItem = new QGraphicsPixmapItem(openedBoxPixmap);
        openedBoxItem->setPos(col * 50, row * 50);
        level_1_Scene->addItem(openedBoxItem);
    }
}

int Map_Level_1::getBoxId(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    QPointF boxPos(col * 50, row * 50);

    // 检查是否为箱子3的位置
    if (QLineF(boxPos, QPointF(300, 700)).length() < 25) {
        return 3;
    }
    // 检查是否为箱子4的位置
    else if (QLineF(boxPos, QPointF(1500, 500)).length() < 25) {
        return 4;
    }
    return 0; // 不是已知的箱子位置
}

void Map_Level_1::handlePlayerInteraction()
{
    // 获取玩家位置
       QPointF playerPos = m_Steve.pos();

       // 检查玩家是否靠近箱子3
       QPointF chest1Pos = QPointF(300, 700);
       if (QLineF(playerPos, chest1Pos).length() < 50) // 50是交互距离
       {
           m_Steve.interactWithChest(3);
       }

       // 检查玩家是否靠近箱子4
       QPointF chest2Pos = QPointF(1500, 500);
       if (QLineF(playerPos, chest2Pos).length() < 50)
       {
           m_Steve.interactWithChest(4);
       }
}

void Map_Level_1::updateBlock(int x, int y, int newBlockType)
{
    // 添加边界检查
    if (x < 0 || x >= 40 || y < 0 || y >= 26)
        return;
    
    // 检查是否是Cobblestone，如果是则更新为Road
    if (map_1[y][x] == Cobblestone && newBlockType == Road)
    {
        // 更新地图数据
        map_1[y][x] = Road;
        
        // 移除原有的Cobblestone图片项
        QList<QGraphicsItem*> itemsAtPos = level_1_Scene->items(QPointF(x * 50, y * 50));
        for (QGraphicsItem* item : itemsAtPos) {
            if (dynamic_cast<QGraphicsPixmapItem*>(item)) {
                level_1_Scene->removeItem(item);
                delete item;
                break;
            }
        }
        
        // 创建一个新的Road图像项
        QGraphicsPixmapItem* roadItem = new QGraphicsPixmapItem(QPixmap(""));
        roadItem->setPos(x * 50, y * 50);
        
        // 添加到场景
        level_1_Scene->addItem(roadItem);
    }
    
    // 检查是否是Cobblestone，如果是则更新为Road
    if (map_1[y][x] == Cobblestone && newBlockType == Road)
    {
        qDebug() << "TNT爆炸更新地图块:" << x << "," << y << "从Cobblestone到Road";
        
        // 更新地图数据
        map_1[y][x] = Road;
        
        // 创建一个新的Road图像项
        QGraphicsPixmapItem* roadItem = new QGraphicsPixmapItem(QPixmap(" "));
        roadItem->setPos(x * 50, y * 50);
        
        // 添加到场景
        level_1_Scene->addItem(roadItem);
    }
}

