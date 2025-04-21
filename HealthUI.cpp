#include "HealthUI.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>

HealthUI::HealthUI(QObject *parent) : QObject(parent), QGraphicsItemGroup()
{
    // 初始化血量
    m_maxHealth = 20;      // 最大血量为20（10颗心）
    m_currentHealth = 20;  // 初始血量为满
    m_canHeal = false;
    
    // 加载心形图标
    m_fullHeartPixmap = QPixmap(":/Heart/img/character/Heart/Full_Heart.png");
    m_halfHeartPixmap = QPixmap(":/Heart/img/character/Heart/Half_Heart.png");
    m_emptyHeartPixmap = QPixmap(":/Heart/img/character/Heart/Empty_Heart.png");
    
    // 如果图标加载失败，创建默认图标
    if (m_fullHeartPixmap.isNull()) 
    {
        m_fullHeartPixmap = QPixmap(40, 40);
        m_fullHeartPixmap.fill(Qt::red);
    }
    if (m_halfHeartPixmap.isNull()) 
    {
        m_halfHeartPixmap = QPixmap(40, 40);
        m_halfHeartPixmap.fill(QColor(255, 150, 150));
    }
    if (m_emptyHeartPixmap.isNull()) 
    {
        m_emptyHeartPixmap = QPixmap(40, 40);
        m_emptyHeartPixmap.fill(Qt::gray);
    }
    
    // 创建10颗心形图标
    for (int i = 0; i < 10; ++i) {
        QGraphicsPixmapItem* heart = new QGraphicsPixmapItem(m_fullHeartPixmap);
        heart->setPos(i * 40, 0);  // 每颗心之间间隔40像素
        addToGroup(heart);
        m_hearts.append(heart);
    }
    
    // 设置位置为(2000, 0)，大小为400*40
    setPos(2000, 0);
    
    // 初始化恢复计时器
    m_healingCheckTimer = new QTimer(this);
    connect(m_healingCheckTimer, &QTimer::timeout, this, &HealthUI::checkHealing);
    m_healingCheckTimer->start(5000);  // 5秒检查一次是否可以恢复
    
    m_healingTimer = new QTimer(this);
    connect(m_healingTimer, &QTimer::timeout, this, &HealthUI::performHealing);
    
    m_lastDamageTime = 0;
}

HealthUI::~HealthUI()
{
    // 清理资源
    m_healingCheckTimer->stop();
    m_healingTimer->stop();
    delete m_healingCheckTimer;
    delete m_healingTimer;
    
    // 清理心形图标
    qDeleteAll(m_hearts);
    m_hearts.clear();
}

void HealthUI::setHealth(int health)
{
    // 确保血量在有效范围内
    m_currentHealth = qBound(0, health, m_maxHealth);
    updateHeartsDisplay();
    
    // 如果血量变化，重置恢复计时器
    resetHealingTimer();
}

void HealthUI::takeDamage(int damage)
{
    // 受到伤害，减少血量
    m_currentHealth = qMax(0, m_currentHealth - damage);// 确保血量不超过最大血量
    updateHeartsDisplay();
    
    // 记录受伤时间并重置恢复计时器
    m_lastDamageTime = QDateTime::currentMSecsSinceEpoch();// 记录受伤时间
    m_canHeal = false;
    m_healingTimer->stop();
    m_healingCheckTimer->start(5000);  // 5秒后检查是否可以恢复
    
    qDebug() << "Steve受到伤害:" << damage << "点，当前血量:" << m_currentHealth;
}

void HealthUI::heal(int amount)
{
    // 恢复血量
    m_currentHealth = qMin(m_maxHealth, m_currentHealth + amount);// 确保血量不超过最大血量
    updateHeartsDisplay();// 更新显示
    
    qDebug() << "Steve恢复血量:" << amount << "点，当前血量:" << m_currentHealth;
}

void HealthUI::checkHealing()
{
    // 检查上次受伤时间，如果超过5秒，开始恢复
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastDamageTime >= 5000 && m_currentHealth < m_maxHealth) 
    {
        m_canHeal = true;
        m_healingCheckTimer->stop();
        m_healingTimer->start(3000);  // 每3秒恢复一次
    }
}

void HealthUI::performHealing()
{
    // 如果可以恢复且血量未满，恢复半颗心（1点血量）
    if (m_canHeal && m_currentHealth < m_maxHealth)
     {
        heal(1);  // 恢复1点血量（半颗心）       
        // 如果血量已满，停止恢复
        if (m_currentHealth >= m_maxHealth) 
        {
            m_healingTimer->stop();
        }
    }
}

void HealthUI::updateHeartsDisplay()
{
    // 更新心形显示
    int health = m_currentHealth;
    
    for (int i = 0; i < 10; ++i)
     {
        if (health >= 2)
         {
            // 完整的心
            m_hearts[i]->setPixmap(m_fullHeartPixmap);
            health -= 2;
        } 
        else if (health == 1) 
        {
            // 半颗心
            m_hearts[i]->setPixmap(m_halfHeartPixmap);
            health -= 1;
        }
         else 
         {
            // 空心
            m_hearts[i]->setPixmap(m_emptyHeartPixmap);
        }
    }
}

void HealthUI::resetHealingTimer()
{
    // 重置恢复计时器
    m_lastDamageTime = QDateTime::currentMSecsSinceEpoch();
    m_canHeal = false;
    m_healingTimer->stop();
    m_healingCheckTimer->start(5000);  // 5秒后检查是否可以恢复
}
