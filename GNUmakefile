CXX=/usr/local/opt/llvm/bin/clang++
CPPFLAGS=-std=c++20 -nostdinc
CXXFLAGS=-fprebuilt-module-path=.pcm

PRECOMPILE.cc=$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -x c++-module --precompile

all:

clean:
	-$(RM) .bin/*
	-$(RM) .lib/*
	-$(RM) .obj/*
	-$(RM) .pcm/*

test: .bin/list_test
	.bin/list_test

.bin/list_test: .obj/list_test.o .obj/funcpp.o .obj/funcpp-list.o .obj/funcpp-maybe.o .obj/funcpp-common.o

.obj/list_test.o: .pcm/funcpp.pcm

.obj/funcpp.o .pcm/funcpp.pcm: .pcm/funcpp-common.pcm .pcm/funcpp-list.pcm .pcm/funcpp-maybe.pcm

.obj/funcpp-list.o .pcm/funcpp-list.pcm: .pcm/funcpp-common.pcm

.obj/funcpp-maybe.o .pcm/funcpp-maybe.pcm: .pcm/funcpp-common.pcm

.pcm/%.pcm: src/%.cc
	$(PRECOMPILE.cc) $(OUTPUT_OPTION) $<

.obj/%.o: src/%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

.obj/%_test.o: test/%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

.bin/%_test: .obj/%_test.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
