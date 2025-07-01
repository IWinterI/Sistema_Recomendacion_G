/********************************************************************************
** Form generated from reading UI file 'productodialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTODIALOG_H
#define UI_PRODUCTODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_productodialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *lblNombre;
    QLabel *lblMarca;
    QLabel *lblPrecio;
    QLabel *lblCalidad;
    QLabel *lblCategoria;

    void setupUi(QDialog *productodialog)
    {
        if (productodialog->objectName().isEmpty())
            productodialog->setObjectName("productodialog");
        productodialog->resize(400, 300);
        label = new QLabel(productodialog);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 20, 61, 16));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setPointSize(10);
        font.setBold(true);
        label->setFont(font);
        label_2 = new QLabel(productodialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 70, 61, 16));
        label_2->setFont(font);
        label_3 = new QLabel(productodialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 120, 49, 16));
        label_3->setFont(font);
        label_4 = new QLabel(productodialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 170, 61, 16));
        label_4->setFont(font);
        label_5 = new QLabel(productodialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 220, 71, 16));
        label_5->setFont(font);
        lblNombre = new QLabel(productodialog);
        lblNombre->setObjectName("lblNombre");
        lblNombre->setGeometry(QRect(90, 20, 49, 16));
        lblMarca = new QLabel(productodialog);
        lblMarca->setObjectName("lblMarca");
        lblMarca->setGeometry(QRect(80, 70, 49, 16));
        lblPrecio = new QLabel(productodialog);
        lblPrecio->setObjectName("lblPrecio");
        lblPrecio->setGeometry(QRect(80, 120, 49, 16));
        lblCalidad = new QLabel(productodialog);
        lblCalidad->setObjectName("lblCalidad");
        lblCalidad->setGeometry(QRect(90, 170, 49, 16));
        lblCategoria = new QLabel(productodialog);
        lblCategoria->setObjectName("lblCategoria");
        lblCategoria->setGeometry(QRect(100, 220, 49, 16));

        retranslateUi(productodialog);

        QMetaObject::connectSlotsByName(productodialog);
    } // setupUi

    void retranslateUi(QDialog *productodialog)
    {
        productodialog->setWindowTitle(QCoreApplication::translate("productodialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("productodialog", "Nombre:", nullptr));
        label_2->setText(QCoreApplication::translate("productodialog", "Marca:", nullptr));
        label_3->setText(QCoreApplication::translate("productodialog", "Precio:", nullptr));
        label_4->setText(QCoreApplication::translate("productodialog", "Calidad:", nullptr));
        label_5->setText(QCoreApplication::translate("productodialog", "Categoria:", nullptr));
        lblNombre->setText(QCoreApplication::translate("productodialog", "TextLabel", nullptr));
        lblMarca->setText(QCoreApplication::translate("productodialog", "TextLabel", nullptr));
        lblPrecio->setText(QCoreApplication::translate("productodialog", "TextLabel", nullptr));
        lblCalidad->setText(QCoreApplication::translate("productodialog", "TextLabel", nullptr));
        lblCategoria->setText(QCoreApplication::translate("productodialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class productodialog: public Ui_productodialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTODIALOG_H
