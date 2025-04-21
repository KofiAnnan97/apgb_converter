#include "mainwindow.h"
#include "./ui/ui_mainwindow.h"
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Analogue Pocket GB Palette Creator");

    // Set up image viewer
    scene = new QGraphicsScene(this);
    ui->view_img->setScene(scene);

    // Initialize image (Find a more clever way of retrieving this file from a folder)
    QString imgDirPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator()
                                         + ".." + QDir::separator() + ".." + QDir::separator()
                                         + "graphics" + QDir::separator() + "image_view");
    if(!QDir(imgDirPath).exists()){
        QMessageBox::critical(this, "Image Error", "Could not find image_view/ directory.");
    }
    this->imgImporter.setImgDirectory(imgDirPath);
    image = new QImage(this->imgImporter.width, this->imgImporter.height, QImage::Format_RGB666);
    this->updateImageView(0);
    this->image_initialized = true;

    // Initialize QObject UI Connections
    this->initializeUIConnects();

    // Initialize editor hexadecimal values
    this->initializeEditorValues();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::initializeUIConnects(){
    // Set traversal buttons for image viewer
    connect(ui->btn_img_next, &QPushButton::clicked, this, [=](){updateImageView(1);});
    connect(ui->btn_img_prev, &QPushButton::clicked, this, [=](){updateImageView(-1);});

    // Set button action for bringing up color picker
    connect(ui->btn_bg_0, &QPushButton::clicked, this, [=](){pickColor(ui->txt_bg_0);});
    connect(ui->btn_bg_1, &QPushButton::clicked, this, [=](){pickColor(ui->txt_bg_1);});
    connect(ui->btn_bg_2, &QPushButton::clicked, this, [=](){pickColor(ui->txt_bg_2);});
    connect(ui->btn_bg_3, &QPushButton::clicked, this, [=](){pickColor(ui->txt_bg_3);});
    connect(ui->btn_obj0_0, &QPushButton::clicked, this, [=](){pickColor(ui->txt_obj0_0);});
    connect(ui->btn_obj0_1, &QPushButton::clicked, this, [=](){pickColor(ui->txt_obj0_1);});
    connect(ui->btn_obj0_2, &QPushButton::clicked, this, [=](){pickColor(ui->txt_obj0_2);});
    connect(ui->btn_obj0_3, &QPushButton::clicked, this, [=](){pickColor(ui->txt_obj0_3);});
    connect(ui->btn_obj1_0, &QPushButton::clicked, this, [=](){pickColor(ui->txt_obj1_0);});
    connect(ui->btn_obj1_1, &QPushButton::clicked, this, [=](){pickColor(ui->txt_obj1_1);});
    connect(ui->btn_obj1_2, &QPushButton::clicked, this, [=](){pickColor(ui->txt_obj1_2);});
    connect(ui->btn_obj1_3, &QPushButton::clicked, this, [=](){pickColor(ui->txt_obj1_3);});
    connect(ui->btn_window_0, &QPushButton::clicked, this, [=](){pickColor(ui->txt_window_0);});
    connect(ui->btn_window_1, &QPushButton::clicked, this, [=](){pickColor(ui->txt_window_1);});
    connect(ui->btn_window_2, &QPushButton::clicked, this, [=](){pickColor(ui->txt_window_2);});
    connect(ui->btn_window_3, &QPushButton::clicked, this, [=](){pickColor(ui->txt_window_3);});

    // Set text edit action to update button color
    connect(ui->txt_bg_0, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_bg_0, ui->txt_bg_0);
    });
    connect(ui->txt_bg_1, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_bg_1, ui->txt_bg_1);
    });
    connect(ui->txt_bg_2, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_bg_2, ui->txt_bg_2);
    });
    connect(ui->txt_bg_3, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_bg_3, ui->txt_bg_3);
    });
    connect(ui->txt_obj0_0, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_obj0_0, ui->txt_obj0_0);
    });
    connect(ui->txt_obj0_1, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_obj0_1, ui->txt_obj0_1);
    });
    connect(ui->txt_obj0_2, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_obj0_2, ui->txt_obj0_2);
    });
    connect(ui->txt_obj0_3, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_obj0_3, ui->txt_obj0_3);
    });
    connect(ui->txt_obj1_0, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_obj1_0, ui->txt_obj1_0);
    });
    connect(ui->txt_obj1_1, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_obj1_1, ui->txt_obj1_1);
    });
    connect(ui->txt_obj1_2, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_obj1_2, ui->txt_obj1_2);
    });
    connect(ui->txt_obj1_3, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_obj1_3, ui->txt_obj1_3);
    });
    connect(ui->txt_window_0, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_window_0, ui->txt_window_0);
    });
    connect(ui->txt_window_1, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_window_1, ui->txt_window_1);
    });
    connect(ui->txt_window_2, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_window_2, ui->txt_window_2);
    });
    connect(ui->txt_window_3, &QTextEdit::textChanged, this, [=](){
        updateBtnColorFromText(ui->btn_window_3, ui->txt_window_3);
    });

    // Set flip palette buttons
    connect(ui->btn_flip_bg, &QPushButton::clicked, this, [=](){
        vector<QTextEdit*> vte = {ui->txt_bg_0, ui->txt_bg_1,
                                  ui->txt_bg_2, ui->txt_bg_3};
        flipTextEntries(vte);
    });
    connect(ui->btn_flip_obj0, &QPushButton::clicked, this, [=](){
        vector<QTextEdit*> vte = {ui->txt_obj0_0, ui->txt_obj0_1,
                                  ui->txt_obj0_2, ui->txt_obj0_3};
        flipTextEntries(vte);
    });
    connect(ui->btn_flip_obj1, &QPushButton::clicked, this, [=](){
        vector<QTextEdit*> vte = {ui->txt_obj1_0, ui->txt_obj1_1,
                                  ui->txt_obj1_2, ui->txt_obj1_3};
        flipTextEntries(vte);
    });
    connect(ui->btn_flip_window, &QPushButton::clicked, this, [=](){
        vector<QTextEdit*> vte = {ui->txt_window_0, ui->txt_window_1,
                                  ui->txt_window_2, ui->txt_window_3};
        flipTextEntries(vte);
    });
    connect(ui->btn_flip_all, &QPushButton::clicked, this, [=](){
        vector<QTextEdit*> vte = {ui->txt_bg_0, ui->txt_bg_1,
                                  ui->txt_obj0_0, ui->txt_obj0_1,
                                  ui->txt_obj1_0, ui->txt_obj1_1,
                                  ui->txt_window_0, ui->txt_window_1,
                                  ui->txt_window_2, ui->txt_window_3,
                                  ui->txt_obj1_2, ui->txt_obj1_3,
                                  ui->txt_obj0_2, ui->txt_obj0_3,
                                  ui->txt_bg_2, ui->txt_bg_3};
        flipTextEntries(vte);
    });
}

