#include "InventoryUI.h"
#include <QDebug>
#include <QContextMenuEvent>
#include <QTimer>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QFileInfo>
#include <QMenu>
#include <QLabel>
#include <QToolTip>
#include <QScreen>
#include "Equipment.h"
#include "Armor.h"

InventoryUI::InventoryUI(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("物品栏");
    setFixedSize(800, 600);
    setAcceptDrops(true);
    
    // 将窗口移动到屏幕中央，并向上调整300像素
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    move((screenGeometry.width() - width()) / 2, (screenGeometry.height() - height()) / 2 - 300);

    m_layout = new QGridLayout(this);
    createInventorySlots();
}

void InventoryUI::createInventorySlots()
{
    for (int row = 0; row < INVENTORY_ROWS; ++row)
    {
        for (int col = 0; col < INVENTORY_COLS; ++col)
        {
            QPushButton* slot = new QPushButton(this);
            slot->setFixedSize(60, 60);
            slot->setStyleSheet(
                "QPushButton { background-color: #444444; border: 2px solid #666666; }"
                "QPushButton:hover { border-color: #888888; background-color: #555555; }"
                "QPushButton { icon-size: 40px 40px; }"
                "QPushButton { text-align: right; padding-right: 5px; padding-bottom: 2px; color: white; font-weight: bold; }"
            );
            slot->setProperty("slotIndex", row * INVENTORY_COLS + col);

            m_layout->addWidget(slot, row + 2, col);
            m_slots.append(slot);

            connect(slot, &QPushButton::clicked, this, &InventoryUI::onSlotClicked);
            // 启用鼠标追踪以支持悬停效果显示工具提示
            slot->setMouseTracking(true);
        }
    }
}



void InventoryUI::updateInventory(const QList<Item*>& items, const QList<Equipment*>&)
{
    // 创建一个映射来存储相同类型物品的数量
    QMap<QString, QPair<Item*, int>> itemStacks;
    QMap<QString, int> itemSlots;

    // 首先统计所有相同类型的物品
    for (Item* item : items)
    {
        if (item)
        {
            QString itemKey = item->getName();
            // 如果已经存在相同类型的物品，累加数量
            if (!itemStacks.contains(itemKey)) {
                // 如果是新物品类型，直接添加
                itemStacks[itemKey] = qMakePair(item, item->getCount());
            } else {
                // 如果已经存在相同类型的物品，累加数量
                itemStacks[itemKey].second += item->getCount();
            }
            itemSlots[itemKey] = -1;
        }
    }

    // 清空所有槽位
    for (int i = 0; i < m_slots.size(); ++i) {
        m_slots[i]->setText("");
        m_slots[i]->setIcon(QIcon());
        m_slots[i]->setToolTip("");
        m_slots[i]->setProperty("item", QVariant());// 清空物品指针属性
    }

    // 更新物品栏，将相同类型的物品堆叠在一起
    int currentSlot = 0;
    for (auto it = itemStacks.begin(); it != itemStacks.end(); ++it)
    {
        if (currentSlot < m_slots.size())
        {
            Item* item = it.value().first;
            int count = it.value().second;
            item->setCount(count); // 更新物品数量
            updateSlot(currentSlot, item);
            itemSlots[it.key()] = currentSlot;
            currentSlot++;
        }
    }


}

void InventoryUI::updateSlot(int index, Item* item)
{
    if (index >= 0 && index < m_slots.size() && item)
    {
        QPushButton* slot = m_slots[index];

        // 存储物品指针到按钮属性中，以便后续访问
        slot->setProperty("item", QVariant::fromValue(static_cast<void*>(item)));

        // 设置图标
        QString iconPath = item->getIconPath();
        qDebug() << "原始图片路径:" << iconPath;
        
        if (!iconPath.isEmpty())
        {
            // 检查图标路径是否存在，如果不存在尝试修正
            QFileInfo fileInfo(iconPath);
            if (!fileInfo.exists())
            {
                // 尝试不同的路径组合
                QStringList pathsToTry = 
                {
                    QString("img/Item/%1").arg(iconPath),
                    QString(":/img/Item/%1").arg(iconPath),
                    QString(":/Item/%1").arg(iconPath),
                    iconPath
                };
                
                for (const QString& path : pathsToTry)
                {
                    qDebug() << "尝试加载图片路径:" << path;
                    QFileInfo tryInfo(path);
                    if (tryInfo.exists() || QFile::exists(":" + path))
                    {
                        iconPath = path;
                        qDebug() << "成功找到图片路径:" << iconPath;
                        break;
                    }
                }
            }
            
            QIcon icon(iconPath);
            if (!icon.isNull())
            {
                slot->setIcon(icon);
                slot->setIconSize(QSize(60, 60));
                
                // 显示物品数量
                if (item->getCount() > 1) 
                {
                    slot->setText(QString::number(item->getCount()));
                } 
                else 
                {
                    slot->setText("");
                }
            }
            else
            {
                qDebug() << "无法加载图标:" << iconPath;
                // 如果图标加载失败，显示物品名称
                slot->setText(item->getName());
            }
        }
        else
        {
            // 如果没有图标，显示物品名称和数量
            if (item->getCount() > 1)
            {
                slot->setText(QString("%1 x%2").arg(item->getName()).arg(item->getCount()));
            }
            else
            {
                slot->setText(item->getName());
            }
        }

        // 设置工具提示显示物品详细信息
        QString tooltip = createDetailedTooltip(item);
        slot->setToolTip(tooltip);
        
        // 启用鼠标追踪以支持悬停效果
        slot->setMouseTracking(true);
    }
}

