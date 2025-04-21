#include "Enderman.h"
#include <ctime>

Enderman::Enderman() : QObject(nullptr), Character(), QGraphicsPixmapItem()
{
    // 初始化随机数种子
    std::srand(static_cast<unsigned int>(time(nullptr)));

    // 设置基本属性
    m_maxHealth = 200;
    setHealth(m_maxHealth);
    setAttackPower(7); // 普通攻击力为7

    // 设置初始位置
    setPosition(1700, 700);
    setX(1700);
    setY(700);
        // 设置移动速度
    m_moveSpeed = 4; // 正常状态下的移动速度
    m_angryMoveSpeed = 8; // 愤怒状态下速度翻倍

    // 设置初始状态
    m_currentState = NORMAL;
    m_moveDirection = 1; // 1表示向右，-1表示向左

    // 设置瞬移相关参数
    m_teleportThreshold = m_maxHealth / 5; // 每损失20%血量触发瞬移
    m_lastTeleportHealth = m_maxHealth;

    // 设置图片路径
    // 正常状态图片
    m_normal_Left_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Normal_Left.png";
    m_normal_Right_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Normal_Right.png";
    // 愤怒状态图片
    m_angry_Left_0_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Angry_Left_0.png";
    m_angry_Left_1_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Angry_Left_1.png";
    m_angry_Left_2_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Angry_Left_2.png";
    m_angry_Right_0_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Angry_Right_0.png";
    m_angry_Right_1_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Angry_Right_1.png";
    m_angry_Right_2_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Angry_Right_2.png";
    // 受伤状态图片
    m_hurt_Left_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Hurt_Left.png";
    m_hurt_Right_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Hurt_Right.png";
    // 攻击状态图片
    m_attack_Left_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Attack_Left.png";
    m_attack_Right_ImagePath = ":/Enderman/img/character/Enderman/Enderman_Attack_Right.png";
    // 初始化动画帧计数器
    m_animationFrame = 0;

    // 设置初始图片
    this->setPixmap(QPixmap(m_normal_Left_ImagePath));

    // 初始化计时器
    m_moveTimer = new QTimer(this);
    connect(m_moveTimer, &QTimer::timeout, this, &Enderman::update);
    m_moveTimer->start(50); // 每50毫秒更新一次

    // 初始化动画计时器
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &Enderman::updateImage);
    m_animationTimer->start(200); // 每200毫秒更新一次动画帧

    // 初始化瞬移计时器
    m_teleportTimer = new QTimer(this);
    connect(m_teleportTimer, &QTimer::timeout, this, &Enderman::teleportBehindSteve);
    m_teleportTimer->start(5000); // 每5秒瞬移一次

    m_randomMoveTimer = new QTimer(this);
    connect(m_randomMoveTimer, &QTimer::timeout, this, &Enderman::onRandomMoveTimeout);
    m_randomMoveTimer->start(1500); // 缩短方向改变间隔，使移动更加自然

    m_hurtTimer = new QTimer(this);
    connect(m_hurtTimer, &QTimer::timeout, this, &Enderman::onHurtTimerFinished);
    m_hurtTimer->setSingleShot(true); // 单次触发

    m_attackTimer = new QTimer(this);
    m_attackTimer->setSingleShot(true);

    // 初始化血条
    m_healthBarBackground = new QGraphicsLineItem();
    m_healthBarBackground->setPen(QPen(Qt::black, 50));
    m_healthBarBackground->setLine(0, 0, 1000, 0);
    m_healthBarBackground->setPos(600, 50);

    m_healthBar = new QGraphicsLineItem();
    m_healthBar->setPen(QPen(Qt::red, 50));
    m_healthBar->setLine(0, 0, 1000, 0);
    m_healthBar->setPos(600, 50);

    // 初始化Steve指针为空
    m_Steve = nullptr;
}

Enderman::~Enderman()
{
    // 释放计时器资源
    delete m_moveTimer;
    delete m_randomMoveTimer;
    delete m_hurtTimer;
    delete m_attackTimer;
    delete m_animationTimer;

    // 释放血条资源
    delete m_healthBar;
    delete m_healthBarBackground;
}

void Enderman::setScene(QGraphicsScene* scene)
{
    // 将血条添加到场景
    if (scene) {
        scene->addItem(m_healthBarBackground);
        scene->addItem(m_healthBar);
    }
}

