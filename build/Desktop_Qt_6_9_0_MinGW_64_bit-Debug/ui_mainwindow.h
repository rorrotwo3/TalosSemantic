/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbrir;
    QAction *actionGuardar;
    QAction *actionSalir;
    QWidget *centralwidget;
    QTextEdit *textEditCodigo;
    QTextEdit *textEditToken;
    QTextEdit *textEditSintexis;
    QTextEdit *textEditError;
    QPushButton *btnAnaliza;
    QPushButton *btnLimpia;
    QPushButton *btnSintactico;
    QMenuBar *menubar;
    QMenu *menuArchivo;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1163, 600);
        actionAbrir = new QAction(MainWindow);
        actionAbrir->setObjectName("actionAbrir");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPageSetup));
        actionAbrir->setIcon(icon);
        actionGuardar = new QAction(MainWindow);
        actionGuardar->setObjectName("actionGuardar");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave));
        actionGuardar->setIcon(icon1);
        actionSalir = new QAction(MainWindow);
        actionSalir->setObjectName("actionSalir");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit));
        actionSalir->setIcon(icon2);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        textEditCodigo = new QTextEdit(centralwidget);
        textEditCodigo->setObjectName("textEditCodigo");
        textEditCodigo->setGeometry(QRect(10, 0, 661, 511));
        textEditToken = new QTextEdit(centralwidget);
        textEditToken->setObjectName("textEditToken");
        textEditToken->setGeometry(QRect(680, 0, 471, 201));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(12);
        font.setBold(false);
        textEditToken->setFont(font);
        textEditSintexis = new QTextEdit(centralwidget);
        textEditSintexis->setObjectName("textEditSintexis");
        textEditSintexis->setGeometry(QRect(680, 210, 471, 151));
        textEditSintexis->setFont(font);
        textEditError = new QTextEdit(centralwidget);
        textEditError->setObjectName("textEditError");
        textEditError->setGeometry(QRect(680, 370, 471, 141));
        textEditError->setFont(font);
        btnAnaliza = new QPushButton(centralwidget);
        btnAnaliza->setObjectName("btnAnaliza");
        btnAnaliza->setGeometry(QRect(1070, 520, 83, 29));
        btnLimpia = new QPushButton(centralwidget);
        btnLimpia->setObjectName("btnLimpia");
        btnLimpia->setGeometry(QRect(980, 520, 83, 29));
        btnSintactico = new QPushButton(centralwidget);
        btnSintactico->setObjectName("btnSintactico");
        btnSintactico->setGeometry(QRect(880, 520, 83, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1163, 25));
        menuArchivo = new QMenu(menubar);
        menuArchivo->setObjectName("menuArchivo");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuArchivo->menuAction());
        menuArchivo->addAction(actionAbrir);
        menuArchivo->addAction(actionGuardar);
        menuArchivo->addAction(actionSalir);

        retranslateUi(MainWindow);
        QObject::connect(actionSalir, &QAction::triggered, MainWindow, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionAbrir->setText(QCoreApplication::translate("MainWindow", "Abrir", nullptr));
        actionGuardar->setText(QCoreApplication::translate("MainWindow", "Guardar", nullptr));
        actionSalir->setText(QCoreApplication::translate("MainWindow", "Salir", nullptr));
        btnAnaliza->setText(QCoreApplication::translate("MainWindow", "Analilzar", nullptr));
        btnLimpia->setText(QCoreApplication::translate("MainWindow", "Limpiar", nullptr));
        btnSintactico->setText(QCoreApplication::translate("MainWindow", "Sintactico", nullptr));
        menuArchivo->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
