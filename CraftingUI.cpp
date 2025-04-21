#include "CraftingUI.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QDebug>

CraftingUI::CraftingUI(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("工作台");
    setFixedSize(800, 800);
    initializeUI();
    setupConnections();
}

void CraftingUI::initializeUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    // 创建标题
    QLabel* titleLabel = new QLabel("工作台", this);
    titleLabel->setStyleSheet("QLabel { color: #FFFFFF; font-size: 24px; font-weight: bold; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // 创建合成选项按钮
    QPushButton* tntButton = new QPushButton("合成TNT (需要: 5个火药 + 4个沙子)", this);
    tntButton->setObjectName("tntButton");
    tntButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 15px; font-size: 16px; border-radius: 5px; }"
                            "QPushButton:hover { background-color: #45a049; }");
    mainLayout->addWidget(tntButton);
    
    QPushButton* flintButton = new QPushButton("合成打火石 (需要: 1个铁 + 1个火药)", this);
    flintButton->setObjectName("flintButton");
    flintButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 15px; font-size: 16px; border-radius: 5px; }"
                              "QPushButton:hover { background-color: #45a049; }");
    mainLayout->addWidget(flintButton);
    
    // 创建物品栏视图
    m_inventoryView = new InventoryUI(this);
    m_inventoryView->setFixedSize(600, 200);
    mainLayout->addWidget(m_inventoryView);
    
    // 设置窗口样式和布局
    setStyleSheet("QWidget { background-color: rgba(0, 0, 0, 180); }");
    setLayout(mainLayout);
    
    // 将窗口移动到屏幕中央，并向上调整300像素
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    move((screenGeometry.width() - width()) / 2, (screenGeometry.height() - height()) / 2 - 300);
}

void CraftingUI::setupConnections()
{
    // 连接TNT合成按钮
    QPushButton* tntButton = findChild<QPushButton*>("tntButton");
    connect(tntButton, &QPushButton::clicked, this, [this]() {
        // 检查材料是否足够
        int gunpowderCount = 0;
        int sandCount = 0;
        for (Item* item : m_inventory) {
            if (item && item->getName() == "Gunpowder") {
                gunpowderCount += item->getCount();
            } else if (item && item->getName() == "Sand") {
                sandCount += item->getCount();
            }
        }
        
        if (gunpowderCount >= 5 && sandCount >= 4) {
            // 移除材料
            int removedGunpowder = 0;
            int removedSand = 0;
            for (auto it = m_inventory.begin(); it != m_inventory.end();) {
                if ((*it)->getName() == "Gunpowder" && removedGunpowder < 5) {
                    it = m_inventory.erase(it);
                    removedGunpowder++;
                } else if ((*it)->getName() == "Sand" && removedSand < 4) {
                    it = m_inventory.erase(it);
                    removedSand++;
                } else {
                    ++it;
                }
            }
            
            // 创建TNT并直接添加到物品栏
            Item* tnt = new Material("TNT", "爆炸物", ":/Item/img/Item/Tnt.jpg");
            m_inventory.append(tnt);
            m_inventoryView->updateInventory(m_inventory, QList<Equipment*>());
            emit craftingCompleted(tnt);
        }
    });

    // 连接打火石合成按钮
    QPushButton* flintButton = findChild<QPushButton*>("flintButton");
    connect(flintButton, &QPushButton::clicked, this, [this]() {
        // 检查材料是否足够
        int iconCount = 0;
        int gunpowderCount = 0;
        for (Item* item : m_inventory) {
            if (item && item->getName() == "Icon") {
                iconCount += item->getCount();
            } else if (item && item->getName() == "Gunpowder") {
                gunpowderCount += item->getCount();
            }
        }
        
        if (iconCount >= 1 && gunpowderCount >= 1) {
            // 移除材料
            int removedIcon = 0;
            int removedGunpowder = 0;
            for (auto it = m_inventory.begin(); it != m_inventory.end();) {
                if ((*it)->getName() == "Icon" && removedIcon < 1) {
                    it = m_inventory.erase(it);
                    removedIcon++;
                } else if ((*it)->getName() == "Gunpowder" && removedGunpowder < 1) {
                    it = m_inventory.erase(it);
                    removedGunpowder++;
                } else {
                    ++it;
                }
            }
            
            // 创建打火石并直接添加到物品栏
            Item* flint = new Material("Flint", "打火石", ":/Item/img/Item/flint.jpg");
            m_inventory.append(flint);
            m_inventoryView->updateInventory(m_inventory, QList<Equipment*>());
            emit craftingCompleted(flint);
        }
    });
}

void CraftingUI::setInventory(const QList<Item*>& inventory)
{
    // 清空当前物品栏
    m_inventory.clear();
    // 添加新物品
    for (Item* item : inventory) 
    {
        m_inventory.append(item);
    }
    if (m_inventoryView)
    {
        m_inventoryView->updateInventory(m_inventory, QList<Equipment*>());
    }
}

void CraftingUI::show()
{
    QWidget::show();
}

void CraftingUI::hide()
{
    QWidget::hide();
}

bool CraftingUI::isVisible() const
{
    return QWidget::isVisible();
}
