#include <fstream>
#include <sstream>

#include "fileimporter.h"

FileImporter::FileImporter() {
    black = "#000000";
    dark = "#1a1a1a";
    light = "#828483";
    white = "#FFFFFF";
}

bool FileImporter::isKeyword(string word){
    vector<string> keywords = {"BG", "OBJ0", "OBJ1", "WINDOW", "bg", "obj0", "obj1", "window"};
    for(int i = 0; i < keywords.size(); i++){
        if(word == keywords[i]) return true;
    }
    return false;
}

string FileImporter::trim(string data){
    int startIdx = 0;
    int endIdx = data.length()-1;
    while(data[startIdx] == ' ') startIdx++;
    while(data[endIdx] == ' ') endIdx--;
    return data.substr(startIdx, endIdx-startIdx+1);
}

string FileImporter::fromHexToStr(string hexStr){
    char hexIdx[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                       'A', 'B', 'C', 'D', 'E', 'F'};
    int hexVals[2];
    string str;
    for (size_t i = 0; i < hexStr.length(); i += 1) {
        string byte = hexStr.substr(i, 1);
        unsigned temp = (unsigned char)byte.c_str()[0];
        hexVals[1] = temp%16;
        temp /= 16;
        hexVals[0] = temp%16;
        str.push_back(hexIdx[hexVals[0]]);
        str.push_back(hexIdx[hexVals[1]]);
    }
    return str;
}

bool FileImporter::isJASCFormat(string filename){
    fstream pal;
    string jascId;
    bool isFormat = false;
    pal.open(filename, ios::binary | ios::in);
    if(pal.is_open()){
        getline(pal, jascId);
        jascId.erase(jascId.size()-1, 1);
        if(jascId == "JASC-PAL") isFormat = true;
    }
    pal.close();
    return isFormat;
}

bool FileImporter::isAPGBFormat(string filename){
    fstream pal;
    string line;
    bool isFormat = false;
    pal.open(filename, ios::binary | ios::in);
    if(pal.is_open()){
        getline(pal, line);
        if(strcmp(line.substr(52, 4).c_str(),"APGB") == 0) isFormat = true;
    }
    pal.close();
    return isFormat;
}

APGB_Palette FileImporter::assignPalettesByColorNum(vector<QString> colorVals){
    APGB_Palette temp;
    temp.bg = new string[4];
    temp.obj0 = new string[4];
    temp.obj1 = new string[4];
    temp.window = new string[4];
    if(colorVals.size() == 4){
        temp.bg[0] = colorVals[0].toStdString();
        temp.bg[1] = colorVals[1].toStdString();
        temp.bg[2] = colorVals[2].toStdString();
        temp.bg[3] = colorVals[3].toStdString();
        temp.obj0[0] = colorVals[0].toStdString();
        temp.obj0[1] = colorVals[1].toStdString();
        temp.obj0[2] = colorVals[2].toStdString();
        temp.obj0[3] = colorVals[3].toStdString();
        temp.obj1[0] = colorVals[0].toStdString();
        temp.obj1[1] = colorVals[1].toStdString();
        temp.obj1[2] = colorVals[2].toStdString();
        temp.obj1[3] = colorVals[3].toStdString();
        temp.window[0] = colorVals[0].toStdString();
        temp.window[1] = colorVals[1].toStdString();
        temp.window[2] = colorVals[2].toStdString();
        temp.window[3] = colorVals[3].toStdString();
    }
    else if(colorVals.size() == 8){
        temp.bg[0] = colorVals[0].toStdString();
        temp.bg[1] = colorVals[1].toStdString();
        temp.bg[2] = colorVals[2].toStdString();
        temp.bg[3] = colorVals[3].toStdString();
        temp.obj0[0] = colorVals[4].toStdString();
        temp.obj0[1] = colorVals[5].toStdString();
        temp.obj0[2] = colorVals[6].toStdString();
        temp.obj0[3] = colorVals[7].toStdString();
        temp.obj1[0] = colorVals[4].toStdString();
        temp.obj1[1] = colorVals[5].toStdString();
        temp.obj1[2] = colorVals[6].toStdString();
        temp.obj1[3] = colorVals[7].toStdString();
        temp.window[0] = colorVals[0].toStdString();
        temp.window[1] = colorVals[1].toStdString();
        temp.window[2] = colorVals[2].toStdString();
        temp.window[3] = colorVals[3].toStdString();
    }
    else if(colorVals.size() == 12){
        temp.bg[0] = colorVals[0].toStdString();
        temp.bg[1] = colorVals[1].toStdString();
        temp.bg[2] = colorVals[2].toStdString();
        temp.bg[3] = colorVals[3].toStdString();
        temp.obj0[0] = colorVals[4].toStdString();
        temp.obj0[1] = colorVals[5].toStdString();
        temp.obj0[2] = colorVals[6].toStdString();
        temp.obj0[3] = colorVals[7].toStdString();
        temp.obj1[0] = colorVals[8].toStdString();
        temp.obj1[1] = colorVals[9].toStdString();
        temp.obj1[2] = colorVals[10].toStdString();
        temp.obj1[3] = colorVals[11].toStdString();
        temp.window[0] = colorVals[0].toStdString();
        temp.window[1] = colorVals[1].toStdString();
        temp.window[2] = colorVals[2].toStdString();
        temp.window[3] = colorVals[3].toStdString();
    }
    else if(colorVals.size() == 16){
        temp.bg[0] = colorVals[0].toStdString();
        temp.bg[1] = colorVals[1].toStdString();
        temp.bg[2] = colorVals[2].toStdString();
        temp.bg[3] = colorVals[3].toStdString();
        temp.obj0[0] = colorVals[4].toStdString();
        temp.obj0[1] = colorVals[5].toStdString();
        temp.obj0[2] = colorVals[6].toStdString();
        temp.obj0[3] = colorVals[7].toStdString();
        temp.obj1[0] = colorVals[8].toStdString();
        temp.obj1[1] = colorVals[9].toStdString();
        temp.obj1[2] = colorVals[10].toStdString();
        temp.obj1[3] = colorVals[11].toStdString();
        temp.window[0] = colorVals[12].toStdString();
        temp.window[1] = colorVals[13].toStdString();
        temp.window[2] = colorVals[14].toStdString();
        temp.window[3] = colorVals[15].toStdString();
    }
    return temp;
}


