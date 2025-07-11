#include "core/recomendacion.h"
#include "core/utilidades.h"
#include "core/catalogo.h"
#include <algorithm> // Para std::min

// Función auxiliar para ordenar un mapa y obtener los N mejores elementos
ListaString obtenerTopN(MapaStringInt mapa, int n) {
    ListaString resultado = nullptr;
    int longitud = 0;
    NodoMapaStringInt* actual = mapa;
    while(actual != nullptr) {
        longitud++;
        actual = actual->siguiente;
    }

    for (int i = 0; i < n && i < longitud; ++i) {
        NodoMapaStringInt* mejor = nullptr;
        NodoMapaStringInt* ptr = mapa;
        while (ptr != nullptr) {
            if (!existeEnLista(resultado, ptr->clave)) {
                if (mejor == nullptr || ptr->valor > mejor->valor) {
                    mejor = ptr;
                }
            }
            ptr = ptr->siguiente;
        }
        if (mejor != nullptr) {
            insertarLista(resultado, mejor->clave);
        }
    }
    return resultado;
}

EstadisticasUsuario obtenerEstadisticasUsuario(Usuario *usuario)
{
    EstadisticasUsuario stats;
    MapaStringInt conteoCategorias = nullptr;
    MapaStringInt conteoMarcas = nullptr;
    MapaIntInt conteoCalidades = nullptr;

    Nodolista *actualHistorial = usuario->historial;
    while (actualHistorial != nullptr)
    {
        int id = std::stoi(actualHistorial->dato);
        NodoProducto* actualProducto = catalogoGlobal;
        while (actualProducto != nullptr)
        {
            if (actualProducto->dato.id == id)
            {
                incrementarEnMapa(conteoCategorias, actualProducto->dato.categoria);
                incrementarEnMapa(conteoMarcas, actualProducto->dato.marca);
                incrementarEnMapa(conteoCalidades, actualProducto->dato.calidad);
                break;
            }
            actualProducto = actualProducto->siguiente;
        }
        actualHistorial = actualHistorial->siguiente;
    }

    if (!mapaEstaVacio(conteoCategorias))
    {
        stats.categoriasFrecuentes = obtenerTopN(conteoCategorias, 2);
    }

    if (!mapaEstaVacio(conteoMarcas))
    {
        stats.marcasFrecuentes = obtenerTopN(conteoMarcas, 2);
    }

    if (!mapaEstaVacio(conteoCalidades))
    {
        NodoMapaIntInt* mejor = nullptr;
        NodoMapaIntInt* ptr = conteoCalidades;
        while (ptr != nullptr) {
            if (mejor == nullptr || ptr->valor > mejor->valor) {
                mejor = ptr;
            }
            ptr = ptr->siguiente;
        }
        stats.calidadFrecuente = mejor->clave;
    }

    return stats;
}

void actualizarPreferenciasDinamicas(Usuario *usuario)
{
    if (usuario == nullptr || usuario->historial == nullptr) return;

    MapaStringInt conteoCategorias = nullptr;
    MapaStringInt conteoMarcas = nullptr;

    Nodolista *actualHistorial = usuario->historial;
    while (actualHistorial != nullptr)
    {
        int id = std::stoi(actualHistorial->dato);
        NodoProducto* actualProducto = catalogoGlobal;
        while (actualProducto != nullptr)
        {
            if (actualProducto->dato.id == id)
            {
                incrementarEnMapa(conteoCategorias, actualProducto->dato.categoria);
                incrementarEnMapa(conteoMarcas, actualProducto->dato.marca);
                break;
            }
            actualProducto = actualProducto->siguiente;
        }
        actualHistorial = actualHistorial->siguiente;
    }

    // Limpiar preferencias de categorías anteriores para recalcular
    while (usuario->categoriasPreferidas != nullptr)
    {
        Nodolista* temp = usuario->categoriasPreferidas;
        usuario->categoriasPreferidas = usuario->categoriasPreferidas->siguiente;
        delete temp;
    }

    if (!mapaEstaVacio(conteoCategorias))
    {
        usuario->categoriasPreferidas = obtenerTopN(conteoCategorias, 2);
    }

    if (!mapaEstaVacio(conteoMarcas))
    {
        ListaString topMarcas = obtenerTopN(conteoMarcas, 2);
        Nodolista* actualMarca = topMarcas;
        while (actualMarca != nullptr)
        {
            if (!existeEnLista(usuario->preferencias, actualMarca->dato))
            {
                insertarLista(usuario->preferencias, actualMarca->dato);
            }
            actualMarca = actualMarca->siguiente;
        }
    }
}

