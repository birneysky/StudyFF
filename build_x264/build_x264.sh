

GAS_PREPROCESSOR=/usr/local/bin/gas-preprocessor.pl
export CC="xcrun -sdk iphoneos clang -Wno-error=unused-command-line-argument-hard-error-in-future"
export AS="gas-preprocessor.pl -arch arm -- xcrun -sdk iphoneos clang"
CFLAGS="-arch armv7 -mios-version-min=7.0"
LDFLAGS="$CFLAGS"
HOST="arm-apple-darwin"
./configure \
--enable-static \
--enable-pic \
--host= \
--extra-cflags=$CFLAGS \
--extra-ldfalgs=$LDFLAGS \
--prefix="`pwd`/../thin/$ARCH/"

mkdir extras
ln -s $GAS_PREPROCESSOR extras

make clean
make -j3
make install
