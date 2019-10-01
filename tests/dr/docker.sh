#!/usr/bin/env sh

set -e

winpty docker run --rm -it \
  -v /$(pwd)/../..://wrk \
  -w //wrk/tests/dr \
  aptman/dbhi:bionic-dr bash -c "$(cat test.sh)"

#  $(command -v winpty) docker run --rm  -itu "$(id -u)" \
#       -e DBHI_CASE="$DBHI_CASE" \
#       -e DBHI_ORIG="$DBHI_ORIG" \
#       -e DBHI_UUT="$DBHI_UUT" \
#       -e DBHI_FUNC="$DBHI_FUNC" \
#       -e DBHI_ARGD="$DBHI_ARGD" \
#       -e DBHI_DRARGS="$DBHI_DRARGS" \
#       -e DBHI_HAND="$DBHI_HAND" \
#       -e BUILD_DIR="//src/examples/$DBHI_BASE/build" \
#       -v "/$(pwd)/..://src" \
#       -w "//src/examples/$DBHI_BASE/build" \
#       "aptman/dbhi:bionic-$1" \
#       sh -c "../../run b_$1"