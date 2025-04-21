#ifndef ARMOR_H
#define ARMOR_H
#include "Item.h"

class Armor : public Item
{
public:
    Armor(const QString& name, const QString& description, const QString& iconPath, int defense = 2);
    int getDefense() const { return m_defense; }
    void setDefense(int defense) { m_defense = defense; }
    virtual void use() override {}

private:
    int m_defense;
};

#endif // ARMOR_H
