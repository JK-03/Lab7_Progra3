#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <QString>
#include <QDataStream>

class Producto {
public:
    int codigo;
    QString nombre;
    double precio;
    int cantidadStock;

    Producto() : codigo(0), precio(0.0), cantidadStock(0) {}
    Producto(int codigo, const QString& nombre, double precio, int cantidadStock)
        : codigo(codigo), nombre(nombre), precio(precio), cantidadStock(cantidadStock) {}

    friend QDataStream& operator<<(QDataStream& out, const Producto& producto) {
        out << producto.codigo << producto.nombre << producto.precio << producto.cantidadStock;
        return out;
    }

    friend QDataStream& operator>>(QDataStream& in, Producto& producto) {
        in >> producto.codigo >> producto.nombre >> producto.precio >> producto.cantidadStock;
        return in;
    }
};

#endif // PRODUCTO_H
