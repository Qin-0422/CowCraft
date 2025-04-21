#include "Steve.h"
#include "Widget.h"
#include "Map_Level_1.h"
#include "Map_Level_2.h"  
#include "FakeWeapon.h"
#include "Material.h"
#include <QDebug>
Steve::Steve()
{
    
    setHealth(20);
    setAttackPower(1);
    setattackDistance_x(50);
    setattackDistance_y(100);
    setPosition(50,1000);
    this->setPixmap(QPixmap(":/Steve/img/character/Steve/Steve_front.png"));
    m_MoveSpeed =5;
    jump_can_height = 100;
    m_isJumping = false;
    m_facingRight = true;
    m_currentArmor = nullptr;  // 初始化盔甲指针为空
    QGraphicsPixmapItem::setAcceptHoverEvents(true);
    QGraphicsPixmapItem::setAcceptedMouseButtons(Qt::AllButtons);

    m_SteveMoveTimer = new QTimer(this);
    connect(m_SteveMoveTimer, &QTimer::timeout, this, &Steve::SteveMove);
    m_SteveMoveTimer->start(10);  // 每10毫秒触发一次SteveMove

    m_jumpTimer= new QTimer(this);
    connect(m_jumpTimer, &QTimer::timeout, this, &Steve::jump);

    m_fallTimer =new QTimer(this);

    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsFocusable);
    QGraphicsPixmapItem::setFocus();
    m_map = nullptr;  // 初始化地图指针

    // 连接位置改变信号到检测位置的槽函数
    connect(this, &Steve::positionChanged, this, &Steve::checkPosition);
    //物品栏相关
    // 初始化物品栏相关
    m_inventoryUI = new InventoryUI();
    connect(m_inventoryUI, &InventoryUI::itemUsed, this, &Steve::onItemUsed);
    connect(m_inventoryUI, &InventoryUI::itemEquipped, this, &Steve::onItemEquipped);
    connect(m_inventoryUI, &InventoryUI::itemDropped, this, &Steve::onItemDropped);

    m_inventoryUI = new InventoryUI();
    m_inventoryUI->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);


    // 初始化合成界面
    m_craftingUI = new CraftingUI();
    connect(m_craftingUI, &CraftingUI::craftingCompleted, this, &Steve::onCraftingCompleted);

    // 初始化血量UI
    m_healthUI = new HealthUI();
    m_healthUI->setHealth(getHealth());
    m_healthUI->setPos(20, 20);  // 设置血量UI的位置
    if (scene()) {
        scene()->addItem(m_healthUI);
    }
    
    // 初始化伤害冷却计时器
    m_damageTimer = new QTimer(this);
    m_damageTimer->setSingleShot(true);

    // 初始化Magma伤害计时器
    m_magmaDamageTimer = new QTimer(this);
    m_magmaDamageTimer->setInterval(500); // 设置0.5秒的间隔
    m_magmaDamageTimer->start();


    // 初始化箱子物品
    initializeChestItems();

    // 初始化打火石
    Material* flint = new Material("打火石","用于打开地狱门",":/Steve/img/Item/flint.jpg");
    m_inventory.append(flint);

    // 初始化fake-weapon
    FakeWeapon* fakeWeapon = new FakeWeapon();
    m_inventory.append(fakeWeapon);

    // 连接信号
    connect(m_inventoryUI, &InventoryUI::chestItemsReceived, this, &Steve::addItemsToInventory);

    // 攻击方法
    m_attackTimer = new QTimer(this);
    m_attackTimer->setInterval(300);
    m_attackTimer->setSingleShot(true);
    connect(m_attackTimer, &QTimer::timeout, this, &Steve::resetAttackPower);
}

Steve::~Steve()
{
    if (m_currentArmor) {
        delete m_currentArmor;
        m_currentArmor = nullptr;
    }
    if (m_inventoryUI)
    {
        m_inventoryUI->hide();
        delete m_inventoryUI;
        m_inventoryUI = nullptr;
    }
    
    // 清理血量UI资源
    if (m_healthUI)
    {
        delete m_healthUI;
        m_healthUI = nullptr;
    }
    
    if (m_damageTimer)
    {
        m_damageTimer->stop();
        delete m_damageTimer;
        m_damageTimer = nullptr;
    }
}

bool Steve::hasWeapon() const 
{
    for (Item* item : m_inventory) 
    {
        if (item->getName() == "fake weapon" || item->getName() == "real_Weapon")
        {
            return true;
        }
    }
    return false;
}

int Steve::getWeaponType() const
{
    for (Item* item : m_inventory)
    {
        if (item->getName() == "fake weapon")
        {
            return 1; // fake weapon
        }
        else if (item->getName() == "real_Weapon")
        {
            return 2; // real weapon
        }
    }
    return 0; // no weapon
}



