#!/usr/bin/env bash

source set_android_env.sh

function init() {
  export CC=$CC
  export CXX=$CXX
  export LD=$LD
  export STRINGS=$STRINGS
  export AR=$AR
  export RANLIB=$RANLIB
  export NM=$NM
  CROSS_PREFIX=$CROSS_PREFIX
}

function build_one() {
  echo "target=$TARGET"
  echo "prefix=$PREFIX"
  echo "corss_prefix=$CROSS_PREFIX"
    $TOP_DIR/extra/x264/configure --host=$TARGET \
    --enable-pic \
    --enable-static \
    --extra-cflags="-Os -fpic $CFLAGS" \
    --prefix=$PREFIX \
    --cross-prefix=$CROSS_PREFIX \
    --enable-pic \
    --enable-strip

    make clean
    make -j8
    make install
}

function android_init_env(){
  if [ "$1" == "armeabi-v7a" ]
  then
      init_armev7a $2
  elif [ "$1" == "arm64-v8a" ]
  then
      init_arme64 $2
  else
      echo unspported Android ABI $1
      return
  fi
    init
    create_build_dir $2
    build_one
}

function buildAll() {
    local abis="armeabi-v7a arm64-v8a"
    for abi in ${abis};
    do
        echo "build android ${abi} start"
        cd $TOP_DIR
        android_init_env ${abi} "x264"
    done
}

buildAll

