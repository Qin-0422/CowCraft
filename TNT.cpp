#include "TNT.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include "Map_Level_1.h"
#include "Map_Level_2.h"

TNT::TNT(QObject* parent) :QObject(parent),  QGraphicsPixmapItem(nullptr)
{
    // 初始化属性
    m_map = nullptr;
    m_isExploding = false;
    m_showAlternateImage = false;
    m_blinkCount = 0;
    
    // 设置TNT图片路径
    m_normalImagePath = ":/level_2/img/block/TNT.jpg";
    m_alternateImagePath = ":/level_2/img/block/TNT_willing.jpg";
    
    // 设置初始图片
    setPixmap(QPixmap(m_normalImagePath));
    
    // 初始化计时器
    m_explosionTimer = new QTimer(this);
    m_blinkTimer = new QTimer(this);
    
    // 连接信号和槽
    connect(m_explosionTimer, &QTimer::timeout, this, &TNT::onExplosionTimerFinished);
    connect(m_blinkTimer, &QTimer::timeout, this, &TNT::switchExplosionImage);
    
    // 设置为实体，不可穿过
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
}

TNT::~TNT()
{
    // 停止所有计时器
    if (m_explosionTimer && m_explosionTimer->isActive())
        m_explosionTimer->stop();
        
    if (m_blinkTimer && m_blinkTimer->isActive())
        m_blinkTimer->stop();
}

void TNT::placeTNT(int x, int y, Level_Map* map)
{
    if (!map) return;
    
    // 检查放置位置是否为Road类型
    if (!map->isRoad(x, y))
    {
        qDebug() << "无法在非Road类型方块上放置TNT";
        return;
    }
    
    // 创建新的TNT实例
    TNT* tnt = new TNT();
    tnt->setMap(map);
    
    // 设置TNT位置
    tnt->setPos(x, y);
    
    // 将TNT添加到场景
    QGraphicsScene* scene = map->getScene();
    if (scene)
    {
        scene->addItem(tnt);
        qDebug() << "TNT已放置在位置:" << x << "," << y;
        
        // 开始检查附近的Trigger
        QTimer::singleShot(100, tnt, &TNT::checkNearbyTrigger);
    }
}

void TNT::checkNearbyTrigger()
{
    if (!m_map) return;
    
    // 获取TNT当前位置
    int tntX = static_cast<int>(x());
    int tntY = static_cast<int>(y());
    
    // 检查100像素范围内是否有Trigger
    int checkRange = 100;
    int tileSize = 50; // 假设每个方块大小为50x50
    
    // 转换为地图坐标
    int centerTileX = tntX / tileSize;
    int centerTileY = tntY / tileSize;
    
    // 计算检查范围内的方块坐标
    int rangeInTiles = checkRange / tileSize + 1;
    
    for (int i = centerTileX - rangeInTiles; i <= centerTileX + rangeInTiles; ++i)
    {
        for (int j = centerTileY - rangeInTiles; j <= centerTileY + rangeInTiles; ++j)
        {
            // 检查坐标是否在地图范围内
            if (i < 0 || j < 0) continue;
            
            // 计算当前方块到TNT的距离
            int blockX = i * tileSize + tileSize / 2;
            int blockY = j * tileSize + tileSize / 2;
            int distance = static_cast<int>(sqrt(pow(blockX - tntX, 2) + pow(blockY - tntY, 2)));
            
            // 只要在检查范围内发现Trigger就触发
            if (distance <= checkRange && m_map->isTrigger(i * tileSize, j * tileSize))
            {
                qDebug() << "TNT检测到附近有Trigger，位置:" << i << "," << j;
                startExplosionPreparation();
                return;
            }
        }
    }
    
    // 如果没有找到Trigger，继续定期检查
    QTimer::singleShot(500, this, &TNT::checkNearbyTrigger);
}

void TNT::startExplosionPreparation()
{
    if (m_isExploding) return;
    
    qDebug() << "TNT开始爆炸准备!";
    m_isExploding = true;
    
    // 开始闪烁效果，每500毫秒切换一次图片
    m_blinkTimer->start(500);
    
    // 设置2秒后爆炸
    m_explosionTimer->start(2000);
}

void TNT::switchExplosionImage()
{
    if (!m_isExploding) return;
    
    m_showAlternateImage = !m_showAlternateImage;
    
    if (m_showAlternateImage)
    {
        // 使用交替图片或调整亮度
        QPixmap alternatePixmap(m_alternateImagePath);
        // 可以在这里调整图片亮度或颜色以创建闪烁效果
        setPixmap(alternatePixmap);
    }
    else
    {
        // 使用正常图片
        setPixmap(QPixmap(m_normalImagePath));
    }
    
    // 增加闪烁计数
    m_blinkCount++;
    
    // 如果已经闪烁了4次（2秒），停止闪烁
    if (m_blinkCount >= 4)
    {
        m_blinkTimer->stop();
    }
}

