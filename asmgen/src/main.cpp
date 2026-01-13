#include "main.h"

int            optimize_level = 0;
vector<string> ir_lines(0);
vector<string> asm_lines = {
    ".option nopic",
    ".attribute  arch, \"rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0\"",
    ".attribute  unaligned_access, 0",
    ".attribute  stack_align, 16"
};

int main(int argc, char* argv[])
{
	// argument
	if(argc < 3)
	{
		cerr << "usage: " << argv[0] << " <output-file> <ir-file> [-O1]"
		     << endl;
		return 1;
	}

	ofstream asm_file(argv[1]);
	if(!asm_file)
	{
		cerr << "Error: Could not open output file " << argv[1] << endl;
		return 1;
	}

	ifstream ir_file(argv[2]);
	if(!ir_file)
	{
		cerr << "Error: Could not open IR file " << argv[1] << endl;
		return 1;
	}

	if(argc > 3 && strcmp(argv[3], "-O1") == 0)
	{
		cerr << "Optimization level 1 enabled." << endl;
		optimize_level = 1;
	}
	else
	{
		cerr << "Optimization level 0 (default) enabled." << endl;
	}

	// input

	cerr << "Reading IR file..." << endl;

	string ir_str;
	while(getline(ir_file, ir_str))
	{
		ir_lines.push_back(ir_str);
	}
	ir_file.close();

	cerr << "IR file read finished." << endl;
	cerr << "Number of lines: " << ir_lines.size() << endl;

	// asmgen

	cerr << "Starting ASM generate..." << endl;

	asmgen();

	cerr << "ASM generated finished." << endl;

	// output

	cerr << "Writing ASM file..." << endl;

	for(auto line : asm_lines)
	{
		asm_file << line << endl;
	}
	asm_file.close();

	cerr << "ASM file written finished." << endl;

	return 0;
}
