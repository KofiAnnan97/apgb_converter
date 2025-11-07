#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QColor>

using namespace std;

class HexColors{
public:
    inline static const QString BLACK = "#000000";
    inline static const QString DARK_GRAY = "#1a1a1a";
    inline static const QString LIGHT_GRAY = "#828483";
    inline static const QString WHITE = "#FFFFFF";
};

class Color{
public:
    inline static const QColor EMPTY_COLOR = Qt::black;
};

class APGBKeywords{
public:
    inline static const string BG = "BG";
    inline static const string OBJ0 = "OBJ0";
    inline static const string OBJ1 = "OBJ1";
    inline static const string WINDOW = "WINDOW";
    inline static const string bg = "bg";
    inline static const string obj0 = "obj0";
    inline static const string obj1 = "obj1";
    inline static const string window = "window";
    inline static const string FOOTER = "8141504742";
    inline static const string LCDOFF = "FFFFFF";
};

class ImportTypes{
public:
    inline static const QString CSV_ID = "CSV";
    inline static const QString JASC_ID = "PAL (JASC)";
    inline static const QString GPL_ID = "GIMP GPL V2";
    inline static const QString HEX_ID = "HEX (Text)";
};

#endif // CONSTANTS_H
