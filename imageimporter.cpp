#include <fstream>

#include "imageimporter.h"

ImageImporter::ImageImporter() {
    width  = 160;  // GameBoy screen width:  160
    height = 144;  // GameBoy screen height: 144
    fileIdx = 0;
}

void ImageImporter::setImgDirectory(QString folderPath){
    imgDir = QDir(folderPath);
}

void ImageImporter::changeFile(int direction, QImage *image, vector<QString> palettes){
    QStringList files = imgDir.entryList(QStringList() << "*.lci", QDir::Files);
    fileIdx += direction;
    if(fileIdx < 0) fileIdx = files.count()-1;
    else if(fileIdx > files.count()-1) fileIdx = 0;
    string filename = QDir::cleanPath(imgDir.path() + QDir::separator() + files[fileIdx]).toStdString();
    this->decodeImageTxt(filename, image, palettes);
}

vector<QColor> ImageImporter::setImageColors(vector<QString> hexColorIdx){
    vector<QColor> temp;
    for(int i = 0; i < hexColorIdx.size(); i++){
        QString hexColor = hexColorIdx[i];
        if(QColor(hexColor).isValid()) temp.push_back(QColor(hexColor));
        else temp.push_back(emptyColor);
    }
    return temp;
}

int ImageImporter::getIdxFromHexChar(char hexChar){
    if(hexChar >= '0' && hexChar <= '9') return hexChar - '0';
    else if(hexChar >= 'A' && hexChar <= 'F') return 10 + (hexChar - 'A');
    else if(hexChar >= 'a' && hexChar <= 'f') return 10 + (hexChar - 'a');
    else return -1;
}

void ImageImporter::decodeImageTxt(string filename, QImage *image, vector<QString> palettes){
    image->fill(emptyColor);
    fstream decode;
    int row = 0;
    int col = 0;
    decode.open(filename, ios::in);
    if(decode.is_open()){
        string line, type;
        int width, height, numOfColors;
        getline(decode, type);
        getline(decode, line);
        width = stoi(line);
        this->width = width;
        getline(decode, line);
        height = stoi(line);
        this->height = height;
        getline(decode, line);
        numOfColors = stoi(line);
        vector<QColor> colorIdxs = this->setImageColors(palettes);
        for(int n = 0; n < numOfColors; n++) getline(decode, line);
        while(getline(decode, line)){
            if(image->size().isEmpty()) break;
            else if(row >= this->height) break;
            else if(line.empty()){
                for(int i = 0; i < this->width-1; i++){
                    image->setPixelColor(i, row, emptyColor);
                }
                row++;
            }
            else{
                for(auto c: line){
                    int hexIdx = this->getIdxFromHexChar(c);
                    if(col >= this->width) break;
                    else if(hexIdx != -1){
                        image->setPixelColor(col, row, colorIdxs[hexIdx]);
                    }
                    else{
                        image->setPixelColor(col, row, emptyColor);
                    }
                    col++;
                }
                row++;
                col = 0;
            }
        }
        while(row > 0 && row < this->height){
            qDebug() << "Entered";
            for(int j = 0; j < this->width-1; j++){
                image->setPixelColor(j, row, emptyColor);
            }
            row ++;
        }
    }
    else qDebug() << "Could not find file: " + filename;
}
