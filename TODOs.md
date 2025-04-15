# TODOs
Future work for this project.
## Overview
- [Bug Fixes](#bug-fixes)
- [Features](#features)
- [Optimizations](#optimizations)
- [Misc.](#miscellaneous)

### Bug Fixes
- Fix issue where some previously created APGB PAL files cannot be imported (can occur on some JASC PAL files as well)
- When an incorrect color is given the color picker button still shows the previous color (during import)

### Features
- Make the application scale dynamically (along with the dynamic palette image viewer)
- Implement a dot matrix effect for image for more retro feel

### Optimizations
- Find a more dynamic solution for finding the`image_view/` directory for the dynamic palette image viewer
- Simplify graphical interface logic

### Miscellaneous
- Upload more sample images for testing GB palettes dynamically
- Test application in Windows

# Completed 
The tasks that have been completed for the next release are as follows:
- Added functionality for reversing the order of a singular palette or all palettes
- Added functionality for populating all palettes and their intensities based on the BG palette intensities
- Added support for GIMP GPL v2 files
- Added support for Hex (text) files
- Added disclaimer that the colors used in this tool may not be fully representative of what the Analogue Pocket will show
- Fix color picker resetting to black when user cancels out of image dialog
- Updated image within the dynamic palette image viewer to dynamically scale