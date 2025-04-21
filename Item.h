#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QPixmap>
class Item {
public:
    enum ItemType
    {
        NONE,
        WEAPON,     // 武器
        ARMOR,      // 防具
        POTION,     // 药水
        MATERIAL,   // 材料
        KEY_ITEM ,  // 关键道具
    };
    Item(const QString& name, const QString& description, ItemType type,
         bool isConsumable, int stackLimit = 1, const QString& iconPath = "");//名字 描述 种类 是否可消耗 堆叠限制 图片路径
    virtual ~Item() = default;

    // 基本属性
    bool isConsumable() const { return m_isConsumable; }
    void setCount(int count) { m_count = count; }
    void decreaseCount() { if (m_count > 0) m_count--; }  // 添加decreaseCount函数
    ItemType getType() const { return m_type; }
    QPixmap getIcon() const { return m_icon; }
    QString getName() const { return m_name; }
    QString getDescription() const { return m_description; }
    int getStackLimit() const { return m_stackLimit; }
    int getCount() const { return m_count; }
    // 堆叠相关
    bool canStack() const { return m_stackLimit > 1; }
    bool canAddToStack(int amount) const { return m_count + amount <= m_stackLimit; }
    void addToStack(int amount);
    bool removeFromStack(int amount);

    // 虚函数，供子类重写
    virtual void use() = 0;
    virtual bool canUse() const;
    virtual QString getTypeString() const;

     virtual QString getIconPath() const { return m_iconPath; }
protected:
    QString m_name;
    QString m_description;
    bool m_isConsumable;
    int m_stackLimit;
    int m_count;
    ItemType m_type;
    QPixmap m_icon;

    QString m_iconPath; //图片路径
   };

#endif // ITEM_H
