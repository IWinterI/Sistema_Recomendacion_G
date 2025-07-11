#include "gui/ventana_productos.h"
#include "ui_ventana_productos.h"
#include "gui/mainwindow.h"
#include "core/catalogo.h"
#include "core/recomendacion.h"
#include "gui/productodialog.h"
#include <QListWidgetItem>
#include <QStringList>
#include <QDebug> // Para depuración

extern ListaProducto catalogoGlobal;

Ventana_Productos::Ventana_Productos(Usuario* usuario, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Ventana_Productos), usuarioActual(usuario)
{
    ui->setupUi(this);

    // Depuración: Verificar si catalogoGlobal está inicializado
    qDebug() << "Inicializando Ventana_Productos...";
    qDebug() << "Usuario:" << (usuario ? QString::fromStdString(usuario->usuario) : "null");

    // Verificar catálogo
    int contadorCatalogo = 0;
    NodoProducto* temp = catalogoGlobal;
    while (temp) {
        contadorCatalogo++;
        temp = temp->siguiente;
    }
    qDebug() << "Productos en catalogoGlobal:" << contadorCatalogo;

    ui->comboBox->clear();
    ui->comboBox->addItem("Categorias");
    NodoProducto* actualCat = catalogoGlobal;
    QSet<QString> categoriasSet;
    while (actualCat != nullptr) {
        categoriasSet.insert(QString::fromStdString(actualCat->dato.categoria));
        actualCat = actualCat->siguiente;
    }
    for (const QString& categoria : categoriasSet) {
        ui->comboBox->addItem(categoria);
    }

    // Llenar combobox de marcas
    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("Marca");
    NodoProducto* actualMarca = catalogoGlobal;
    QSet<QString> marcasSet;
    while (actualMarca != nullptr) {
        marcasSet.insert(QString::fromStdString(actualMarca->dato.marca));
        actualMarca = actualMarca->siguiente;
    }
    for (const QString& marca : marcasSet) {
        ui->comboBox_2->addItem(marca);
    }

    // Llenar comboboxes de precios
    QStringList precios = {"10", "20", "30", "40", "50", "60", "70", "80", "90", "100",
                           "110", "120", "130", "140", "150", "160", "170", "180", "190", "200"};

    ui->comboBox_3->clear();
    ui->comboBox_3->addItem("Precio min");
    for (const QString& precio : precios) {
        ui->comboBox_3->addItem(precio + "$");
    }

    ui->comboBox_4->clear();
    ui->comboBox_4->addItem("Precio Max");
    for (const QString& precio : precios) {
        ui->comboBox_4->addItem(precio + "$");
    }


    // Configurar conexiones
    connect(ui->listWidget_3, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        int id = item->data(Qt::UserRole).toInt();
        mostrarDetallesProducto(id);
    });

    connect(ui->listWidget, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        int id = item->data(Qt::UserRole).toInt();
        mostrarDetallesProducto(id);
    });

    connect(ui->pushButton, &QPushButton::clicked, this, &Ventana_Productos::on_pushButton_clicked);

    // Conectar la lista de resultados de búsqueda
    connect(ui->listWidget_2, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        int id = item->data(Qt::UserRole).toInt();
        mostrarDetallesProducto(id);
    });

    // Cargar datos iniciales
    cargarCategorias();
    cargarProductosPorCategoria("Todas las categorías");
    cargarRecomendaciones();
}

void Ventana_Productos::cargarCategorias()
{
    qDebug() << "Cargando categorías...";


    // Obtener categorías únicas
    NodoProducto* actual = catalogoGlobal;
    QSet<QString> categoriasSet;
    int contador = 0;

    while (actual != nullptr) {
        QString categoria = QString::fromStdString(actual->dato.categoria);
        categoriasSet.insert(categoria);
        actual = actual->siguiente;
        contador++;
    }

    qDebug() << "Categorías encontradas:" << categoriasSet.size() << "de" << contador << "productos";


}

void Ventana_Productos::on_categoriaComboBox_currentIndexChanged(const QString &categoria)
{
    qDebug() << "Categoría seleccionada:" << categoria;
    cargarProductosPorCategoria(categoria);
}

void Ventana_Productos::cargarProductosPorCategoria(const QString& categoria)
{
    qDebug() << "Cargando productos para categoría:" << categoria;
    ui->listWidget_3->clear();

    int productosCargados = 0;
    NodoProducto* actual = catalogoGlobal;

    while (actual != nullptr) {
        QString catProducto = QString::fromStdString(actual->dato.categoria);

        if (categoria == "Todas las categorías" || catProducto == categoria) {
            Producto p = actual->dato;
            QString itemText = QString("%1 - %2 - $%3")
                                   .arg(QString::fromStdString(p.descripcion))
                                   .arg(QString::fromStdString(p.marca))
                                   .arg(p.precio);

            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, p.id);
            ui->listWidget_3->addItem(item);

            productosCargados++;
            qDebug() << "  - Añadido:" << itemText;
        }
        actual = actual->siguiente;
    }

    qDebug() << "Total productos cargados:" << productosCargados;
    qDebug() << "Items en listWidget_3:" << ui->listWidget_3->count();
}

