#!/usr/bin/env dash
cd ~/Devel/badge2016/branches/lloyd/src
exec find . -name .svn -prune -o -name docs -prune -o -type f -print0 | xargs -0 -I'{}' diff -q -w "{}" ../../../trunk/src/"{}"
