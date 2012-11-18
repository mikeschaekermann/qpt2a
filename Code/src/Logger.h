#pragma once

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

enum LogSeverity
{
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

class Logger
{
public:
	~Logger(void);

	/*
		@brief returns the single instance of the logger class
	*/
	static Logger * const getInstance();

	/*
		@brief sets the filename for logging
	*/
	void configure(string filename);

	/*
		@brief logs the message in the before configured file
	*/
	void log(LogSeverity lvl, string message);

private:
	/// singleton logger instance
	static Logger* logger;
	
	/// file handle
	ofstream file;
	/// the mutex for thread safe file handling
	std::_Mutex streamWriteMutex;

	/*
		@brief private constructor as defined for the singleton pattern
	*/
	Logger(void);
};