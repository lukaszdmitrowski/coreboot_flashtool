#-------------------------------------------------
#
# Project created by QtCreator 2015-05-15T10:08:29
#
#-------------------------------------------------

QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flash_tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    utests.cpp \
    supported.cpp \
    about.cpp \
    addpayload.cpp \
    addcomponent.cpp \
    deletecomponents.cpp \
    multisortfiltermodel.cpp \
    datagatherer.cpp \
    progressdialog.cpp \
    choosechip.cpp

HEADERS  += mainwindow.h \
    utests.h \
    supported.h \
    about.h \
    addpayload.h \
    addcomponent.h \
    deletecomponents.h \
    multisortfiltermodel.h \
    datagatherer.h \
    progressdialog.h \
    choosechip.h

FORMS    += mainwindow.ui \
    supported.ui \
    about.ui \
    addpayload.ui \
    addcomponent.ui \
    deletecomponents.ui \
    progressdialog.ui \
    choosechip.ui

INCLUDEPATH += libflashrom \
                libbiosext \
                libcbfstool

LIBS    += -L"$$_PRO_FILE_PWD_/libflashrom"\
        -lflashrom

LIBS    += -L"$$_PRO_FILE_PWD_/libbiosext"\
        -lbiosext

LIBS    += -L"$$_PRO_FILE_PWD_/libcbfstool"\
        -lcbfstool

LIBS    += -lftdi \
        -lusb \
        -lpci

# Enable all programmers which the flashrom command-line tool enables.
#QMAKE_CXXFLAGS += -D'CONFIG_INTERNAL=1' -D'CONFIG_SERPROG=1' -D'CONFIG_RAYER_SPI=1' -D'CONFIG_BITBANG_SPI=1' -D'CONFIG_NIC3COM=1' -D'CONFIG_GFXNVIDIA=1' -D'CONFIG_SATASII=1' -D'CONFIG_FT2232_SPI=1' -D'CONFIG_DUMMY=1' -D'CONFIG_DRKAISER=1' -D'CONFIG_NICREALTEK=1' -D'CONFIG_NICINTEL=1' -D'CONFIG_NICINTEL_SPI=1' -D'CONFIG_OGP_SPI=1' -D'CONFIG_BUSPIRATE_SPI=1' -D'CONFIG_SATAMV=1' -D'NEED_PCI=1' -D'HAVE_UTSNAME=1'

RESOURCES += \
    resources.qrc

DISTFILES += \
    Makefile
