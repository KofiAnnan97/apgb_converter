#!/usr/bin/env bash

# Get script directory
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
echo "Setup script location: $SCRIPT_DIR"

# Create if app bash profile exists
APP_NAME="apgb_converter"
PROFILE_DIR="/etc/profile.d/"
APP_PROFILE=$PROFILE_DIR$APP_NAME.sh
if [ ! -f $APP_PROFILE ]; then
    echo "Creating new app profile: $APP_PROFILE" 
    touch $APP_PROFILE
    echo -e "export APGB_IMG_VIEW_PATH=$SCRIPT_DIR/graphics/image_view" >> $APP_PROFILE
fi

# Check if enivornment variable exists for dynamic image files
if [[ ! -n "$APGB_IMG_VIEW_PATH" ]]; then
echo -e "
# APGB Conveter dynamic image view path
# Make sure any added image uses the limited color format (.lci)" >> ~/.bashrc 
echo "source $APP_PROFILE" >> ~/.bashrc        
echo "Dynamic images location: $APGB_IMG_VIEW_PATH"
else 
    echo "APGB_IMG_VIEW_PATH is set"
fi

# Build project and generate executable
cmake .
make

# Source bashrc
source ~/.bashrc