void Enderman::takeDamage(int damage, Steve* attacker)
{
    // 如果已经死亡，不处理伤害
    if (getHealth() <= 0) return;
    if(damage > 0)
   { // 减少血量
    setHealth(getHealth() - damage);

    // 更新血条显示
    if (m_healthBar) {
        // 计算血量百分比
        float healthPercent = static_cast<float>(getHealth()) / m_maxHealth;
        // 更新红色血条长度
        m_healthBar->setLine(0, 0, 1000 * healthPercent, 0);
    }

    qDebug() << "Enderman受到伤害:" << damage << "点，当前血量:" << getHealth();

    // 检查是否需要瞬移（每损失20%血量触发一次）
    int healthLost = m_lastTeleportHealth - getHealth();
    if (healthLost >= m_teleportThreshold) {
        teleportBehindSteve();
        m_lastTeleportHealth = getHealth();
    }

    // 如果是正常状态，切换到愤怒状态
    if (m_currentState == NORMAL) {
        m_currentState = ANGRY;
        updateImage();
    }

    // 切换到受伤状态
    m_currentState = HURT;
    updateImage();

    // 启动受伤计时器，300毫秒后恢复
    m_hurtTimer->start(300);

    // 检查是否死亡
    if (getHealth() <= 0) {
        this->hide(); // 隐藏Enderman
        qDebug() << "Enderman已被击败!";
    }
    else
    {
        knockBack(attacker); // 触发击退效果
    }}
}

void Enderman::knockBack(Steve* attacker)
{
    int dx = getX() - attacker->getX();
    double distance = std::abs(dx);

    if (distance > 0)
    {
        int knockBackDistance = 30; // 减小击退距离
        int knockBackSteps = 5; // 减少击退步数，使动作更快
        int knockBackX = static_cast<int>((dx / distance) * knockBackDistance / knockBackSteps);

        for (int i = 0; i < knockBackSteps; ++i)
        {
            int newX = getX() + knockBackX;
            setPosition(newX, getY());
            setX(newX);
        }
    }
}

void Enderman::update()
{
    // 如果没有设置Steve或者Enderman已经死亡，不执行更新
    if (!m_Steve || getHealth() <= 0) return;

    // 根据当前状态执行不同的行为
    switch (m_currentState)
    {
        case NORMAL:
            randomMove();
            break;
        case ANGRY:
            chaseSteve();
            // 在愤怒状态下检查是否可以攻击Steve
            if (this->collidesWithItem(m_Steve) && !m_attackTimer->isActive()) {
                attackSteve();
                m_attackTimer->start(1000); // 攻击冷却1秒
            }
            break;
        case HURT:
            // 受伤状态下不移动，等待受伤计时器结束
            break;
        case TELEPORTING:
            // 瞬移状态，不执行其他操作
            break;
    }
}

void Enderman::onHurtTimerFinished()
{
    // 受伤状态结束，恢复到愤怒状态
    if (m_currentState == HURT)
    {
        m_currentState = ANGRY;
        updateImage();
    }
}

void Enderman::onRandomMoveTimeout()
{
    // 只在正常状态下随机改变方向
    if (m_currentState == NORMAL) {
        // 随机选择新的移动方向（左或右）
        m_moveDirection = (std::rand() % 2) * 2 - 1; // -1表示左，1表示右
    }
}

void Enderman::randomMove()
{
    // 在正常状态下随机移动
    if (m_currentState != NORMAL) return;



    // 计算新位置（只在X轴移动）
    int newX = getX() + m_moveDirection * m_moveSpeed;

    // 设置X轴移动范围限制
    const int MIN_X = 50;   // 左边界
    const int MAX_X = 1800; // 右边界

    // 确保不超出边界
    newX = std::max(MIN_X, std::min(MAX_X, newX));

    // 设置边界限制
    if (newX <= MIN_X) {
        m_moveDirection = 1; // 碰到左边界时改变方向
        newX = getX() + m_moveDirection * m_moveSpeed;
    } else if (newX >= MAX_X) {
        m_moveDirection = -1; // 碰到右边界时改变方向
        newX = getX() + m_moveDirection * m_moveSpeed;
    }

    // 更新位置（只更新X坐标）
    setX(newX);
    setPosition(newX, getY());

    // 移动方向已更新，updateImage()方法会根据m_moveDirection和m_currentState更新正确的图片
}

