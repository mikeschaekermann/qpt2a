#pragma once

#define LOG_INFO(message) Logger::getInstance()->log(logInfo, message)
#define LOG_WARNING(message) Logger::getInstance()->log(logWarning, message)
#define LOG_ERROR(message) Logger::getInstance()->log(logError, message)

#include <string>
#include <sstream>
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
	static Logger * const getInstance() { return (m_pLogger != nullptr ? m_pLogger : (m_pLogger = new Logger())); }

	/**
		@brief sets the filename for logging
	 */
	void configure(string filename);

	/**
		@brief logs the message into the file configured before
	 */
	template<typename T> void log(LogSeverity lvl, T message);
private:
	/// singleton logger instance
	static Logger* m_pLogger;
	
	/// file handle
	ofstream m_file;
	/// the mutex for thread safe file handling
	std::_Mutex m_streamWriteMutex;

	/// @brief private constructor as defined for the singleton pattern
	Logger(void);
	/// @brief private copy constructor
	Logger(const Logger &);
};

template<typename T> void Logger::log(LogSeverity lvl, T message)
{
#ifdef _LOG

	if(!m_file.is_open())
	{
		configure("main.log");
	}

	stringstream output;

	// set line prefix
	if(lvl == logInfo)
	{
		output << "INFO\t";
	}
	else if(lvl == logWarning)
	{
		output << "WARNING\t";
	}
	else
	{
		output << "ERROR\t";
	}

	// write the current time in the line
	time_t t = time(0);
	
	// TODO: Change function to localtime_s to avoid warning
	#pragma warning(push)
	#pragma warning(disable: 4996)
	struct tm * now = localtime(&t);
	output << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " " << now->tm_mday << "." << (now->tm_mon + 1) << "." << (now->tm_year + 1900) << "\t";
	#pragma warning(pop)

	// append the message
	output << message;

	m_streamWriteMutex._Lock();
	m_file << output.str() << "\n";
	m_file.flush();
#ifdef _DEBUG
#ifdef _CLIENT
	ci::app::console() << output.str() << std::endl;
#else
	std::cout << output.str() << std::endl;
#endif
#endif

	m_streamWriteMutex._Unlock();
#endif
}