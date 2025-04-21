#include "Item.h"

Item::Item(const QString& name, const QString& description, ItemType type,
           bool isConsumable, int stackLimit, const QString& iconPath)
    : m_name(name)
    , m_description(description)
    , m_type(type)
    , m_isConsumable(isConsumable)
    , m_stackLimit(stackLimit)
    , m_count(1)
{
    if (!iconPath.isEmpty()) {
        m_icon = QPixmap(iconPath);
    }
}

void Item::addToStack(int amount)
{
    if (canAddToStack(amount))
    {
        m_count += amount;
    }
}

bool Item::removeFromStack(int amount)
{
    if (m_count >= amount)
    {
        m_count -= amount;
        return true;
    }
    return false;
}

bool Item::canUse() const
{
    return m_count > 0;
}

QString Item::getTypeString() const
{
    switch (m_type) {
    case WEAPON:    return "武器";
    case ARMOR:     return "防具";
    case POTION:    return "药水";
    case MATERIAL:  return "材料";
    case KEY_ITEM:  return "关键道具";
    default:        return "未知";
    }
}
