#!/usr/bin/env dash
SRC_DIR=~/Devel/badge2016/software/branches/lloyd/src
TRUNK_DIR=~/Devel/badge2016/software/trunk/src

if [ -d ${SRC_DIR} ]; then
    cd ${SRC_DIR}
    exec find . -name .svn -prune -o -name docs -prune -o -type f -print0 | xargs -0 -I'{}' diff -q -w "{}" ${TRUNK_DIR}/"{}"
else
    echo "Can't find directory ${SRC_DIR}." >&2
fi

