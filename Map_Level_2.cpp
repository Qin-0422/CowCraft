#include "Map_Level_2.h"

Map_Level_2::Map_Level_2() : QObject(nullptr), Level_Map_Impl()
{
    //初始化地图元素
    level_2_Scene = new QGraphicsScene;
    filePath = "../Map/level_2.txt";
    map_2 = InitByFile(filePath);
    map_data = map_2; // 将地图数据同时存储到基类的map_data中
    QPixmap backgroundImage(":/back/img/background/level_2_back.jpg");
    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(backgroundImage);
    backgroundItem->setPos(50, 0);
    level_2_Scene->addItem(backgroundItem);
    
    // 初始化视野遮罩
    m_visionMask = new QGraphicsPathItem();
    QPainterPath initialPath;
    initialPath.addRect(0, 0, 2000, 1300);
    m_visionMask->setPath(initialPath);
    m_visionMask->setBrush(QBrush(Qt::black));
    m_visionMask->setOpacity(0.9); // 设置不透明度
    m_visionMask->setZValue(100); // 确保遮罩在最上层
    level_2_Scene->addItem(m_visionMask);
    m_isInUpperArea = false;
    
    // 创建视野更新定时器
    m_visibilityTimer = new QTimer(this);
    connect(m_visibilityTimer, &QTimer::timeout, this, &Map_Level_2::updateVisibility);
    m_visibilityTimer->start(50); // 每50毫秒更新一次视野

    int tileSize = 50;
    for(int row = 0;row< map_2.size() ;++row)
    {
        for(int col = 0;col<map_2[static_cast<std::vector<int>::size_type>(row)].size();++col)
        {
            QGraphicsRectItem *tile = new QGraphicsRectItem(col * tileSize, row * tileSize, tileSize, tileSize);
            switch (map_2[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)])
            {
            case bedrock:
            {
                QPixmap pixmap(":/level_2/img/block/bedrock.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
            case Road:
            {
                QPixmap pixmap("");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
            case Stone:
            {
                QPixmap pixmap(":/level_2/img/block/Stone.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }

            case Cobblestone:
            {
                QPixmap pixmap(":/level_2/img/block/Cobblestone.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
           case craftingTable:
            {
                QPixmap pixmap(":/level_2/img/block/Crafting_Table.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
            case Ladder:
            {
                QPixmap pixmap(":/level_2/img/block/Ladder.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
            case MossyStone:
            {
                QPixmap pixmap(":/level_2/img/block/MossyStone.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }

            case TransmitBlock:
            {
                QPixmap pixmap(":/level_2/img/block/TransmitBlock.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
            case TransmitBlockBack:
            {
                QPixmap pixmap(":/level_2/img/block/TransmitBlock.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
            case LauncherBlock:
            {
                QPixmap pixmap(":/level_2/img/block/Launcher.jpg");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
            case box:
            {
                QPixmap pixmap(":/level_2/img/block/box.png");
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setPos(col*tileSize,row*tileSize);
                level_2_Scene->addItem(pixmapItem);
                break;
            }
            case Monster_Spawner:
            {

                SpawnerBlock* spawner = new SpawnerBlock();
                spawner->setPosition(col * tileSize, row * tileSize);
                level_2_Scene->addItem(spawner);
                spawner->startAnimation();
                break;
            }

            }
        }
    }

    m_Steve.setX(150);
    m_Steve.setY(100);
    level_2_Scene->addItem(&m_Steve);
    
    // 添加血量UI到场景
    level_2_Scene->addItem(m_Steve.getHealthUI());
    // 设置血量UI的Z值高于视野遮罩，确保它不会被遮挡
    m_Steve.getHealthUI()->setZValue(101);

    m_Zombie.setX(700);
    m_Zombie.setY(1050);
    level_2_Scene->addItem(&m_Zombie);
    
    // 初始化Creeper
    m_Creeper.setX(1800);
    m_Creeper.setY(1050);
    level_2_Scene->addItem(&m_Creeper);
    
    m_Steve.setMap(this);  // 设置Steve的地图指针

    // 创建定时器，每50毫秒更新一次僵尸状态
    m_ZombieTimer = new QTimer(this);
    connect(m_ZombieTimer, &QTimer::timeout, this, [this]() {
        m_Zombie.update(&m_Steve);
    });
    m_ZombieTimer->start(50);
    
    // 创建定时器，每50毫秒更新一次Creeper状态
    m_CreeperTimer = new QTimer(this);
    connect(m_CreeperTimer, &QTimer::timeout, this, [this]() {
        m_Creeper.update(&m_Steve);
    });
    m_CreeperTimer->start(50);
}

Map_Level_2::~Map_Level_2()
{
    delete level_2_Scene;
    delete m_ZombieTimer;
    delete m_CreeperTimer;
    delete m_visibilityTimer;
}


void Map_Level_2::updateBlock(int x, int y, int newBlockType)
{
    // 添加边界检查
    if (x < 0 || x >= 40 || y < 0 || y >= 26)
        return;
    
    // 检查是否是Cobblestone，如果是则更新为Road
    if (map_2[y][x] == Cobblestone && newBlockType == Road)
    {
        qDebug() << "TNT爆炸更新地图块:" << x << "," << y << "从Cobblestone到Road";
        
        // 更新地图数据
        map_2[y][x] = Road;
        
        // 移除原有的Cobblestone图片项
        QList<QGraphicsItem*> itemsAtPos = level_2_Scene->items(QPointF(x * 50, y * 50));
        for (QGraphicsItem* item : itemsAtPos) {
            if (dynamic_cast<QGraphicsPixmapItem*>(item)) {
                level_2_Scene->removeItem(item);
                delete item;
                break;
            }
        }
        
        // 创建一个新的Road图像项
        QGraphicsPixmapItem* roadItem = new QGraphicsPixmapItem(QPixmap(""));
        roadItem->setPos(x * 50, y * 50);
        
        // 添加到场景
        level_2_Scene->addItem(roadItem);
    }
}
void Map_Level_2::setTrigger(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_2.size() && col >= 0 && col < map_2[row].size())
    {
        // 先检查该位置是否已经有Trigger
        if (map_2[row][col] != Trigger)
        {
            map_2[row][col] = Trigger;
            // 添加Trigger图片
            QPixmap pixmap(":/level_2/img/block/Trigger.png");
            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
            pixmapItem->setPos(col * 50, row * 50);
            pixmapItem->setData(0, "trigger"); // 设置标识符
            level_2_Scene->addItem(pixmapItem);
        }
    }
}

void Map_Level_2::removeTrigger(int x, int y)
{
    //空实现
}
bool Map_Level_2::isTrigger(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size()) {
        return map_2[static_cast<std::vector<int>::size_type>(row)][col] == Trigger;
    }
    return false;
}

bool Map_Level_2::isRoad(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size())
    {
        int blockType = map_2[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)];
        return (blockType == 1 || blockType == 9 || blockType == 12 || blockType == 15 || blockType == TNT || blockType == Trigger);  // 修改为检查是否为路、传送门、梯子、TNT或Trigger
    }
    return false;
}
bool Map_Level_2::isLadder(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && row < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size())
    {
        return map_2[static_cast<std::vector<int>::size_type>(row)][static_cast<std::vector<int>::size_type>(col)] ==12;  // 修改为检查 map_1[row][col] 是否为 梯子
    }
    return false;
}
bool Map_Level_2::isTransmit(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size()) {
        return map_2[static_cast<std::vector<int>::size_type>(row)][col] == 9;  // 修改为检查 map_1[row][col] 是否为 9
    }
    return false;
}
bool Map_Level_2::isTransmitBack(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size()) {
        return map_2[static_cast<std::vector<int>::size_type>(row)][col] == 15;  // 修改为检查 map_1[row][col] 是否为 15
    }
    return false;
}

bool Map_Level_2::Trigger_Arrow()
{
    return false;
}

bool Map_Level_2::isCraftingTable(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size()) {
        return map_2[static_cast<std::vector<int>::size_type>(row)][col] == craftingTable;
    }
    return false;
}

bool Map_Level_2::isInteractive(int x, int y) const
{
    int col = x / 50; // assuming tileSize is 50
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size()) {
        // 检查是否为可交互物品（工作台、未打开的箱子等）
        return map_2[static_cast<std::vector<int>::size_type>(row)][col] == craftingTable || 
               (map_2[static_cast<std::vector<int>::size_type>(row)][col] == box && !isBoxOpened(x, y));
    }
    return false;
}


bool Map_Level_2::isBox(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size()) {
        return map_2[static_cast<std::vector<int>::size_type>(row)][col] == box;
    }
    return false;
}

bool Map_Level_2::isBoxOpened(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size()) {
        return map_2[static_cast<std::vector<int>::size_type>(row)][col] == box_open;
    }
    return false;
}

void Map_Level_2::changeBoxState(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && static_cast<std::vector<int>::size_type>(row) < map_2.size() && col >= 0 && col < map_2[static_cast<std::vector<int>::size_type>(row)].size()) {
        if (map_2[static_cast<std::vector<int>::size_type>(row)][col] == box) {
            map_2[static_cast<std::vector<int>::size_type>(row)][col] = box_open;
            // 更新箱子的图形显示
            QPixmap pixmap(":/level_2/img/block/box_open.png");
            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
            pixmapItem->setPos(col * 50, row * 50);
            level_2_Scene->addItem(pixmapItem);
        }
    }
}

void Map_Level_2::handlePlayerInteraction()
{
    // 获取玩家位置
       QPointF playerPos = m_Steve.pos();

       // 检查玩家是否靠近箱子1
       QPointF chest1Pos = QPointF(550, 650);
       if (QLineF(playerPos, chest1Pos).length() < 50) // 50是交互距离
       {
           m_Steve.interactWithChest(1);
       }

       // 检查玩家是否靠近箱子2
       QPointF chest2Pos = QPointF(1650, 350);
       if (QLineF(playerPos, chest2Pos).length() < 50) {
           m_Steve.interactWithChest(2);
       }
}

int Map_Level_2::getBoxId(int x, int y) const
{
    int col = x / 50;
    int row = y / 50;
    QPointF boxPos(col * 50, row * 50);
    
    // 检查是否为箱子1的位置
    if (QLineF(boxPos, QPointF(550, 650)).length() < 25) {
        return 1;
    }
    // 检查是否为箱子2的位置
    else if (QLineF(boxPos, QPointF(1650, 350)).length() < 25) {
        return 2;
    }
    return 0; // 不是已知的箱子位置
}

void Map_Level_2::updateVisibility()
{
    // 获取Steve的当前位置
    QPointF stevePos = m_Steve.pos();
    int steveX = stevePos.x();
    int steveY = stevePos.y();
    
    // 判断Steve是否在上方区域(Y<250)
    bool isInUpperArea = (steveY < 250);
    
    // 如果区域状态发生变化，更新视野
    if (isInUpperArea != m_isInUpperArea || true) // 始终更新以确保实时响应
    { 
        m_isInUpperArea = isInUpperArea;
        
        if (m_isInUpperArea) // 在上方区域，显示整个2000*250区域
        {    
            QPainterPath visiblePath;
            visiblePath.addRect(0, 0, 2000, 250);
            
            // 创建一个大矩形作为遮罩的基础
            QPainterPath maskPath;
            maskPath.addRect(0, 0, 2000, 1300);
            
            // 从遮罩中减去可见区域
            maskPath = maskPath.subtracted(visiblePath);
            
            // 应用遮罩路径
            m_visionMask->setPath(maskPath);
        } else {
            // 在下方区域，只显示以Steve为中心的150像素范围
            QPainterPath visiblePath;
            visiblePath.addEllipse(steveX - 150, steveY - 150, 300, 300);
            
            // 创建一个大矩形作为遮罩的基础
            QPainterPath maskPath;
            maskPath.addRect(0, 0, 2000, 1300);
            
            // 从遮罩中减去可见区域
            maskPath = maskPath.subtracted(visiblePath);
            
            // 应用遮罩路径
            m_visionMask->setPath(maskPath);
        }
        
        // 确保血量UI始终在视野遮罩之上显示
        HealthUI* healthUI = m_Steve.getHealthUI();
        if (healthUI) {
            // 将血量UI的Z值设置为比遮罩更高，确保它始终显示在最上层
            healthUI->setZValue(101);
        }
    }
}



