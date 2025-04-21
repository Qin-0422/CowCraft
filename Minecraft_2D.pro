#-------------------------------------------------
#
# Project created by QtCreator 2025-03-14T22:49:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Minecraft_2D
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        Widget.cpp \
    Init.cpp \
    BeginInterface.cpp \
    HelpInterface.cpp \
    Level_Map.cpp \
    Map_Level_1.cpp \
    Character.cpp \
    Steve.cpp \
    Map_Level_2.cpp \
    Level_Map_Impl.cpp \
    Zombie.cpp \
    TNT.cpp \
    Equipment.cpp \
    Item.cpp \
    InventorySystem.cpp \
    Sidebar.cpp \
    Creeper.cpp \
    SpawnerBlock.cpp \
    InventoryUI.cpp \
    Map_Level_3.cpp \
    Villager_Fake.cpp \
    KeyItem.cpp \
    FakeWeapon.cpp \
    Material.cpp \
    CraftingUI.cpp \
    HealthUI.cpp \
    Map_Level_4.cpp \
    Armor.cpp \
    Enderman.cpp

HEADERS += \
        Widget.h \
    Init.h \
    BeginInterface.h \
    HelpInterface.h \
    Level_Map.h \
    Map_Level_1.h \
    Character.h \
    Steve.h \
    Map_Level_2.h \
    Level_Map_Impl.h \
    Zombie.h \
    TNT.h \
    Item.h \
    Equipment.h \
    InventorySystem.h \
    Sidebar.h \
    Creeper.h \
    SpawnerBlock.h \
    InventoryUI.h \
    Map_Level_3.h \
    Villager_Fake.h \
    KeyItem.h \
    FakeWeapon.h \
    Material.h \
    CraftingUI.h \
    HealthUI.h \
    Map_Level_4.h \
    Armor.h \
    Enderman.h \
    HealthUI.h

FORMS += \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img_back.qrc
