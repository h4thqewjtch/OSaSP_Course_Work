/********************************************************************************
** Form generated from reading UI file 'searchwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHWINDOW_H
#define UI_SEARCHWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SearchWindow
{
public:
    QListWidget *listWidget;
    QPushButton *btnOK;

    void setupUi(QDialog *SearchWindow)
    {
        if (SearchWindow->objectName().isEmpty())
            SearchWindow->setObjectName("SearchWindow");
        SearchWindow->resize(400, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/kisspng-computer-icons-search-icon-5b30c8ad36d571.3385379615299237572246-removebg-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
        SearchWindow->setWindowIcon(icon);
        listWidget = new QListWidget(SearchWindow);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(10, 10, 381, 231));
        btnOK = new QPushButton(SearchWindow);
        btnOK->setObjectName("btnOK");
        btnOK->setGeometry(QRect(300, 260, 89, 25));

        retranslateUi(SearchWindow);

        QMetaObject::connectSlotsByName(SearchWindow);
    } // setupUi

    void retranslateUi(QDialog *SearchWindow)
    {
        SearchWindow->setWindowTitle(QCoreApplication::translate("SearchWindow", "Dialog", nullptr));
        btnOK->setText(QCoreApplication::translate("SearchWindow", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchWindow: public Ui_SearchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHWINDOW_H
