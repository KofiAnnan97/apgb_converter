#include "mainwindow.h"
#include "./ui/ui_mainwindow.h"
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    setWindowTitle("Analogue Pocket GB Palette Creator");

    // Set up image viewer
    scene = new QGraphicsScene(this);
    ui->view_img->setScene(scene);

    // Initialize editor hexadecimal values
    this->initialize_editor_values();

    // Initialize image
    // Find a more clever way of retrieving this file from a folder
    QString imgDirPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator()
                                         + ".." + QDir::separator() + ".." + QDir::separator()
                                         + "graphics" + QDir::separator() + "image_view");
    if(!QDir(imgDirPath).exists()){
        QMessageBox::critical(this, "Image Error", "Could not find image_view/ directory.");
    }
    this->imgImporter.setImgDirectory(imgDirPath);
    image = new QImage(this->imgImporter.width, this->imgImporter.height, QImage::Format_RGB666);
    this->update_image_view(0);
    this->image_initialized = true;

    //Set button action for bringing up color picker
    //connect(ui->btn_bg_0, SIGNAL(clicked()), this, SLOT(on_hexBtn_clicked(QString("btn_bg_0"))));

    // Set button action for generating a file
    //connect(ui->btn_gen_file, SIGNAL(clicked()), this, SLOT(on_genBtn_clicked()));
}

MainWindow::~MainWindow(){
    delete ui;
}

QString MainWindow::getHexColor(QString prevColorName){
    QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"));
    QString hexColor;
    if(color.isValid()) hexColor = color.name();
    else hexColor = prevColorName;
    return hexColor;
}

void MainWindow::initialize_editor_values(){
    ui->txt_bg_0->setText(fImporter.black);
    ui->txt_bg_1->setText(fImporter.dark);
    ui->txt_bg_2->setText(fImporter.light);
    ui->txt_bg_3->setText(fImporter.white);
    ui->txt_obj0_0->setText(fImporter.black);
    ui->txt_obj0_1->setText(fImporter.dark);
    ui->txt_obj0_2->setText(fImporter.light);
    ui->txt_obj0_3->setText(fImporter.white);
    ui->txt_obj1_0->setText(fImporter.black);
    ui->txt_obj1_1->setText(fImporter.dark);
    ui->txt_obj1_2->setText(fImporter.light);
    ui->txt_obj1_3->setText(fImporter.white);
    ui->txt_window_0->setText(fImporter.black);
    ui->txt_window_1->setText(fImporter.dark);
    ui->txt_window_2->setText(fImporter.light);
    ui->txt_window_3->setText(fImporter.white);
}

vector<QString> MainWindow::getChosenPalettes(){
    vector<QString> temp;
    temp.push_back(ui->txt_bg_0->toPlainText());
    temp.push_back(ui->txt_bg_1->toPlainText());
    temp.push_back(ui->txt_bg_2->toPlainText());
    temp.push_back(ui->txt_bg_3->toPlainText());
    temp.push_back(ui->txt_obj0_0->toPlainText());
    temp.push_back(ui->txt_obj0_1->toPlainText());
    temp.push_back(ui->txt_obj0_2->toPlainText());
    temp.push_back(ui->txt_obj0_3->toPlainText());
    temp.push_back(ui->txt_obj1_0->toPlainText());
    temp.push_back(ui->txt_obj1_1->toPlainText());
    temp.push_back(ui->txt_obj1_2->toPlainText());
    temp.push_back(ui->txt_obj1_3->toPlainText());
    temp.push_back(ui->txt_window_0->toPlainText());
    temp.push_back(ui->txt_window_1->toPlainText());
    temp.push_back(ui->txt_window_2->toPlainText());
    temp.push_back(ui->txt_window_3->toPlainText());
    return temp;
}

