CXX=g++
CXXFLAGS=-Wall

SRCDIR=src
BUILDDIR=build


compile:
	$(CXX) $(CXXFLAGS) $(SRCDIR)/* -o $(BUILDDIR)/compiler

clean:
	rm *.o

clean-build:
	rm $(BUILDDIR)/*
