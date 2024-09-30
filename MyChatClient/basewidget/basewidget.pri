#包含一个目录
INCLUDEPATH +=$$PWD
#DEPENDPATH目录里的头文件发生了变化，所有cpp重新编译
DEPENDPATH +=$$PWD

HEADERS+= \
    $$PWD/animationstackedwidget.h\
    $$PWD/chatbubble.h \
    $$PWD/customwidget.h\
    $$PWD/iplineeidt.h\
    $$PWD/iconlineedit.h\
    $$PWD/chatstackedwidget.h \
    $$PWD/qqlist/clineedit.h \
    $$PWD/qqlist/qqlistviewchild.h \
    $$PWD/qqlist/qqlistviewgroup.h \
    $$PWD/qqlist/qqlistwidget.h \
    $$PWD/qqlist/qqpopmenuwidget.h

SOURCES+= \
    $$PWD/animationstackedwidget.cpp\
    $$PWD/chatbubble.cpp \
    $$PWD/iplineeidt.cpp\
    $$PWD/iconlineedit.cpp\
    $$PWD/chatstackedwidget.cpp\
    $$PWD/customwidget.cpp\
    $$PWD/qqlist/clineedit.cpp \
    $$PWD/qqlist/qqlistviewchild.cpp \
    $$PWD/qqlist/qqlistviewgroup.cpp \
    $$PWD/qqlist/qqlistwidget.cpp \
    $$PWD/qqlist/qqpopmenuwidget.cpp

FORMS += \
    $$PWD/qqlist/qqlistviewgroup.ui
