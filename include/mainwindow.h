#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QColor>
#include <QTextEdit>
#include <QColorDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "fileimporter.h"
#include "imageimporter.h"
#include "save.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Setup Functions
    void initializeUIConnects();
    void initializeEditorValues();

    // Helper Functions
    vector<QString> getChosenPalettes();
    void updateImageView(int direction);

    // Classes instances
    FileImporter fImporter;
    ImageImporter imgImporter;
    FileSave fsave;

public slots:
    void pickColor(QTextEdit *te);
    void updateBtnColorFromText(QPushButton *pb, QTextEdit *te);
    void flipTextEntries(std::vector<QTextEdit *> vte);

private slots:
    // Editor tab
    void on_btn_import_clicked();
    void on_btn_save_clicked();

    // Converter tab
    void on_btn_convert_reset_clicked();
    void on_btn_get_load_clicked();
    void on_btn_get_save_clicked();
    void on_btn_convert_save_clicked();

    // Image Viewer Filters
    void on_btn_dot_matrix_clicked(bool checked);

    // Populate intensities based on BG
    void on_btn_populate_all_clicked();

    //void updateSizes(int index);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QImage *image;
    bool image_initialized = false;
};
#endif // MAINWINDOW_H
