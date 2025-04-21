#include "Creeper.h"
#include <ctime>   // 用于初始化随机数种子

Creeper::Creeper(): m_KnockBackTimer(new QTimer(this)), m_KnockBackSteps(0), m_KnockBackX(0), m_KnockBackY(0), m_TargetSteve(nullptr), m_isExploding(false), m_facingRight(true)
{
    // 初始化随机数种子，确保每次运行生成不同的随机序列
    std::srand(static_cast<unsigned int>(time(nullptr)));
    m_MoveDirction = 1;
    m_moveSpeed = 5;
    m_randomMoveTimer = new QTimer(this);
    m_directionTimer = new QTimer(this);
    m_explosionTimer = new QTimer(this);
    m_randomMoveTimer->start(1000);
    m_directionTimer->start(50);
    
    // 设置Creeper的活动范围（地图二）
    this->setHomeAreaX1(1000);
    this->setHomeAreaX2(1850);
    this->setHomeAreaY1(950);
    this->setHomeAreaY2(1150);
    this->setHealth(20);
    
    // 由于没有Creeper的图片，暂时使用Steve的图片代替
    // 实际项目中应该替换为Creeper的图片
    m_normalImagePath = ":/Creeper/img/character/Creeper/Creeper_Left.png";
    m_explosionImagePathLeft = ":/Creeper/img/character/Creeper/Creeper_Left_WillingBoom.png";
    m_explosionImagePathRight = ":/Creeper/img/character/Creeper/Creeper_Right_WillingBoom.png";
    this->setPixmap(QPixmap(m_normalImagePath));
    
    // 设置初始位置
    this->setPosition(1800, 1050);
    this->setX(1800);
    this->setY(1050);
    
    connect(m_randomMoveTimer, &QTimer::timeout, this, &Creeper::randomMove);
    connect(m_directionTimer, &QTimer::timeout, this, &Creeper::updatePosition);
    connect(m_KnockBackTimer, &QTimer::timeout, this, &Creeper::updateKnockBack);
    connect(m_explosionTimer, &QTimer::timeout, this, &Creeper::onExplosionTimerFinished);
    
    m_respawnTimer = new QTimer(this);
    connect(m_respawnTimer, &QTimer::timeout, this, &Creeper::respawn);
}

// 随机移动函数的定义，Creeper在规定区域内连续移动
void Creeper::randomMove()
{
    // 如果正在爆炸准备中，不进行随机移动
    if (m_isExploding) return;
    
    // 随机选择新的移动方向（左或右）
    m_MoveDirction = (std::rand() % 2) * 2 - 1; // -1表示左，1表示右
    
    // 更新朝向
    m_facingRight = (m_MoveDirction > 0);
}

// 更新移动位置
void Creeper::updatePosition()
{
    // 如果正在爆炸准备中，不进行移动
    if (m_isExploding) return;
    
    // 根据当前方向计算新位置
    int newX = getX() + m_MoveDirction * m_moveSpeed;
    
    // 检查是否到达边界
    if (newX < m_HomeAreaX1)
    {
        m_MoveDirction = 1; // 碰到左边界时改变方向
        m_facingRight = true;
        newX = getX() + m_MoveDirction * m_moveSpeed; // 使用新方向重新计算位置
    }
    else if (newX > m_HomeAreaX2)
    {
        m_MoveDirction = -1; // 碰到右边界时改变方向
        m_facingRight = false;
        newX = getX() + m_MoveDirction * m_moveSpeed; // 使用新方向重新计算位置
    }
    
    setX(newX);
    setPosition(newX, getY()); // 同时更新Character类中的逻辑位置
    
    // 根据朝向更新图片
    if (m_MoveDirction > 0)
    {
        setPixmap(QPixmap(":/Creeper/img/character/Creeper/Creeper_Left.png"));
    }
    else
    {
        setPixmap(QPixmap(":/Creeper/img/character/Creeper/Creeper_Right.png"));
    }
}

