#include "Widget.h"
#include "ui_widget.h"
#include <QToolButton>
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    m_view = new QGraphicsView(this);
    m_demo_view = new QGraphicsView(this);
    m_demo_view->setGeometry(QRect(2000,0,400,1300));
    m_demo_view->setScene(m_sidebar.m_sidebar_Scene);
    m_demo_view->close();
    this->setFixedSize(2400,1300);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 关闭滚动条
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_demo_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 关闭滚动条
    m_demo_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setSceneRect(QRect(0,0,2400,1300));

    m_view->setScene(m_Init.m_InitGameScene);
   //简单到level——1
    connect(&m_Init.m_BeginBtn,&QToolButton::clicked,[this](){
        m_view->setScene(m_Map_1.level_1_Scene);
        m_Map_1.m_Steve.setFocus();
        m_demo_view->show();
        m_view->show();
    });


     //主界面和帮助的往返
    connect(&m_Init.m_HelpBtn,&QToolButton::clicked,[this](){
        m_view->setScene(m_HelpInterface.m_HelpGameScene);
        m_view->show();
    });
    connect(&m_HelpInterface.m_ExitBtn,&QToolButton::clicked,[this](){
        m_view->setScene(m_Init.m_InitGameScene);
        m_view->show();
    });


    //从关卡1到关卡2
    connect(&m_Map_1.m_Steve,&Steve::changeLevel,[this](){
        m_view->setScene(m_Map_2.level_2_Scene);
        m_Map_2.m_Steve.setX(150);
        m_Map_2.m_Steve.setY(100);
        // 传递物品栏和血量
        m_Map_2.m_Steve.setInventory(m_Map_1.m_Steve.getInventory());
        m_Map_2.m_Steve.setHealth(m_Map_1.m_Steve.getHealth());
        m_Map_2.m_Steve.getHealthUI()->setHealth(m_Map_1.m_Steve.getHealth());
        m_Map_2.m_Steve.setFocus();
        m_view->show();
    });
    //从关卡2到关卡3
    connect(&m_Map_2.m_Steve,&Steve::changeLevel,[this](){
        m_view->setScene(m_Map_3.level_3_Scene );
        m_Map_3.m_Steve.setX(150);
        m_Map_3.m_Steve.setY(550);
        // 传递物品栏和血量
        m_Map_3.m_Steve.setInventory(m_Map_2.m_Steve.getInventory());
        m_Map_3.m_Steve.setHealth(m_Map_2.m_Steve.getHealth());
        m_Map_3.m_Steve.getHealthUI()->setHealth(m_Map_2.m_Steve.getHealth());
        m_Map_3.m_Steve.setFocus();
        m_view->show();
    });
    //从关卡3到关卡4
    connect(&m_Map_3.m_Steve,&Steve::changeLevel,[this](){
        m_view->setScene(m_Map_4.level_4_Scene );
        m_Map_4.m_Steve.setX(100);
        m_Map_4.m_Steve.setY(800);
        // 传递物品栏和血量
        m_Map_4.m_Steve.setInventory(m_Map_3.m_Steve.getInventory());
        m_Map_4.m_Steve.setHealth(m_Map_3.m_Steve.getHealth());
        m_Map_4.m_Steve.getHealthUI()->setHealth(m_Map_3.m_Steve.getHealth());
        m_Map_4.m_Steve.setFocus();
        m_view->show();
    });
    //从关卡3到关卡2
    connect(&m_Map_3.m_Steve,&Steve::changeBackLevel,[this](){
        m_view->setScene(m_Map_2.level_2_Scene );
        m_Map_2.m_Steve.setX(1800);
        m_Map_2.m_Steve.setY(100);
        // 传递物品栏和血量
        m_Map_2.m_Steve.setInventory(m_Map_3.m_Steve.getInventory());
        m_Map_2.m_Steve.setHealth(m_Map_3.m_Steve.getHealth());
        m_Map_2.m_Steve.getHealthUI()->setHealth(m_Map_3.m_Steve.getHealth());
        m_Map_2.m_Steve.setFocus();
        m_view->show();
    });
    //从关卡2到关卡1
    connect(&m_Map_2.m_Steve,&Steve::changeBackLevel,[this](){
       m_view->setScene(m_Map_1.level_1_Scene);
       m_Map_1.m_Steve.setX(1850);
       m_Map_1.m_Steve.setY(1100);
       // 传递物品栏和血量
       m_Map_1.m_Steve.setInventory(m_Map_2.m_Steve.getInventory());
       m_Map_1.m_Steve.setHealth(m_Map_2.m_Steve.getHealth());
       m_Map_1.m_Steve.getHealthUI()->setHealth(m_Map_2.m_Steve.getHealth());
       m_Map_1.m_Steve.setFocus();
       m_view->show();
    });

    ui->setupUi(this);
    
    // 设置窗口和视图都能接收焦点
    setFocusPolicy(Qt::StrongFocus);
    m_view->setFocusPolicy(Qt::StrongFocus);
}
Widget::~Widget()
{
    delete ui;
}

