#include "InventorySystem.h"

InventorySystem::InventorySystem(int inventorySize)
    : m_maxInventorySize(inventorySize)
{
}

bool InventorySystem::hasSpace() const 
{
    return m_inventory.size() < m_maxInventorySize;
}

bool InventorySystem::addItem(Item* item) 
{
    if (!item) return false;

    // 检查是否有相同类型的可堆叠物品
    for (Item* existingItem : m_inventory) 
    {
        if (existingItem->getName() == item->getName() && 
            existingItem->getCount() < existingItem->getStackLimit()) 
        {    
            int spaceLeft = existingItem->getStackLimit() - existingItem->getCount();
            int amountToAdd = qMin(spaceLeft, item->getCount());
            existingItem->setCount(existingItem->getCount() + amountToAdd);
            item->setCount(item->getCount() - amountToAdd);
            
            if (item->getCount() == 0) 
            {
                delete item;
                return true;
            }
        }
    }

    // 如果物品还有剩余且背包有空间，添加到新格子
    if (hasSpace()) {
        m_inventory.append(item);
        return true;
    }

    return false;
}

bool InventorySystem::removeItem(Item* item)
{
    int index = m_inventory.indexOf(item);
    if (index != -1) {
        m_inventory.removeAt(index);
        return true;
    }
    return false;
}

bool InventorySystem::equipItem(Equipment* equipment)
{
    if (!equipment) return false;

    Equipment::Type slot = equipment->getType();
    
    // 如果该槽位已有装备，先卸下
    if (m_equipped.contains(slot))
    {
        unequipItem(slot);
    }

    m_equipped[slot] = equipment;
    return true;
}

bool InventorySystem::unequipItem(Equipment::Type slot) {
    if (!m_equipped.contains(slot)) return false;

    Equipment* equipment = m_equipped[slot];
    if (hasSpace()) {
        m_inventory.append(equipment);
        m_equipped.remove(slot);
        return true;
    }

    return false;
}

Equipment* InventorySystem::getEquippedItem(Equipment::Type slot) const {
    return m_equipped.value(slot, nullptr);
}

QMap<QString, int> InventorySystem::getTotalAttributeModifiers() const {
    QMap<QString, int> totalModifiers;

    // 累加所有已装备物品的属性加成
    for (const Equipment* equipment : m_equipped.values()) {
        const QMap<QString, int>& modifiers = equipment->getAttributeModifiers();
        for (auto it = modifiers.constBegin(); it != modifiers.constEnd(); ++it) {
            totalModifiers[it.key()] += it.value();
        }
    }

    return totalModifiers;
}
