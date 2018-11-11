BUILDDIR := build
TARGETDIR := bin
SRCS := $(shell find . -name *.cpp)
OBJECTS := $(patsubst %, $(BUILDDIR)/%,$(notdir $(SRCS:%.cpp=%.o)))
SPATH := $(sort $(dir $(SRCS)))
VPATH := $(SPATH)
DEP := $(OBJECTS:%.o=%.d)
LIBS := -lavutil -lavformat -lSDL2
# -m - MM 表示输出文件依赖关系
# -MT 指定目标文件名

av:$(OBJECTS)
	@mkdir -p $(TARGETDIR)
	clang++ $^ $(LIBS) -g -o $(TARGETDIR)/av

$(BUILDDIR)/%.d: %.cpp
	@mkdir -p $(BUILDDIR)
	clang++ -MM $< -o $@ -MT $(patsubst %.d, %.o, $@)
	
include $(DEP)

$(BUILDDIR)/%.o: %.cpp
	clang++ -Wall -std=c++11 -stdlib=libc++ -g -c $<  -o $@

.PHONY : test
test:
	@echo "objects" $(OBJECTS)
	@echo "srcs:" $(SRCS)
	@echo "dir:" $(dir $(SRCS))		
	@echo "notdir:" $(notdir $(SRCS))
	@echo "sort": $(SPATH)
	@echo "dep:" $(DEP)
	@echo "cpp:" $(notdir $(DEP:%.d=%.cpp))
