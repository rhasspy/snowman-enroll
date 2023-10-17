.PHONY: clean

all:
	cmake -Bbuild
	cmake --build build --config Release

clean:
	rm -rf build
