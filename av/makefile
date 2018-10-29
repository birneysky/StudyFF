#目录
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%, %, $(SOURCES:.cpp=.o))

OBJS    := $(SOURCES:.cpp=.o)
# $@ 目标文件的名称
# $^ 所有依赖文件，以空格分开，不包含重复的依赖文件
# $< 

av:$(OBJECTS)
	echo $(OBJECTS)
	clang++ $(OBJECTS) -lavutil -o av

$(OBJECTS): %.cpp 
	echo $(OBJECTS)
	mkdir -p build
	clang++ -Wall -std=c++11 -stdlib=libc++ -c $< -o $@
#.PHONY : clean 
#clean : `
#rm av av.o
