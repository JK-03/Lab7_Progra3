#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

class Estudiante {
public:
    QString primerNombre;
    QString apellido;
    int nota;

    Estudiante(const QString& primerNombre, const QString& apellido, int nota)
        : primerNombre(primerNombre), apellido(apellido), nota(nota) {}

    bool aprobo() const {
        return nota >= 60;
    }

    QString nombreCompleto() const {
        return primerNombre + " " + apellido;
    }
};

#endif // ESTUDIANTE_H
