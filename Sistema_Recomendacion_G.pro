QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/include

SOURCES +=     main.cpp     src/core/catalogo.cpp     src/core/recomendacion.cpp     src/core/usuario.cpp     src/core/utilidades.cpp     src/gui/mainwindow.cpp     src/gui/productodialog.cpp     src/gui/registro_usuario.cpp     src/gui/ventana_productos.cpp

HEADERS += \
    include/core/catalogo.h \
    include/core/estructuras.h \
    include/core/recomendacion.h \
    include/core/usuario.h \
    include/core/utilidades.h \
    include/gui/mainwindow.h \
    include/gui/productodialog.h \
    include/gui/registro_usuario.h \
    include/gui/ventana_productos.h

FORMS += \
    resources/ui/mainwindow.ui \
    resources/ui/productodialog.ui \
    resources/ui/registro_usuario.ui \
    resources/ui/ventana_productos.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/Recursos.qrc