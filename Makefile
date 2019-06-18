
.PHONY: clean

all: build
	make -C build -f ../build.make

build:
	mkdir -p build

clean:
	rm -rf build
