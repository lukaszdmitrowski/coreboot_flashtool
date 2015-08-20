#-------------------------------------------------
#
# Project created by QtCreator 2015-05-15T10:08:29
#
#-------------------------------------------------

QT       += core gui testlib xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flash_tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    utests.cpp \
    supported.cpp \
    about.cpp \
    addcomponent.cpp \
    deletecomponents.cpp \
    multisortfiltermodel.cpp \
    datagatherer.cpp \
    choosechip.cpp \
    hashlibpp/hl_sha256wrapper.cpp \
    hashlibpp/hl_sha256.cpp \
    infodialog.cpp \
    preferences.cpp \
    flasher.cpp

HEADERS  += mainwindow.h \
    utests.h \
    supported.h \
    about.h \
    addcomponent.h \
    deletecomponents.h \
    multisortfiltermodel.h \
    datagatherer.h \
    choosechip.h \
    hashlibpp/hashlibpp.h \
    hashlibpp/hl_hashwrapper.h \
    hashlibpp/hl_exception.h \
    hashlibpp/hl_sha256.h \
    hashlibpp/hl_sha256wrapper.h \
    infodialog.h \
    constants.h \
    preferences.h \
    flasher.h \
    loggers.h

FORMS    += mainwindow.ui \
    supported.ui \
    about.ui \
    addcomponent.ui \
    deletecomponents.ui \
    choosechip.ui \
    infodialog.ui \
    preferences.ui

INCLUDEPATH += libflashrom \
                libbiosext \
                libcbfstool \
                hashlibpp

LIBS    += -L"$$_PRO_FILE_PWD_/libflashrom"\
        -lflashrom

LIBS    += -L"$$_PRO_FILE_PWD_/libbiosext"\
        -lbiosext

LIBS    += -L"$$_PRO_FILE_PWD_/libcbfstool"\
        -lcbfstool

LIBS    += -lftdi \
        -lusb \
        -lpci

RESOURCES += \
    resources.qrc

DISTFILES += \
    Makefile
