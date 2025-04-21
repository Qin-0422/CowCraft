#ifndef VILLAGER_H
#define VILLAGER_H

#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QTimer>
#include <QObject>
#include "Steve.h"
class Villager_F : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Villager_F(QGraphicsItem* parent = nullptr);
    ~Villager_F();

    // 设置和获取生命值
    void setHealth(int health) { m_health = health; }
    int getHealth() const { return m_health; }

    // 设置位置
    void setPosition(int x, int y);

    // 显示对话
    void showDialog();
    void showSpecialDialog(); // 添加特殊对话方法
    void showThirdDialog(); // 添加第三种对话方法（已获得武器后）
    void hideDialog();

    // 交互
    void interact();

    // 检查是否在交互范围内
    bool isInRange(int x, int y) const;
    // 添加设置Steve的方法
    void setSteve(Steve* steve) { m_Steve = steve; }

    // 添加检查玩家距离的方法
    void checkPlayerDistance();
protected:
    // 鼠标事件
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int m_health;
    QLabel* m_dialogLabel;
    const int INTERACTION_RANGE = 100; // 交互范围（像素）
    QTimer* m_dialogTimer;
    QTimer* m_checkTimer;  // 添加检测计时器
    Steve* m_Steve;
};

#endif // VILLAGER_H