void Steve::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
    case Qt::Key_D:
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_Space:
        mKeyList.append(event->key());
        break;
    case Qt::Key_E:
        toggleInventory();
        break;
    case Qt::Key_J:
        if (hasWeapon()) 
        {
            attack();
        }
        break;
    case Qt::Key_1:
        // 遍历物品栏查找FakeWeapon
        for (int i = 0; i < m_inventory.size(); ++i)
        {
            if (FakeWeapon* fakeWeapon = dynamic_cast<FakeWeapon*>(m_inventory[i]))
            {
                onItemEquipped(i);
                break;
            }
        }
        break;
    case Qt::Key_2:
    {
        // 检查是否拥有打火石
        bool hasFlint = false;
        for (Item* item : m_inventory)
        {
            if (item->getName() == "打火石")
            {
                hasFlint = true;
                break;
            }
        }
        if (hasFlint && m_map)
        {
            // 检查是否处于指定区域
            if (this->x() >= 1750 && this->x() <= 2000 && this->y()>= 450 && this->y()<= 650)
            {
                qDebug()<<"到位";
                // 将指定区域转换为TransmitBlock
                for (int x = 1850; x <= 1950; x += 50)
                {
                    qDebug()<<"切换传送";
                    for (int y = 500; y <= 650; y += 50)
                    {
                        m_map->updateBlock(x/50, y/50, TransmitBlock);
                    }
                }
            }
        }
    }
        break;
    case Qt::Key_3:
        // 遍历物品栏查找TNT
        for (int i = 0; i < m_inventory.size(); ++i)
        {
            if (m_inventory[i]->getName() == "TNT")
            {
                onItemUsed(i);
                break;
            }
        }
        break;
    case Qt::Key_4:
        // 遍历物品栏查找Trigger
        for (int i = 0; i < m_inventory.size(); ++i)
        {
            if (m_inventory[i]->getName() == "Trigger")
            {
                onItemUsed(i);
                break;
            }
        }
        break;


    }
}
void Steve::keyReleaseEvent(QKeyEvent *event)
{
    if(mKeyList.contains(event->key()))
    {
        mKeyList.removeOne(event->key());
    }
}
bool Steve::canMoveTo(int newX, int newY)
{
    // 优化碰撞检测，只检查关键点
    const int width = pixmap().width();
    const int height = pixmap().height();
    
    // 检查四个角点和中心点
    const int points[][2] = {
        {newX, newY},                    // 左上
        {newX + width - 1, newY},        // 右上
        {newX, newY + height - 1},       // 左下
        {newX + width - 1, newY + height - 1}, // 右下
        {newX + width/2, newY + height/2},      // 中心
        {newX + width/2, newY},          // 上中
        {newX + width/2, newY + height - 1},    // 下中
        {newX, newY + height/2},         // 左中
        {newX + width - 1, newY + height/2}     // 右中
    };
    
    for (const auto& point : points)
    {
        if (!m_map->isRoad(point[0], point[1]))
        {
            return false;
        }
    }
    return true;
}

bool Steve::canFallTo(int newX, int newY)
{
    // 检查目标位置的包围盒是否在 Ladder 上
    int width = pixmap().width();
    int height = pixmap().height();
    for (int x = newX; x < newX + width; ++x)
    {
        for (int y = newY; y < newY + height; ++y)
        {
            if (m_map->isLadder(x, y))
            {
                return false;
            }
        }
    }
    return true;
}

void Steve::jump()//跳跃函数
{
    if (current_jumpHeight < jump_can_height)
    {
        m_isJumping = true;
        int newY = static_cast<int>(QGraphicsItem::y()) - m_MoveSpeed;
        if (canMoveTo(static_cast<int>(QGraphicsItem::x()), newY))
        {
            moveBy(0, -m_MoveSpeed);
            current_jumpHeight += m_MoveSpeed;
            emit positionChanged(); // 触发位置改变信号
        }
        else//处理头顶碰方块
        {
            m_jumpTimer->stop();
        }
    }
    else
    {
        m_jumpTimer->stop();
    }

        if(m_isJumping&&!m_jumpTimer->isActive())
        {
            m_fallTimer->start(20);
            connect(m_fallTimer,&QTimer::timeout,this,&Steve::fall);
        }


}

void Steve::fall()//下落函数
{
    int newY = static_cast<int>(QGraphicsItem::y()) + m_MoveSpeed;
    if (canMoveTo(static_cast<int>(QGraphicsItem::x()), newY)&&canFallTo(static_cast<int>(QGraphicsItem::x()), newY))
    {
        moveBy(0, m_MoveSpeed);
        emit positionChanged(); // 触发位置改变信号
    }
    else
    {
        m_isJumping = false;
        m_fallTimer->stop();
        m_jumpTimer->start(20);
    }

}

void Steve::climbLadder()
{

}

