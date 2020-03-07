#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <set>
#include <math.h>
using namespace std;

typedef pair<double, double> Point2D;

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
	bool parse(int argc, char* argv[]) {
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
			return true;
		}
		else
			showError();
		return false;
	}

	string getInputFile() { return inputFile; }

	string getOutputFile() { return outputFile;  }

	friend ostream& operator<<(ostream& os, const CommandLineParser& p);
};

ostream& operator<<(ostream& os, const CommandLineParser& p) {
	os << p.inputFile << " " << p.outputFile;
	return os;
}

enum class GeomType {
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
	virtual pair<bool, Point2D> getIntersectPoint(shared_ptr<GeometryObject> &other) = 0;
	friend class FileReader;
	friend class Interset;
	friend class Line;
};

Point2D saveBit(const Point2D p) {
	return {int(p.first * 100000) / 100000.0, int(p.second * 100000) / 100000.0};
}

class Line : public GeometryObject {
protected:
	int x1, x2, y1, y2;

	bool isParToY() { return x1 == x2; }

	Point2D getLineParam() {
		double k = (y1 - y2) / (x1 - x2);
		double b = y1 - k * x1;
		return { k, b };
	}

	Point2D getPointAtX(double x) {
		double k = (y1 - y2) / (x1 - x2);
		double b = y1 - k * x1;
		return { x, k * x + b };
	}

public:
	Line(int a, int b, int c, int d) : GeometryObject(GeomType::_LINE), x1(a), y1(b), x2(c), y2(d) { }

	pair<bool, Point2D> getIntersectPoint(shared_ptr<GeometryObject> &other) override {
		if (other->type == GeomType::_LINE) {
			shared_ptr<Line> otherLine = dynamic_pointer_cast<Line>(other);
			if (this->isParToY() && otherLine->isParToY())
				return { false, {0, 0} };
			else if (this->isParToY()) 
				return { true, saveBit(otherLine->getPointAtX(this->x1)) };
			else if (otherLine->isParToY()) 
				return { true, saveBit(this->getPointAtX(otherLine->x1)) };
			else {
				Point2D param1 = this->getLineParam();
				Point2D param2 = otherLine->getLineParam();
				if (fabs(param1.first - param2.first) < 1e-5)
					return { false, {0, 0} };
				double x = (param1.second - param2.second) / (param2.first - param1.first);
				double y = param1.first * x + param1.second;
				return { true, saveBit({ x, y }) };
			}
		}
		else {
			// for circle
			return { false, {0, 0} };
		}
	}
	friend class FileReader;
	friend class Intersect;
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
	int getIntersect(FileReader& reader) {
		int lineCount = reader.readIntLine(), result = 0;
		vector<shared_ptr<GeometryObject>> objList;
		set<Point2D> pointSet;
		while (lineCount--) {
			shared_ptr<GeometryObject> t = reader.readGeomObject();
			for (auto& obj : objList) {
				auto p = t->getIntersectPoint(obj);
				if (p.first && pointSet.count(p.second) == 0) {
					pointSet.insert(p.second);
					result++;
				}
			}
			objList.push_back(t);
		}
		return result;
	}
};