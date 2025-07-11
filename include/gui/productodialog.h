#ifndef PRODUCTODIALOG_H
#define PRODUCTODIALOG_H

#include <QDialog>
#include "core/estructuras.h"

namespace Ui {
class productodialog;
}

class productodialog : public QDialog
{
    Q_OBJECT

public:
    explicit productodialog(const Producto& producto, QWidget *parent = nullptr);
    ~productodialog();

private:
    Ui::productodialog *ui;
};

#endif // PRODUCTODIALOG_H