QString InventoryUI::createDetailedTooltip(Item* item)
{
    if (!item) return "";
    
    QString tooltip = QString("<div style='background-color: #333333; color: white; padding: 5px; border: 1px solid #555555;'>");
    
    // 物品名称 (使用不同颜色区分不同类型)
    QString nameColor;
    switch (item->getType()) {
        case Item::WEAPON: nameColor = "#ff9900"; break; // 橙色
        case Item::ARMOR: nameColor = "#3399ff"; break; // 蓝色
        case Item::MATERIAL: nameColor = "#99cc33"; break; // 绿色
        case Item::KEY_ITEM: nameColor = "#cc33ff"; break; // 紫色
        default: nameColor = "white"; break;
    }
    
    tooltip += QString("<div style='font-weight: bold; color: %1; font-size: 12px;'>%2</div>").arg(nameColor).arg(item->getName());
    
    // 物品类型
    tooltip += QString("<div style='color: #aaaaaa; font-size: 10px;'>类型: %1</div>").arg(item->getTypeString());
    
    // 物品描述
    tooltip += QString("<div style='margin-top: 5px;'>%1</div>").arg(item->getDescription());
    
    // 堆叠数量
    if (item->canStack()) {
        tooltip += QString("<div style='margin-top: 5px;'>数量: %1/%2</div>").arg(item->getCount()).arg(item->getStackLimit());
    }
    
    // 特殊属性 (根据物品类型显示不同信息)
    if (item->getType() == Item::WEAPON) {
        // 尝试将物品转换为Equipment类型以获取属性修饰符
        if (Equipment* weapon = dynamic_cast<Equipment*>(item)) {
            QMap<QString, int> modifiers = weapon->getAttributeModifiers();
            if (modifiers.contains("attackPower")) {
                tooltip += QString("<div style='color: #ff6666;'>伤害: +%1</div>").arg(modifiers["attackPower"]);
            }
        }
    } 
    else if (item->getType() == Item::ARMOR) {
        // 尝试将物品转换为Armor类型以获取防御值
        if (Armor* armor = dynamic_cast<Armor*>(item)) {
            tooltip += QString("<div style='color: #6699ff;'>防御: +%1</div>").arg(armor->getDefense());
        }
    }
    
    // 是否可消耗
    if (item->isConsumable()) {
        tooltip += "<div style='color: #aaaaaa; font-style: italic; margin-top: 5px;'>使用后消耗</div>";
    }
    
    tooltip += "</div>";
    return tooltip;
}



void InventoryUI::onSlotRightClicked()
{
    // 右键点击不再显示菜单，保留函数以便将来可能的扩展
    // 物品信息通过鼠标悬停时的工具提示(tooltip)显示
}

void InventoryUI::onSlotClicked()
{
    // 获取发送信号的按钮
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    // 获取按钮对应的物品栏位置
    int slotIndex = button->property("slotIndex").toInt();

    // 只处理中键点击操作，右键点击不再触发任何操作
    if (QApplication::mouseButtons() & Qt::MiddleButton)
    {
        // 中键点击：丢弃物品
        emit itemDropped(slotIndex);
    }
    // 右键点击不做任何处理，鼠标悬停时会自动显示工具提示
}

void InventoryUI::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-item")) {
        event->acceptProposedAction();
    }
}

void InventoryUI::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPushButton* slot = qobject_cast<QPushButton*>(childAt(event->pos()));
        if (slot && m_slots.contains(slot)) {
            // 只有当槽位中有物品时才记录拖拽起始位置
            Item* item = static_cast<Item*>(slot->property("item").value<void*>());
            if (item) 
            {
                m_dragStartPosition = event->pos();
            }
        }
    }
}