void MainWindow::on_btn_import_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Import"), tr(""));
    APGB_Palette p;
    string fn = filename.toStdString();
    try{

        if(filename.endsWith(".csv"))                 p = this->fImporter.importPalettesFromCSV(fn);
        else if(filename.endsWith(".pal")){
            if(this->fImporter.isJASCFormat(fn))      p = this->fImporter.importPalettesJASC(fn);
            else if(this->fImporter.isAPGBFormat(fn)) p = this->fImporter.importPalettesFromAPGB(fn);
            else QMessageBox::critical(this, "Format Error", "There was an issue parsing " + filename);
        }
        else if(filename.endsWith(".gpl"))            p = this->fImporter.importPalettesGPLv2(fn);
        else if(filename.endsWith(".hex"))            p = this->fImporter.importPalettesHEXTxt(fn);
        else if(filename.isEmpty()) {}
        else QMessageBox::critical(this, "File Type Error", filename + " must use one of the following extensions:\n  .csv, .pal, .gpl, .hex");
    }
    catch(const std::out_of_range e){
        QMessageBox::critical(this, "Import Error", "There was an error try to import data \"" + filename
                                                        + "\" check to make sure the data is correct.");
    }
    bool canPopulate = p.bg != nullptr && p.obj0 != nullptr && p.obj1 != nullptr && p.window != nullptr;
    if(canPopulate == true){
        ui->txt_bg_0->setText(QString::fromStdString(p.bg[0]));
        ui->txt_bg_1->setText(QString::fromStdString(p.bg[1]));
        ui->txt_bg_2->setText(QString::fromStdString(p.bg[2]));
        ui->txt_bg_3->setText(QString::fromStdString(p.bg[3]));
        ui->txt_obj0_0->setText(QString::fromStdString(p.obj0[0]));
        ui->txt_obj0_1->setText(QString::fromStdString(p.obj0[1]));
        ui->txt_obj0_2->setText(QString::fromStdString(p.obj0[2]));
        ui->txt_obj0_3->setText(QString::fromStdString(p.obj0[3]));
        ui->txt_obj1_0->setText(QString::fromStdString(p.obj1[0]));
        ui->txt_obj1_1->setText(QString::fromStdString(p.obj1[1]));
        ui->txt_obj1_2->setText(QString::fromStdString(p.obj1[2]));
        ui->txt_obj1_3->setText(QString::fromStdString(p.obj1[3]));
        ui->txt_window_0->setText(QString::fromStdString(p.window[0]));
        ui->txt_window_1->setText(QString::fromStdString(p.window[1]));
        ui->txt_window_2->setText(QString::fromStdString(p.window[2]));
        ui->txt_window_3->setText(QString::fromStdString(p.window[3]));
    }
}

void MainWindow::on_btn_save_clicked()
{
    QString errorMsg;
    QString bg_0 = ui->txt_bg_0->toPlainText();
    QString bg_1 = ui->txt_bg_1->toPlainText();
    QString bg_2 = ui->txt_bg_2->toPlainText();
    QString bg_3 = ui->txt_bg_3->toPlainText();
    QString obj0_0 = ui->txt_obj0_0->toPlainText();
    QString obj0_1 = ui->txt_obj0_1->toPlainText();
    QString obj0_2 = ui->txt_obj0_2->toPlainText();
    QString obj0_3 = ui->txt_obj0_3->toPlainText();
    QString obj1_0 = ui->txt_obj1_0->toPlainText();
    QString obj1_1 = ui->txt_obj1_1->toPlainText();
    QString obj1_2 = ui->txt_obj1_2->toPlainText();
    QString obj1_3 = ui->txt_obj1_3->toPlainText();
    QString window_0 = ui->txt_window_0->toPlainText();
    QString window_1 = ui->txt_window_1->toPlainText();
    QString window_2 = ui->txt_window_2->toPlainText();
    QString window_3 = ui->txt_window_3->toPlainText();

    if(bg_0.isEmpty()) errorMsg += "bg_0\n";
    else if(bg_0.startsWith("#")) bg_0.remove(0,1);
    if(bg_1.isEmpty()) errorMsg += "bg_1\n";
    else if(bg_1.startsWith("#")) bg_1.remove(0,1);
    if(bg_2.isEmpty()) errorMsg += "bg_2\n";
    else if(bg_2.startsWith("#")) bg_2.remove(0,1);
    if(bg_3.isEmpty()) errorMsg += "bg_3\n";
    else if(bg_3.startsWith("#")) bg_3.remove(0,1);
    if(obj0_0.isEmpty()) errorMsg += "obj0_0\n";
    else if(obj0_0.startsWith("#")) obj0_0.remove(0,1);
    if(obj0_1.isEmpty()) errorMsg += "obj0_1\n";
    else if(obj0_1.startsWith("#")) obj0_1.remove(0,1);
    if(obj0_2.isEmpty()) errorMsg += "obj0_2\n";
    else if(obj0_2.startsWith("#")) obj0_2.remove(0,1);
    if(obj0_3.isEmpty()) errorMsg += "obj0_3\n";
    else if(obj0_3.startsWith("#")) obj0_3.remove(0,1);
    if(obj1_0.isEmpty()) errorMsg += "obj1_0\n";
    else if(obj1_0.startsWith("#")) obj1_0.remove(0,1);
    if(obj1_1.isEmpty()) errorMsg += "obj1_1\n";
    else if(obj1_1.startsWith("#")) obj1_1.remove(0,1);
    if(obj1_2.isEmpty()) errorMsg += "obj1_2\n";
    else if(obj1_2.startsWith("#")) obj1_2.remove(0,1);
    if(obj1_3.isEmpty()) errorMsg += "obj1_3\n";
    else if(obj1_3.startsWith("#")) obj1_3.remove(0,1);
    if(window_0.isEmpty()) errorMsg += "window_0\n";
    else if(window_0.startsWith("#")) window_0.remove(0,1);
    if(window_1.isEmpty()) errorMsg += "window_1\n";
    else if(window_0.startsWith("#")) window_1.remove(0,1);
    if(window_2.isEmpty()) errorMsg += "window_2\n";
    else if(window_0.startsWith("#")) window_2.remove(0,1);
    if(window_3.isEmpty()) errorMsg += "window_3\n";
    else if(window_0.startsWith("#")) window_3.remove(0,1);

    if(!errorMsg.isEmpty()){
        errorMsg = "The following values are empty:\n" + errorMsg;
        QMessageBox::critical(this, "Data Error", errorMsg);
    }else{
        string *bg = new string[4]{bg_0.toStdString(), bg_1.toStdString(), bg_2.toStdString(), bg_3.toStdString()};
        string *obj0 = new string[4]{obj0_0.toStdString(),obj0_1.toStdString(),obj0_2.toStdString(),obj0_3.toStdString()};
        string *obj1 = new string[4]{obj1_0.toStdString(),obj1_1.toStdString(),obj1_2.toStdString(),obj1_3.toStdString()};
        string *window = new string[4]{window_0.toStdString(), window_1.toStdString(), window_2.toStdString(), window_3.toStdString()};
        APGB_Palette p = this->fsave.getPalette(bg, obj0, obj1, window);

        QString filename = QFileDialog::getSaveFileName(this, tr("Save GB Palette"), tr("*.pal"));
        this->fsave.savePalette(filename.toStdString(), p);
    }
 }

