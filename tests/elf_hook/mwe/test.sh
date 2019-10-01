#!/usr/bin/env sh

set -e

enable_color() {
  ENABLECOLOR='-c '
  ANSI_RED="\033[31m"
  ANSI_GREEN="\033[32m"
  ANSI_YELLOW="\033[33m"
  ANSI_BLUE="\033[34m"
  ANSI_MAGENTA="\033[35m"
  ANSI_GRAY="\033[90m"
  ANSI_CYAN="\033[36;1m"
  ANSI_DARKCYAN="\033[36m"
  ANSI_NOCOLOR="\033[0m"
}

disable_color() { unset ENABLECOLOR ANSI_RED ANSI_GREEN ANSI_YELLOW ANSI_BLUE ANSI_MAGENTA ANSI_CYAN ANSI_DARKCYAN ANSI_NOCOLOR; }

enable_color

gstart () {
  printf "${ANSI_MAGENTA}${1}$ANSI_NOCOLOR\n"
}

cd $(dirname $0)

mkdir -p build
cd build

gcc -g3 -fPIC -shared -o libfunc.so ../libfunc.c

gstart "· Test A"
gcc -g3 -I../../ELF-Hook -L${PWD} -o testA ../hook.c ../testA.c ../../ELF-Hook/elf_hook.c -lfunc -ldl
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./testA

gstart "· Test B"
gcc -g3 -I../../ELF-Hook -L${PWD} -o testB ../hook.c ../testB.c ../../ELF-Hook/elf_hook.c -lfunc -ldl
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./testB

gstart "· Build app"
gcc -g3 -fPIC -rdynamic -Wall -o app ../libfunc.c ../app.c

gstart "· Test C"
gcc -g3 -I../../ELF-Hook -L${PWD} -o testC ../hook.c ../testC.c ../../ELF-Hook/elf_hook.c -lfunc -ldl
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./testC

gstart "· Test D"
gcc -g3 -I../../ELF-Hook -L${PWD} -o testD ../hook.c ../testD.c ../../ELF-Hook/elf_hook.c -lfunc -ldl
LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" ./testD
