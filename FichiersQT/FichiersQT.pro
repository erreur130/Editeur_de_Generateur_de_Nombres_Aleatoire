QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    additionconstante.cpp \
    demandenomclasse.cpp \
    egna.cpp \
    inversementbits.cpp \
    listemodulesactifs.cpp \
    listemodulestemplate.cpp \
    main.cpp \
    mainwindow.cpp \
    module.cpp \
    multiplicationconstante.cpp \
    nonlogique.cpp \
    rotationbitsdroite.cpp \
    rotationbitsgauche.cpp \
    xorshiftdroite.cpp \
    xorshiftgauche.cpp

HEADERS += \
    additionconstante.h \
    demandenomclasse.h \
    egna.h \
    inversementbits.h \
    listemodulesactifs.h \
    listemodulestemplate.h \
    mainwindow.h \
    module.h \
    multiplicationconstante.h \
    nonlogique.h \
    rotationbitsdroite.h \
    rotationbitsgauche.h \
    xorshiftdroite.h \
    xorshiftgauche.h

FORMS += \
    aideModules.ui \
    aideTests.ui \
    demandenomclasse.ui \
    infoProjet.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