Recomendaciones generarRecomendaciones(Usuario *usuario)
{
    actualizarPreferenciasDinamicas(usuario);
    Recomendaciones rec;
    SetInt productosExcluidos = nullptr;

    auto agregarIDs = [&](Nodolista *lista)
    {
        Nodolista *actual = lista;
        while (actual != nullptr)
        {
            try
            {
                int id = std::stoi(actual->dato);
                insertarEnSet(productosExcluidos, id);
            }
            catch (...)
            {
                // Ignorar conversiones fallidas
            }
            actual = actual->siguiente;
        }
    };

    agregarIDs(usuario->historial);
    agregarIDs(usuario->carrito);
    agregarIDs(usuario->listaDeseos);

    // Función para filtrar productos no vistos
    auto filtrarNoVistos = [&](ListaProducto lista) {
        ListaProducto resultado = nullptr;
        NodoProducto* actual = lista;
        while (actual != nullptr) {
            if (!buscarEnSet(productosExcluidos, actual->dato.id)) {
                insertarEnLista(resultado, actual->dato);
            }
            actual = actual->siguiente;
        }
        return resultado;
    };

    // Recomendación por marcas preferidas
    if (usuario->preferencias != nullptr)
    {
        ListaProducto candidatos = nullptr;
        Nodolista *pref = usuario->preferencias;
        while (pref != nullptr)
        {
            NodoProducto* actualProd = catalogoGlobal;
            while(actualProd != nullptr)
            {
                if (actualProd->dato.marca == pref->dato)
                {
                    insertarEnLista(candidatos, actualProd->dato);
                }
                actualProd = actualProd->siguiente;
            }
            pref = pref->siguiente;
        }
        barajarLista(candidatos);
        rec.porMarcasPreferidas = filtrarNoVistos(candidatos);
    }

    EstadisticasUsuario stats = obtenerEstadisticasUsuario(usuario);

    // Recomendación por otras marcas frecuentes
    if (stats.marcasFrecuentes != nullptr)
    {
        ListaProducto candidatos = nullptr;
        Nodolista* marcaFrecuente = stats.marcasFrecuentes;
        while(marcaFrecuente != nullptr)
        {
            if (!existeEnLista(usuario->preferencias, marcaFrecuente->dato))
            {
                NodoProducto* actualProd = catalogoGlobal;
                while(actualProd != nullptr)
                {
                    if (actualProd->dato.marca == marcaFrecuente->dato)
                    {
                        insertarEnLista(candidatos, actualProd->dato);
                    }
                    actualProd = actualProd->siguiente;
                }
            }
            marcaFrecuente = marcaFrecuente->siguiente;
        }
        barajarLista(candidatos);
        rec.porOtrasMarcasFrecuentes = filtrarNoVistos(candidatos);
    }

    // Recomendación por categorías frecuentes
    if (stats.categoriasFrecuentes != nullptr)
    {
        ListaProducto candidatos = nullptr;
        Nodolista* catFrecuente = stats.categoriasFrecuentes;
        while(catFrecuente != nullptr)
        {
            NodoProducto* actualProd = catalogoGlobal;
            while(actualProd != nullptr)
            {
                if (actualProd->dato.categoria == catFrecuente->dato)
                {
                    insertarEnLista(candidatos, actualProd->dato);
                }
                actualProd = actualProd->siguiente;
            }
            catFrecuente = catFrecuente->siguiente;
        }
        barajarLista(candidatos);
        rec.porCategoriaFrecuente = filtrarNoVistos(candidatos);
    }

    // Recomendación por categorías preferidas
    if (usuario->categoriasPreferidas != nullptr)
    {
        ListaProducto candidatos = nullptr;
        Nodolista* pref = usuario->categoriasPreferidas;
        while (pref != nullptr)
        {
            NodoProducto* actualProd = catalogoGlobal;
            while(actualProd != nullptr)
            {
                if (actualProd->dato.categoria == pref->dato)
                {
                    insertarEnLista(candidatos, actualProd->dato);
                }
                actualProd = actualProd->siguiente;
            }
            pref = pref->siguiente;
        }
        barajarLista(candidatos);
        rec.porCategoriaPreferida = filtrarNoVistos(candidatos);
    }

    // Recomendación por calidad similar
    if (stats.calidadFrecuente > 0)
    {
        ListaProducto candidatos = nullptr;
        NodoProducto* actualProd = catalogoGlobal;
        while(actualProd != nullptr)
        {
            if (actualProd->dato.calidad == stats.calidadFrecuente)
            {
                insertarEnLista(candidatos, actualProd->dato);
            }
            actualProd = actualProd->siguiente;
        }
        barajarLista(candidatos);
        rec.porCalidad = filtrarNoVistos(candidatos);
    }

    return rec;
}
