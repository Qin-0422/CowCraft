#include "Level_Map_Impl.h"

Level_Map_Impl::Level_Map_Impl()
{
    scene = new QGraphicsScene();
}

Level_Map_Impl::~Level_Map_Impl()
{
    delete scene;
}

bool Level_Map_Impl::isRoad(int x, int y) const
{
    int col = x / 50; // 假设 tileSize 是 50
    int row = y / 50;
    if (row >= 0 && row < map_data.size() && col >= 0 && col < map_data[row].size())
    {
        return( map_data[row][col] == 1||map_data[row][col] ==9||map_data[row][col] ==12||map_data[row][col] ==15); // 检查 map_data[row][col] 是否为 1,9
    }
    return false;
}
bool Level_Map_Impl::isLadder(int x, int y) const
{
    int col = x / 50; // 假设 tileSize 是 50
    int row = y / 50;
    if (row >= 0 && row < map_data.size() && col >= 0 && col < map_data[row].size())
    {
        return map_data[row][col] ==12; // 检查 map_data[row][col] 是否为 1,9
    }
    return false;
}
bool Level_Map_Impl::isTransmit(int x, int y) const
{
    int col = x / 50; // 假设 tileSize 是 50
    int row = y / 50;
    if (row >= 0 && row < map_data.size() && col >= 0 && col < map_data[row].size())
    {
        return map_data[row][col] == 9; // 检查 map_data[row][col] 是否为 9
    }
    return false;
}
bool Level_Map_Impl::isTransmitBack(int x, int y) const
{
    int col = x / 50; // 假设 tileSize 是 50
    int row = y / 50;
    if (row >= 0 && row < map_data.size() && col >= 0 && col < map_data[row].size())
    {
        return map_data[row][col] == 15; // 检查 map_data[row][col] 是否为 15
    }
    return false;
}

QGraphicsScene* Level_Map_Impl::getScene() const
{
    return scene;
}

void Level_Map_Impl::setTrigger(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_data.size() && col >= 0 && col < map_data[row].size())
    {
        map_data[row][col] = Trigger;
        // 添加Trigger图片
        QPixmap pixmap(":/level_1/img/block/Trigger.png");
        QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
        pixmapItem->setPos(col * 50, row * 50);
        scene->addItem(pixmapItem);
    }
}

void Level_Map_Impl::removeTrigger(int x, int y)
{
    int col = x / 50;
    int row = y / 50;
    if (row >= 0 && row < map_data.size() && col >= 0 && col < map_data[row].size())
    {
        if (map_data[row][col] == Trigger)
        {
            map_data[row][col] = Road;
            // 移除原有的图片
            QList<QGraphicsItem*> items = scene->items(QRectF(col * 50, row * 50, 50, 50));
            for (QGraphicsItem* item : items)
            {
                if (QGraphicsPixmapItem* pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item))
                {
                    if (pixmapItem->pixmap().cacheKey() == QPixmap(":/level_1/img/block/Trigger.png").cacheKey())
                    {
                        scene->removeItem(pixmapItem);
                        delete pixmapItem;
                        break;
                    }
                }
            }
        }
    }
}
