
#!/usr/bin/env bash
mkdir -p $TOP_DIR/frameworks

cd $TOP_DIR/frameworks

cmake $TOP_DIR/platform/iOS/
make -j8
cd -