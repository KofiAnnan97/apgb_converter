#ifndef FILEIMPORTER_H
#define FILEIMPORTER_H

#include <QString>
#include <QDebug>
#include <QColor>
#include <QMessageBox>

#include "palettes.h"

using namespace std;

class FileImporter
{
public:
    FileImporter();

    // Helper functions
    bool isKeyword(string word);
    string trim(string data);
    string fromHexToStr(string hexStr);
    bool isJASCFormat(string filename);
    bool isAPGBFormat(string filename);

    // Import functions
    APGB_Palette importPalettesFromCSV(string filename);
    APGB_Palette importPalettesFromAPGB(string filename);
    APGB_Palette assignPalettesByColorNum(vector<QString> colors);
    APGB_Palette importPalettesJASC(string filename);
    APGB_Palette importPalettesGPLv2(string filename);
    APGB_Palette importPalettesHEXTxt(string filename);


    // Variables
    QString black, dark, light, white;
};

#endif // FILEIMPORTER_H
