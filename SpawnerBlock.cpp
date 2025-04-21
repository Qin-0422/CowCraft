#include "SpawnerBlock.h"
#include <QDebug>

SpawnerBlock::SpawnerBlock(QGraphicsItem *parent) : QGraphicsPixmapItem(parent), m_currentFrame(0)
{
    // 加载三张刷怪笼图片
    m_frames.append(QPixmap(":/level_2/img/block/Monster_Spawner_1.jpg"));
    m_frames.append(QPixmap(":/level_2/img/block/Monster_Spawner_2.jpg"));
    m_frames.append(QPixmap(":/level_2/img/block/Monster_Spawner_3.jpg"));

    // 设置初始图片
    this->setPixmap(m_frames[0]);

    // 创建动画定时器
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &SpawnerBlock::updateFrame);
}

SpawnerBlock::~SpawnerBlock()
{
    if (m_animationTimer->isActive())
    {
        m_animationTimer->stop();
    }
}

void SpawnerBlock::setPosition(int x, int y)
{
    setPos(x, y);
}

void SpawnerBlock::startAnimation()
{
    if (!m_animationTimer->isActive())
    {
        m_animationTimer->start(200); // 每200毫秒切换一次图片
    }
}

void SpawnerBlock::stopAnimation()
{
    if (m_animationTimer->isActive())
    {
        m_animationTimer->stop();
    }
}

void SpawnerBlock::updateFrame()
{
    // 切换到下一帧
    m_currentFrame = (m_currentFrame + 1) % m_frames.size();
    setPixmap(m_frames[m_currentFrame]);
}
