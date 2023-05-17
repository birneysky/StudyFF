#!/usr/bin/env bash


export TOP_DIR=`pwd`

export FFMPEG_CONFIGURE_FLAGS="--enable-demuxer=flv \
--enable-demuxer=aac \
--enable-demuxer=live_flv \
--enable-demuxer=webvtt \
--enable-demuxer=srt \
--enable-demuxer=mov \
--enable-demuxer=mp3 \
--enable-demuxer=mpegts \
--enable-demuxer=matroska \
--enable-demuxer=h264 \
--enable-demuxer=ac3 \
--enable-demuxer=eac3 \
--enable-demuxer=ass \
--enable-protocol=file \
--enable-protocol=rtmp \
--enable-protocol=udp \
--disable-everything \
--disable-programs \
--disable-doc \
--disable-filters \
--disable-avdevice \
--disable-postproc \
--disable-hwaccels \
--disable-bzlib \
--disable-videotoolbox \
--enable-pic \
--enable-decoder=aac \
--enable-decoder=aac_latm \
--enable-decoder=h264 \
--enable-decoder=hevc \
--enable-decoder=mpeg4 \
--enable-decoder=mp3 \
--enable-decoder=mp3adu \
--enable-decoder=mp3float \
--enable-decoder=mp3on4float \
--enable-decoder=mp3adufloat \
--enable-decoder=mp3on4 \
--enable-decoder=pcm_s16le \
--enable-decoder=ac3_at \
--enable-decoder=eac3_at \
--enable-demuxer=flv \
--enable-demuxer=aac \
--enable-demuxer=live_flv \
--enable-demuxer=webvtt \
--enable-demuxer=srt \
--enable-demuxer=mov \
--enable-demuxer=mp3 \
--enable-demuxer=mpegts \
--enable-demuxer=matroska \
--enable-demuxer=h264 \
--enable-demuxer=ac3 \
--enable-demuxer=eac3 \
--enable-demuxer=ass \
--enable-muxer=mp4 \
--enable-muxer=adts \
--enable-muxer=mpegts \
--enable-muxer=mp4 \
--enable-muxer=adts \
--enable-muxer=mpegts \
--enable-parser=aac \
--enable-parser=h264 \
--enable-parser=hevc \
--enable-parser=aac_latm \
--enable-parser=ac3 \
--enable-protocol=file \
--enable-protocol=rtmp \
--enable-protocol=udp \
--enable-bsf=aac_adtstoasc \
--enable-bsf=h264_mp4toannexb \
--enable-bsf=hevc_mp4toannexb \
--enable-bsf=extract_extradata \
--enable-filter=atempo \
--enable-filter=aresample \
--enable-filter=aformat \
--enable-filter=volume \
--enable-cross-compile \
--enable-small \
--disable-runtime-cpudetect \
--enable-neon \
--enable-thumb"


if [[ "$1" == "iOS" ]];then
    ./build_x264_iOS.sh
    ./build_ffmpeg_iOS.sh
    ./make_framework_iOS.sh
elif [[ "$1" == "macOS" ]];then
    echo $1
elif [[ "$1" == "Linux" ]];then
    echo $1
elif [[ "$1" == "Windows" ]];then
    echo $1
elif [[ "$1" == "maccatalyst" ]];then
    echo $1
fi




