
cd ffmpeg

CONFIGURE_FLAGS="--enable-gpl \
--disable-shared \
--disable-stripping \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-avdevice \
--disable-indevs \
--disable-filters \
--disable-devices \
--disable-parsers \
--disable-postproc \
--disable-debug \
--disable-asm \
--disable-x86asm \
--disable-doc \
--disable-bsfs \
--disable-muxers \
--disable-demuxers \
--disable-ffplay \
--disable-ffprobe \
--disable-indevs \
--disable-outdevs \
--enable-cross-compile \
--enable-filters
--enable-filter=aresample \
--enable-bsf=aac_adtstoasc \
--enable-small \
--enable-dct \
--enable-dwt \
--enable-lsp \
--enable-mdct \
--enable-rdft \
--enable-fft \
--enable-static \
--enable-version3 \
--enable-nonfree \
--disable-encoders \
--enable-encoder=pcm_s16le \
--enable-encoder=aac \
--enable-encoder=mp2 \
--disable-decoders \
--enable-decoder=aac \
--enable-decoder=mp3 \
--enable-decoder=h264 \
--enable-decoder=pcm_s16le \
--disable-parsers \
--enable-parser=aac \
--enable-parser=mpeg4video \
--enable-parser=mpegvideo \
--enable-parser=mpegaudio \
--enable-parser=aac \
--disable-muxers \
--enable-muxer=flv \
--enable-muxer=mp4 \
--enable-muxer=wav \
--enable-muxer=adts \
--disable-demuxers \
--enable-demuxer=flv \
--enable-demuxer=mpegvideo \
--enable-demuxer=mpegtsraw \
--enable-demuxer=mpegts \
--enable-demuxer=mpegps \
--enable-demuxer=h264 \
--enable-demuxer=wav \
--enable-demuxer=aac \
--enable-demuxer=hls \
--enable-demuxer=mov \
--enable-demuxer=m4v \
--disable-protocols \
--enable-protocol=rtmp \
--enable-protocol=http \
--enable-protocol=file"


build() {
	ARCH=$1
	SDK=$2
	./configure $CONFIGURE_FLAGS \
	--target-os=darwin  \
	--arch=$ARCH \
	--cc="xcrun -sdk $SDK clang" \
	--extra-cflags="-arch $ARCH -fembed-bitcode -miphoneos-version-min=8.0" \
	--extra-ldflags="-arch $ARCH -fembed-bitcode -miphoneos-version-min=8.0" \
	--prefix="`pwd`/../thin/$ARCH/"

	make clean
	make -j3 install
}


build "arm64" "iphoneos"
build "x86_64" "iphonesimulator"

cd ../
echo "building fat binaries..."
for LIB in thin/arm64/lib/*.a
do
	FILENAME=${LIB##*/}
	target=`find ./thin -name $FILENAME`
	#echo $target
	lipo -create $target -output ../vendor/ffmpeg/lib/$FILENAME
done
cp -rf thin/arm64/include ../vendor/ffmpeg/include
echo done
say "I'm done"
