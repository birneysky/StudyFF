BUILDDIR := build
TARGETDIR := bin
SRCS := $(shell find ./src -name *.cpp)
OBJECTS := $(patsubst %, $(BUILDDIR)/%,$(notdir $(SRCS:%.cpp=%.o)))
SPATH := $(sort $(dir $(SRCS)))
VPATH := $(SPATH)
DEP := $(OBJECTS:%.o=%.d)
LIBS := 

playground:$(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@clang++ $^ $(LIBS) -g -o $(TARGETDIR)/playground

$(BUILDDIR)/%.d: %.cpp
	@mkdir -p $(BUILDDIR)
	@clang++ -MM $< -o $@ -MT $(patsubst %.d, %.o, $@)
	
include $(DEP)

$(BUILDDIR)/%.o: %.cpp
	@echo "Compiling " $<
	@clang++ -Wall -std=c++11 -stdlib=libc++ -g -c $<  -o $@

.PHONY : test
test:
	@echo "objects" $(OBJECTS)
	@echo "srcs:" $(SRCS)
	@echo "dir:" $(dir $(SRCS))		
	@echo "notdir:" $(notdir $(SRCS))
	@echo "sort": $(SPATH)
	@echo "dep:" $(DEP)
	@echo "cpp:" $(notdir $(DEP:%.d=%.cpp))
