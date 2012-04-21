/********************************************************************************
** Form generated from reading UI file 'gamma_corr_ui.ui'
**
** Created: Fri Apr 20 22:26:37 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMMA_CORR_UI_H
#define UI_GAMMA_CORR_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>

QT_BEGIN_NAMESPACE

class Ui_gamma_corr_ui
{
public:
    QSlider *gammaSlider;
    QPushButton *ok;
    QPushButton *cancel;
    QLabel *gammaValue;

    void setupUi(QDialog *gamma_corr_ui)
    {
        if (gamma_corr_ui->objectName().isEmpty())
            gamma_corr_ui->setObjectName(QString::fromUtf8("gamma_corr_ui"));
        gamma_corr_ui->resize(315, 77);
        gammaSlider = new QSlider(gamma_corr_ui);
        gammaSlider->setObjectName(QString::fromUtf8("gammaSlider"));
        gammaSlider->setGeometry(QRect(10, 10, 291, 19));
        gammaSlider->setMinimum(100);
        gammaSlider->setMaximum(1000);
        gammaSlider->setOrientation(Qt::Horizontal);
        ok = new QPushButton(gamma_corr_ui);
        ok->setObjectName(QString::fromUtf8("ok"));
        ok->setGeometry(QRect(10, 40, 75, 23));
        cancel = new QPushButton(gamma_corr_ui);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setGeometry(QRect(90, 40, 75, 23));
        gammaValue = new QLabel(gamma_corr_ui);
        gammaValue->setObjectName(QString::fromUtf8("gammaValue"));
        gammaValue->setGeometry(QRect(220, 40, 71, 21));

        retranslateUi(gamma_corr_ui);

        QMetaObject::connectSlotsByName(gamma_corr_ui);
    } // setupUi

    void retranslateUi(QDialog *gamma_corr_ui)
    {
        gamma_corr_ui->setWindowTitle(QApplication::translate("gamma_corr_ui", "gamma_corr_ui", 0, QApplication::UnicodeUTF8));
        ok->setText(QApplication::translate("gamma_corr_ui", "Ok", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("gamma_corr_ui", "Cancel", 0, QApplication::UnicodeUTF8));
        gammaValue->setText(QApplication::translate("gamma_corr_ui", "Gamma: 1", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class gamma_corr_ui: public Ui_gamma_corr_ui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMMA_CORR_UI_H
