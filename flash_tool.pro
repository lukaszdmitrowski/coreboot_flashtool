#-------------------------------------------------
#
# Project created by QtCreator 2015-05-15T10:08:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flash_tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += libflashrom/headers

LIBS     += -L/home/lukasz/coreboot_tool/flash_tool/libflashrom\
        -lflashrom\
        -lftdi \
        -lusb

win32 {

} else {
    # Only add libpci on non-Windows, not supported/available yet on Windows.
    LIBS += -lpci

    # Enable all programmers which the flashrom command-line tool enables.
    #QMAKE_CXXFLAGS += -D'CONFIG_INTERNAL=1' -D'CONFIG_SERPROG=1' -D'CONFIG_RAYER_SPI=1' -D'CONFIG_BITBANG_SPI=1' -D'CONFIG_NIC3COM=1' -D'CONFIG_GFXNVIDIA=1' -D'CONFIG_SATASII=1' -D'CONFIG_FT2232_SPI=1' -D'CONFIG_DUMMY=1' -D'CONFIG_DRKAISER=1' -D'CONFIG_NICREALTEK=1' -D'CONFIG_NICINTEL=1' -D'CONFIG_NICINTEL_SPI=1' -D'CONFIG_OGP_SPI=1' -D'CONFIG_BUSPIRATE_SPI=1' -D'CONFIG_SATAMV=1' -D'NEED_PCI=1' -D'HAVE_UTSNAME=1' -D'FLASHROM_VERSION="0.9.3-r1368"'
}
