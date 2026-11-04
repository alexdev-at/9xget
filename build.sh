#!/bin/sh
if [ -z "$WATCOM" ] && [ -f /opt/watcom/owsetenv.sh ]; then
    . /opt/watcom/owsetenv.sh
fi
wmake -h "$@"

