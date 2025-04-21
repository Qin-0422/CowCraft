#include "Equipment.h"

#include "Equipment.h"

Equipment::Equipment(const QString& name, const QString& description, Type type,
                      const QMap<QString, int>& attributeModifiers, const QString& iconPath)
     : Item(name, description, Item::ARMOR, false, 1, iconPath)  // 修改基类构造函数调用
     , m_type(type)
     , m_attributeModifiers(attributeModifiers)
 {
 }

void Equipment::use() {
    // 装备的使用方法在InventorySystem中通过equipItem实现
}