void Steve::updateFacing(bool facingRight)
{
    m_facingRight = facingRight;
    if (facingRight)
    {
        this->setPixmap(QPixmap(":/Steve/img/character/Steve/Steve_Right.png"));

    }
    else
    {
        this->setPixmap(QPixmap(":/Steve/img/character/Steve/Steve_Left.png"));

    }
}

void Steve::SteveMove()
{
    if (!m_map) return;  // 如果地图未设置，则不进行移动
    for(int KeyCode :mKeyList)
    {
        int deltaX = 0;
        int deltaY = 0;
        switch (KeyCode)
        {
        case Qt::Key_A:
            deltaX = -m_MoveSpeed;
            updateFacing(false); // 朝向左边
            break;
        case Qt::Key_D:
            deltaX = m_MoveSpeed;
            updateFacing(true);  // 朝向右边
            break;
        case Qt::Key_W: deltaY = -m_MoveSpeed;break;
        case Qt::Key_S: deltaY = m_MoveSpeed;break;
        case Qt::Key_Space:
            if (!m_isJumping )
            {
                m_isJumping = true;
                current_jumpHeight = 0;
                m_jumpTimer->start(10); // 每10毫秒触发一次jump
            }
            break;
        }
        int newX = static_cast<int>(QGraphicsItem::x())  + deltaX;
        int newY = static_cast<int>(QGraphicsItem::y())  + deltaY;
        if (canMoveTo(newX, static_cast<int>(QGraphicsItem::y())) ) // 使用 canMoveTo 函数进行碰撞检测
        {
                moveBy(deltaX, 0);
                emit positionChanged(); // 触发位置改变信号
        }
        if(canMoveTo(static_cast<int>(QGraphicsItem::x()),newY)&&!canFallTo(static_cast<int>(QGraphicsItem::x()),newY))
        {
            moveBy(0, deltaY);
            emit positionChanged(); // 触发位置改变信号
        }
    }

}

void Steve::setMap(Level_Map *map)
{
    m_map = map;
}
void Steve::checkPosition()
{
    int _x = static_cast<int>(QGraphicsItem::x());
    int _y = static_cast<int>(QGraphicsItem::y());

    static QTime lastTeleportTime = QTime::currentTime();
    QTime currentTime = QTime::currentTime();
    
    // 添加传送冷却时间（500毫秒）
    if (currentTime.msecsTo(lastTeleportTime) > -500) 
    {
        return;
    }

    // 传送门检测
    if (m_map->isTransmit(_x,_y))
    {
        lastTeleportTime = currentTime;
        // 重置速度和位置状态
        setVelocity(0, 0);
        // 清空按键列表
        mKeyList.clear();
        emit changeLevel();
    }
    if(m_map->isTransmitBack(_x,_y))
    {
        lastTeleportTime = currentTime;
        // 重置速度和位置状态
        setVelocity(0, 0);
        // 清空按键列表
        mKeyList.clear();
        emit changeBackLevel();
    }

    // 检测压力板触发
    Map_Level_3* level3Map = dynamic_cast<Map_Level_3*>(m_map);
    if (level3Map)
    {
        // 检测Steve底部是否接触到压力板的上部10像素区域
        int steveBottom = _y + pixmap().height();
        for (int checkY = steveBottom - 50; checkY <= steveBottom-40; checkY++)
        {
            if (level3Map->isTouch_pressure_plate(_x, checkY))
            {
                // 触发TNT爆炸
                level3Map->triggerTNTExplosion(_x, checkY);
                break;
            }
        }
    }


}

void Steve::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 不再使用鼠标位置更新朝向
}

void Steve::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        // 移除武器检查，让右键始终能触发交互功能
        checkInteractiveArea();
    }
    else if (event->button() == Qt::LeftButton && hasWeapon())
    {
        // 执行攻击
        attack();
    }
}



