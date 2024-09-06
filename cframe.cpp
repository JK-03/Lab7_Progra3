#include "cframe.h"
#include "ui_cframe.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <Producto.h>
#include <QInputDialog>
#include <Pelicula.h>

cframe::cframe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cframe)
{
    ui->setupUi(this);

    this->setWindowTitle("Laboratorio #7");
}

cframe::~cframe()
{
    delete ui;
}

void cframe::on_pushButton_clicked()
{
    std::string opcion = "";

    QMessageBox msgBox;
    msgBox.setText("Por favor, seleccione una opción:");

    QPushButton *opcion4 = msgBox.addButton("Salir", QMessageBox::ActionRole);
    QPushButton *opcion3 = msgBox.addButton("Ejercicio #3", QMessageBox::ActionRole);
    QPushButton *opcion2 = msgBox.addButton("Ejercicio #2", QMessageBox::ActionRole);
    QPushButton *opcion1 = msgBox.addButton("Ejercicio #1", QMessageBox::ActionRole);

    msgBox.exec();

    if (msgBox.clickedButton() == opcion1) {
        ui->tabWidget->setCurrentIndex(1);
    } else if (msgBox.clickedButton() == opcion2) {
        ui->tabWidget->setCurrentIndex(2);
    } else if (msgBox.clickedButton() == opcion3) {
        ui->tabWidget->setCurrentIndex(3);
    } else if (msgBox.clickedButton() == opcion4) {
        this->close();
    }
}

// EJERCICIO #1
void cframe::on_botonAgregarEstudiante_clicked()
{
    QString primerNombre = ui->LE_Nombre->text();
    QString apellido = ui->LE_Apellido->text();
    int nota = ui->SPB_Nota->value();

    if (primerNombre.isEmpty() || apellido.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos", "Por favor, completa todos los campos.");
        return;
    }

    Estudiante nuevoEstudiante(primerNombre, apellido, nota);

    listaEstudiantes.append(nuevoEstudiante);
    QString infoEstudiante = nuevoEstudiante.nombreCompleto() + " - Nota: " + QString::number(nuevoEstudiante.nota);

    ui->LE_Nombre->clear();
    ui->LE_Apellido->clear();
    ui->SPB_Nota->setValue(0);

    QMessageBox::warning(this, "Éxito", "Estudiante agregado éxitosamente.");
}

void cframe::leerEstudiantesDesdeArchivo(const QString& nombreArchivo) {
    QFile archivo(nombreArchivo);

    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo de entrada.");
        return;
    }

    QTextStream in(&archivo);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList datos = linea.split(' ');

        if (datos.size() == 3) {
            QString primerNombre = datos[0];
            QString apellido = datos[1];
            int nota = datos[2].toInt();

            Estudiante nuevoEstudiante(primerNombre, apellido, nota);
            listaEstudiantes.append(nuevoEstudiante);
        }
    }

    archivo.close();
}

void cframe::escribirReporteAArchivo(const QString& nombreArchivo) {
    QFile archivo(nombreArchivo);

    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo de salida.");
        return;
    }

    QTextStream out(&archivo);
    for (const Estudiante& estudiante : listaEstudiantes) {
        QString resultado = estudiante.aprobo() ? "Aprobado" : "Reprobado";
        out << estudiante.nombreCompleto() << " - Nota: " << estudiante.nota << " - " << resultado << "\n";
    }

    archivo.close();
    QMessageBox::information(this, "Éxito", "El reporte ha sido guardado correctamente.");
}

void cframe::guardarEstudiantesEnArchivo(const QString& nombreArchivo) {
    QFile archivo(nombreArchivo);

    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo para guardar.");
        return;
    }

    QTextStream out(&archivo);
    for (const Estudiante& estudiante : listaEstudiantes) {
        out << estudiante.primerNombre << " " << estudiante.apellido << " " << estudiante.nota << "\n";
    }

    archivo.close();
    listaEstudiantes.clear();

    QMessageBox::information(this, "Éxito", "Los estudiantes han sido guardados.");
}

void cframe::on_botonReporte_3_clicked()
{
    std::string opcion = "";

    QMessageBox msgBox;
    msgBox.setText("Por favor, seleccione una opción:");

    QPushButton *opcion4 = msgBox.addButton("Salir", QMessageBox::ActionRole);
    QPushButton *opcion3 = msgBox.addButton("Crear Archivo", QMessageBox::ActionRole);
    QPushButton *opcion2 = msgBox.addButton("Generar Reporte", QMessageBox::ActionRole);
    QPushButton *opcion1 = msgBox.addButton("Leer Archivo", QMessageBox::ActionRole);

    msgBox.exec();

    if (msgBox.clickedButton() == opcion1) {
        QString nombreArchivo = QFileDialog::getOpenFileName(this, "Abrir archivo de estudiantes", "", "Text Files (*.txt);;All Files (*)");
        if (!nombreArchivo.isEmpty()) {
            leerEstudiantesDesdeArchivo(nombreArchivo);
        }

    } else if (msgBox.clickedButton() == opcion2) {
        QString nombreArchivo = QFileDialog::getSaveFileName(this, "Guardar reporte", "", "Text Files (*.txt);;All Files (*)");
        if (!nombreArchivo.isEmpty()) {
            escribirReporteAArchivo(nombreArchivo);
        }

    } else if (msgBox.clickedButton() == opcion3) {
        QString nombreArchivo = QFileDialog::getSaveFileName(this, "Guardar estudiantes", "", "Text Files (*.txt);;All Files (*)");
        if (!nombreArchivo.isEmpty()) {
            guardarEstudiantesEnArchivo(nombreArchivo);
        }

    }
}

