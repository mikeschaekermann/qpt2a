#include "ConfigurationDataHandler.h"
#include "boost/property_tree/xml_parser.hpp"

ConfigurationDataHandler * ConfigurationDataHandler::instance = 0;

ConfigurationDataHandler * const ConfigurationDataHandler::getInstance()
{
	if (!instance)
	{
		instance = new ConfigurationDataHandler();
	}
	return instance;
}

void ConfigurationDataHandler::readFromXML(std::string fullFilePath)
{
	try
	{
		/**
			* flags:
			* no_concat_text	-- Prevents concatenation of text nodes into datastring of property tree. Puts them in separate <xmltext> strings instead.
			* no_comments		-- Skip XML comments.
			* trim_whitespace	-- Trim leading and trailing whitespace from text, and collapse sequences of whitespace
			*/
		boost::property_tree::read_xml(fullFilePath, data, boost::property_tree::xml_parser::trim_whitespace);
	}
	catch (boost::property_tree::xml_parser::xml_parser_error & e)
	{
		LOG_ERROR(e.what());
		throw loadErr;
	}
	
}

ConfigurationDataHandler::ConfigurationDataHandler() { }

ConfigurationDataHandler::ConfigurationDataHandler(const ConfigurationDataHandler & cpy) { }