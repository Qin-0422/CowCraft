#include "FakeWeapon.h"

FakeWeapon::FakeWeapon()
    : Equipment("fake weapon", "村民给予的假武器", Equipment::Type::WEAPON,
              {{"attackPower", 4}}, ":/Item/img/Item/fake.jpg")
{
    // 设置武器属性
}

void FakeWeapon::use()
{
    // 武器的使用方法在Steve类中通过左键点击实现
}
