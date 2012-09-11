#include <iostream>
#include <sstream>
using namespace std;

bool parse (istream &stream, long double &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		stringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, float &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		stringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, string &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the string
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, unsigned &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, unsigned long &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, unsigned long long int &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parseColor (istream &stream, unsigned short int &R, unsigned short int &G, unsigned short int &B) { //takes input in the form of [RRR, GGG, BBB], default color is fuschia (if color cannot be read)

	string line;

	if (getline (stream, line)) {
		istringstream iss (line);

		if (iss >> R) {
			iss.ignore (2, ',');
			if (iss >> G) {
				iss.ignore (2, ',');
				if (iss >> B);
			} else
				return false;
		} else
			return false;
	} else
		return false;

	return true;
}
