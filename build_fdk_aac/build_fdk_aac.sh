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
if [ ! -s configure ]; then
	./autogen.sh
fi

build() 
{
	ARCH=$1
	SDK=$2	
	./configure --enable-static \
	--with-pic=yes \
	--disable-shared \
	--host=arm-apple-darwin \
	--prefix="`pwd`/../thin/$ARCH/" \
	CC="xcrun -sdk $SDK clang" \
	AS="./gas-preprocessof.pl xcrun -sdk $SDK clang" \
	CXX="xcrun -sdk $SDK clang" \
	CPP="xcrun -sdk $SDK clang -E" \
	CFLAGS="-arch $ARCH -fembed-bitcode -miphoneos-version-min=7.0" \
	LDFLAGS="-arch $ARCH -fembed-bitcode -miphoneos-version-min=7.0" \
	CPPFLAGS="-arch $ARCH -fembed-bitcode -miphoneos-version-min=7.0"

	make clean
	make -j8
	make install
}

build "arm64" "iphoneos"
build "x86_64" "iphonesimulator"

if [ ! -s ../../vendor/fdk-aac/lib ]; then
	mkdir ../../vendor/fdk-aac/lib
fi

lipo -create \
	../thin/arm64/lib/libfdk-aac.a \
	../thin/x86_64/lib/libfdk-aac.a \
-output ../../vendor/fdk-aac/lib/libfdk-aac.a


mv  ../thin/arm64/include ../../vendor/fdk-aac/include
lipo -info ../../vendor/fdk-aac/lib/libfdk-aac.a
rm -rf ../thin
rm -rf ../fdk_aac
