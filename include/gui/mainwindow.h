#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gui/registro_usuario.h"
#include "gui/ventana_productos.h"
#include "core/usuario.h"
#include "core/utilidades.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Regis_clicked();

    void on_salir_clicked();

    void on_Inicio_clicked();

    void on_volver_clicked();

    void on_ingresar_clicked();

private:

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
