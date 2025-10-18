all: grammar build

grammar:
	java -jar ./deps/antlr-4.13.1-complete.jar -Dlanguage=Cpp ./grammar/Hello.g4 -visitor -no-listener

build: grammar
	mkdir -p ./build
	cmake -S . -B ./build
	make -C ./build -j

clean:
	rm -rf ./build

.PHONY: grammar build clean