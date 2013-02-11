#pragma once

/**
 * Singleton data handler for the configurations
 */

#include <string>
#include <vector>
#include <exception>

#include "boost/foreach.hpp"
#include "boost/property_tree/ptree.hpp"

#include "Config.h"

#define foreach BOOST_FOREACH

#define CONFIG_MGR ConfigurationDataHandler::getInstance()

#define CONFIG_FLOAT1(key) CONFIG_MGR->get<float>(key)
#define CONFIG_FLOAT2(key, value) CONFIG_MGR->get<float>(key, value)

#define CONFIG_INT1(key) CONFIG_MGR->get<int>(key)
#define CONFIG_INT2(key, value) CONFIG_MGR->get<int>(key, value)

#define CONFIG_FLOATS2(path, key) CONFIG_MGR->getChildrenDataVector<float>(path,key)

class ConfigurationDataHandler
{
public:
	class load_error : public std::exception
	{
		virtual const char * what() const { return "xml file was not correctly loaded"; }
	} loadErr;

	class data_not_found_error : public std::exception
	{
		virtual const char * what() const { return "data was not found"; }
	} dataNotFoundErr;

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
			throw dataNotFoundErr;
		}
		catch (boost::property_tree::ptree_bad_path & e)
		{
			LOG_ERROR(e.what());
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
			{
				if (v.first == descendantPropertyKeyHierachy.substr(0, descendantPropertyKeyHierachy.find(".")))
				{
					auto text = descendantPropertyKeyHierachy.substr(descendantPropertyKeyHierachy.find(".") + 1, descendantPropertyKeyHierachy.size() - 1);
					auto data = v.second.get<T>(text);
					childrenData.push_back(data);
				}
			}
			return childrenData;
		}
		catch (boost::property_tree::ptree_bad_path & e)
		{
			LOG_ERROR(e.what());
			throw dataNotFoundErr;
		}
		catch(...)
		{
			LOG_ERROR("rest");
		}
	}
private:
	static ConfigurationDataHandler * instance;
	boost::property_tree::ptree data;

	ConfigurationDataHandler();
	ConfigurationDataHandler(const ConfigurationDataHandler & cpy);
};