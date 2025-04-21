#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "Item.h"
#include <QMap>

class Equipment : public Item 
{
public:
    enum class Type 
    {
        CHESTPLATE,//
        WEAPON
    };

    Equipment(const QString& name, const QString& description, Type type,
              const QMap<QString, int>& attributeModifiers, const QString& iconPath);

    Type getType() const 
    {
         return m_type; 
    }    
    QMap<QString, int> getAttributeModifiers() const { return m_attributeModifiers; }

    // 实现物品使用方法
    virtual void use() override;

private:
    Type m_type;
    QMap<QString, int> m_attributeModifiers; // 存储装备对各属性的加成值
};

#endif // EQUIPMENT_H
