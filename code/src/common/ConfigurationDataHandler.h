#pragma once

/**
 * Singleton data handler for the configurations
 */

#include <string>
#include <vector>

#include "boost/foreach.hpp"
#include "boost/property_tree/ptree.hpp"

#include "Config.h"

#define foreach BOOST_FOREACH

#define CONFIG_FLOAT1(key) ConfigurationDataHandler::getInstance()->get<float>(key)
#define CONFIG_FLOAT2(key, value) ConfigurationDataHandler::getInstance()->get<float>(key, value)

#define CONFIG_INT1(key) ConfigurationDataHandler::getInstance()->get<int>(key)
#define CONFIG_INT2(key, value) ConfigurationDataHandler::getInstance()->get<int>(key, value)

class ConfigurationDataHandler
{
public:
	static ConfigurationDataHandler * const getInstance();

	/**
	 * Load the XML file into the property tree. If reading fails
	 * (cannot open file, parse error), an exception is thrown.
	 */
	void readFromXML(std::string fullFilePath);

	/**
	 * Gets the value of the key-string. Note that we construct the 
	 * path to the value by separating the individual keys with dots. 
     * If the key is not found, an exception is thrown.
	 */
	template<typename T> T get(std::string propertyKeyHierachy) const
	{
		try
		{
			return data.get<T>(propertyKeyHierachy);
		}
		catch (boost::property_tree::ptree_bad_data & e)
		{
			LOG_ERROR(e.what());
			return T(0);
		}
	}

	/**
	 * Gets the value of the key-string. Note that we construct the 
	 * path to the value by separating the individual keys with dots. 
     * If the key is not found, the default value is returned.
	 */
	template<typename T> T get(std::string propertyKeyHierachy, const T & defaultValue) const
	{
		return data.get<T>(propertyKeyHierachy, defaultValue);
	}

	/**
	 * Returns a set of the keys children data. Note that we construct the 
	 * path to the value by separating the individual keys with dots. 
     * If the key is not found, an exception is thrown.
	 */
	template<typename T> std::vector<T> getChildrenDataVector(std::string parentPropertyKeyHierachy, std::string descendantPropertyKeyHierachy) const
	{
		try
		{
			std::vector<T> childrenData;
			foreach(boost::property_tree::ptree::value_type const & v, data.get_child(parentPropertyKeyHierachy))
				if (v.first == descendantPropertyKeyHierachy.substr(0, descendantPropertyKeyHierachy.find(".")))
				{
					childrenData.push_back(v.second.get<T>(descendantPropertyKeyHierachy));
				}
			return childrenData;
		}
		catch (boost::property_tree::ptree_bad_path & e)
		{
			LOG_ERROR(e.what());
		}
	}
private:
	static ConfigurationDataHandler * instance;
	boost::property_tree::ptree data;

	ConfigurationDataHandler();
	ConfigurationDataHandler(const ConfigurationDataHandler & cpy);
};