export CC="xcrun -sdk iphoneos clang -Wno-error=unused-command-line-argument-hard-error-in-future"
export AS="./gas-preprocessor.pl  xcrun -sdk iphoneos clang"
cp gas-preprocessor.pl ./x264
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
