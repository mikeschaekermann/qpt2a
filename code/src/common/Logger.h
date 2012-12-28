#pragma once

#define LOG_INFO(message) Logger::getInstance()->log(logInfo, message)
#define LOG_WARNING(message) Logger::getInstance()->log(logWarning, message)
#define LOG_ERROR(message) Logger::getInstance()->log(logError, message)

#include <string>
#include <iostream>
#include <fstream>
#include "cinder\app\AppBasic.h"

using namespace std;

enum LogSeverity
{
	logInfo,
	logWarning,
	logError
};

class Logger
{
public:
	~Logger(void);

	/**
		@brief returns the single instance of the logger class
	 */
	static Logger * const getInstance() { return m_pLogger; }

	/**
		@brief sets the filename for logging
	 */
	void configure(string filename);

	/**
		@brief logs the message in the before configured file
	 */
	void log(LogSeverity lvl, string message);

private:
	/// singleton logger instance
	static Logger* m_pLogger;
	
	/// file handle
	ofstream m_file;
	/// the mutex for thread safe file handling
	std::_Mutex m_streamWriteMutex;

	/// @brief private constructor as defined for the singleton pattern
	Logger(void);
	/// @brief privagte copy constructor
	Logger(const Logger &);
};