void Steve::attack() 
{
    if (!hasWeapon())
    {
        return; // 没有武器不能攻击
    }

    // 只在非冷却状态下执行攻击逻辑
    if (!m_attackTimer->isActive())
    {
        int weaponType = getWeaponType();
        if (weaponType == 1) // fake weapon
        {
            setAttackPower(5);
        }
        else if (weaponType == 2) // real weapon
        {
            setAttackPower(10);
        }
        m_attackTimer->start();
        
        // 执行攻击逻辑
        qDebug() << "Steve使用武器攻击";

        // 获取Steve的像素大小
        int steveWidth = pixmap().width(); // 50像素宽
        int steveHeight = pixmap().height(); // 100像素高

        // 根据角色朝向确定攻击区域的起始X坐标和结束X坐标
        int areaStartX, areaEndX;

        if (m_facingRight)
        {
            // 朝右时，攻击范围为以人物右侧向右100
            areaStartX = x() + steveWidth;
            areaEndX = areaStartX + 100;
        }
        else
        {
            // 朝左时，攻击范围为以人物左侧向左100
            areaEndX = x();
            areaStartX = areaEndX - 100;
        }

        // 攻击区域的起始Y坐标（从角色中心开始）
        int areaY = y() + (steveHeight / 2) - 50;

        // 遍历场景中的所有僵尸
        if (scene())
        {
            QList<QGraphicsItem*> items = scene()->items();
            for (QGraphicsItem* item : items)
            {
                // 检查是否为僵尸
                Zombie* zombie = dynamic_cast<Zombie*>(item);
                if (zombie)
                {
                    // 获取僵尸的位置
                    int zombieX = zombie->x();
                    int zombieY = zombie->y();

                    // 检查僵尸是否在攻击范围内
                    if (zombieX >= areaStartX && zombieX <= areaEndX &&
                        zombieY >= areaY && zombieY <= areaY + 100)
                    {
                        // 对僵尸造成伤害并击退
                        zombie->takeDamage(getAttackPower(), this);
                        qDebug() << "Hit zombie at position:" << zombieX << "," << zombieY;
                    }
                }
                
                // 检查是否为Creeper
                Creeper* creeper = dynamic_cast<Creeper*>(item);
                if (creeper)
                {
                    // 获取Creeper的位置
                    int creeperX = creeper->x();
                    int creeperY = creeper->y();

                    // 检查Creeper是否在攻击范围内
                    if (creeperX >= areaStartX && creeperX <= areaEndX &&
                        creeperY >= areaY && creeperY <= areaY + 100)
                    {
                        // 对Creeper造成伤害并击退
                        creeper->takeDamage(getAttackPower(), this);
                        qDebug() << "Hit creeper at position:" << creeperX << "," << creeperY;
                    }
                }

                // 检查是否为Enderman
                Enderman* enderman = dynamic_cast<Enderman*>(item);
                if (enderman)
                {
                    // 获取Enderman的位置
                    int endermanX = enderman->x();
                    int endermanY = enderman->y();

                    // 检查Enderman是否在攻击范围内，考虑Enderman的高度
                    if (endermanX >= areaStartX && endermanX <= areaEndX &&
                        ((endermanY >= areaY - 50 && endermanY <= areaY + 150) || // 检查Enderman上半身
                         (endermanY + enderman->pixmap().height() >= areaY - 50 && // 检查Enderman下半身
                          endermanY + enderman->pixmap().height() <= areaY + 150)))
                    {
                        // 对Enderman造成伤害并击退
                        enderman->takeDamage(getAttackPower(), this);
                        qDebug() << "Hit enderman at position:" << endermanX << "," << endermanY;
                    }
                }
            }
        }
    }
}

void Steve::resetAttackPower() 
{
    setAttackPower(1);
}





void Steve::checkInteractiveAreaAtPosition(int x, int y)
{
    // 直接检查点击位置是否存在可交互物品
    if (m_map)
    {
        // 检查该点是否存在可交互物品
        if (m_map)
        {
            // 检查是否在地图一中
            Map_Level_1* level1Map = dynamic_cast<Map_Level_1*>(m_map);
            if (level1Map && level1Map->isBox(x, y))
            {
                level1Map->changeBoxState(x, y);
                qDebug() << "Box opened at:" << x << "," << y;
                // 获取箱子ID并调用interactWithChest
                int chestId = level1Map->getBoxId(x, y);
                if (chestId > 0) {
                    interactWithChest(chestId);
                }
            }
            // 检查是否在地图二中
            Map_Level_2* level2Map = dynamic_cast<Map_Level_2*>(m_map);
            if (level2Map && level2Map->isBox(x, y))
            {
                level2Map->changeBoxState(x, y);
                qDebug() << "Box opened at:" << x << "," << y;
                // 获取箱子ID并调用interactWithChest
                int chestId = level2Map->getBoxId(x, y);
                if (chestId > 0)
                {
                    interactWithChest(chestId);
                }
            }
            // 检查是否在地图三中
            Map_Level_3* level3Map = dynamic_cast<Map_Level_3*>(m_map);
            if (level3Map && level3Map->isBox(x, y))
            {
                level3Map->changeBoxState(x, y);
                qDebug() << "Box opened at:" << x << "," << y;
                // 获取箱子ID并调用interactWithChest
                int chestId = level3Map->getBoxId(x, y);
                if (chestId > 0)
                {
                    interactWithChest(chestId);
                }
            }
        }
    }
}




// 给予假武器
void Steve::giveFakeWeapon()
{
    // 创建假武器并添加到物品栏
    FakeWeapon* fakeWeapon = new FakeWeapon();
    m_inventory.append(fakeWeapon);

    // 自动装备假武器
    equipWeapon(fakeWeapon);
    emit inventoryChanged();


}

