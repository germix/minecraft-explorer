#-------------------------------------------------
#
# Project created by QtCreator 2020-09-25T15:10:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

LIBS += -lz

#---------------------------------------------------------------------------------------------------
# Version
#---------------------------------------------------------------------------------------------------
DEFINES += MAJOR_VERSION=0
DEFINES += MINOR_VERSION=1

#---------------------------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
TARGET = MinecraftExplorer-0.1_debug
DEFINES += DEBUG
} else {
TARGET = MinecraftExplorer-0.1
}

#---------------------------------------------------------------------------------------------------
# Destination folder
#---------------------------------------------------------------------------------------------------
DESTDIR = ../bin

#---------------------------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------------------------

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/AboutDialog.cpp \
    src/TreeItem.cpp \
    src/TreeModel.cpp \
    src/TreeItemNbtTagInt.cpp \
    src/TreeItemNbtTagShort.cpp \
    src/TreeItemNbtTagByte.cpp \
    src/TreeItemNbtTagLong.cpp \
    src/TreeItemNbtTagFloat.cpp \
    src/TreeItemNbtTagDouble.cpp \
    src/TreeItemNbtTagList.cpp \
    src/TreeItemNbtTagByteArray.cpp \
    src/TreeItemNbtTagString.cpp \
    src/TreeItemNbtTagCompound.cpp \
    src/TreeItemNbtTagIntArray.cpp \
    src/TreeItemNbtFile.cpp \
    src/TreeItemJsonFile.cpp \
    src/TreeItemWorld.cpp \
    src/TreeItemJsonObject.cpp \
    src/TreeItemJsonPair.cpp \
    src/TreeItemJsonArray.cpp \
    src/TreeItemRegionFile.cpp \
    src/TreeItemRegionChunk.cpp

HEADERS += \
    src/MainWindow.h \
    src/AboutDialog.h \
    src/TreeItem.h \
    src/TreeModel.h

FORMS += \
    src/MainWindow.ui \
    src/AboutDialog.ui


#---------------------------------------------------------------------------------------------------
# Resource files
#---------------------------------------------------------------------------------------------------

RESOURCES +=res/resource.qrc

win32:RC_FILE = res/resource_win32.rc

#---------------------------------------------------------------------------------------------------
# Translate files
#---------------------------------------------------------------------------------------------------
TRANSLATIONS = \
    translate/mcexplorer_en.ts
    translate/mcexplorer_es.ts

DISTFILES += \
    res/resource_win32.rc