//EJERCICIO #2
QVector<Producto> cframe::leerProductosDesdeArchivo(const QString& nombreArchivo) {
    QFile archivo(nombreArchivo);
    QVector<Producto> listaProductos;

    if (!archivo.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo binario.");
        return listaProductos;
    }

    QDataStream in(&archivo);
    Producto producto;

    while (!in.atEnd()) {
        in >> producto;
        listaProductos.append(producto);
    }

    archivo.close();
    return listaProductos;
}

void cframe::guardarProductosEnArchivo(const QString& nombreArchivo, const QVector<Producto>& productos) {
    QFile archivo(nombreArchivo);

    if (!archivo.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo binario.");
        return;
    }

    QDataStream out(&archivo);
    for (const Producto& producto : productos) {
        out << producto;
    }

    archivo.close();
}

void cframe::on_botonCargarProductos_clicked()
{
    QString archivo = QFileDialog::getOpenFileName(this, "Seleccionar Archivo de Productos", "", "Archivos Binarios (*.bin)");

    if (archivo.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se seleccionó ningún archivo.");
        return;
    }

    listaProductos = leerProductosDesdeArchivo(archivo);
}

void cframe::guardarProductosEnArchivo(const QString& nombreArchivo)
{
    QFile archivo(nombreArchivo);

    if (!archivo.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo para escritura.");
        return;
    }

    QDataStream out(&archivo);
    for (const Producto& producto : listaProductos) {
        out << producto;
    }

    archivo.close();
}

void cframe::on_botonActualizar_clicked()
{
    QString codigoProducto = ui->SPB_Codigo->text();

    if (codigoProducto.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debe ingresar un código de producto válido.");
        return;
    }

    bool encontrado = false;
    for (Producto& producto : listaProductos) {
        if (producto.codigo == codigoProducto.toInt()) {
            encontrado = true;

            bool ok;
            int nuevaCantidad = QInputDialog::getInt(this, "Actualizar Stock", "Ingrese la nueva cantidad en stock:", producto.cantidadStock, 0, 10000, 1, &ok);

            if (ok) {
                producto.cantidadStock = nuevaCantidad;
                guardarProductosEnArchivo(codigoProducto);
                QMessageBox::information(this, "Éxito", "La cantidad de stock ha sido actualizada.");
            }
            break;
        }
    }

    if (!encontrado) {
        QMessageBox::warning(this, "Error", "No se encontró un producto con ese código.");
    }
}

void cframe::crearArchivoBinarioDePrueba()
{
    QVector<Producto> productosDePrueba;
    productosDePrueba.append(Producto(1, "Producto A", 10.50, 100));
    productosDePrueba.append(Producto(2, "Producto B", 20.75, 150));
    productosDePrueba.append(Producto(3, "Producto C", 30.00, 200));

    QString archivo = QFileDialog::getSaveFileName(this, "Guardar Archivo de Productos de Prueba", "", "Archivos Binarios (*.bin)");

    if (archivo.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se seleccionó ningún archivo para guardar.");
        return;
    }

    QFile file(archivo);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escritura.");
        return;
    }

    QDataStream out(&file);
    for (const Producto& producto : productosDePrueba) {
        out << producto;
    }

    file.close();
    QMessageBox::information(this, "Éxito", "Archivo binario de prueba creado con éxito.");
}

void cframe::on_botonCargarProductos_2_clicked()
{
    crearArchivoBinarioDePrueba();
}

//EJERCICIO #3
QVector<Pelicula> leerPeliculasDesdeCSV(const QString& archivo)
{
    QVector<Pelicula> peliculas;
    QFile file(archivo);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "No se pudo abrir el archivo.");
        return peliculas;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList campos = linea.split(',');

        if (campos.size() != 4) {
            continue;
        }

        QString titulo = campos[0];
        int year = campos[1].toInt();
        int duracion = campos[2].toInt();
        double calificacion = campos[3].toDouble();

        peliculas.append(Pelicula(titulo, year, duracion, calificacion));
    }

    file.close();
    return peliculas;
}