void MainWindow::update_image_view(int direction = 0){
    vector<QString> temp = this->getChosenPalettes();
    this->imgImporter.changeFile(direction, image, temp);
    QPixmap imgView = QPixmap::fromImage(*image).scaled(ui->view_img->width(),
                                                        ui->view_img->height(),
                                                        Qt::KeepAspectRatio);
    this->scene->addPixmap(imgView);
}

void MainWindow::on_cb_dot_matrix_clicked(bool checked){
    if(checked){
        this->imgImporter.width *= 2;
        this->imgImporter.height *= 2;
        this->imgImporter.step = 2;
    }
    else{
        this->imgImporter.width /= 2;
        this->imgImporter.height /= 2;
        this->imgImporter.step = 1;
    }
    image = new QImage(this->imgImporter.width, this->imgImporter.height, QImage::Format_RGB666);
    this->update_image_view(0);
}

void MainWindow::on_btn_img_prev_clicked()
{
    this->update_image_view(-1);
}

void MainWindow::on_btn_img_next_clicked()
{
    this->update_image_view(1);
}

void MainWindow::on_btn_bg_0_clicked()
{
    QString prevColorName = ui->txt_bg_0->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_bg_0->setText(colorData);
}

void MainWindow::on_btn_bg_1_clicked()
{
    QString prevColorName = ui->txt_bg_1->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_bg_1->setText(colorData);
}

void MainWindow::on_btn_bg_2_clicked()
{
    QString prevColorName = ui->txt_bg_2->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_bg_2->setText(colorData);
}

void MainWindow::on_btn_bg_3_clicked()
{
    QString prevColorName = ui->txt_bg_3->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_bg_3->setText(colorData);
}