void Enderman::chaseSteve()
{
    // 获取Steve的位置
    int steveX = m_Steve->x();
    int endermanX = getX();

    // 确定移动方向
    if (steveX > endermanX) {
        m_moveDirection = 1; // 向右移动
    } else {
        m_moveDirection = -1; // 向左移动
    }

    // 计算新位置
    int newX = endermanX + m_moveDirection * m_angryMoveSpeed;

    // 更新位置
    setX(newX);
    setPosition(newX, getY());

    // 移动方向已更新，updateImage()方法会根据m_moveDirection和m_currentState更新正确的图片
}

void Enderman::attackSteve()
{
    // 如果Steve有盔甲，造成5点伤害，否则造成7点伤害
    int damage = m_Steve->hasArmor() ? 5 : 7;

    // 对Steve造成伤害
    m_Steve->takeDamage(damage);

    // 切换到攻击图片
    this->setPixmap(QPixmap(m_angry_Left_0_ImagePath));

    qDebug() << "Enderman攻击Steve造成" << damage << "点伤害!";
}

void Enderman::teleportBehindSteve()
{
        if (m_currentState == NORMAL)
        {
        // 在正常状态下随机瞬移（X轴50-1800，Y轴固定为700）
        int newX = std::rand() % 1751 + 50; // 生成50到1800之间的随机数
        setX(newX);
        setPosition(newX, 700); // Y轴固定在700
        setVisible(true); // 确保Enderman可见

        qDebug() << "Enderman随机瞬移到位置:" << newX << ", 700";
        }
        else if (m_currentState == ANGRY)
        {
        // 在被攻击状态下瞬移到Steve身后100像素处
        int steveX = m_Steve->x();

        // 简单判断Steve的朝向 - 假设向右移动时朝右，向左移动时朝左
        // 这是一个简化的判断，实际游戏中可能需要更准确的方法
        int moveDirection = 0;
        if (m_Steve->x() > getX())
        {
            moveDirection = 1; // Steve在Enderman右边
        }
        else
        {
            moveDirection = -1; // Steve在Enderman左边
        }

        // 计算瞬移位置（Steve身后100像素）
        int newX = steveX - (moveDirection * 100); // 瞬移到Steve相反方向100像素处

        // 确保瞬移位置在地图范围内
        newX = qBound(50, newX, 1800);//qBound 返回值要在最大值和最小值之间

        // 更新位置
        setX(newX);
        setPosition(newX, 700);

        qDebug() << "Enderman瞬移到Steve身后!";
    }

    // 瞬移后恢复到原来的状态
    updateImage();
}

void Enderman::updateImage()
{
    QString imagePath;

    // 根据当前状态和方向更新图片
    switch (m_currentState)
    {
        case NORMAL:
            if (m_moveDirection > 0)
            { // 向右
                imagePath = m_normal_Right_ImagePath;
            }
            else
            { // 向左
                imagePath = m_normal_Left_ImagePath;
            }
            break;

        case ANGRY:
            // 愤怒状态下循环播放三帧动画
            if (m_moveDirection > 0) { // 向右
                m_animationFrame = (m_animationFrame + 1) % 3; // 循环0-2
                imagePath = QString(":/Enderman/img/character/Enderman/Enderman_Angry_Right_%1").arg(m_animationFrame);
                // 修正文件名中的特殊情况
                if (m_animationFrame == 1) {
                    imagePath += ".png";
                } else if (m_animationFrame == 2) {
                    imagePath += ".png";
                } else {
                    imagePath += ".png";
                }
            } else { // 向左
                m_animationFrame = (m_animationFrame + 1) % 3; // 循环0-2
                imagePath = QString(":/Enderman/img/character/Enderman/Enderman_Angry_Left_%1.png").arg(m_animationFrame);
            }
            break;

        case HURT:
            if (m_moveDirection > 0) { // 向右
                imagePath = m_hurt_Right_ImagePath;
            } else { // 向左
                imagePath = m_hurt_Left_ImagePath;
            }
            break;

        case TELEPORTING:
            // 瞬移状态可以使用特殊图片
            if (m_moveDirection > 0) { // 向右
                imagePath = m_normal_Right_ImagePath;
            } else { // 向左
                imagePath = m_normal_Left_ImagePath;
            }
            break;
    }

    this->setPixmap(QPixmap(imagePath));
}
