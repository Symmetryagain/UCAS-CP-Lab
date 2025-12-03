all:
	g++ -o interpreter ./src/main.cpp ./src/interpreter.cpp -std=c++23 -ggdb3
clean:
	rm interpreter
test: all
	./interpreter ./test/test076.ir 
gdb: all
	gdb --args ./interpreter ./test/test076.ir 

.phony: all clean test gdb