TEMPLATE = app

TARGET = threads_test1
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

SOURCES += main.c \
    sync_threads_func.c

HEADERS += \
    sync_threads_func.h