void MainWindow::initializeEditorValues(){
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

void MainWindow::updateImageView(int direction = 0){
    vector<QString> temp = this->getChosenPalettes();
    this->imgImporter.changeFile(direction, image, temp);
    QPixmap imgView = QPixmap::fromImage(*image).scaled(ui->view_img->width(),
                                                        ui->view_img->height(),
                                                        Qt::KeepAspectRatio);
    this->scene->addPixmap(imgView);
}

void MainWindow::pickColor(QTextEdit *te){
    QString prevColorName = te->toPlainText();
    QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"));
    QString colorData;
    if(color.isValid()) colorData = color.name();
    else colorData = prevColorName;
    te->setText(colorData);
}

void MainWindow::updateBtnColorFromText(QPushButton *pb, QTextEdit *te){
    QString colorData = te->toPlainText();
    QString colorStyle = "QPushButton { background-color : " + colorData + "}";
    if(QColor(colorData).isValid()) pb->setStyleSheet(colorStyle);
    if(this->image_initialized == true) this->updateImageView(0);
}

void MainWindow::flipTextEntries(vector<QTextEdit *> vte){
    int startIdx = 0;
    int endIdx = vte.size() - 1;
    while (startIdx < endIdx) {
        QString temp = vte[startIdx]->toPlainText();
        vte[startIdx]->setText(vte[endIdx]->toPlainText());
        vte[endIdx]->setText(temp);
        ++startIdx;
        --endIdx;
    }
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
        delete[] p.bg;
        delete[] p.obj0;
        delete[] p.obj1;
        delete[] p.window;
    }
}

bool MainWindow::isValidColorFormat(QString cStr){
    return !cStr.isEmpty() && cStr.size() == 7 &&
           cStr.startsWith("#") && QColor(cStr).isValid();
}

