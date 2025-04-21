#ifndef STEVE_H
#define STEVE_H
#include"Character.h"
#include <QKeyEvent>
#include <QTimer>
#include "Level_Map.h"
#include <QObject>
#include <QSet>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include "InventoryUI.h"
#include "CraftingUI.h"
#include "KeyItem.h"
#include "TNT.h"
#include "HealthUI.h"
#include <QMessageBox>
#include "Armor.h"
class Steve:  public QObject ,public Character ,public QGraphicsPixmapItem
{
Q_OBJECT

public:
    Steve();
    ~Steve();
public:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override; // 添加焦点丢失事件处理
    void updateFacing(bool facingRight); // 更新朝向
    bool hasWeapon() const; // 检查是否装备武器
    void checkInteractiveArea(); // 检查交互区域
    void checkInteractiveAreaAtPosition(int x, int y);
    bool eventFilter(QObject *watched, QEvent *event);
    const QList<Item*>& getInventory() const { return m_inventory; } // 获取物品栏
    void setInventory(const QList<Item*>& inventory) { m_inventory = inventory; } // 设置物品栏
    void giveFakeWeapon(); // 给予假武器
    //按键事件
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void SteveMove();
    void setScene(QGraphicsScene *scene);  // 设置场景函数
    void setMap(Level_Map *map);  // 设置地图函数 // 新增函数
    bool canMoveTo(int newX, int newY);
    bool canFallTo(int newX, int newY);//判断是否挂在梯子上，从而取消重力作用
    void jump();
    void fall(); // 下落函数
    void climbLadder();
    void checkPosition(); // 添加检测位置的函数
    //箱子相关函数
    void interactWithChest(int chestId);
    bool hasOpenedChest(int chestId) const;
    
    // 血量系统相关方法
    void takeDamage(int damage) ;  // 受到伤害
    void heal(int amount);       // 恢复血量
    void equipArmor(Armor* armor);  // 装备盔甲
    void unequipArmor();  // 卸下盔甲
    bool hasArmor() const { return m_currentArmor != nullptr; }  // 是否装备盔甲
    HealthUI* getHealthUI() const { return m_healthUI; }  // 获取血量UI
    void onItemUsed(int index);
    void onItemEquipped(int index);
    void onItemDropped(int index);
    void onCraftingCompleted(Item* craftedItem);
    void equipWeapon(Item* weapon);
    void unequipWeapon();
    void attack(); // 添加攻击方法
    int getWeaponType() const;
    void initializeChestItems(); // 初始化箱子物品
    void addItemsToInventory(const QList<Item*>& items); // 添加物品到物品栏
    void addItem(Item *);//收集怪物死亡后物品
signals:
    void positionChanged(); // 添加位置改变的信号
    void changeLevel(); // 添加切换下一关卡的信号
    void changeBackLevel();//添加切换到上一关卡的信号
    void inventoryChanged();//物品栏发生改变的信号
    void craftingCompleted(Item* craftedItem); // 合成完成的信号
public slots:
    void toggleInventory();//切换（显示/隐藏）物品栏界面
    void toggleCraftingTable();//切换（显示/隐藏）工作台界面
    void resetAttackPower();
private:
    bool m_isJumping;
    int  current_jumpHeight;
    int jump_can_height;
    bool m_facingRight; // 朝向标志，true表示朝右
    //物品栏
    InventoryUI* m_inventoryUI;
    CraftingUI* m_craftingUI;
    QList<Item*> m_inventory;
    QList<Equipment*> m_equipments;
    Armor* m_currentArmor ;  // 当前装备的盔甲
    //按键组合
    QList<int> mKeyList;
    QTimer *m_SteveMoveTimer;
    QTimer *m_jumpTimer;
    QTimer *m_fallTimer;
    QTimer *m_attackTimer;
    QTimer *m_magmaDamageTimer;  // Magma方块伤害计时器
    Level_Map *m_map;  // 添加指向地图的指针

    Item* m_currentWeapon = nullptr;
    QSet<int> m_openedChests; // 记录已经打开过的箱子ID
    QMap<int, QList<Item*>> m_chestItems; // 存储不同箱子的物品

    
    // 血量UI相关
    HealthUI* m_healthUI;  // 血量UI
    QTimer* m_damageTimer; // 伤害冷却计时器
};

#endif // STEVE_H