// 检查角色前方的交互区域
void Steve::checkInteractiveArea()
{
    // 获取Steve的像素大小
    int steveWidth = pixmap().width();
    int steveHeight = pixmap().height();
    
    // 检查是否在工作台附近
    Map_Level_2* level2Map = dynamic_cast<Map_Level_2*>(m_map);
    if (level2Map)
    {
        // 扩大检测范围，检查Steve周围的区域
        for (int offsetX = -100; offsetX <= 100; offsetX += 50)
        {
            for (int offsetY = -100; offsetY <= 100; offsetY += 50)
            {
                int checkX = static_cast<int>(x()) + offsetX;
                int checkY = static_cast<int>(y()) + offsetY;
                if (level2Map->isCraftingTable(checkX, checkY))
                {
                    qDebug() << "Found crafting table at:" << checkX << "," << checkY;
                    toggleCraftingTable();
                    return;
                }
            }
        }
    }

    // 根据角色朝向确定交互区域的起始X坐标
    // 如果朝右，从角色右边缘开始；如果朝左，从角色左边缘向左50像素开始
    int areaX = m_facingRight ? (x() + steveWidth) : (x() - 50);

    // 交互区域的起始Y坐标（从角色中心开始）
    int areaY = y() + (steveHeight / 2) - 50;

    // 遍历交互区域内的每个点，区域大小为50x100像素
    for (int checkX = areaX; checkX < areaX + 50; ++checkX)
    {
        for (int checkY = areaY; checkY < areaY + 100; ++checkY)
        {
            // 检查该点是否存在可交互物品
            if (m_map && m_map->isInteractive(checkX, checkY))
            {
                // 检查是否在地图一中
                Map_Level_1* level1Map = dynamic_cast<Map_Level_1*>(m_map);
                if (level1Map && level1Map->isBox(checkX, checkY))
                {
                    level1Map->changeBoxState(checkX, checkY);
                    qDebug() << "Box opened at:" << checkX << "," << checkY;
                    // 获取箱子ID并调用interactWithChest
                    int chestId = level1Map->getBoxId(checkX, checkY);
                    if (chestId > 0) {
                        interactWithChest(chestId);
                    }
                }
                // 检查是否在地图二中
                Map_Level_2* level2Map = dynamic_cast<Map_Level_2*>(m_map);
                if (level2Map && level2Map->isBox(checkX, checkY))
                {
                    level2Map->changeBoxState(checkX, checkY);
                    qDebug() << "Box opened at:" << checkX << "," << checkY;
                    // 获取箱子ID并调用interactWithChest
                    int chestId = level2Map->getBoxId(checkX, checkY);
                    if (chestId > 0) {
                        interactWithChest(chestId);
                    }
                }
                return; // 找到可交互物品后立即返回
            }
        }
    }
}

void Steve::toggleCraftingTable()
{
    Map_Level_2* level2Map = dynamic_cast<Map_Level_2*>(m_map);
    if (!level2Map) {
        qDebug() << "Not in Level 2 map";
        return;
    }

    // 获取Steve的像素大小
    int steveWidth = pixmap().width();
    int steveHeight = pixmap().height();

    // 检查Steve周围100像素范围内是否有工作台
    bool foundCraftingTable = false;
    for (int offsetX = -100; offsetX <= 100; offsetX += 50) {
        for (int offsetY = -100; offsetY <= 100; offsetY += 50) {
            int checkX = static_cast<int>(x()) + offsetX;
            int checkY = static_cast<int>(y()) + offsetY;
            
            if (level2Map->isCraftingTable(checkX, checkY)) {
                foundCraftingTable = true;
                qDebug() << "Found crafting table at:" << checkX << "," << checkY;
                break;
            }
        }
        if (foundCraftingTable) break;
    }

    if (foundCraftingTable) {
        if (!m_craftingUI->isVisible()) {
            // 获取主窗口并设置工作台界面位置
            if (scene() && scene()->views().first()) {
                QWidget* mainWindow = scene()->views().first()->viewport();
                // 设置为子窗口并保持焦点在主窗口
                m_craftingUI->setParent(mainWindow);
                m_craftingUI->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
                // 将工作台界面居中显示在主窗口
                QPoint center = mainWindow->mapToGlobal(mainWindow->rect().center());
                m_craftingUI->move(center.x() - m_craftingUI->width()/2,
                                  center.y() - m_craftingUI->height()/2);

                qDebug() << "Opening crafting UI";
                m_craftingUI->setInventory(m_inventory);
                m_craftingUI->show();

                // 确保主窗口保持焦点
                mainWindow->setFocus();
            }
        } else {
            qDebug() << "Closing crafting UI";
            m_craftingUI->hide();
        }
    } else {
        qDebug() << "No crafting table nearby";
    }
}

void Steve::onCraftingCompleted(Item* craftedItem)
{
    if (craftedItem) {
        m_inventory.append(craftedItem);
        emit inventoryChanged();
    }
}

