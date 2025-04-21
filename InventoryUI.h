#ifndef INVENTORYUI_H
#define INVENTORYUI_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QApplication>
#include "Item.h"
#include "Equipment.h"

class InventoryUI : public QWidget
{
    Q_OBJECT
public:
    explicit InventoryUI(QWidget *parent = nullptr);
    void updateInventory(const QList<Item*>& items, const QList<Equipment*>& equipments);
    // 添加箱子相关功能
    void addItemsFromChest(const QList<Item*>& chestItems);
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QGridLayout* m_layout;
    QList<QPushButton*> m_slots;
    static const int INVENTORY_ROWS = 2;
    static const int INVENTORY_COLS = 9;

    void createInventorySlots();
    void setupSlotMenu(QPushButton* slot, Item* item);
    void updateSlot(int index, Item* item);
    QString createDetailedTooltip(Item* item); // 创建详细的物品提示信息
    QPoint m_dragStartPosition;
signals:
    void itemUsed(int index);
    void itemEquipped(int index);
    void itemDropped(int index);
    void chestItemsReceived(const QList<Item*>& items);

private slots:
    void onSlotClicked();
    void onSlotRightClicked();
};

#endif // INVENTORYUI_H