void Creeper::chase(Steve* target)
{    
    // 如果正在爆炸准备中，不进行追击
    if (m_isExploding) return;
    
    int targetX = target->x();
    int cx = getX();

    // 只在X轴方向追击
    if (targetX > cx) 
    {
        setX(cx + 5);
        setPosition(cx+5, getY());
        setPixmap(QPixmap(":/Creeper/img/character/Creeper/Creeper_Right.png"));
        m_facingRight = true;
    }
    else if (targetX < cx) 
    {  
        setX(cx - 5);
        setPosition(cx-5, getY());
        setPixmap(QPixmap(":/Creeper/img/character/Creeper/Creeper_Left.png"));
        m_facingRight = false;
    }
}

void Creeper::update(Steve* target)
{
    m_TargetSteve = target;
    if (target == nullptr) {
        if (!m_randomMoveTimer->isActive() && !m_isExploding)
        {
            m_randomMoveTimer->start(1000);
            m_directionTimer->start(50);
        }
        return;
    }

    // 检查是否非常接近（50像素内）
    if (isVeryClose(target))
    {
        // 如果还没有开始爆炸准备，则开始
        if (!m_isExploding)
        {
            startExplosion();
        }
    }
    // 如果正在爆炸准备中，检查是否已经远离（100像素外）
    else if (m_isExploding && isFarEnough(target))
    {
        cancelExplosion();
    }
    // 如果在追击范围内但不是非常接近
    else if (isNear(target) && !m_isExploding)
    {
        m_randomMoveTimer->stop();
        m_directionTimer->stop();
        chase(target);
    }
    // 如果不在追击范围内且不在爆炸准备中
    else if (!isNear(target) && !m_isExploding)
    {
        if (!m_randomMoveTimer->isActive())
        {
            m_randomMoveTimer->start(1000);
            m_directionTimer->start(50);
        }
    }
}

bool Creeper::isNear(Steve* target)
{
    // 只考虑X轴方向的距离，因为Creeper只在X轴移动
    double distance = std::abs(getX() - target->x());
    return distance < 300 && std::abs(getY() - target->y()) < 100; // 调整追击距离阈值，并增加Y轴距离判断
}

bool Creeper::isVeryClose(Steve* target)
{
    // 只有在Creeper可见时才检查距离
    if (!this->isVisible()) return false;
    
    // 检查是否在150像素范围内
    double distance = std::sqrt(std::pow(getX() - target->x(), 2) + std::pow(getY() - target->y(), 2));
    return distance <= 150;
}

bool Creeper::isFarEnough(Steve* target)
{
    // 如果Creeper不可见，则认为距离足够远
    if (!this->isVisible()) return true;
    
    // 检查是否在200像素范围外
    double distance = std::sqrt(std::pow(getX() - target->x(), 2) + std::pow(getY() - target->y(), 2));
    return distance > 200;
}

void Creeper::takeDamage(int damage, Steve* attacker)
{
    setHealth(getHealth() - damage);
    qDebug() << "Creeper takes " << damage << " damage! Health: " << getHealth();
    
    if (getHealth() <= 0)
    {
        // 掉落火药的概率处理
        int dropChance = rand() % 100;
        if (dropChance < 50) {
            // 50%概率掉落1个火药
            Item* gunpowder = new Material("Gunpowder", "火药", ":/Item/img/Item/Gunpowder.jpg");
            attacker->addItem(gunpowder);
        } else {
            // 50%概率掉落2个火药
            for (int i = 0; i < 2; i++) {
                Item* gunpowder = new Material("Gunpower", "火药", ":/Item/img/Item/flint.jpg");
                attacker->addItem(gunpowder);
            }
        }

        // 如果正在爆炸准备中，取消爆炸
        if (m_isExploding) {
            cancelExplosion();
        }
        
        // 停止所有计时器
        m_randomMoveTimer->stop();
        m_directionTimer->stop();
        if (m_explosionTimer->isActive()) {
            m_explosionTimer->stop();
        }
        
        // 隐藏Creeper并启动复活计时器
        this->setVisible(false);
        m_respawnTimer->start(3000);
        return; // 生命值为0时，不执行击退效果
    }
    
    // 生命值大于0时，执行击退效果
    knockBack(attacker);
}

