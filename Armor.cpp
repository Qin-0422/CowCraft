#include "Armor.h"

Armor::Armor(const QString& name, const QString& description, const QString& iconPath,int defense)
 : Item(name, description, Item::ARMOR, false, 64, iconPath), m_defense(defense)
{
}
