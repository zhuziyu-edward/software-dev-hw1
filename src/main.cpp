#include <iostream>
#include "intersect.h"

int main(int argc, char *argv[]) {
	CommandLineParser cmdParser;
	cmdParser.parse(argc, argv);
	FileReader input(cmdParser.getInputFile());
	FileWriter output(cmdParser.getOutputFile());
	Intersect solution;
	output.writeIntLine(solution.getIntersect(input));
	return 0;
}


