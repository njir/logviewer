#pragma once
#include <Pdh.h>
#include <string>
#include "psapi.h"
#pragma comment(lib,"Pdh.lib")

class CpuUsage {
public:
	CpuUsage() {
		// set Total Cpu query
		PdhOpenQuery(NULL, NULL, &m_query);
		PdhAddCounter(m_query, "\\Processor(_Total)\\% Processor Time", NULL, &m_cpuTotal);
		PdhCollectQueryData(m_query);

		/*
		// set Process Cpu query
		std::string queryForProcess = "\\Process(" + m_procName + ")\\% Processor Time";
		PdhOpenQuery(NULL, 0, &m_processQuery);
		PdhAddCounter(m_processQuery, queryForProcess.c_str(), 0, &m_processCpu);
		PdhCollectQueryData(m_processQuery);
		*/
	}

	~CpuUsage()
	{
		PdhRemoveCounter(m_cpuTotal);
		PdhCloseQuery(m_query);
	}

	double getTotalUsage() {
		PdhCollectQueryData(m_query);
		PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE, NULL, &m_counterVal);
		return m_counterVal.doubleValue;
	}
	
	/*
	double getProcessUsage() {
		PdhCollectQueryData(m_processQuery);
		PdhGetFormattedCounterValue(m_processCpu, PDH_FMT_LONG | PDH_FMT_NOCAP100, 0, &m_processCounterVal);
		return m_processCounterVal.longValue;
	}
	*/

private:
	PDH_HQUERY m_query;
	PDH_HCOUNTER m_cpuTotal;
	PDH_FMT_COUNTERVALUE m_counterVal;
};

// Helper function

int verify(char* string)
{
	int x = 0;
	int len = strlen(string);

	while (x < len) {
		if (!isdigit(*(string + x)))
			return 1;

		++x;
	}

	return 0;
}

bool checkValidArugment(int argc, char* argv[]) {
	if (argc != 3)				return false;
	if (verify(argv[2]) != 0)   return false;

	return true;
}
