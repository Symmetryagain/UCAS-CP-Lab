all: grammar build

grammar:
	java -jar ./deps/antlr-4.13.1-complete.jar -Dlanguage=Cpp ./grammar/CACT.g4 -visitor -no-listener

build:
	mkdir -p ./build
	cmake -S . -B ./build
	make -C ./build -j

clean:
	rm -rf ./build

rebuild: clean grammar build

.PHONY: grammar build clean rebuild