#!/usr/bin/env sh

# change TOPDIR to reflect your local setup
TOPDIR=${HOME}/Devel/badge2016/software/trunk
BINARY=${TOPDIR}/Badge16.X/dist/default/production/Badge16.X.production.hex

${TOPDIR}/pic32usb1388_v1.0/pic32usb1388 ${BINARY}
