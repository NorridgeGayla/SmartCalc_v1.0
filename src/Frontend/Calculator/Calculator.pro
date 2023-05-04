QT += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ./sources/main.cpp \
    ./sources/mainwindow.cpp \
    ./sources/qcustomplot.cpp \
    ./sources/credit_calc.cpp \
    ./sources/deposit_calc.cpp \
    ./sources/create_graph.cpp \
    ../../Backend/sources/validation.c \
    ../../Backend/sources/reverse_polish_notation.c \
    ../../Backend/sources/calculation.c \
    ../../Backend/sources/common.c \
    ../../Backend/sources/stack_tokens.c
HEADERS += \
    ./headers/mainwindow.h \
    ./headers/qcustomplot.h \
    ../../Backend/headers/validation.h \
    ../../Backend/headers/reverse_polish_notation.h\
    ../../Backend/headers/calculation.h \
    ../../Backend/headers/common.h \
    ../../Backend/headers/stack_tokens.h

FORMS += \
    ./forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
