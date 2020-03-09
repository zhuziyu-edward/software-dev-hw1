#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <set>
#include <math.h>
#include <unordered_set>
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
	return {p.first, p.second};
}

class Line : public GeometryObject {
protected:
	int x1, x2, y1, y2;

	bool isParToY() { return x1 == x2; }

	Point2D getLineParam() {
		double k = (y1 - y2) / double(x1 - x2);
		double b = y1 - k * x1;
		return { k, b };
	}

	Point2D getPointAtX(double x) {
		return { x, (y1 - y2) * (x - x1) /  double(x1 -x2) + y1};
	}

	bool isParToOther(shared_ptr<Line> otherLine) {
		if ((y1 - y2) * (otherLine->x1 - otherLine->x2) == (otherLine->y1 - otherLine->y2) * (x1 - x2))
			return true;
		else
			return false;
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
				if (this->isParToOther(otherLine))
					return { false, {0, 0} };
				Point2D param1 = this->getLineParam();
				Point2D param2 = otherLine->getLineParam();
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

struct pairHash {
	template <class T1, class T2>
	size_t operator()(const pair<T1, T2>& pair) const {
		return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
	}
};

class Intersect {
public:
	int getIntersect(FileReader& reader) {
		int lineCount = reader.readIntLine(), result = 0;
		vector<shared_ptr<GeometryObject>> objList;
		unordered_set<Point2D, pairHash> pointSet;
		while (lineCount--) {
			shared_ptr<GeometryObject> t = reader.readGeomObject();
			for (auto& obj : objList) {
				auto p = t->getIntersectPoint(obj);
				if (p.first && pointSet.find(p.second) == pointSet.end()) {
					pointSet.insert(p.second);
					result++;
				}
			}
			objList.push_back(t);
		}
		return result;
	}
};