#ifndef PELICULA_H
#define PELICULA_H

#include <QString>
#include <QDataStream>

class Pelicula {
public:
    QString titulo;
    int year;
    int duracion;
    double calificacion;

    Pelicula(const QString& titulo = "", int year = 0, int duracion = 0, double calificacion = 0.0)
        : titulo(titulo), year(year), duracion(duracion), calificacion(calificacion) {}

    friend QDataStream& operator<<(QDataStream& out, const Pelicula& pelicula) {
        out << pelicula.titulo << pelicula.year << pelicula.duracion << pelicula.calificacion;
        return out;
    }

    friend QDataStream& operator>>(QDataStream& in, Pelicula& pelicula) {
        in >> pelicula.titulo >> pelicula.year >> pelicula.duracion >> pelicula.calificacion;
        return in;
    }
};

#endif // PELICULA_H
