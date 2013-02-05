#include "Logger.h"
#include <assert.h>
#include <time.h>
#include <sstream>
#include "cinder/app/AppBasic.h"

Logger* Logger::m_pLogger = new Logger();

Logger::Logger(void)
{
}

Logger::~Logger(void)
{
	if(m_file)
	{
		m_file.close();
	}
}

void Logger::configure(string filename)
{
	m_file.open(filename, fstream::in | fstream::app);
}