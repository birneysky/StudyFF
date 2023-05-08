
#!/usr/bin/env bash
mkdir -p $TOP_DIR/platform/iOS/framework

cd $TOP_DIR/platform/iOS/framework

cmake $TOP_DIR/platform/iOS/
make
cd -