// 实现setScene方法，在场景变化时自动设置焦点
void Steve::setScene(QGraphicsScene *scene)
{
    QGraphicsPixmapItem::setParentItem(nullptr); // 移除旧的父项
    scene->addItem(this); // 添加到新场景
    QGraphicsPixmapItem::setFocus(); // 设置焦点

    // 为场景安装事件过滤器
    scene->installEventFilter(this);

    // 添加血量UI到新场景
    if (m_healthUI) 
    {
        // 确保血量UI从旧场景中移除
        if (m_healthUI->scene()) 
        {
            m_healthUI->scene()->removeItem(m_healthUI);
        }
        m_healthUI->setParentItem(nullptr);
        scene->addItem(m_healthUI);
        // 设置固定位置显示血量
        m_healthUI->setPos(20, 20);
        // 确保血量UI始终显示在最上层
        m_healthUI->setZValue(1000);
    }
}

// 添加事件过滤器来捕获场景上的鼠标事件
bool Steve::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMousePress)
    {
        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
        if (mouseEvent->button() == Qt::RightButton && !hasWeapon())
        {
            QPointF scenePos = mouseEvent->scenePos();
            qDebug() << "Scene clicked at:" << scenePos.x() << scenePos.y();
            checkInteractiveAreaAtPosition(static_cast<int>(scenePos.x()), static_cast<int>(scenePos.y()));
            return true; // 事件已处理
        }
    }
    return QObject::eventFilter(watched, event); // 继续传递未处理的事件
}

// 重写focusOutEvent，防止Steve失去焦点
void Steve::focusOutEvent(QFocusEvent *event)
{
    // 忽略失去焦点的事件，立即重新获取焦点
    QGraphicsPixmapItem::setFocus();
}
//打开或隐藏物品栏
void Steve::toggleInventory()
{
    if (m_inventoryUI->isVisible())
    {
        m_inventoryUI->hide();
        m_SteveMoveTimer->start(10);
        setEnabled(true);
        QGraphicsPixmapItem::setFocus();
        scene()->setFocusItem(this);
        grabKeyboard();
        mKeyList.clear();

        // 延迟处理焦点和事件，确保UI正确隐藏
        QTimer::singleShot(100, [this]() {
            if (!m_inventoryUI->isVisible()) {
                QGraphicsPixmapItem::setFocus();
                scene()->setFocusItem(this);
                if (m_isJumping && !m_jumpTimer->isActive() && !m_fallTimer->isActive())
                {
                    m_fallTimer->start(20);
                }
            }
        });
    }
    else
    {
        // 获取主窗口并设置物品栏位置
        if (scene() && scene()->views().first()) {
            QWidget* mainWindow = scene()->views().first()->viewport();
            // 设置为子窗口并保持焦点在主窗口
            m_inventoryUI->setParent(mainWindow);
            m_inventoryUI->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
            // 将物品栏居中显示在主窗口
            QPoint center = mainWindow->mapToGlobal(mainWindow->rect().center());
            m_inventoryUI->move(center.x() - m_inventoryUI->width()/2,
                              center.y() - m_inventoryUI->height()/2);

            m_SteveMoveTimer->stop();
            mKeyList.clear();
            m_inventoryUI->updateInventory(m_inventory, m_equipments);
            m_inventoryUI->show();

            // 确保主窗口保持焦点
            mainWindow->setFocus();
        }
    }
}

