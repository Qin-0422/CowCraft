#include "Sidebar.h"

Sidebar::Sidebar()
{
    m_sidebar_Scene = new QGraphicsScene;
    m_sidebar_Scene->setSceneRect(QRect(0,0,400,1300));
    m_sidebar_Scene->addPixmap(QPixmap(":/init/img/init/demo_background.jpg"));

}

Sidebar::~Sidebar()
{
    delete m_sidebar_Scene;
}
