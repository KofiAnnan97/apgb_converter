*** **Disclaimer: The colors from this tool will be close but not fully representative of what the Analogue Pocket will show.** ***

# APGB Converter
A tool designed to provide a convenient way of creating custom GB palettes for the Analogue Pocket.

## Overview
- [Quick Start](#quick-start)
- [Functionality](#functionality)
- [Format](#format)
- [Resources](#resources)

## Quick Start
### Pre-requistes
- C++17 or higher
- QT6 (and supported qmake)

### Build and Run
##### Option 1: QT Creator
1. Open project through interface
2. Build project
3. Navigate to executable from `build/` directory 
4. Run application executable
##### Option 2: Command Line
1. Open the directory for this project.
2. Configure QT and Qmake
    ```
    export QT_VERSION_MAJOR=6
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/Qt/<version>/gcc_64/lib
    export QTDIR=/path/to/Qt/<version>/gcc_64
    ```
3. Build Project
     ```
    qmake -project 
    qmake
    make
    ```
3. Run application executable

## Functionality
- **Editor** := Create a pal file (hex-based) for Analogue Pocket using a graphical interface.
![](/graphics/visual/editor.png)
    - The dynamic palette image window provides a preview or visual representation of how each palette and their intensities affect the overall look.
    - If you would like to include more test images please use this [script](https://github.com/KofiAnnan97/quick_scripts/tree/master/Recreation/limited_color_img_format) to encode your image into a suitable format (with the `.lci` extension). 

- **Import** := Found in the Editor tab. Allows for the conversion of data into the appropriate format and provides the ability to update the colors via the graphical interface. 
    - `Supported file types`: **CSV**, **PAL** (APGB and JASC format), **GPL**, **HEX** (text-based)
    - CSV file structure:
       ```
       BG, 0xE2D9E4, 0x6C9A9A, 0x52675D, 0x37403B
       OBJ0, 0xF3C893, 0xE5987D, 0xCB5E5C, 0x72334C
       OBJ1, 0xC0A5A9, 0xBF7D85, 0x804D53, 0x403038
       WINDOW, 0x7C8FB2, 0x$C5274, 0x2E334D, 0x1F2025
       ```
       - The order is not strict as long as the keyword is first
       - 24-bit colors can have a leading `0x`, `#`, or nothing
    
    - JASC & GPL format:  The color palettes are applied in a sequential manner for each palette and its respective intentisities as shown [here](#example).
        - only supports 4, 8, 12, or 16 colors
    - HEX (text-based) file structure
        ```
        000000 
        52528C 
        8C8CDE 
        FFFFFF
        ```
        - Do not include `#` or `Ox` 
        - Seperate each color by a new line
        - only supports 4, 8, 12, or 16 colors

- **Convert** := For simple conversions that do not require changes to the colors used.
![](/graphics/visual/convert.png)

## Format
- 56 bytes
- 4 total palettes
    - `BG` := background & tile layers
    - `OBJ0` := used on most sprites
    - `OBJ1` := some games use this for certain sprities
    - `Window` := static elements (i.e. status bar); normally the same as te BG palette
- each palette has 4 intensities (from dark to bright)
- each intensity is a 24-bit color
- Footer: character `0x81` + ASCII for `APGB`
    #### Example
    ```
    000000 52528C 8C8CDE FFFFFF   ; BG
    000000 944A4A FF8484 FFFFFF   ; OBJ0
    000000 843100 FFAD63 FFFFFF   ; OBJ1
    000000 555555 AAAAAA FFFFFF   ; Window
    FFFFFF 81 41504742            ; LCDoff and footer
    ```

## Resources
- [Analogue - Custom Palettes](https://www.analogue.co/developer/docs/custom-palettes)
- [JASC Palette Format](https://liero.nl/lierohack/docformats/other-jasc.html)
- [GIMP Palette Format v2](https://developer.gimp.org/core/standards/gpl/)
