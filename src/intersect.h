#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
using namespace std;

class CommandLineParser {
private:
	string inputFile;
	string outputFile;

	void showHelp() {
		cout << "usage ./intersect.ext -i inputFile -o outputFile" << endl;
	}

	void showError() {
		cout << "Error input!" << endl;
		showHelp();
	}

public:
	void parse(int argc, char* argv[]) {
		if (argc == 2 && string(argv[1]) == "-h")
			showHelp();
		else if (argc == 5) {
			if (string(argv[1]) == "-i") {
				inputFile = string(argv[2]);
				outputFile = string(argv[4]);
			}
			else {
				inputFile = string(argv[4]);
				outputFile = string(argv[2]);
			}
		}
		else
			showError();
	}

	string getInputFile() { return inputFile; }

	string getOutputFile() { return outputFile;  }

	friend ostream& operator<<(ostream& os, const CommandLineParser& p);
};


ostream& operator<<(ostream& os, const CommandLineParser& p) {
	os << p.inputFile << " " << p.outputFile;
	return os;
}



enum GeomType {
	_LINE = 0,
	_CIRCLE = 1
};

class GeometryObject {
protected:
	GeomType type;

public:
	GeometryObject(GeomType t) : type(t) { }
	GeometryObject() = default;
	virtual ~GeometryObject() { }
	friend class FileReader;
};

class Line : public GeometryObject {
protected:
	int x1, x2, y1, y2;

public:
	Line(int a, int b, int c, int d) : GeometryObject(GeomType::_LINE), x1(a), y1(b), x2(c), y2(d) { }
	friend class FileReader;
};

/*
class : public GeometryObject {

};
*/

class FileReader {
private:
	ifstream file;

public:
	FileReader(string name) : file(name) { }

	virtual ~FileReader() { file.close();  }

	string readLine() {
		string s;
		getline(file, s);
		return s;
	}

	int readIntLine() {
		return stoi(readLine());
	}

	shared_ptr<GeometryObject> readGeomObject() {
		stringstream stream(readLine());
		char c;
		stream >> c;
		if (c == 'L') {
			int a, b, c, d;
			stream >> a >> b >> c >> d;
			return make_shared<Line>(a, b, c, d);
		}
		else if (c == 'C') {
			return NULL;
		}
		return NULL;
	}
};

class FileWriter {
private:
	ofstream file;

public:
	FileWriter(string name) : file(name) { }

	virtual ~FileWriter() { file.close(); }

	void writeIntLine(int x) {
		file << x << endl;
	}
};


class Intersect {
public:
	int result = 0;

	int getIntersect(FileReader& reader) {
		int lineCount = reader.readIntLine();
		while (lineCount--) {
			reader.readGeomObject();
		}
		return result;
	}
};