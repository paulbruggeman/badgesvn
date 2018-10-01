#!/usr/bin/env dash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) branch_name"
    echo "     Lists differences between lloyd branch and another."
    exit 1
fi

BRANCH_DIR=~/Devel/badge2016/software/branches/$1/src

if [ ! -d ${BRANCH_DIR} ]; then
    echo "$1 branch directory not found (${BRANCH_DIR})."
    exit 2
fi

SRC_DIR=~/Devel/badge2016/software/branches/lloyd/src

if [ -d ${SRC_DIR} ]; then
    cd ${SRC_DIR}
    exec find . -name .svn -prune -o -name docs -prune -o -type f -print0 | xargs -0 -I'{}' diff -q -w "{}" ${BRANCH_DIR}/"{}"
else
    echo "Can't find directory ${SRC_DIR}." >&2
fi

