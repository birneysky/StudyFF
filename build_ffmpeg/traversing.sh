
for LIB in thin/arm64/lib/*.a
do
	FILENAME=${LIB##*/}
	target=`find ./thin -name $FILENAME`
	#echo $target
	lipo -create $target -output ../vendor/ffmpeg/lib/$FILENAME
done
cp -rf thin/arm64/include ../vendor/ffmpeg/include