void Ventana_Productos::cargarRecomendaciones()
{
    qDebug() << "Cargando recomendaciones...";

    if (!ui->listWidget) {
        qDebug() << "ERROR: listWidget no existe!";
        return;
    }

    ui->listWidget->clear();

    if (!usuarioActual) {
        qDebug() << "Usuario no autenticado, no se cargan recomendaciones";
        return;
    }

    Recomendaciones rec = generarRecomendaciones(usuarioActual);
    qDebug() << "Recomendaciones generadas";

    // Combinar todas las recomendaciones
    ListaProducto todas = nullptr;
    int contadorRecomendaciones = 0;

    // Función para agregar productos
    auto agregarProductos = [&](ListaProducto lista) {
        if (!lista) return;

        NodoProducto* actual = lista;
        while (actual != nullptr) {
            // Evitar duplicados
            bool existe = false;
            NodoProducto* temp = todas;
            while (temp != nullptr) {
                if (temp->dato.id == actual->dato.id) {
                    existe = true;
                    break;
                }
                temp = temp->siguiente;
            }

            if (!existe) {
                insertarEnLista(todas, actual->dato);
                contadorRecomendaciones++;
            }
            actual = actual->siguiente;
        }
    };

    agregarProductos(rec.porMarcasPreferidas);
    agregarProductos(rec.porOtrasMarcasFrecuentes);
    agregarProductos(rec.porCategoriaPreferida);
    agregarProductos(rec.porCategoriaFrecuente);
    agregarProductos(rec.porCalidad);

    qDebug() << "Total recomendaciones combinadas:" << contadorRecomendaciones;

    // Mostrar en la lista
    NodoProducto* actual = todas;
    int itemsAgregados = 0;

    while (actual != nullptr) {
        Producto p = actual->dato;
        QString itemText = QString("%1 - %2 - $%3")
                               .arg(QString::fromStdString(p.descripcion))
                               .arg(QString::fromStdString(p.marca))
                               .arg(p.precio);

        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, p.id);
        ui->listWidget->addItem(item);

        itemsAgregados++;
        actual = actual->siguiente;
    }

    qDebug() << "Recomendaciones mostradas:" << itemsAgregados;
    qDebug() << "Items en listWidget:" << ui->listWidget->count();
}


void Ventana_Productos::mostrarDetallesProducto(int id)
{
    NodoProducto* actual = catalogoGlobal;
    while (actual != nullptr) {
        if (actual->dato.id == id) {
            productodialog dialog(actual->dato, this);
            dialog.exec();

            // Registrar en el historial
            if (usuarioActual) {
                insertarLista(usuarioActual->historial, std::to_string(id));
                cargarRecomendaciones();
            }
            break;
        }
        actual = actual->siguiente;
    }
}

Ventana_Productos::~Ventana_Productos()
{
    delete ui;
}

void Ventana_Productos::on_Volver_clicked()
{
    this->close();
    MainWindow *ventanaP = new MainWindow();
    ventanaP->show();
}

void Ventana_Productos::on_pushButton_clicked()
{
    // Obtener los valores de los filtros
    QString textoBusqueda = ui->lineEdit->text();
    QString categoria = ui->comboBox->currentText();
    QString marca = ui->comboBox_2->currentText();
    QString precioMinStr = ui->comboBox_3->currentText();
    QString precioMaxStr = ui->comboBox_4->currentText();

    // Convertir precios a double
    double precioMin = 0.0;
    double precioMax = std::numeric_limits<double>::max();

    // Extraer solo números de las cadenas de precio
    QRegularExpression re("(\\d+\\.?\\d*)");
    QRegularExpressionMatch matchMin = re.match(precioMinStr);
    QRegularExpressionMatch matchMax = re.match(precioMaxStr);

    if (matchMin.hasMatch()) {
        precioMin = matchMin.captured(1).toDouble();
    }

    if (matchMax.hasMatch()) {
        precioMax = matchMax.captured(1).toDouble();
    }

    // Validar rango de precios
    if (precioMin > precioMax) {
        QMessageBox::warning(this, "Error", "El precio mínimo no puede ser mayor al precio máximo");
        return;
    }

    // Iniciar con todos los productos
    ListaProducto resultados = catalogoGlobal;

    // Aplicar filtros en cascada
    if (!textoBusqueda.isEmpty()) {
        resultados = buscarPorDescripcion(resultados, textoBusqueda.toStdString());
    }

    if (categoria != "Categorias" && !categoria.isEmpty()) {
        resultados = buscarPorCategoria(resultados, categoria.toStdString());
    }

    if (marca != "Marca" && !marca.isEmpty()) {
        resultados = buscarPorMarca(resultados, marca.toStdString());
    }

    resultados = buscarPorRangoPrecios(resultados, precioMin, precioMax);

    // Mostrar resultados en listWidget_2
    ui->listWidget_2->clear();

    if (resultados == nullptr) {
        QListWidgetItem* item = new QListWidgetItem("No se encontraron productos");
        ui->listWidget_2->addItem(item);
        return;
    }

    NodoProducto* actual = resultados;
    while (actual != nullptr) {
        Producto p = actual->dato;
        QString itemText = QString::fromStdString(
            p.descripcion + " - " + p.marca + " - $" + std::to_string(p.precio)
            );
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, p.id);
        ui->listWidget_2->addItem(item);
        actual = actual->siguiente;
    }
}

