#!/bin/sh

rm -f aclocal.m4

aclocal && \
libtoolize && \
autoconf && \
autoheader && \
automake --add-missing
