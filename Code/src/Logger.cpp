#include "Logger.h"
#include <assert.h>
#include <time.h>
#include <sstream>

Logger* Logger::logger = new Logger();

Logger::Logger(void)
{
}

Logger::~Logger(void)
{
	file.close();
	delete file;
}

void Logger::configure(string filename)
{
	file.open(filename, fstream::in | fstream::app);
}

void Logger::log(LogSeverity lvl, string message)
{
	assert(file);

	stringstream output;

	// set line prefix
	if(lvl == LOG_INFO)
	{
		output << "INFO\t";
	}
	else if(lvl == LOG_WARNING)
	{
		output << "WARNING\t";
	}
	else
	{
		output << "ERROR\t";
	}

	// write the current time in the line
	time_t t = time(0);
	struct tm * now = localtime(&t);
	output << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " " << now->tm_mday << "." << (now->tm_mon + 1) << "." << (now->tm_year + 1900) << "\t";

	// append the message
	output << message << endl;
	
	streamWriteMutex._Lock();
	file << output.str();
	file.flush();
	streamWriteMutex._Unlock();
}

Logger * const Logger::getInstance()
{
	return logger;
}