#ifndef HEALTHUI_H
#define HEALTHUI_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>

class HealthUI : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    explicit HealthUI(QObject *parent = nullptr);
    ~HealthUI();

    // 设置当前血量并更新显示
    void setHealth(int health);
    // 获取当前血量
    int getHealth() const { return m_currentHealth; }
    // 获取最大血量
    int getMaxHealth() const { return m_maxHealth; }
    // 受到伤害
    void takeDamage(int damage);
    // 恢复血量
    void heal(int amount);

private slots:
    // 检查是否可以恢复血量
    void checkHealing();
    // 执行血量恢复
    void performHealing();

private:
    // 更新心形显示
    void updateHeartsDisplay();
    // 重置恢复计时器
    void resetHealingTimer();

    int m_currentHealth;      // 当前血量
    int m_maxHealth;          // 最大血量
    QList<QGraphicsPixmapItem*> m_hearts;  // 心形图标列表
    
    QTimer* m_healingCheckTimer;  // 检查是否可以恢复血量的计时器
    QTimer* m_healingTimer;       // 执行血量恢复的计时器
    bool m_canHeal;              // 是否可以恢复血量
    QPixmap m_fullHeartPixmap;   // 完整心形图标
    QPixmap m_halfHeartPixmap;   // 半颗心形图标
    QPixmap m_emptyHeartPixmap;  // 空心形图标
    
    int m_lastDamageTime;        // 上次受伤时间
};

#endif // HEALTHUI_H