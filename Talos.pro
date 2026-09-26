QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cubo_semantico.cpp \
    lexer_automata.cpp \
    lexer_lexer.cpp \
    main.cpp \
    mainwindow.cpp \
    parser_parser.cpp \
    parser_productions.cpp \
    parser_syntaxtable.cpp \
    semantic_analyzer.cpp

HEADERS += \
    cuadruplo.h \
    cubo_semantico.h \
    lexer_automata.h \
    lexer_lexer.h \
    mainwindow.h \
    parser_parser.h \
    parser_productions.h \
    parser_syntaxtable.h \
    semantic_analyzer.h \
    token.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Talos_es_MX.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
