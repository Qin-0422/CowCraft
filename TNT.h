#ifndef TNT_H
#define TNT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "Material.h"
#include "Level_Map.h"

class TNT : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    TNT(QObject* parent = nullptr);
    ~TNT();
    
    // 放置TNT到指定位置
    static void placeTNT(int x, int y, Level_Map* map);
    
    // 检查附近是否有被触发的Trigger
    void checkNearbyTrigger();
    
    // 开始爆炸准备
    void startExplosionPreparation();
    
    // 执行爆炸
    void explode();
    
    // 设置地图引用
    void setMap(Level_Map* map) { m_map = map; }
    
    // 创建TNT物品
    static Material* createTNTItem();
    
    // QGraphicsItem虚函数
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    
    // 碰撞检测相关函数
    QPainterPath shape() const override;
    bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const override;

private slots:
    // 切换爆炸准备图片
    void switchExplosionImage();
    
    // 爆炸倒计时结束
    void onExplosionTimerFinished();

private:
    Level_Map* m_map;                // 地图引用
    QTimer* m_explosionTimer;        // 爆炸倒计时
    QTimer* m_blinkTimer;            // 爆炸准备闪烁计时器
    bool m_isExploding;              // 是否处于爆炸准备状态
    bool m_showAlternateImage;       // 是否显示交替图片
    int m_blinkCount;                // 闪烁次数计数
    
    // TNT图片路径
    QString m_normalImagePath;       // 正常状态图片
    QString m_alternateImagePath;    // 交替状态图片
};

#endif // TNT_H