#!/usr/bin/env bash

export DEPLOYMENT_TARGET="13.0"
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$TOP_DIR/extra/av1/iOS/fat/lib/pkgconfig/:$TOP_DIR/instll/x264/iOS/fat/lib/pkgconfig/



function create_x264_pkg_config_file() {
mkdir -p $TOP_DIR/install/x264/iOS/fat/lib/pkgconfig
rm -f $TOP_DIR/install/x264/iOS/fat/lib/pkgconfig/x264.pc
#staring writing file
echo "prefix=$TOP_DIR/install/x264/iOS/fat/
exec_prefix=\${prefix}
libdir=\${prefix}/lib
includedir=\${prefix}/include

Name: x264
Description: H.264 (MPEG4 AVC) encoder library
Version: 0.164.x
Libs: -L\${libdir} -lx264 -lpthread -lm
Libs.private: 
Cflags: -I\${includedir}
" >> $TOP_DIR/install/x264/iOS/fat/lib/pkgconfig/x264.pc
echo "created x264.pc at $TOP_DIR/install/x264/iOS/fat/lib/pkgconfig/x264.pc"
}

function create_av1d_pkg_config_file() {
rm -f $TOP_DIR/extra/av1/iOS/fat/lib/pkgconfig/av1d.pc
#staring writing file
echo "prefix=$TOP_DIR/extra/av1/iOS/fat
exec_prefix=\${prefix}
libdir=\${prefix}/lib
includedir=\${prefix}/include

Name: libav1d
Description: TXAV1 decoding library
Version: 1.2.0
Libs: -L\${libdir} -lav1d
Cflags: -I\${includedir}
" >> $TOP_DIR/extra/av1/iOS/fat/lib/pkgconfig/av1d.pc
echo "created av1d.pc at $TOP_DIR/extra/av1/iOS/fat/lib/pkgconfig/av1d.pc"
}

FFMPEG_CONFIGURE_FLAGS="$FFMPEG_CONFIGURE_FLAGS --enable-libx264 --enable-gpl --enable-encoder=libx264"
FFMPEG_CONFIGURE_FLAGS="$FFMPEG_CONFIGURE_FLAGS --enable-videotoolbox  --enable-encoder=h264_videotoolbox"
FFMPEG_CONFIGURE_FLAGS="$FFMPEG_CONFIGURE_FLAGS --enable-filters --enable-coreimage --enable-avfoundation"

build() {
	ARCH=$1
	SDK=$2
    PRIFIX="$TOP_DIR/install/ffmpeg/iOS/$ARCH/"
    mkdir -p $PRIFIX
	$TOP_DIR/extra/ffmpeg/configure $FFMPEG_CONFIGURE_FLAGS \
	--target-os=darwin  \
    --pkg-config=pkg-config \
	--arch=$ARCH \
	--cc="xcrun -sdk $SDK clang" \
	--extra-cflags="-arch $ARCH -miphoneos-version-min=$DEPLOYMENT_TARGET -I$TOP_DIR/install/x264/iOS/fat/include" \
	--extra-ldflags="-arch $ARCH -miphoneos-version-min=$DEPLOYMENT_TARGET -L$TOP_DIR/install/x264/iOS/fat/lib -lx264" \
	--prefix="$PRIFIX"
    #--docdir="$TOP_DIR/install/ffmpeg/" --datadir="$TOP_DIR/install/ffmpeg/"
    #--objdir="$PRIFIX"
    #-I$TOP_DIR/extra/av1/iOS/fat/include
    #-L$TOP_DIR/extra/av1/iOS/fat/lib -lav1d
    #-fembed-bitcode
    #-fembed-bitcode

	make clean
	make -j8
    make install

}

removeFiles() {
    rm -rf config.h
    rm -rf config.asm
    rm -rf doc
    rm -rf libavcodec
    rm -rf libavdevice
    rm -rf libavfilter
    rm -rf libavformat
    rm -rf libavresample
    rm -rf libavutil
    rm -rf libswresample
    rm -rf libswscale
    rm -rf Makefile
    rm -rf src
    rm -rf tests
    rm -rf .version
}

function build_fat_lib(){
    echo $1
    if [[ "$1" == "x264" ]];then
        return
    fi
    local lib_names=
    local searchLibs=$(find install/$1/iOS -name *.a)
    for searchLib in ${searchLibs}
    do
        lib=$(basename $searchLib)
        [[ $lib_names =~ $lib ]] || lib_names="$lib_names $(basename $searchLib)"
    done

    if  [[ ! -n "$lib_names" ]] ;then
        echo "break create $1 fat"
        return
    fi
    echo "lib_names: $lib_names"

    local fat_dir=install/$1/iOS/fat/lib
    rm -rf ${fat_dir}
    mkdir -p  ${fat_dir}
    for libname in ${lib_names}
    do
         libs=$(find install/$1/iOS -name ${libname})
         lipo -create ${libs}  -output install/$1/iOS/fat/lib/${libname}
    done
}


function build_fat_libs(){
    lib_names=$(cd $TOP_DIR/install; ls)
    for libname in ${lib_names}
    do
        build_fat_lib ${libname}
    done
}


create_x264_pkg_config_file
#create_av1d_pkg_config_file

#echo $PKG_CONFIG_PATH
#pkg-config --libs av1d

mkdir build
cd build
build "arm64" "iphoneos"
build "x86_64" "iphonesimulator"

cd -
rm -rf build

echo "building fat iOS binaries..."
mkdir -p $TOP_DIR/install/ffmpeg/iOS/fat/lib/

build_fat_libs

cp -rf $TOP_DIR/install/ffmpeg/iOS/arm64/include $TOP_DIR/install/ffmpeg/iOS/fat/
cp -rf $TOP_DIR/install/x264/iOS/arm64/include $TOP_DIR/install/x264/iOS/fat/
echo "FFmpeg build is complete"

