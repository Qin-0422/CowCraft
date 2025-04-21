#ifndef FAKEWEAPON_H
#define FAKEWEAPON_H

#include "Equipment.h"

class FakeWeapon : public Equipment
{
public:
    FakeWeapon();
    virtual void use() override;
};

#endif // FAKEWEAPON_H