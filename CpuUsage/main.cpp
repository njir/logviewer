#include <Pdh.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include "CpuUsage.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (!checkValidArugment(argc, argv)) {
		cout << "Usage: CpuUsage.exe [output] [measure time]\n" << '\n';
		cout << "Example: ParsingXML.exe c15.csv 60\n" << '\n';
		cin.get();
		exit(1);
	}

	// start
	cout << "Setup Cpu Query... Please wait for seconds\n" << '\n';

	CpuUsage *myCpu = new CpuUsage;
	ofstream outputFile(argv[1], ios::trunc);

	double totalUsage = 0;
	double echoLoaderUsage = 0;
	double average = 0;

	cout << "Write CPU Usage to " << argv[1] << '\n';
	outputFile << "Time,Total Cpu\n";
	
	for (int i = 0; i < atoi(argv[2]); i++) {
		totalUsage = myCpu->getTotalUsage();
		
		cout << std::internal << std::setw(3) << i << ". Cpu Usage: " << fixed << setprecision(4) << totalUsage << "%" << '\n';
		outputFile << i << "," << totalUsage << "\n";

		if(i != 0) // drop the first data
			average += totalUsage;
		Sleep(1000);
	}

	// calculate average
	average = (average / (atoi(argv[2]) - 1));
	cout << "Average: " << average << "%" << '\n';
	outputFile << "Average" << "," << average << "\n";

	delete myCpu;
	outputFile.close();

	return 0;
}
