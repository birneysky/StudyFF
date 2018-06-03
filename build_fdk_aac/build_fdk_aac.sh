if [ ! -s fdk_aac ]; then
	curl -X GET https://jaist.dl.sourceforge.net/project/opencore-amr/fdk-aac/fdk-aac-0.1.6.tar.gz \
	-L -o ./fdk-aac-0.1.6.tar.gz
	tar xvf fdk-aac-0.1.6.tar.gz
	mv fdk-aac-0.1.6 fdk_aac
	rm fdk-aac-0.1.6.tar.gz

	curl -X GET https://github.com/applexiaohao/gas-preprocessor/blob/master/gas-preprocessor.pl \
	-L -o ./fdk_aac/gas-preprocessor.pl
fi

cd fdk_aac
./autogen.sh

./configure --enable-static \
--with-pic=yes \
--disable-shared \
--host=arm-apple-darwin \
--prefix="`pwd`/../thin/armv7/" \
CC="xcrun -sdk iphoneos clang" \
AS="./gas-preprocessof.pl $CC" \
CXX="$CC" \
CPP="/usr/bin/cpp" \
CFLAGS="-arch armv7 -fembed-bitcode -miphoneos-version-min=7.0" \
LDFLAGS="-arch armv7 -fembed-bitcode -miphoneos-version-min=7.0" \
CPPFLAGS="$CFLAGS"
make clean
make -j8
make install

