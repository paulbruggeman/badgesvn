#!/usr/bin/env dash
cd ${HOME}/Devel/badge2016/software/branches/lloyd

find src \( -name '*.h' -o -name '*.c' \) -print0 | xargs -0 grep "$@"
