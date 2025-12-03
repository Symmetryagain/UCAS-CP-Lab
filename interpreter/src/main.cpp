#include "interpreter.h"

#include <algorithm>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cerr << "Usage: " << argv[0] << " <ir-file>" << endl;
		return 1;
	}
	ifstream file(argv[1]);
	if(!file)
	{
		cerr << "Error: Could not open IR file " << argv[1] << endl;
		return 1;
	}
	cerr << "Reading IR file..." << endl;

	string lstr;
	while(getline(file, lstr))
	{
		lstrs.push_back(lstr);
		cerr << lstr << endl;
	}
	file.close();

	cerr << "IR file read successfully." << endl;
	cerr << "Number of lines: " << lstrs.size() << endl;
	cerr << "Starting initialization..." << endl;

	gtable_init();

	cerr << "Initialization complete." << endl;
	cerr << "Starting interpretation..." << endl;

	Local_State main;
	Value       main_val = func_call("%main", {});

	cerr << "Interpretation complete." << endl;
	cerr << "Result: ";
	cout << main_val.ival << endl;

	return 0;
}
