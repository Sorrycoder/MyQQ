#包含一个目录
INCLUDEPATH +=$$PWD
#DEPENDPATH目录里的头文件发生了变化，所有cpp重新编译
DEPENDPATH +=$$PWD

HEADERS+= \
    $$PWD/cutdialog.h \
    $$PWD/photoshotdialog.h \
    $$PWD/pictureedit.h


SOURCES+= \
    $$PWD/cutdialog.cpp \
    $$PWD/photoshotdialog.cpp \
    $$PWD/pictureedit.cpp


FORMS += \
    $$PWD/pictureedit.ui

