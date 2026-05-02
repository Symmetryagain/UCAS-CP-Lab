GRAMMAR_JAR := deps/antlr-4.13.1-complete.jar
BUILD_DIR   := build

.PHONY: grammar build rebuild clean interpreter all

all: build interpreter

grammar:
	java -jar $(GRAMMAR_JAR) -Dlanguage=Cpp grammar/CACT.g4 -visitor -no-listener
	mv grammar/CACTBaseVisitor.cpp grammar/CACTBaseVisitor.h \
	   grammar/CACTLexer.cpp      grammar/CACTLexer.h \
	   grammar/CACTParser.cpp     grammar/CACTParser.h \
	   grammar/CACTVisitor.cpp    grammar/CACTVisitor.h \
	   generated/grammar/

build:
	mkdir -p $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR)
	make -C $(BUILD_DIR) -j

interpreter:
	@if [ ! -d $(BUILD_DIR) ]; then \
		mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR); \
	fi
	make -C $(BUILD_DIR) interpreter -j

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean grammar build