void TNT::onExplosionTimerFinished()
{
    // 爆炸倒计时结束，执行爆炸
    explode();
}

void TNT::explode()
{
    if (!m_map) {
        qDebug() << "TNT爆炸失败: 地图指针为空";
        return;
    }
    
    qDebug() << "TNT爆炸开始!";
    
    // 停止所有计时器并断开连接
    if (m_explosionTimer) {
        if (m_explosionTimer->isActive()) {
            m_explosionTimer->stop();
        }
        m_explosionTimer->disconnect();
    }
    
    if (m_blinkTimer) {
        if (m_blinkTimer->isActive()) {
            m_blinkTimer->stop();
        }
        m_blinkTimer->disconnect();
    }
    
    // 获取TNT当前位置
    int tntX = static_cast<int>(x());
    int tntY = static_cast<int>(y());
    
    // 爆炸范围（150150像素）
    const int explosionRange = 150;
    const int tileSize = 50; // 假设每个方块大小为50x50
    
    // 转换为地图坐标
    int centerTileX = tntX / tileSize;
    int centerTileY = tntY / tileSize;
    
    // 计算爆炸范围内的方块坐标
    int rangeInTiles = explosionRange / tileSize + 1;
    
    // 获取场景
    QGraphicsScene* scene = m_map->getScene();
    if (!scene) {
        qDebug() << "TNT爆炸失败: 场景指针为空";
        return;
    }
    
    // 获取地图尺寸并进行错误检查
    int mapWidth = 0;
    int mapHeight = 0;
    try {
        mapWidth = m_map->getWidth();
        mapHeight = m_map->getHeight();
        
        if (mapWidth <= 0 || mapHeight <= 0) {
            qDebug() << "TNT爆炸失败: 地图尺寸无效 (宽度:" << mapWidth << ", 高度:" << mapHeight << ")";
            return;
        }
    } catch (const std::exception& e) {
        qDebug() << "TNT爆炸失败: 获取地图尺寸时发生错误 -" << e.what();
        return;
    }
    
    try {
        // 遍历爆炸范围内的所有方块
        for (int i = std::max(0, centerTileX - rangeInTiles); 
             i <= std::min(mapWidth - 1, centerTileX + rangeInTiles); ++i) {
            
            for (int j = std::max(0, centerTileY - rangeInTiles); 
                 j <= std::min(mapHeight - 1, centerTileY + rangeInTiles); ++j) {
                
                // 计算当前方块到TNT的距离
                int blockX = i * tileSize + tileSize / 2;
                int blockY = j * tileSize + tileSize / 2;
                double distance = std::sqrt(std::pow(blockX - tntX, 2.0) + std::pow(blockY - tntY, 2.0));
                
                // 如果在爆炸范围内
                if (distance <= explosionRange) {
                    try {
                        // 检查是否是地图一
                        if (Map_Level_1* map1 = dynamic_cast<Map_Level_1*>(m_map)) {
                            // 只对CobbleStone方块生效
                            if (map1->getBlockType(i, j) == Cobblestone) {
                                map1->updateBlock(i, j, Road);
                            }
                        }
                        // 检查是否是地图二
                        else if (Map_Level_2* map2 = dynamic_cast<Map_Level_2*>(m_map)) {
                            // 只对CobbleStone方块生效
                            if (map2->getBlockType(i, j) == Cobblestone) {
                                map2->updateBlock(i, j, Road);
                            }
                        }
                    } catch (const std::exception& e) {
                        qDebug() << "更新地图块时发生错误:" << e.what() << "位置:" << i << "," << j;
                        continue; // 继续处理其他方块
                    }
                }
            }
        }
        
        qDebug() << "TNT爆炸完成，准备清理TNT对象";
        
        // 在删除前确保所有信号都已断开
        this->disconnect();
        
        // 从场景中移除并删除TNT
        if (scene->items().contains(this)) {
            scene->removeItem(this);
            deleteLater();
        }
        
    } catch (const std::exception& e) {
        qDebug() << "TNT爆炸过程中发生错误:" << e.what();
        
        // 确保在发生错误时也能清理TNT
        this->disconnect();
        if (scene && scene->items().contains(this)) {
            scene->removeItem(this);
            deleteLater();
        }
    }
}


Material* TNT::createTNTItem()
{
    // 创建TNT物品
    return new Material("TNT", "爆炸物", ":/img/Item/Tnt.jpg");
}

QRectF TNT::boundingRect() const
{
    // 返回TNT的边界矩形，使用当前图片的大小
    return QRectF(0, 0, pixmap().width(), pixmap().height());
}

void TNT::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // 绘制TNT图片
    painter->drawPixmap(0, 0, pixmap());
}

QPainterPath TNT::shape() const
{
    // 创建一个路径，表示TNT的形状（用于碰撞检测）
    QPainterPath path;
    path.addRect(boundingRect());//使用当前图片的大小
    return path;
}

bool TNT::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    // 实现碰撞检测逻辑
    return QGraphicsItem::collidesWithItem(other, mode);
}
