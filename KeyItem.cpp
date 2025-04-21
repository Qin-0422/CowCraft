#include "KeyItem.h"
#include <QDebug>

KeyItem::KeyItem(const QString& name, const QString& description, bool isConsumable, 
                 int stackLimit, const QString& iconPath)
    : Item(name, description, Item::KEY_ITEM, isConsumable, stackLimit, iconPath)
{
}

void KeyItem::placeTrigger(int x, int y, Level_Map* map)
{
    if (map) {
        // 将指定位置设置为Trigger
        map->setTrigger(x, y);
        qDebug() << "Trigger placed at:" << x << "," << y;
    }
}

void KeyItem::use()
{
    if (canUse()) {
        if (isConsumable()) {
            removeFromStack(1);
        }
    }
}
