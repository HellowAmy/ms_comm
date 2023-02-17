TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#LIBS += -pthread -lsqlite3
LIBS += -lsqlite3

SOURCES += \
        inter_sqlist.cpp \
        main.cpp

HEADERS += \
    inter_sqlist.h
