#!/bin/bash
PROGRAMMER=./pic32usb_x86
HEX_PATH=../../Badge15.X/dist/default/production/Badge15.X.production.hex

if [ "$#" -eq 0 ]
then
    echo "Using DEFAULT hex: \"$HEX_PATH\""
else
    echo "Using PROVIDED hex: \"$1\""
    HEX_PATH=$1
fi

echo ""
 
# If the HEX path doesn't exist, throw out some help and exit in error
if [ ! -e $HEX_PATH ] 
then
    echo "The .hex file provided doesn't exists:"
    echo $HEX_PATH

    # Trying to use default hex (no args)?
    if [ "$#" -eq 0 ] 
    then
        echo ""
        echo "You are trying to flash from the default location..."
        echo -e "\t->Have you compiled the firmware?"
        echo -e "\t->Is the hex not in the default location?"
    fi
    exit 1
fi

# Flash!
$PROGRAMMER $HEX_PATH