void calcularEstadisticas(const QVector<Pelicula>& peliculas, QString& resultado)
{
    if (peliculas.isEmpty()) {
        resultado = "No hay datos para calcular estadísticas.";
        return;
    }

    double sumaCalificaciones = 0.0;
    int cantidadPeliculas = peliculas.size();
    int cantidadAltas = 0;
    double umbral = 7.0;

    for (const Pelicula& pelicula : peliculas) {
        sumaCalificaciones += pelicula.calificacion;
        if (pelicula.calificacion > umbral) {
            cantidadAltas++;
        }
    }

    double promedioCalificacion = sumaCalificaciones / cantidadPeliculas;
    resultado = QString("Cantidad de películas: %1\nPromedio de calificación: %2\nCantidad de películas con calificación superior a %3: %4")
                    .arg(cantidadPeliculas)
                    .arg(promedioCalificacion)
                    .arg(umbral)
                    .arg(cantidadAltas);
}

void cframe::on_botonGenerar3_clicked()
{
    QVector<Pelicula> peliculas;
    peliculas.append(Pelicula("Inception", 2010, 148, 8.8));
    peliculas.append(Pelicula("The Dark Knight", 2008, 152, 9.0));
    peliculas.append(Pelicula("Interstellar", 2014, 169, 8.6));
    peliculas.append(Pelicula("The Matrix", 1999, 136, 8.7));
    peliculas.append(Pelicula("The Godfather", 1972, 175, 9.2));
    peliculas.append(Pelicula("Pulp Fiction", 1994, 154, 8.9));
    peliculas.append(Pelicula("The Shawshank Redemption", 1994, 142, 9.3));
    peliculas.append(Pelicula("Forrest Gump", 1994, 142, 8.8));
    peliculas.append(Pelicula("Fight Club", 1999, 139, 8.8));
    peliculas.append(Pelicula("The Lion King", 1994, 88, 8.5));
    peliculas.append(Pelicula("Gladiator", 2000, 155, 8.5));
    peliculas.append(Pelicula("The Departed", 2006, 151, 8.5));
    peliculas.append(Pelicula("The Prestige", 2006, 130, 8.5));
    peliculas.append(Pelicula("Braveheart", 1995, 178, 8.4));
    peliculas.append(Pelicula("Titanic", 1997, 195, 7.8));

    QString archivo = QFileDialog::getSaveFileName(this, "Guardar Archivo CSV", "", "Archivos CSV (*.csv)");

    if (archivo.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se seleccionó ningún archivo.");
        return;
    }

    QFile file(archivo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escritura.");
        return;
    }

    QTextStream out(&file);

    for (const Pelicula& pelicula : peliculas) {
        out << pelicula.titulo << ","
            << pelicula.year << ","
            << pelicula.duracion << ","
            << pelicula.calificacion << "\n";
    }

    file.close();
    QMessageBox::information(this, "Éxito", "Archivo CSV generado exitosamente.");
}

void cframe::on_botonCargarArchivo_clicked()
{
    QString nombreArchivo = QFileDialog::getOpenFileName(this, "Abrir archivo de películas", "", "Archivos CSV (*.csv);;Todos los archivos (*)");

    if (nombreArchivo.isEmpty()) {
        return;
    }

    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }

    QTextStream in(&archivo);
    peliculas.clear();

    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList campos = linea.split(',');

        if (campos.size() == 4) {
            Pelicula pelicula;
            pelicula.titulo = campos[0];
            pelicula.year = campos[1].toInt();
            pelicula.duracion = campos[2].toInt();
            pelicula.calificacion = campos[3].toDouble();

            peliculas.append(pelicula);
        }
    }

    archivo.close();
}

void cframe::on_botonEstadisticas_clicked()
{
    if (peliculas.isEmpty()) {
        QMessageBox::warning(this, "No hay datos", "No se han cargado datos de películas.");
        return;
    }

    double sumaCalificaciones = 0.0;
    QMap<int, int> peliculasPorAno;
    int cantidadPeliculas = peliculas.size();

    for (const Pelicula& pelicula : peliculas) {
        sumaCalificaciones += pelicula.calificacion;

        if (peliculasPorAno.contains(pelicula.year)) {
            peliculasPorAno[pelicula.year]++;
        } else {
            peliculasPorAno[pelicula.year] = 1;
        }
    }

    double promedioCalificaciones = (cantidadPeliculas > 0) ? (sumaCalificaciones / cantidadPeliculas) : 0.0;

    QString mensaje;
    mensaje += QString("Promedio de calificaciones: %1\n").arg(promedioCalificaciones, 0, 'f', 2);
    mensaje += QString("Cantidad de películas por año:\n");

    for (auto it = peliculasPorAno.begin(); it != peliculasPorAno.end(); ++it) {
        mensaje += QString("Año %1: %2 película(s)\n").arg(it.key()).arg(it.value());
    }

    QMessageBox::information(this, "Estadísticas", mensaje);
}
