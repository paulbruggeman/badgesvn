#!/usr/bin/env sh

if [ $# -eq 0 ]; then
    echo "Usage: $(basename $0) branch_name [which_binary]" >&2
    echo '   branch_name -- "trunk" or the name of a branch directory.' >&2
    echo '   which_ binary -- 2 to select alternate hex file (with diff. badge ID).' >&2
    echo '' >&2
    echo 'branches found:' >&2
    for BRANCH in ${HOME}/Devel/badge2016/software/branches/*
    do
        if [ -d ${BRANCH} ]; then
            echo -n "$(basename ${BRANCH}) " >&2
        fi
    done
    echo >&2
    exit 1
fi

BADGEDIR=${HOME}/Devel/badge2016
PICEXE=${BADGEDIR}/pic32usb1388_v1.0/pic32usb1388

# change TOPDIR to reflect your local setup
if [ "$1" = "trunk" ]; then
    TOPDIR=${BADGEDIR}/software/trunk
else
    TOPDIR=${BADGEDIR}/software/branches/$1
fi

if [ -z $2 ]; then
    BINARY=${TOPDIR}/Badge16.X/dist/default/production/Badge16.X.production.hex
else
    BINARY=${TOPDIR}/Badge16.X/dist/default/production/Badge16.X.production.hex.2
fi

echo $(basename ${PICEXE}) $(basename ${BINARY})

${PICEXE} ${BINARY}
