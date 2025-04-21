#ifndef CRAFTINGUI_H
#define CRAFTINGUI_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include "Item.h"
#include "Material.h"
#include "InventoryUI.h"

class CraftingUI : public QWidget
{
    Q_OBJECT

public:
    explicit CraftingUI(QWidget *parent = nullptr);
    void show();
    void hide();
    bool isVisible() const;
    void setInventory(const QList<Item*>& inventory);

signals:
    void craftingCompleted(Item* craftedItem);

private:
    QList<Item*> m_inventory;            // 玩家的物品栏引用
    InventoryUI* m_inventoryView;        // 物品栏视图

    void initializeUI();
    void setupConnections();
};

#endif // CRAFTINGUI_H
