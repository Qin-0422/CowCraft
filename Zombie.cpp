#include "Zombie.h"
#include <ctime>   // 添加time.h头文件，用于初始化随机数种子

Zombie::Zombie(): m_KnockBackTimer(new QTimer(this)), m_KnockBackSteps(0), m_KnockBackX(0), m_KnockBackY(0), m_TargetSteve(nullptr)
{
    // 初始化随机数种子，确保每次运行生成不同的随机序列
    std::srand(static_cast<unsigned int>(time(nullptr)));
    m_MoveDirction = 1;
    m_moveSpeed = 5;
    m_randomMoveTimer = new QTimer(this);
    m_directionTimer = new QTimer(this);
    m_randomMoveTimer->start(1000);
    m_directionTimer->start(50);
    this->setHomeAreaX1(400);
    this->setHomeAreaX2(750);
    this->setHomeAreaY1(800);
    this->setHomeAreaY2(1150);
    this->setHealth(20);
    this->setAttackPower(5);
    this->setPixmap(QPixmap(":/Zombie/img/character/Zombie/Zombie_Left.png"));
    
    // 设置初始位置
    this->setPosition(700, 1050);
    this->setX(700);
    this->setY(1050);
    
    connect(m_randomMoveTimer, &QTimer::timeout, this, &Zombie::randomMove);
    connect(m_directionTimer, &QTimer::timeout, this, &Zombie::updatePosition);
    connect(m_KnockBackTimer, &QTimer::timeout, this, &Zombie::updateKnockBack);
    m_respawnTimer = new QTimer(this);
    connect(m_respawnTimer, &QTimer::timeout, this, &Zombie::respawn);
}


// 随机移动函数的定义，僵尸在规定区域内连续移动
void Zombie::randomMove()
{
    // 随机选择新的移动方向（左或右）
    m_MoveDirction = (std::rand() % 2) * 2 - 1; // -1表示左，1表示右
}

// 更新移动位置
void Zombie::updatePosition()
{
    // 根据当前方向计算新位置
    int newX = getX() + m_MoveDirction * m_moveSpeed;
    
    // 检查是否到达边界
    if (newX < m_HomeAreaX1)
    {
        m_MoveDirction = 1; // 碰到左边界时改变方向
        newX = getX() + m_MoveDirction * m_moveSpeed; // 使用新方向重新计算位置
        setPixmap(QPixmap(":/Zombie/img/character/Zombie/Zombie_Right.png"));
    }
    else if (newX > m_HomeAreaX2)
    {
        m_MoveDirction = -1; // 碰到右边界时改变方向
        newX = getX() + m_MoveDirction * m_moveSpeed; // 使用新方向重新计算位置
        setPixmap(QPixmap(":/Zombie/img/character/Zombie/Zombie_Left.png"));
    }
    setX(newX);
    setPosition(newX, getY()); // 同时更新Character类中的逻辑位置
}
void Zombie::chase(Steve* target)
{    
    int targetX = target->x();
    int zx = getX();

    // 只在X轴方向追击
    if (targetX > zx) {
        setX(zx + 5);
        setPosition(zx+5, getY());
        setPixmap(QPixmap(":/Zombie/img/character/Zombie/Zombie_Right.png"));
    }
    else if (targetX < zx) {
        setX(zx - 5);
        setPosition(zx-5, getY());
        setPixmap(QPixmap(":/Zombie/img/character/Zombie/Zombie_Left.png"));
    }
    qDebug()<<this->pos();
}
void Zombie::attack(Steve* target)
{
    // 只有在僵尸可见（存活）状态下才能攻击
    if (this->isVisible() && this->QGraphicsPixmapItem::collidesWithItem(target))
    {
        target->takeDamage(getAttackPower());
        qDebug() << "Zombie attacks " << target->getX() << " for " << getAttackPower() << " damage!";
    }
}
void Zombie::update(Steve* target)
{
    m_TargetSteve = target;
    if (target == nullptr) {
        if (!m_randomMoveTimer->isActive()) {
            m_randomMoveTimer->start(1000);
            m_directionTimer->start(50);
        }
        return;
    }

    if (this->QGraphicsPixmapItem::collidesWithItem(target))
    {
        attack(target);
    }
    else if (isNear(target))
    {
        m_randomMoveTimer->stop();
        m_directionTimer->stop();
        chase(target);
    }
    else
    {
        if (!m_randomMoveTimer->isActive())
        {
            m_randomMoveTimer->start(1000);
            m_directionTimer->start(50);
        }
    }
}
bool Zombie::isNear(Steve* target)
{
    // 只考虑X轴方向的距离，因为僵尸只在X轴移动
    double distance = std::abs(getX() - target->x() );
    return distance < 300 && std::abs(getY() - target->y()) < 100; // 调整追击距离阈值，并增加Y轴距离判断
}
void Zombie::takeDamage(int damage, Steve* attacker)
{
    setHealth(getHealth() - damage);
    if (getHealth() <= 0)
    {
        // 30%概率掉落铁
        if (rand() % 100 < 30) 
        {
            Item* iron = new Material("Icon", "铁", ":/Item/img/Item/icon.jpg");
            attacker->addItem(iron);
        }
        // 只有在地图二才复活
        if (m_HomeAreaX1 == 400 && m_HomeAreaX2 == 750) {
            m_respawnTimer->start(3000); // 启动复活计时器
        }
        this->setVisible(false); // 暂时隐藏僵尸
    }
    else
    {
        qDebug() << "Zombie takes " << damage << " damage!";
        knockBack(attacker); // 触发击退效果
    }
}

void Zombie::knockBack(Steve* attacker)
{
    int dx = getX() - attacker->getX();
    double distance = std::abs(dx);

    if (distance > 0)
    {
        int knockBackDistance = 30; // 减小击退距离
        int knockBackSteps = 5; // 减少击退步数，使动作更快
        int knockBackX = static_cast<int>((dx / distance) * knockBackDistance / knockBackSteps);

        // 根据Steve的方向进行反方向击退
        if (attacker->getX() > getX()) {
            knockBackX = -knockBackX; // Steve在右边，向左击退
        } else {
            knockBackX = knockBackX; // Steve在左边，向右击退
        }

        for (int i = 0; i < knockBackSteps; ++i)
        {
            int newX = getX() + knockBackX;
            setPosition(newX, getY());
            setX(newX);
        }
    }
}

// 更新击退效果的槽函数
void Zombie::updateKnockBack()
{
    if (m_KnockBackSteps > 0)
    {
        int newX = getX() + m_KnockBackX;
        setPosition(newX, getY());
        setX(newX);
        m_KnockBackSteps--;
    }
    else
    {
        m_KnockBackTimer->stop();
    }
}
void Zombie::respawn() 
{
    this->setHealth(20); // 重置生命值
    this->setPosition(700,1050); // 设置复活位置
    this->setX(700); // 更新X坐标
    this->setY(1050); // 更新Y坐标
    this->setVisible(true); // 显示僵尸
    m_respawnTimer->stop(); // 停止计时器
}
