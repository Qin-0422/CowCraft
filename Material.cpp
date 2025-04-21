#include "Material.h"

Material::Material(const QString& name, const QString& description, const QString& iconPath )
    : Item(name, description,  Item::MATERIAL, false, 64, iconPath)
{
}

void Material::use()
{
    // 材料类物品通常不能直接使用
}
