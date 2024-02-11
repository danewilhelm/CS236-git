#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

void count_char(string str) {
	int n = str.length();
	int lines = 0;
	int spaces = 0;
	int digits = 0;
	int letters = 0;

	for (int i = 0; i < n; i++) {
		if (isspace(str[i]))
			spaces++;
			if (str[i] == '\n')
				lines++;
		else if (isdigit(str[i]))
			digits++;
		else if (isalpha(str[i]))
			letters++;
	}
	cout << "chars: " << n << endl;
	cout << "lines: " << lines << endl;
	cout << "spaces: " << spaces << endl;
	cout << "digits: " << digits << endl;
	cout << "letters: " << letters << endl;
}



int main(int argc, char* argv[]) {
	// cout << "argc: " << argc << endl;
	// cout << "argv[0]: " << argv[0] << endl;
	// cout << "argv[1]: " << argv[1] << endl;
	string filename = argv[1];
	ifstream in;
 	in.open(filename);
  	stringstream ss;
  	ss << in.rdbuf();
  	string input = ss.str();
  	in.close();

	count_char(input);
	// cout << "*** Start of Input ***" << endl;
	// cout << input;
	// cout << "*** End of Input ***" << endl;
}