void MainWindow::on_txt_bg_0_textChanged()
{
    QString colorData = ui->txt_bg_0->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_bg_0->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_bg_1_textChanged()
{
    QString colorData = ui->txt_bg_1->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_bg_1->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_bg_2_textChanged()
{
    QString colorData = ui->txt_bg_2->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_bg_2->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_bg_3_textChanged()
{
    QString colorData = ui->txt_bg_3->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_bg_3->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_btn_obj0_0_clicked()
{
    QString prevColorName = ui->txt_obj0_0->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_obj0_0->setText(colorData);
}

void MainWindow::on_btn_obj0_1_clicked()
{
    QString prevColorName = ui->txt_obj0_1->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_obj0_1->setText(colorData);
}

void MainWindow::on_btn_obj0_2_clicked()
{
    QString prevColorName = ui->txt_obj0_2->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_obj0_2->setText(colorData);
}

void MainWindow::on_btn_obj0_3_clicked()
{
    QString prevColorName = ui->txt_obj0_3->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_obj0_3->setText(colorData);
}

void MainWindow::on_txt_obj0_0_textChanged()
{
    QString colorData = ui->txt_obj0_0->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_obj0_0->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_obj0_1_textChanged()
{
    QString colorData = ui->txt_obj0_1->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_obj0_1->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_obj0_2_textChanged()
{
    QString colorData = ui->txt_obj0_2->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_obj0_2->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_obj0_3_textChanged()
{
    QString colorData = ui->txt_obj0_3->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_obj0_3->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_btn_obj1_0_clicked()
{
    QString prevColorName = ui->txt_obj1_0->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_obj1_0->setText(colorData);
}

void MainWindow::on_btn_obj1_1_clicked()
{
    QString prevColorName = ui->txt_obj1_1->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_obj1_1->setText(colorData);
}

void MainWindow::on_btn_obj1_2_clicked()
{
    QString prevColorName = ui->txt_obj1_2->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_obj1_2->setText(colorData);
}

void MainWindow::on_btn_obj1_3_clicked()
{
    QString prevColorName = ui->txt_obj1_3->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_obj1_3->setText(colorData);
}

void MainWindow::on_txt_obj1_0_textChanged()
{
    QString colorData = ui->txt_obj1_0->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_obj1_0->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_obj1_1_textChanged()
{
    QString colorData = ui->txt_obj1_1->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_obj1_1->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_obj1_2_textChanged()
{
    QString colorData = ui->txt_obj1_2->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_obj1_2->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_obj1_3_textChanged()
{
    QString colorData = ui->txt_obj1_3->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_obj1_3->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_btn_window_0_clicked()
{
    QString prevColorName = ui->txt_window_0->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_window_0->setText(colorData);
}

void MainWindow::on_btn_window_1_clicked()
{
    QString prevColorName = ui->txt_window_1->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_window_1->setText(colorData);
}

void MainWindow::on_btn_window_2_clicked()
{
    QString prevColorName = ui->txt_window_2->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_window_2->setText(colorData);
}

void MainWindow::on_btn_window_3_clicked()
{
    QString prevColorName = ui->txt_window_3->toPlainText();
    QString colorData = this->getHexColor(prevColorName);
    ui->txt_window_3->setText(colorData);
}

void MainWindow::on_txt_window_0_textChanged()
{
    QString colorData = ui->txt_window_0->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_window_0->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_window_1_textChanged()
{
    QString colorData = ui->txt_window_1->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_window_1->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_window_2_textChanged()
{
    QString colorData = ui->txt_window_2->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_window_2->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_txt_window_3_textChanged()
{
    QString colorData = ui->txt_window_3->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) ui->btn_window_3->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->update_image_view();
}

void MainWindow::on_btn_convert_reset_clicked()
{
    ui->txt_convert_load->setText("");
    ui->txt_convert_save->setText("");
}

void MainWindow::on_btn_get_load_clicked()
{
    QString loadFilename = QFileDialog::getOpenFileName(this, tr("Load Palette"), tr(""));
    ui->txt_convert_load->setText(loadFilename);
    if(loadFilename.endsWith(".csv")) ui->r_csv->click();
    else if(loadFilename.endsWith(".pal") && this->fImporter.isJASCFormat(loadFilename.toStdString())){
        ui->r_jasc->click();
    }
    else if(loadFilename.endsWith(".gpl")) ui->r_gpl->click();
    else if(loadFilename.endsWith(".hex")) ui->r_hex_txt->click();
    int extensionIdx = loadFilename.lastIndexOf(".");
    ui->txt_convert_save->setText(loadFilename.left(extensionIdx) + "-apgb.pal");
}

void MainWindow::on_btn_get_save_clicked()
{
    QString saveFilename = QFileDialog::getSaveFileName(this, tr("Save Palette"), tr(""));
    ui->txt_convert_save->setText(saveFilename);
}

void MainWindow::on_btn_convert_save_clicked()
{
    APGB_Palette p;
    QString loadFile = ui->txt_convert_load->toPlainText();
    QString saveFile = ui->txt_convert_save->toPlainText();

    string lFn = loadFile.toStdString();
    if(!loadFile.isEmpty()){
        if(ui->r_csv->isChecked() && loadFile.endsWith(".csv")){
            p = this->fImporter.importPalettesFromCSV(lFn);
        }
        else if(ui->r_jasc->isChecked() && this->fImporter.isJASCFormat(lFn)){
            p = this->fImporter.importPalettesJASC(lFn);
        }
        else if(ui->r_gpl->isChecked() && loadFile.endsWith(".gpl")){
            p = this->fImporter.importPalettesGPLv2(lFn);
        }
        else if(ui->r_hex_txt->isChecked() && loadFile.endsWith(".hex")){
            p = this->fImporter.importPalettesHEXTxt(lFn);
        }

        bool paletteInitialized = p.bg != nullptr && p.obj0 != nullptr &&
                                  p.obj1 != nullptr && p.window != nullptr;
        if(!saveFile.isEmpty() && paletteInitialized){
            this->fsave.savePalette(saveFile.toStdString(), p);
            ui->txt_convert_load->setText("");
            ui->txt_convert_save->setText("");
            QMessageBox::information(this, " ", saveFile + " was successfully saved.");
        }
        else if(!saveFile.isEmpty() && !paletteInitialized){
            QMessageBox::critical(this, "Format Error", "There was an error processing the file. Please make sure this file uses a supported file type.");
        }
        else QMessageBox::critical(this, "File Error", "No destination file was given.");
    }
    else QMessageBox::critical(this, "File Error", "No source file was given.");
}


void MainWindow::on_btn_flip_bg_clicked()
{
    QString temp = ui->txt_bg_0->toPlainText();
    ui->txt_bg_0->setText(ui->txt_bg_3->toPlainText());
    ui->txt_bg_3->setText(temp);
    temp = ui->txt_bg_1->toPlainText();
    ui->txt_bg_1->setText(ui->txt_bg_2->toPlainText());
    ui->txt_bg_2->setText(temp);
}


void MainWindow::on_btn_flip_obj0_clicked()
{
    QString temp = ui->txt_obj0_0->toPlainText();
    ui->txt_obj0_0->setText(ui->txt_obj0_3->toPlainText());
    ui->txt_obj0_3->setText(temp);
    temp = ui->txt_obj0_1->toPlainText();
    ui->txt_obj0_1->setText(ui->txt_obj0_2->toPlainText());
    ui->txt_obj0_2->setText(temp);
}


void MainWindow::on_btn_flip_obj1_clicked()
{
    QString temp = ui->txt_obj1_0->toPlainText();
    ui->txt_obj1_0->setText(ui->txt_obj1_3->toPlainText());
    ui->txt_obj1_3->setText(temp);
    temp = ui->txt_obj1_1->toPlainText();
    ui->txt_obj1_1->setText(ui->txt_obj1_2->toPlainText());
    ui->txt_obj1_2->setText(temp);
}


void MainWindow::on_btn_flip_window_clicked()
{
    QString temp = ui->txt_window_0->toPlainText();
    ui->txt_window_0->setText(ui->txt_window_3->toPlainText());
    ui->txt_window_3->setText(temp);
    temp = ui->txt_window_1->toPlainText();
    ui->txt_window_1->setText(ui->txt_window_2->toPlainText());
    ui->txt_window_2->setText(temp);
}


void MainWindow::on_btn_flip_all_clicked()
{
    this->on_btn_flip_bg_clicked();
    this->on_btn_flip_obj0_clicked();
    this->on_btn_flip_obj1_clicked();
    this->on_btn_flip_window_clicked();
}


void MainWindow::on_btn_populate_all_clicked()
{
    QString val0 = ui->txt_bg_0->toPlainText();
    QString val1 = ui->txt_bg_1->toPlainText();
    QString val2 = ui->txt_bg_2->toPlainText();
    QString val3 = ui->txt_bg_3->toPlainText();
    if(QColor(val0).isValid() && QColor(val1).isValid() &&
       QColor(val2).isValid() && QColor(val3).isValid()){
        ui->txt_obj0_0->setText(val0);
        ui->txt_obj0_1->setText(val1);
        ui->txt_obj0_2->setText(val2);
        ui->txt_obj0_3->setText(val3);
        ui->txt_obj1_0->setText(val0);
        ui->txt_obj1_1->setText(val1);
        ui->txt_obj1_2->setText(val2);
        ui->txt_obj1_3->setText(val3);
        ui->txt_window_0->setText(val0);
        ui->txt_window_1->setText(val1);
        ui->txt_window_2->setText(val2);
        ui->txt_window_3->setText(val3);
    }
    else QMessageBox::critical(this, "Populate All Error", "Make sure all intensities for BG are correct");

}
