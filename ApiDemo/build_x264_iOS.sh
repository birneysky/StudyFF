#!/usr/bin/env bash

export TOP_DIR=`pwd`

# export AS="./gas-preprocessor.pl  xcrun -sdk iphoneos clang"
# cp gas-preprocessor.pl ./x264
# cd x264

export DEPLOYMENT_TARGET="10.0"

build() {
	ARCH=$1
	SDK=$2
    HOST=$3
    PRIFIX="$TOP_DIR/install/x264/iOS/$ARCH"
    mkdir -p $PRIFIX

    export CC="xcrun -sdk $SDK clang"

    $TOP_DIR/extra/x264/configure \
    --prefix="$PRIFIX" \
    --enable-static \
    --disable-cli \
    --disable-opencl \
    --host=$HOST \
    --disable-asm \
    --extra-cflags="-arch $ARCH -miphoneos-version-min=10.0" \
    --extra-asflags="-arch $ARCH -miphoneos-version-min=10.0" \
    --extra-ldflags="-arch $ARCH -miphoneos-version-min=10.0" \

    make clean
    make -j8
    #make install -lib-static prefix="$PRIFIX"
    mkdir -p ${PRIFIX}/lib
    mkdir -p ${PRIFIX}/include
    mkdir -p ${PRIFIX}/lib/pkgconfig
    cp ./libx264.a ${PRIFIX}/lib
    cp $TOP_DIR/extra/x264/x264.h ${PRIFIX}/include/
    cp ./x264_config.h ${PRIFIX}/include/
    cp ./x264.pc ${PRIFIX}/lib/pkgconfig
}


 


mkdir build
cd build
build "arm64" "iphoneos" "arm-apple-darwin"
build "x86_64" "iphonesimulator" "x86_64-apple-darwin"
mkdir -p $TOP_DIR/install/x264/iOS/fat/lib/
lipo -create $TOP_DIR/install/x264/iOS/x86_64/lib/libx264.a $TOP_DIR/install/x264/iOS/arm64/lib/libx264.a -output $TOP_DIR/install/x264/iOS/fat/lib/libx264.a
cp -rf $TOP_DIR/install/x264/iOS/arm64/include $TOP_DIR/install/x264/iOS/fat/
cd -
rm -rf build