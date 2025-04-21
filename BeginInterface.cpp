#include "BeginInterface.h"

BeginInterface::BeginInterface()
{
    m_BeginGameScene = new QGraphicsScene;
    m_BeginGameScene->setSceneRect(QRect(0,0,2400,1300));
    m_BeginGameScene->addPixmap(QPixmap(":/back/img/background/IntiBackground.png"));

    m_BeginBtn.resize(800,97);
    m_BeginBtn.setIcon(QIcon(":/init/img/init/easy.png"));
    m_BeginBtn.setIconSize(QSize(800,97));
    m_BeginBtn.setStyleSheet("QToolButton { background: transparent; }");//使背景透明
    m_BeginBtn.move(780,498);
    m_BeginBtn.setAutoRaise(true);



    m_ExitBtn.resize(493,129);
    m_ExitBtn.setIcon(QIcon(":/init/img/init/Exit.jpg"));
    m_ExitBtn.setIconSize(QSize(493,129));
    m_ExitBtn.setStyleSheet("QToolButton { background: transparent; }");//使背景透明
    m_ExitBtn.move(1800,1100);
    m_ExitBtn.setAutoRaise(true);

    m_BeginGameScene->addWidget(&m_BeginBtn);
    m_BeginGameScene->addWidget(&m_ExitBtn);
}

BeginInterface::~BeginInterface()
{
    delete m_BeginGameScene;
}
