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
	cout << "Reading IR file..." << endl;

	string lstr;
	while(getline(file, lstr))
	{
		lstrs.push_back(lstr);
		cout << lstr << endl;
	}
	file.close();

	cout << "IR file read successfully." << endl;
	cout << "Number of lines: " << lstrs.size() << endl;
	cout << "Starting initialization..." << endl;

	gtable_init();

	cout << "Initialization complete." << endl;
	cout << "Starting interpretation..." << endl;

	Local_State main;
	Value main_val = func_call("main", {});

	cout << "Interpretation complete." << endl;
	cout << "Result: " << main_val.ival << endl;

	return 0;
}