APGB_Palette FileImporter::importPalettesFromCSV(string filename){
    APGB_Palette ap;
    ap.bg = new string[4];
    ap.obj0 = new string[4];
    ap.obj1 = new string[4];
    ap.window = new string[4];
    fstream csv;
    csv.open(filename, ios::in);
    if(csv.is_open()){
        vector<string> row;
        string line, word, temp;
        while(getline(csv, line)){
            row.clear();
            stringstream s(line);
            while (getline(s, word, ',')){
                string trimmedWord = this->trim(word);
                if(!this->isKeyword(trimmedWord) && trimmedWord.substr(0, 2) == "0x"){
                    trimmedWord.erase(0, 2);
                    trimmedWord = "#" + trimmedWord;
                }
                else if(!this->isKeyword(trimmedWord) && trimmedWord[0] != '#'){
                    trimmedWord = "#" + trimmedWord;
                }
                row.push_back(trimmedWord);
            }
            string *pt;
            auto pType = row[0].c_str();
            if(strcmp(pType , "BG") == 0 || strcmp(pType ,"bg") == 0)              pt = ap.bg;
            else if(strcmp(pType , "OBJ0") == 0 || strcmp(pType ,"obj0") == 0)     pt = ap.obj0;
            else if(strcmp(pType , "OBJ1") == 0 || strcmp(pType ,"obj1") == 0)     pt = ap.obj1;
            else if(strcmp(pType , "WINDOW") == 0 || strcmp(pType ,"window") == 0) pt = ap.window;
            pt[0] = row[1];
            pt[1] = row[2];
            pt[2] = row[3];
            pt[3] = row[4];
        }
    }
    return ap;
}