void Steve::onItemUsed(int index)
{
    if (index >= 0 && index < m_inventory.size())
    {
        Item* item = m_inventory[index];
        if (item->canUse())
        {
            // 先减少物品数量
            item->decreaseCount();
            
            // 检查是否是TNT或Trigger物品
            if ((item->getName() == "TNT" || item->getName() == "Trigger") && m_map)
            {
                // 获取Steve的位置和朝向
                int steveX = static_cast<int>(QGraphicsItem::x());
                int steveY = static_cast<int>(QGraphicsItem::y());
                int placeX, placeY;
                
                // 根据朝向确定放置位置（在Steve前方50像素处）
                if (m_facingRight)
                {
                    placeX = steveX + 50 ; // Steve宽度50
                    placeY = steveY + 50; // 放在Steve脚下位置
                }
                else
                {
                    placeX = steveX - 50; // 向左50
                    placeY = steveY + 50; // 放在Steve脚下位置
                }
                
                // 检查放置位置是否有效
                // 确保只能在Road类型的方块上放置物品
                if (m_map->isRoad(placeX, placeY))
                {
                    if (item->getName() == "TNT")
                    {
                        // 放置TNT
                        TNT::placeTNT(placeX, placeY, m_map);
                        qDebug() << "放置了TNT在位置:" << placeX << "," << placeY;
                        
                        // 检查物品数量是否为0
                        if (item->getCount() <= 0)
                        {
                            m_inventory.removeAt(index);
                            delete item;
                            item = nullptr;
                        }
                    }
                    else if (item->getName() == "Trigger")
                    {
                        // 放置Trigger
                        m_map->setTrigger(placeX, placeY);
                        qDebug() << "放置了Trigger在位置:" << placeX << "," << placeY;
                        
                        // 检查物品数量是否为0
                        if (item->getCount() <= 0)
                        {
                            m_inventory.removeAt(index);
                            delete item;
                            item = nullptr;
                        }
                    }
                    else
                    {
                        qDebug() << "无法在此位置放置物品";
                    }
                }
                else
                {
                    // 其他物品的正常使用逻辑
                    item->use();
                    if (item->isConsumable() && item->getCount() <= 0)
                    {
                        m_inventory.removeAt(index);//移除指定索引位置的物品
                        delete item;
                    }
                }
                emit inventoryChanged();
            }
        }

    }
}
void Steve::onItemEquipped(int index)
{
    if (index >= 0 && index < m_inventory.size())
    {
        Item* item = m_inventory[index];
        if (Equipment* equipment = dynamic_cast<Equipment*>(item))
        {
            if (equipment->getType() == Equipment::Type::WEAPON)
            {
                if (m_currentWeapon != item) // 检查是否已装备
                {
                    equipWeapon(item);
                    m_equipments.append(equipment); // 将武器添加到装备列表
                    emit inventoryChanged();
                    qDebug() << "装备了武器:" << item->getName();
                    m_inventoryUI->updateInventory(m_inventory, m_equipments); // 更新UI显示
                }
            }
            else
            {
                qDebug() << "该物品不是武器，无法装备";
            }
        }
    }
}
void Steve::onItemDropped(int index)
{
    if (index >= 0 && index < m_inventory.size())
    {
        Item* item = m_inventory.takeAt(index);
        if (item == m_currentWeapon)
        {
            unequipWeapon();
        }
        delete item;
        emit inventoryChanged();
    }
}
void Steve::equipWeapon(Item* weapon)
{
    if (m_currentWeapon)
    {
        unequipWeapon();
    }
    m_currentWeapon = weapon;
    // 先重置基础攻击力为1
    setAttackPower(1);
    // 根据武器属性更新攻击力
    if (Equipment* equipment = dynamic_cast<Equipment*>(weapon))
    {
        QMap<QString, int> modifiers = equipment->getAttributeModifiers();
        if (modifiers.contains("attackPower"))
        {
            setAttackPower(getAttackPower() + modifiers["attackPower"]);
        }
        // 更新人物图片为手持武器状态
        if (m_facingRight)
        {
            this->setPixmap(QPixmap(":/Steve/img/character/Steve/Steve_Right_with_weapon.png"));
        }
        else
        {
            this->setPixmap(QPixmap(":/Steve/img/character/Steve/Steve_Left_with_weapon.png"));
        }
    }
}

void Steve::unequipWeapon()
{
    if (m_currentWeapon)
    {
        if (Equipment* equipment = dynamic_cast<Equipment*>(m_currentWeapon))
        {
            QMap<QString, int> modifiers = equipment->getAttributeModifiers();
            if (modifiers.contains("attackPower"))
            {
                setAttackPower(getAttackPower() - modifiers["attackPower"]);
            }
        }
        m_currentWeapon = nullptr;
    }
}
// 初始化箱子物品
void Steve::initializeChestItems()
{
    // 地图一的箱子物品
    QList<Item*> chest3Items;
    chest3Items.append(new KeyItem("Trigger", "可消耗的触发器", true, 64, ":/Item/img/Item/Trigger.png"));
    m_chestItems.insert(3, chest3Items);

    QList<Item*> chest4Items;
    chest4Items.append(new KeyItem("Trigger", "可消耗的触发器", true, 64, ":/Item/img/Item/Trigger.png"));
    m_chestItems.insert(4, chest4Items);

    // 地图二的箱子物品
    QList<Item*> chest2Items;
    Material* sand = new Material("沙子", "沙子是一种常见的材料", ":/Item/img/Item/sand.jpg");
    sand->setCount(64);
    chest2Items.append(sand);
    m_chestItems.insert(2, chest2Items);
    // 地图三的箱子物品
    QList<Item*> chest5Items;
    chest5Items.append(new Armor("铁盔甲","盔甲可以很好地减免伤害",":/Item/img/Item/breastplate.jpg"));
    m_chestItems.insert(5, chest5Items);
    QList<Item*> chest6Items;
    chest5Items.append(new Armor("燧石","合成打火石的必要物品",":/Item/img/Item/flint.jpg"));
    m_chestItems.insert(6, chest6Items);
}

