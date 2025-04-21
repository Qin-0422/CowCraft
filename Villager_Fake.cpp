#include "Villager_Fake.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QDebug>
#include <QTimer>
#include <QtMath>
Villager_F::Villager_F(QGraphicsItem* parent) : QGraphicsPixmapItem(parent)
{

    // 设置村民图片
    setPixmap(QPixmap(":/villager/img/character/Villager/villager.png"));
    
    // 初始化生命值
    m_health = 50;
    
    // 预加载对话框
    m_dialogLabel = new QLabel();
    m_dialogLabel->setStyleSheet("QLabel { background-color : white; color : black; padding: 10px; border-radius: 5px; }");// 设置对话框样式
    
    // 获取主窗口并预设对话框
    QWidget* mainWindow = QApplication::activeWindow();// 获取主窗口
    if (mainWindow) {
        m_dialogLabel->setParent(mainWindow);
        m_dialogLabel->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);// 设置为子窗口并保持焦点在主窗口
        m_dialogLabel->move(850, 850);// 计算对话框位置（在村民头顶上方）
        m_dialogLabel->hide();
    }

    // 创建对话框显示计时器
    m_dialogTimer = new QTimer;
    connect(m_dialogTimer, &QTimer::timeout, this, &Villager_F::hideDialog);

    // 设置可接受鼠标事件
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    m_checkTimer = new QTimer(this);
    connect(m_checkTimer, &QTimer::timeout, this, &Villager_F::checkPlayerDistance);
    m_checkTimer->start(100); // 每100毫秒检测一次
}

Villager_F::~Villager_F()
{
    delete m_dialogLabel;
    delete m_dialogTimer;
    delete m_checkTimer;
}

void Villager_F::setPosition(int x, int y)
{
    setPos(x, y);
}

void Villager_F::showDialog()
{
    if (!scene()) 
    return;
    
    // 设置对话内容
    m_dialogLabel->setText("冒险者，我是这里的村长！\n村子里的绿宝石被末影人偷走了\n你要将它找回来\n另外,为了考察你的实力\n你需要收集到一些Trigger,作为你身法的凭证\n我会给予你镇村之剑\n你可以在附近的箱子里找到Trigger");
    
    // 获取主窗口并设置对话框位置
           if (scene() && scene()->views().first())
           {
               QWidget* mainWindow = scene()->views().first()->viewport();
               // 设置为子窗口并保持焦点在主窗口
               m_dialogLabel->setParent(mainWindow);
               m_dialogLabel->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);

               // 计算对话框位置（在村民头顶上方）
               //QPointF scenePos = mapToScene(boundingRect().center());
               //QPoint screenPos = scene()->views().first()->mapToGlobal(scene()->views().first()->mapFromScene(scenePos));
               m_dialogLabel->move(850,850);

               // 显示对话框
               m_dialogLabel->show();

               // 确保主窗口保持焦点
               mainWindow->setFocus();

               // 3秒后自动隐藏
               m_dialogTimer->start(3000);
           }
}

void Villager_F::hideDialog()
{
    m_dialogLabel->hide();
    m_dialogTimer->stop();
}

void Villager_F::showSpecialDialog()
{
    if (!scene()) 
    return;
    
    // 设置特殊对话内容
    m_dialogLabel->setText("你已完成我的考验，快拿着这把剑把绿宝石找回来");
    
    // 获取主窗口并设置对话框位置
           if (scene() && scene()->views().first())
           {
               QWidget* mainWindow = scene()->views().first()->viewport();
               // 设置为子窗口并保持焦点在主窗口
               m_dialogLabel->setParent(mainWindow);
               m_dialogLabel->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);

               // 计算对话框位置（在村民头顶上方）
               //QPointF scenePos = mapToScene(boundingRect().center());
               //QPoint screenPos = scene()->views().first()->mapToGlobal(scene()->views().first()->mapFromScene(scenePos));
               m_dialogLabel->move(850,850);

               // 显示对话框
               m_dialogLabel->show();

               // 确保主窗口保持焦点
               mainWindow->setFocus();

               // 3秒后自动隐藏
               m_dialogTimer->start(3000);
           }
}

void Villager_F::interact()
{
    // 在这里添加交互逻辑
    qDebug() << "与村民交互";
    
    // 检查Steve物品栏中Trigger的数量和是否已有武器
    if (m_Steve)
    {
        // 检查Steve是否已经有武器
        if (m_Steve->hasWeapon())
        {
            // 显示第三种对话（已获得武器后的对话）
            showThirdDialog();
            return;
        }
        
        int triggerCount = 0;
        for (const auto& item : m_Steve->getInventory())
        {
            if (item->getName() == "Trigger")
            {
                triggerCount += item->getCount();
            }
        }
        
        if (triggerCount >= 2)
        {
            // 显示新的对话
            showSpecialDialog();
            
            // 给予Steve假武器
            m_Steve->giveFakeWeapon();
        }
        else
        {
            // 显示普通对话
            showDialog();
        }
    }
}

bool Villager_F::isInRange(int x, int y) const
{
    // 计算与鼠标位置的距离
    int dx = x - pos().x();
    int dy = y - pos().y();
    int distance = qSqrt(dx*dx + dy*dy);

    // 如果距离小于交互范围，则返回true
    return distance < INTERACTION_RANGE; 
}

void Villager_F::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        interact();
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}
void Villager_F::showThirdDialog()
{
    if (!scene()) 
    return;
    
    // 设置第三种对话内容
    m_dialogLabel->setText("我的绿宝石怎么还没有找到！");
    
    // 获取主窗口并设置对话框位置
    if (scene() && scene()->views().first())
    {
        QWidget* mainWindow = scene()->views().first()->viewport();
        // 设置为子窗口并保持焦点在主窗口
        m_dialogLabel->setParent(mainWindow);
        m_dialogLabel->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);

        m_dialogLabel->move(850,850);

        // 显示对话框
        m_dialogLabel->show();

        // 确保主窗口保持焦点
        mainWindow->setFocus();

        // 3秒后自动隐藏
        m_dialogTimer->start(3000);
    }
}

void Villager_F::checkPlayerDistance()
{
    if (m_Steve && scene())
    {
        if (isInRange(m_Steve->x(), m_Steve->y()))
        {
            if (!m_dialogLabel->isVisible())
            {
                // 检查Steve是否已经有武器
                if (m_Steve->hasWeapon())
                {
                    showThirdDialog();
                    return;
                }
                
                // 检查Steve物品栏中Trigger的数量
                int triggerCount = 0;
                for (const auto& item : m_Steve->getInventory())
                {
                    if (item->getName() == "Trigger")
                    {
                        triggerCount += item->getCount();
                    }
                }
                
                if (triggerCount >= 2)
                {
                    showSpecialDialog();
                }
                else
                {
                    showDialog();
                }
            }
        }
        else
        {
            hideDialog();
        }
    }
}
