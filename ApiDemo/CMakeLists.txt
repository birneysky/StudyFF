cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(IPHONEOS_DEPLOYMENT_TARGET 10.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.0)
set (CMAKE_SYSTEM_NAME "Darwin")
set (CMAKE_OSX_SYSROOT iphoneos)
#set(CMAKE_OSX_ARCHITECTURES "arm64")
set(LINK_OPTS -all_load)
set(LINK_FRAMEWORKS "-framework Security -framework AudioToolbox -framework CoreFoundation -framework CoreMedia -framework VideoToolbox -framework CoreVideo")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch arm64")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")
set(LIB_NAME "jojoffmpeg")
project(${LIB_NAME})

message(${CMAKE_CURRENT_SOURCE_DIR})
#add_definitions(-DENABLE_BITCODE)
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../install/ffmpeg/iOS/fat/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../extra/av1/iOS/fat/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../install/x264/iOS/fat/lib
)
#set(CMAKE_C_FLAGS "-arch arm64 -fembed-bitcode")
#-miphoneos-version-min=$IPHONEOS_DEPLOYMENT_TARGET
#add_compile_options(-x objective-c++)
#list(APPEND CMAKE_LIBRARY_PATH /Users/bruce/source/code/Frameworks/build_extra/install/ffmpeg/iOS/fat/lib/)

file(GLOB_RECURSE MY_ABCDEEF
    ${CMAKE_CURRENT_SOURCE_DIR}/../../install/ffmpeg/iOS/fat/include/*.h
)

foreach(afile IN LISTS MY_ABCDEEF)
    message(${afile})
    list(APPEND FFMPEG_INCLUDE_FILE ${afile})
endforeach(afile)



#message(${MY_ABCDEEF})


add_library(${LIB_NAME} SHARED build_version.cpp build_version.h)

list(APPEND EXTRA_LIBS libavcodec.a)
list(APPEND EXTRA_LIBS libavfilter.a)
list(APPEND EXTRA_LIBS libavformat.a)
list(APPEND EXTRA_LIBS libavresample.a)
list(APPEND EXTRA_LIBS libavutil.a)
list(APPEND EXTRA_LIBS libswresample.a)
list(APPEND EXTRA_LIBS libswscale.a)
list(APPEND EXTRA_LIBS libav1d.a)
list(APPEND EXTRA_LIBS libx264.a)
list(APPEND EXTRA_LIBS iconv)
list(APPEND EXTRA_LIBS z)

message("LIBRARYS is ${EXTRA_LIBS}")
message("the value of bit code is ${BITCODE_VALUE}")
target_link_libraries(${LIB_NAME} PRIVATE ${EXTRA_LIBS})
target_link_libraries(${LIB_NAME} PUBLIC "${LINK_FRAMEWORKS}")

file(GLOB_RECURSE VERSION_HEADEr 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h   
)


set_target_properties(${LIB_NAME} PROPERTIES
    FRAMEWORK TRUE
    XCODE_ATTRIBUTE_ENABLE_BITCODE YES
    FRAMEWORK_VERSION A
    LINKER_LANGUAGE C
    MACOSX_FRAMEWORK_BUNDLE_VERSION 1
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING 1.0
    CODE_ATTRIBUTE_DEFINES_MODULE YES
    LINK_FLAGS "-Wl,${LINK_OPTS}"
    MACOSX_FRAMEWORK_IDENTIFIER com.jojo.${LIB_NAME}
    XCODE_ATTRIBUTE_EXCLUDED_ARCHS[sdk=iphonesimulator*] arm64
    INSTALL_NAME_DIR @rpath
    XCODE_ATTRIBUTE_LD_DYLIB_INSTALL_NAME "@rpath/${LIB_NAME}.framework/${LIB_NAME}"
    PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/build_version.h
)

message(${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../install/ffmpeg/iOS/fat/include/ DESTINATION ${CMAKE_BINARY_DIR}/${LIB_NAME}.framework/Headers/)

# INSTALL(TARGETS JOJOFFmpeg 
#         FRAMEWORK DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/out
#         PUBLIC_HEADER DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/out/
# )

# set_target_properties(JOJOFFmpeg PROPERTIES
#         FRAMEWORK TRUE
#         FRAMEWORK_VERSION A
#         MACOSX_FRAMEWORK_IDENTIFIER com.jojo.JOJOFFmpeg
#         LINK_FLAGS "-Wl,${LINK_OPTS}"
#         PUBLIC_HEADER "${PUBLIC_HEADERS}"
#         LINKER_LANGUAGE C
#         MACOSX_FRAMEWORK_BUNDLE_VERSION 1
#         MACOSX_FRAMEWORK_SHORT_VERSION_STRING 1.0
#         INSTALL_NAME_DIR @rpath
#         XCODE_ATTRIBUTE_LD_DYLIB_INSTALL_NAME "@rpath/JOJOFFmpeg.framework/JOJOFFmpeg"
#         #        MACOSX_FRAMEWORK_INFO_PLIST Info.plist
#         #XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
#         XCODE_ATTRIBUTE_EXCLUDED_ARCHS[sdk=iphonesimulator*] arm64
#         XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=MinSizeRel] YES
#         XCODE_ATTRIBUTE_ENABLE_BITCODE YES
#         XCODE_ATTRIBUTE_DEFINES_MODULE "YES"
#         XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE[variant=MinSizeRel] ${BITCODE_GENERATION_VALUE}
#         )