void InventoryUI::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    QPushButton* sourceSlot = qobject_cast<QPushButton*>(childAt(m_dragStartPosition));
    if (!sourceSlot || !m_slots.contains(sourceSlot)) {
        return;
    }

    int slotIndex = sourceSlot->property("slotIndex").toInt();
    
    // 获取物品指针
    Item* item = static_cast<Item*>(sourceSlot->property("item").value<void*>());
    if (!item) 
    {
        return; // 如果槽位没有物品，不创建拖拽
    }

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << slotIndex;
    mimeData->setData("application/x-item", itemData);
    
    // 添加物品类型信息
    QByteArray typeData;
    QDataStream typeStream(&typeData, QIODevice::WriteOnly);
    typeStream << item->getType();
    mimeData->setData("application/x-item-type", typeData);
    
    // 添加物品名称信息
    QByteArray nameData;
    QDataStream nameStream(&nameData, QIODevice::WriteOnly);
    nameStream << item->getName();
    mimeData->setData("application/x-item-name", nameData);
    
    drag->setMimeData(mimeData);

    // 设置拖动时的图标
    QIcon icon = sourceSlot->icon();
    if (!icon.isNull()) 
    {
        QPixmap pixmap = icon.pixmap(50, 50);
        if (!pixmap.isNull()) {
            drag->setPixmap(pixmap);
            drag->setHotSpot(QPoint(25, 25));
        }
    }
    
    // 输出调试信息
    qDebug() << "物品栏开始拖动，起始位置:" << m_dragStartPosition << "物品:" << item->getName();

    Qt::DropActions supportedActions = Qt::MoveAction;
    Qt::DropAction result = drag->exec(supportedActions, Qt::MoveAction);
    
    if (result == Qt::MoveAction) 
    {
        // 如果拖放成功，清空源槽位
        sourceSlot->setIcon(QIcon());
        sourceSlot->setText("");
        sourceSlot->setProperty("item", QVariant());
        sourceSlot->setToolTip("");
    }
}

void InventoryUI::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasFormat("application/x-item") || mimeData->hasFormat("application/x-crafting-item")) {
        QPushButton* targetSlot = qobject_cast<QPushButton*>(childAt(event->pos()));
        if (!targetSlot || !m_slots.contains(targetSlot)) {
            event->ignore();
            return;
        }

        int targetIndex = targetSlot->property("slotIndex").toInt();
        
        if (mimeData->hasFormat("application/x-item")) {
            QByteArray itemData = mimeData->data("application/x-item");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            int sourceIndex;
            dataStream >> sourceIndex;

            // 获取源槽位和目标槽位的物品指针
            QPushButton* sourceSlot = m_slots[sourceIndex];
            Item* sourceItem = static_cast<Item*>(sourceSlot->property("item").value<void*>());
            Item* targetItem = static_cast<Item*>(targetSlot->property("item").value<void*>());

            // 交换物品
            sourceSlot->setProperty("item", QVariant::fromValue(static_cast<void*>(targetItem)));
            targetSlot->setProperty("item", QVariant::fromValue(static_cast<void*>(sourceItem)));

            // 更新显示
            if (sourceItem) updateSlot(targetIndex, sourceItem);
            if (targetItem) updateSlot(sourceIndex, targetItem);

            event->acceptProposedAction();
        }
    }
}


void InventoryUI::setupSlotMenu(QPushButton* slot, Item* item)
{
    if (!item) return;

    // 注意：我们不再在这里创建菜单，而是在右键点击时动态创建
    // 这样可以避免内存泄漏和菜单显示问题
}

// 添加箱子物品到物品栏
void InventoryUI::addItemsFromChest(const QList<Item*>& chestItems)
{
    if (chestItems.isEmpty()) {
        return;
    }

    // 发出信号通知游戏逻辑处理箱子物品的添加
    emit chestItemsReceived(chestItems);

    // 显示一个简单的提示
    QLabel* notification = new QLabel("获得了箱子中的物品！", this);
    notification->setStyleSheet("background-color: rgba(0, 0, 0, 150); color: white; padding: 10px; border-radius: 5px;");
    notification->setAlignment(Qt::AlignCenter);
    notification->setFixedSize(200, 50);
    notification->move((width() - notification->width()) / 2, height() - notification->height() - 20);
    notification->show();

    // 2秒后自动隐藏提示
    QTimer::singleShot(2000, notification, &QLabel::deleteLater);
}
