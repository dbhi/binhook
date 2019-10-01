#!/usr/bin/env sh

set -e

if [ "$DYNAMORIO_HOME" = "" ]; then
  echo "DYNAMORIO_HOME not set."
  exit 1
fi

#plugin="./src/drclient.c"
#sed_defines

mkdir -p build
cd build

SRCS="../../../src/"

#cp "$SRCS"hook.c ./

gcc -Wall -o app "$SRCS"func.c "$SRCS"app.c
#-Os -s

cmake SHOW_RESULTS \
  -L$SRCS \
  -DDynamoRIO_DIR=$DYNAMORIO_HOME/cmake \
  ../

#-DPLUGIN_ARGS="$(get_plugin_args)" ./src

make

"$DYNAMORIO_HOME"/bin64/drrun -c libdrclient.so -- app

bash

#  -Dorig_name="func" \
#  -Dfunc_name="hook" \
#  -Dfunc_argd='void' \
#  -Dfunc_args='void' \


#define func_name {{ .Func }}
#define func_argd {{ .Argd }}
#define func_args {{ .Hand }}