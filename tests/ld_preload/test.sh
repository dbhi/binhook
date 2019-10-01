#!/usr/bin/env sh

set -e

cd $(dirname $0)

. ../testenv

mkdir -p build
cd build

SRCS="../../../src/"

set +e
rm *.so *.o *.log > /dev/null
set -e

cmd_gcc () {
  gstart "· Build $1"
  gcc -Os -s -Wall -o "$@"
  gend
}

for i in func repl; do
  cmd_gcc "${i}.o" -c "${SRCS}${i}.c"
  cmd_gcc "lib${i}.so" -shared "${SRCS}${i}.c"
done

cmd_gcc app func.o "$SRCS"app.c
cmd_gcc app_lib "$SRCS"app.c -L$(pwd) -lfunc
cmd_gcc patched_app repl.o "$SRCS"app.c

gstart "· Run app (without LD_PRELOAD)"
# output: default
./app | tee app.log
gend

cmd_preload () {
  LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" LD_PRELOAD="$(pwd)/librepl.so" "$@"
}

gstart "· Run app (with LD_PRELOAD)"
# output: default
cmd_preload ./app | tee app_pre.log
gend

gstart "· Run patched_app"
# output: replacement
./patched_app | tee patched_app.log
gend

gstart "· Run app_lib (without LD_PRELOAD)"
# output: default
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./app_lib | tee app_lib.log
gend

gstart "· Run app_lib (with LD_PRELOAD)"
# output: replacement
cmd_preload ./app_lib | tee app_lib_pre.log
gend
