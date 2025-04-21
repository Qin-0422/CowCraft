#ifndef MATERIAL_H
#define MATERIAL_H


#include "Item.h"

class Material : public Item
{
public:
    Material(const QString& name, const QString& description, const QString& iconPath = "");
    void use() override;
};

#endif // MATERIAL_H