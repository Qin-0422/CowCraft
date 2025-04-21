#ifndef KEYITEM_H
#define KEYITEM_H
#include "Level_Map.h"
#include "Item.h"

class KeyItem : public Item
{
public:
    KeyItem(const QString& name, const QString& description, bool isConsumable = true, 
            int stackLimit = 1, const QString& iconPath = "");
    virtual void use() override;
    static void placeTrigger(int x, int y, Level_Map* map);
};

#endif // KEYITEM_H
