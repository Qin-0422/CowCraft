#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include <QVector>
#include <QMap>
#include "Item.h"
#include "Equipment.h"

class InventorySystem 
{
public:
    InventorySystem(int inventorySize = 20);

    // 物品栏操作
    bool addItem(Item* item);
    bool removeItem(Item* item);
    QVector<Item*> getInventory() const { return m_inventory; }

    // 装备栏操作
    bool equipItem(Equipment* equipment);
    bool unequipItem(Equipment::Type slot);
    Equipment* getEquippedItem(Equipment::Type slot) const;
    QMap<QString, int> getTotalAttributeModifiers() const;

private:
    QVector<Item*> m_inventory;  // 物品栏
    QMap<Equipment::Type, Equipment*> m_equipped;  // 装备栏
    int m_maxInventorySize;

    bool hasSpace() const;
};

#endif // INVENTORYSYSTEM_H
