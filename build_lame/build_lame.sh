if [ ! -s lame-3.99.5 ]; then
      	curl -X GET https://sourceforge.net/projects/lame/files/lame/3.99/lame-3.99.5.tar.gz \
	-L -o ./lame-3.99.5.tar.gz
	tar xvf lame-3.99.5.tar.gz
	rm  lame-3.99.5.tar.gz
fi
cd lame-3.99.5
build()
{
	ARCH=$1
	SDK=$2
	./configure --disable-shared \
	--disable-frontend \
	--host=arm-apple-darwin \
	--prefix="`pwd`/../thin/${ARCH}/" \
	CC="xcrun -sdk ${SDK} clang -arch ${ARCH}" \
	CFLAGS="-arch ${ARCH} -fembed-bitcode -miphoneos-version-min=7.0" \
	LDFLAGS="-arch ${ARCH} -fembed-bitcode -miphoneos-version-min=7.0"
	make clean
	make -j8
	make install
}
# build "armv7" "iphoneos"
build "arm64" "iphoneos"
build "x86_64" "iphonesimulator"

if [ ! -s ../../vendor/lame/lib ]; then
	mkdir ../../vendor/lame/lib
fi

lipo -create \
	../thin/arm64/lib/libmp3lame.a \
	../thin/x86_64/lib/libmp3lame.a \
-output ../../vendor/lame/lib/libmp3lame.a


mv  ../thin/arm64/include ../../vendor/lame/include
mv  ../thin/arm64/share ../../vendor/lame/share
lipo -info ../../vendor/lame/lib/libmp3lame.a
rm -rf ../thin
rm -rf ../lame-3.99.5
