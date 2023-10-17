.PHONY: clean docker

all:
	cmake -Bbuild
	cmake --build build --config Release

clean:
	rm -rf build

docker:
	docker buildx build . --platform linux/amd64,linux/arm64,linux/arm/v7 --output 'type=local,dest=dist'
