#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>

#include "Init.h"
#include "HelpInterface.h"
#include "Sidebar.h"
#include "Map_Level_1.h"
#include "Map_Level_2.h"
#include "Map_Level_3.h"
#include "Map_Level_4.h"
#include "Steve.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void resetGame(); // 重置游戏状态
private:
    QGraphicsView * m_view;
    QGraphicsView * m_demo_view;
    Init m_Init;


    HelpInterface m_HelpInterface;
    Map_Level_1 m_Map_1;
    Map_Level_2 m_Map_2;
    Map_Level_3 m_Map_3;
    Map_Level_4 m_Map_4;
    Sidebar m_sidebar;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
