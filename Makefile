BUILD_TYPE ?= Debug

CMAKE_ARGS := -DCMAKE_BUILD_TYPE=${BUILD_TYPE}

BUILDDIR := ./build

all: ${BUILDDIR}/Makefile
	@$(MAKE) -C ${BUILDDIR}
.PHONY: all

test:
		make && cd ${BUILDDIR} && cp -r ../test_levels tests/test_levels && ctest
.PHONY: test

clean:
	rm -rf ${BUILDDIR}
.PHONY: clean

${BUILDDIR}/Makefile:
	cmake -S . -B${BUILDDIR} ${CMAKE_ARGS} 