void Widget::resetGame()
{
    // 重置地图1的状态
    if (m_Map_1.level_1_Scene) {
        m_Map_1.level_1_Scene->clear();
        delete m_Map_1.level_1_Scene;
    }
    m_Map_1.level_1_Scene = new QGraphicsScene();
    m_Map_1.m_Steve.setScene(m_Map_1.level_1_Scene);
    // m_Map_1.updateMap_exit();
    
    // 重置地图2的状态
    if (m_Map_2.level_2_Scene) {
        m_Map_2.level_2_Scene->clear();
        delete m_Map_2.level_2_Scene;
    }
    m_Map_2.level_2_Scene = new QGraphicsScene();
    m_Map_2.m_Steve.setScene(m_Map_2.level_2_Scene);
    // m_Map_2.updateMap_exit();
    
    // 重置地图3的状态
    if (m_Map_3.level_3_Scene) {
        m_Map_3.level_3_Scene->clear();
        delete m_Map_3.level_3_Scene;
    }
    m_Map_3.level_3_Scene = new QGraphicsScene();
    m_Map_3.m_Steve.setScene(m_Map_3.level_3_Scene);
    // m_Map_3.updateMap_exit();
    
    // 重新连接信号
    connect(&m_Map_1.m_Steve, &Steve::changeLevel, [this](){
        m_view->setScene(m_Map_2.level_2_Scene);
        m_Map_2.m_Steve.setX(150);
        m_Map_2.m_Steve.setY(100);
        m_Map_2.m_Steve.setInventory(m_Map_1.m_Steve.getInventory());
        m_Map_2.m_Steve.setFocus();
        m_view->show();
    });
    
    connect(&m_Map_2.m_Steve, &Steve::changeLevel, [this](){
        m_view->setScene(m_Map_3.level_3_Scene);
        m_Map_3.m_Steve.setX(150);
        m_Map_3.m_Steve.setY(550);
        m_Map_3.m_Steve.setInventory(m_Map_2.m_Steve.getInventory());
        m_Map_3.m_Steve.setFocus();
        m_view->show();
    });
    
    connect(&m_Map_2.m_Steve, &Steve::changeBackLevel, [this](){
        m_view->setScene(m_Map_1.level_1_Scene);
        m_Map_1.m_Steve.setX(1850);
        m_Map_1.m_Steve.setY(1100);
        m_Map_1.m_Steve.setInventory(m_Map_2.m_Steve.getInventory());
        m_Map_1.m_Steve.setFocus();
        m_view->show();
    });
    
    // 重置到第一关
    m_view->setScene(m_Map_1.level_1_Scene);
    m_Map_1.m_Steve.setX(50);
    m_Map_1.m_Steve.setY(1000);
    m_Map_1.m_Steve.setFocus();
    m_view->show();
}