APGB_Palette FileImporter::importPalettesFromAPGB(string filename){
    APGB_Palette ap;
    ap.bg = new string[4];
    ap.obj0 = new string[4];
    ap.obj1 = new string[4];
    ap.window = new string[4];
    ifstream pal;
    pal.open(filename, ios::binary | ios::in);
    if(pal.is_open()){
        string line, word, temp;
        while(getline(pal, line)){
            stringstream ss;
            ss << std::hex << temp;
            ss >> line;
            if(line.size() == 56){
                line.erase(48,8);
                ap.bg[0] = "#"+this->fromHexToStr(line.substr(0, 3));
                ap.bg[1] = "#"+this->fromHexToStr(line.substr(3, 3));
                ap.bg[2] = "#"+this->fromHexToStr(line.substr(6, 3));
                ap.bg[3] = "#"+this->fromHexToStr(line.substr(9, 3));
                ap.obj0[0] = "#"+this->fromHexToStr(line.substr(12,3));
                ap.obj0[1] = "#"+this->fromHexToStr(line.substr(15,3));
                ap.obj0[2] = "#"+this->fromHexToStr(line.substr(18,3));
                ap.obj0[3] = "#"+this->fromHexToStr(line.substr(21,3));
                ap.obj1[0] = "#"+this->fromHexToStr(line.substr(24,3));
                ap.obj1[1] = "#"+this->fromHexToStr(line.substr(27,3));
                ap.obj1[2] = "#"+this->fromHexToStr(line.substr(30,3));
                ap.obj1[3] = "#"+this->fromHexToStr(line.substr(33,3));
                ap.window[0] = "#"+this->fromHexToStr(line.substr(36,3));
                ap.window[1] = "#"+this->fromHexToStr(line.substr(39,3));
                ap.window[2] = "#"+this->fromHexToStr(line.substr(42,3));
                ap.window[3] = "#"+this->fromHexToStr(line.substr(45,3));
                break;
            }
        }
        pal.close();
    }
    return ap;
}

APGB_Palette FileImporter::importPalettesJASC(string filename){
    APGB_Palette ap;
    vector<QString> colorVals;
    ifstream jasc;
    string jascId, jascVersion,line;
    unsigned long numOfColors;
    jasc.open(filename);
    if(jasc.is_open()){
        getline(jasc, jascId);
        getline(jasc, jascVersion);
        getline(jasc, line);
        numOfColors = stoi(line);
        while(getline(jasc, line)){
            line.erase(line.size()-1, 1);
            stringstream ss(line);
            string val;
            getline(ss, val, ' ');
            int r = stoi(val);
            getline(ss, val, ' ');
            int g = stoi(val);
            getline(ss, val, ' ');
            int b = stoi(val);
            colorVals.push_back(QColor(r, g, b).name());
        }
        if(numOfColors == colorVals.size() &&
            (numOfColors == 4 || numOfColors == 8 || numOfColors == 12 || numOfColors == 16)){
            ap = this->assignPalettesByColorNum(colorVals);
        }
        jasc.close();
    }
    //else qDebug() << "Failed to find file: " << filename;
    return ap;
}

APGB_Palette FileImporter::importPalettesGPLv2(string filename){
    APGB_Palette ap;
    vector<QString> colorVals;
    ifstream gpl;
    string line, id, name, description;
    int numOfColors;
    gpl.open(filename);
    if(gpl.is_open()){
        getline(gpl, line);
        id = line;
        getline(gpl, line);
        name = line.erase(0, 14);
        while(getline(gpl, line)){
            string colorStr = line.substr(1, 6);
            if(strcmp(colorStr.c_str(), "Colors") == 0) break;
            else description += line;
        }
        numOfColors = stoi(line.erase(0, 8));
        while(getline(gpl, line)){
            stringstream ss(line);
            //qDebug() << line;
            string val;
            getline(ss, val, '\t');
            int r = stoi(val);
            getline(ss, val, '\t');
            int g = stoi(val);
            getline(ss, val, '\t');
            int b = stoi(val);
            //qDebug() << "RGB: " << r << ", " << g << ", " << b;
            colorVals.push_back(QColor(r, g, b).name());
        }
        if(numOfColors == colorVals.size() &&
          (numOfColors == 4 || numOfColors == 8 || numOfColors == 12 || numOfColors == 16)){
            ap = this->assignPalettesByColorNum(colorVals);
        }
        gpl.close();
    }
    return ap;
}

APGB_Palette FileImporter::importPalettesHEXTxt(string filename){
    APGB_Palette ap;
    vector<QString> colorVals;
    ifstream hext;
    string line;
    hext.open(filename);
    if(hext.is_open()){
        while(getline(hext, line)){
            if(line.size() > 6) line = line.erase(6, line.size()-6);
            QString colorName = QString::fromStdString('#'+line);
            if(QColor(colorName).isValid()) colorVals.push_back(colorName);
            else colorVals.push_back("");
        }
        int numOfColors = colorVals.size();
        if(numOfColors == 4 || numOfColors == 8 || numOfColors == 12 || numOfColors == 16){
            ap = this->assignPalettesByColorNum(colorVals);
        }
        hext.close();
    }
    return ap;
}
