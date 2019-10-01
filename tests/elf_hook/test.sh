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

gstart "· Build libfunc.so"
gcc -Os -s -Wall -shared -o libfunc.so "${SRCS}func.c" ../wrapfunc.c
gend

build_test () {
  gcc -Wall -I../ELF-Hook -L${PWD} -o "$@" ../ELF-Hook/elf_hook.c -lfunc -ldl
}
#-Os -s

gstart "· Build testA"
build_test testA "${SRCS}hook.c" ../testA.c
gend

gstart "· Run testA"
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./testA
gend

gstart "· Build testB"
build_test testB "${SRCS}hook.c" ../testB.c
gend

gstart "· Run testB"
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./testB
gend

gstart "· Build app"
gcc -g3 -fPIC -rdynamic -Wall -o app "$SRCS"func.c "$SRCS"app.c
gend

cp app libfunc.so

gstart "· Build testC"
build_test testC "${SRCS}hook.c" ../testC.c
gend

gstart "· Run testC"
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./testC
gend

gstart "· Build testD"
build_test testD "${SRCS}hook.c" ../testD.c
gend

gstart "· Run testD"
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./testD
gend
