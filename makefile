CXX = g++
CXXFLAGS = -Wall -g

SRCDIR = src
BUILDDIR = build

BUILD = compiler

OBJS_ = common.o \
		basic_io.o \
		error.o \
		run_main.o \
		file.o \
		scan.o \
		parse_common.o \
		context_free_parse.o \
		semantic_parse.o \
		main.o

OBJS = $(patsubst %, $(BUILDDIR)/%, $(OBJS_))

$(BUILDDIR)/%.o : $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/$(BUILD) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY : clean
clean :
	rm $(BUILDDIR)/*.o