// 击退效果的处理函数
void Creeper::knockBack(Steve* attacker)
{
    // 即使在爆炸准备状态下也执行击退
    int dx = getX() - attacker->getX();
    double distance = std::abs(dx);

    if (distance > 0)
    {
        int knockBackDistance = 30; // 击退距离
        m_KnockBackSteps = 5; // 击退步数
        m_KnockBackX = static_cast<int>((dx / distance) * knockBackDistance / m_KnockBackSteps);
        m_KnockBackTimer->start(30); // 更新频率
    }
}

// 更新击退效果的槽函数
void Creeper::updateKnockBack()
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

void Creeper::respawn() 
{
    this->setHealth(20); // 重置生命值
    this->setPosition(1800, 1050); // 设置复活位置
    this->setX(1800); // 更新X坐标
    this->setY(1050); // 更新Y坐标
    this->setVisible(true); // 显示Creeper
    this->setPixmap(QPixmap(m_normalImagePath)); // 恢复正常图片
    m_isExploding = false; // 重置爆炸状态
    m_respawnTimer->stop(); // 停止计时器
    
    // 重新启动移动计时器
    if (!m_randomMoveTimer->isActive()) 
    {
        m_randomMoveTimer->start(1000);
    }
    if (!m_directionTimer->isActive()) 
    {
        m_directionTimer->start(50);
    }
}

// 开始爆炸准备
void Creeper::startExplosion()
{
    // 只有在Creeper可见时才能开始爆炸
    if (!this->isVisible()) return;
    
    m_isExploding = true;
    
    // 停止移动
    m_randomMoveTimer->stop();
    m_directionTimer->stop();
    
    // 更新图片为爆炸准备状态
    if (m_facingRight)
    {
        setPixmap(QPixmap(m_explosionImagePathRight));
    }
    else
    {
        setPixmap(QPixmap(m_explosionImagePathLeft));
    }
    
    // 启动爆炸倒计时
    m_explosionTimer->start(2000); // 2秒后爆炸
    
    qDebug() << "Creeper开始爆炸准备!";
}

// 取消爆炸
void Creeper::cancelExplosion()
{
    m_isExploding = false;
    
    // 停止爆炸倒计时
    if (m_explosionTimer->isActive()) {
        m_explosionTimer->stop();
    }
    
    // 恢复正常图片
    setPixmap(QPixmap(m_normalImagePath));
    
    // 重新启动移动
    if (!m_randomMoveTimer->isActive()) {
        m_randomMoveTimer->start(1000);
    }
    if (!m_directionTimer->isActive()) {
        m_directionTimer->start(50);
    }
    
    qDebug() << "Creeper取消爆炸!";
}

// 爆炸倒计时结束
void Creeper::onExplosionTimerFinished()
{
    if (m_isExploding) {
        explode();
    }
}

// 执行爆炸
void Creeper::explode()
{
    qDebug() << "Creeper爆炸!";
    
    // 对附近的Steve造成伤害
    if (m_TargetSteve) {
        // 计算与Steve的距离
        double distance = std::sqrt(std::pow(getX() - m_TargetSteve->x(), 2) + std::pow(getY() - m_TargetSteve->y(), 2));
        
        // 如果Steve在爆炸范围内（200像素），造成伤害
        if (distance <= 200) {
            m_TargetSteve->takeDamage(10); // 造成10点伤害
            qDebug() << "Creeper爆炸对Steve造成10点伤害!";
        }
    }
    
    // 重置状态
    m_isExploding = false;
    
    // 爆炸后Creeper消失，等待复活
    this->setVisible(false);
    m_respawnTimer->start(3000); // 3秒后复活
}
