#ifndef CFRAME_H
#define CFRAME_H

#include <QMainWindow>
#include <Estudiante.h>
#include <Producto.h>
#include <Pelicula.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class cframe;
}
QT_END_NAMESPACE

class cframe : public QMainWindow
{
    Q_OBJECT

public:
    cframe(QWidget *parent = nullptr);
    ~cframe();

    void leerEstudiantesDesdeArchivo(const QString& nombreArchivo);
    void escribirReporteAArchivo(const QString& nombreArchivo);
    void guardarEstudiantesEnArchivo(const QString& nombreArchivo);
    QVector<Producto> leerProductosDesdeArchivo(const QString& nombreArchivo);
    void guardarProductosEnArchivo(const QString& nombreArchivo, const QVector<Producto>& productos);
    void guardarProductosEnArchivo(const QString& nombreArchivo);
    void crearArchivoBinarioDePrueba();
    void guardarEstadisticasEnArchivo(const QString& archivo, double promedioCalificaciones, const QMap<int, int>& peliculasPorAno);

private slots:
    void on_pushButton_clicked();

    void on_botonAgregarEstudiante_clicked();

    void on_botonReporte_3_clicked();

    void on_botonCargarProductos_clicked();

    void on_botonActualizar_clicked();

    void on_botonCargarProductos_2_clicked();

    void on_botonGenerar3_clicked();

    void on_botonCargarArchivo_clicked();

    void on_botonEstadisticas_clicked();

private:
    Ui::cframe *ui;

    QVector<Estudiante> listaEstudiantes;
    QVector<Producto> listaProductos;
    QVector<Pelicula> peliculas;
};
#endif // CFRAME_H