void MainWindow::on_btn_save_clicked(){
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

    QString errorMsg;
    if(this->isValidColorFormat(bg_0)) bg_0.remove(0,1);
    else errorMsg += "\nbg_0";
    if(this->isValidColorFormat(bg_1)) bg_1.remove(0,1);
    else errorMsg += "\nbg_1";
    if(this->isValidColorFormat(bg_2)) bg_2.remove(0,1);
    else errorMsg += "\nbg_2";
    if(this->isValidColorFormat(bg_3)) bg_3.remove(0,1);
    else errorMsg += "\nbg_3";
    if(this->isValidColorFormat(obj0_0)) obj0_0.remove(0,1);
    else errorMsg += "\nobj0_0";
    if(this->isValidColorFormat(obj0_1)) obj0_1.remove(0,1);
    else errorMsg += "\nobj0_1";
    if(this->isValidColorFormat(obj0_2)) obj0_2.remove(0,1);
    else errorMsg += "\nobj0_2";
    if(this->isValidColorFormat(obj0_3)) obj0_3.remove(0,1);
    else errorMsg += "\nobj0_3";
    if(this->isValidColorFormat(obj1_0)) obj1_0.remove(0,1);
    else errorMsg += "\nobj1_0";
    if(this->isValidColorFormat(obj1_1)) obj1_1.remove(0,1);
    else errorMsg += "\nobj1_1";
    if(this->isValidColorFormat(obj1_2)) obj1_2.remove(0,1);
    else errorMsg += "\nobj1_2";
    if(this->isValidColorFormat(obj1_3)) obj1_3.remove(0,1);
    else errorMsg += "\nobj1_3";
    if(this->isValidColorFormat(window_0)) window_0.remove(0,1);
    else errorMsg += "\nwindow_0";
    if(this->isValidColorFormat(window_1)) window_1.remove(0,1);
    else errorMsg += "\nwindow_1";
    if(this->isValidColorFormat(window_2)) window_2.remove(0,1);
    else errorMsg += "\nwindow_2";
    if(this->isValidColorFormat(window_3)) window_3.remove(0,1);
    else errorMsg += "\nwindow_3";

    if(!errorMsg.isEmpty()){
        errorMsg = "The following values are malformed or empty (make sure to use a 24-bit hexadecimal color starting with \'#\'):" + errorMsg;
        QMessageBox::critical(this, "Data Error", errorMsg);
    }else{
        APGB_Palette p;
        p.bg = new string[4]{bg_0.toStdString(), bg_1.toStdString(), bg_2.toStdString(), bg_3.toStdString()};
        p.obj0 = new string[4]{obj0_0.toStdString(),obj0_1.toStdString(),obj0_2.toStdString(),obj0_3.toStdString()};
        p.obj1 = new string[4]{obj1_0.toStdString(),obj1_1.toStdString(),obj1_2.toStdString(),obj1_3.toStdString()};
        p.window = new string[4]{window_0.toStdString(), window_1.toStdString(), window_2.toStdString(), window_3.toStdString()};
        QString filename = QFileDialog::getSaveFileName(this, tr("Save GB Palette"), tr("*.pal"));
        this->fsave.savePalettes(filename.toStdString(), p);
        delete[] p.bg;
        delete[] p.obj0;
        delete[] p.obj1;
        delete[] p.window;
    }
 }

void MainWindow::on_btn_dot_matrix_clicked(bool checked){
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
    this->updateImageView(0);
}

void MainWindow::on_btn_convert_reset_clicked(){
    ui->txt_convert_load->setText("");
    ui->txt_convert_save->setText("");
}

void MainWindow::on_btn_source_clicked(){
    QString loadFilename = QFileDialog::getOpenFileName(this, tr("Load Palette"), tr(""));
    ui->txt_convert_load->setText(loadFilename);
    if(loadFilename.endsWith(".csv")) ui->r_csv->click();
    else if(loadFilename.endsWith(".pal") && this->fImporter.isJASCFormat(loadFilename.toStdString())){
        ui->r_jasc->click();
    }
    else if(loadFilename.endsWith(".gpl")) ui->r_gpl->click();
    else if(loadFilename.endsWith(".hex")) ui->r_hex_txt->click();
    if(!loadFilename.isEmpty()){
        int extensionIdx = loadFilename.lastIndexOf(".");
        ui->txt_convert_save->setText(loadFilename.left(extensionIdx) + "-apgb.pal");
    }
}

void MainWindow::on_btn_dest_clicked(){
    QString saveFilename = QFileDialog::getSaveFileName(this, tr("Save Palette"), tr(""));
    ui->txt_convert_save->setText(saveFilename);
}

void MainWindow::on_btn_convert_save_clicked(){
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
            this->fsave.savePalettes(saveFile.toStdString(), p);
            this->on_btn_convert_reset_clicked();
            QMessageBox::information(this, " ", saveFile + " was successfully saved.");
            delete[] p.bg;
            delete[] p.obj0;
            delete[] p.obj1;
            delete[] p.window;
        }
        else if(!saveFile.isEmpty() && !paletteInitialized){
            QMessageBox::critical(this, "Format Error", "There was an error processing the file. Please make sure this file uses a supported file type.");
        }
        else QMessageBox::critical(this, "File Error", "No destination file was given.");
    }
    else QMessageBox::critical(this, "File Error", "No source file was given.");
}

void MainWindow::on_btn_populate_all_clicked(){
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
