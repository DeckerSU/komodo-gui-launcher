#!/bin/bash
rm -rf autom4te.cache config.log config.status config.h.in config.h aclocal.m4
aclocal
autoheader
autoconf
automake --add-missing
./configure
