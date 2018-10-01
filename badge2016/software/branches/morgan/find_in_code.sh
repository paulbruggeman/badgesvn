#!/usr/bin/env dash

find src \( -name '*.h' -o -name '*.c' \) -print0 | xargs -0 grep "$@"