// 与箱子交互
void Steve::interactWithChest(int chestId)
{
    qDebug() << "正在打开箱子ID:" << chestId;

        // 检查箱子是否已经被打开
        if (m_openedChests.contains(chestId))
        {
            qDebug() << "箱子已经被打开过";
            return;
        }

        // 获取箱子物品
        if (m_chestItems.contains(chestId))
        {
            QList<Item*> chestItems = m_chestItems.value(chestId);
            qDebug() << "箱子中物品数量:" << chestItems.size();

            // 将物品添加到物品栏
            for (Item* item : chestItems)
            {
                if (item->getName() == "Trigger")
                {

                    m_inventory.append(new KeyItem("Trigger", "可消耗的触发器", true, 64, ":/Item/img/Item/Trigger.png"));
                }
                else
                {
                    // 创建新的Material并保留原始物品的数量
                    Material* newMaterial = new Material(item->getName(), item->getDescription(), item->getIconPath());
                    newMaterial->setCount(item->getCount()); // 保留原始物品的数量
                    m_inventory.append(newMaterial);
                }
            }

            // 标记箱子为已打开
            m_openedChests.insert(chestId);

            // 发送物品栏更新信号
            emit inventoryChanged();
            qDebug() << "物品已添加到物品栏";
        }
        else
        {
            qDebug() << "未找到箱子ID:" << chestId << "的物品信息";
        }
}

// 检查箱子是否已经被打开
bool Steve::hasOpenedChest(int chestId) const
{
    return m_openedChests.contains(chestId);
}

// 添加物品到物品栏
void Steve::addItemsToInventory(const QList<Item*>& items)
{
    for (Item* item : items)
    {
        // 检查是否是装备类型
        Equipment* equipment = dynamic_cast<Equipment*>(item);
        if (equipment)
        {
            m_equipments.append(equipment);
        }
        else
        {
            m_inventory.append(item);
        }
    }

    // 更新物品栏UI
    m_inventoryUI->updateInventory(m_inventory, m_equipments);
}

void Steve::addItem(Item *item)
{
    m_inventory.append(item);
    // 更新物品栏UI
    m_inventoryUI->updateInventory(m_inventory, m_equipments);
}

// 血量系统相关方法实现
void Steve::takeDamage(int damage)   // 受到伤害
{
    // 如果伤害冷却计时器正在运行，则不处理伤害
    if (m_damageTimer->isActive()) return;

    // 如果装备了盔甲，减少受到的伤害
    if (m_currentArmor) {
        damage = std::max(0, damage - m_currentArmor->getDefense());
    }

    // 减少生命值
    setHealth(getHealth() - damage);

    // 更新血量UI并触发伤害处理
    if (m_healthUI) 
    {
        m_healthUI->takeDamage(damage); // 调用HealthUI的伤害处理方法
        m_healthUI->update(); // 强制更新UI显示
        
        // 确保血量UI在最上层
        if (m_healthUI->scene()) 
        {
            m_healthUI->setZValue(1000);
        }
    }

    // 启动伤害冷却计时器（1秒）
    m_damageTimer->start(1000);
    
    qDebug() << "Steve受到伤害：" << damage << "，当前血量：" << getHealth();

    // 检查是否死亡
    if (getHealth() <= 0)
    {
        // 停止所有计时器
        m_SteveMoveTimer->stop();
        m_jumpTimer->stop();
        m_fallTimer->stop();
        m_damageTimer->stop();
        m_attackTimer->stop();

        // 创建死亡对话框
        QMessageBox* deathDialog = new QMessageBox(nullptr);
        deathDialog->setWindowTitle("游戏结束");
        deathDialog->setText("你已经死亡！");
        deathDialog->setStandardButtons(QMessageBox::Close);
        deathDialog->button(QMessageBox::Close)->setText("退出游戏");

        // 显示对话框并处理结果
        int result = deathDialog->exec();// 显示对话框并等待用户选择
        if (result == QMessageBox::Reset)
        {
            // 重新开始游戏
            if (scene() && scene()->views().first())
            {
                Widget* mainWidget = qobject_cast<Widget*>(scene()->views().first()->window());// 获取主窗口
                if (mainWidget)
                {
                    mainWidget->resetGame();// 调用主窗口的resetGame函数
                }
            }
        }
        else
        {
            // 退出游戏
            if (scene() && scene()->views().first())
            {
                scene()->views().first()->window()->close();// 关闭主窗口
            }
        }

        delete deathDialog;
    }
}

void Steve::heal(int amount)
{
    // 更新Character基类中的血量
    setHealth(qMin(20, getHealth() + amount));
    
    // 更新血量UI
    if (m_healthUI)
    {
        m_healthUI->heal(amount);
    }
}
void Steve::equipArmor(Armor* armor)
{
    if (m_currentArmor) {
        delete m_currentArmor;
    }
    m_currentArmor = armor;
}

void Steve::unequipArmor()
{
    if (m_currentArmor) {
        delete m_currentArmor;
        m_currentArmor = nullptr;
    }
}

