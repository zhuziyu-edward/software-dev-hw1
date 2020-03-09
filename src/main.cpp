#include <iostream>
#include "intersect.h"
#include <time.h>

int main(int argc, char *argv[]) {
	CommandLineParser cmdParser;
	if (!cmdParser.parse(argc, argv))
		return 0;
	FileReader input(cmdParser.getInputFile());	
	FileWriter output(cmdParser.getOutputFile());
	Intersect solution;
	output.writeIntLine(solution.getIntersect(input));
	return 0;
}
	

