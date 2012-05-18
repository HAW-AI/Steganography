#-------------------------------------------------
#
# Project created by QtCreator 2012-04-02T10:22:32
#
#-------------------------------------------------

QT       += core gui

TARGET = DiffTests
TEMPLATE = app
RC_FILE = ic.rc

SOURCES += main.cpp\
        difftests.cpp \
    tests.cpp \
    bitChanger.cpp \
    stringiterator.cpp\
    steganography.cpp \
    intermediary.cpp \
    crypt.cpp \
    problemdialog.cpp \
    noisewarning.cpp \
    addpicdialog.cpp \
    notenoughinfo.cpp


HEADERS  += difftests.h \
    tests.h \
    bitChanger.h \
    stringiterator.h \
    steganography.h \
    intermediary.h \
    crypt.h \
    problemdialog.h \
    noisewarning.h \
    addpicdialog.h \
    notenoughinfo.h

FORMS    += difftests.ui \
    problemdialog.ui \
    noisewarning.ui \
    addpicdialog.ui \
    notenoughinfo.ui
