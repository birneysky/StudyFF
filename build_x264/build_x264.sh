if [ ! -s last_x264.tar.bz2 ]; then
	curl -X GET ftp://ftp.videolan.org/pub/x264/snapshots/last_x264.tar.bz2 \
	 -L -o last_x264.tar.bz2

	tar xvf last_x264.tar.bz2
	mv x264-snapshot-20180604-2245 x264
	curl -X GET https://github.com/libav/gas-preprocessor/blob/master/gas-preprocessor.pl \
	-L -o ./x264/gas-preprocessor.pl
fi

export CC="xcrun -sdk iphoneos clang -Wno-error=unused-command-line-argument-hard-error-in-future"
export AS="./gas-preprocessor.pl  xcrun -sdk iphoneos clang"
cd x264
./configure \
--enable-static \
--enable-pic \
--disable-cli \
--host=arm-apple-darwin \
--extra-cflags="-arch armv7 -fembed-bitcode  -mios-version-min=7.0" \
--extra-ldfalgs="-arch armv7 -fembed-bitcode -mios-version-min=7.0" \
--prefix="`pwd`/../thin/$ARCH/"

make clean
make -j8
make install
