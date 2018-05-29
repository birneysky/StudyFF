if [ ! -s fdk_aac ]; then
	curl -X GET https://sourceforge.net/code-snapshots/git/o/op/opencore-amr/fdk-aac.git/opencore-amr-fdk-aac-a30bfced6b6d6d976c728552d247cb30dd86e238.zip \
	-L -o ./opencore-amr-fdk-aac-a30bfced6b6d6d976c728552d247cb30dd86e238.zip
	tar xvf opencore-amr-fdk-aac-a30bfced6b6d6d976c728552d247cb30dd86e238.zip
	mv opencore-amr-fdk-aac-a30bfced6b6d6d976c728552d247cb30dd86e238 fdk_aac
	rm opencore-amr-fdk-aac-a30bfced6b6d6d976c728552d247cb30dd86e238.zip

	curl -X GET https://github.com/applexiaohao/gas-preprocessor/blob/master/gas-preprocessor.pl \
	-L -o ./fdk_aac/gas-preprocessor.pl
fi

cd fdk_aac
./autogen.sh

./configure --enable-static \
--disable-shared \
--host=arm-apple-darwin \
--prefix="`pwd`/../thin/armv7/" \
CC="xcrun -sdk iphoneos clang" \
AS="gas-preprocessof.pl $CC"
CFLAGS="-arch armv7 -fembed-bitcode -miphoneos-version-min=7.0" \
LDFLAGS="-arch armv7 -fembed-bitcode -miphoneos-version-min=7.0"
make clean
make -j8
make install
