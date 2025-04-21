#ifndef SPAWNERBLOCK_H
#define SPAWNERBLOCK_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>
#include <QVector>

class SpawnerBlock : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit SpawnerBlock(QGraphicsItem *parent = nullptr);
    ~SpawnerBlock();

    void setPosition(int x, int y);
    void startAnimation();
    void stopAnimation();

private slots:
    void updateFrame();

private:
    QTimer *m_animationTimer;
    QVector<QPixmap> m_frames;
    int m_currentFrame;
};

#endif // SPAWNERBLOCK_H
