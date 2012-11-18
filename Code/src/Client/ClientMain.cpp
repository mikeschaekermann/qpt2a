#pragma once

#include "ClientMain.h"
#include "Config.h"

#include <log4cxx/logstring.h>
#include <stdlib.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/ndc.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

void ClientMain::setup()
{
	setWindowPos(100, 100);
	BasicConfigurator::configure();
    
	LoggerPtr rootLogger = Logger::getRootLogger();

    NDC::push("trivial context");

    LOG4CXX_DEBUG(rootLogger, "debug message");
    LOG4CXX_INFO(rootLogger, "info message");
    LOG4CXX_WARN(rootLogger, "warn message");
    LOG4CXX_ERROR(rootLogger, "error message");
    LOG4CXX_FATAL(rootLogger, "fatal message");

    NDC::pop();
}

void ClientMain::update()
{
	
}

void ClientMain::draw()
{

}


CINDER_APP_BASIC( ClientMain, RendererGl )