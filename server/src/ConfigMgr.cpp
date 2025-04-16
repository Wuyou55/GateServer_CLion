//
// Created by wuyou on 25-4-16.
//

#include "ConfigMgr.h"

SectionInfo::SectionInfo(const SectionInfo& src)
{
    sections_datas_ = src.sections_datas_;
}

SectionInfo& SectionInfo::operator=(const SectionInfo& src)
{
    if (this == &src)
    {
        return *this;
    }
    sections_datas_ = src.sections_datas_;
    return *this;
}

std::string SectionInfo::operator[](std::string key)
{
    if (!sections_datas_.contains(key))
    {
        return "";
    }
    return sections_datas_[key];
}

ConfigMgr::ConfigMgr()
{
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path config_path = current_path / "config.ini";
    std::cout << "config_file_directory: " << config_path << std::endl;

    boost::property_tree::ptree ptree;
    boost::property_tree::read_ini(config_path.string(), ptree);

    for (const auto& section_pair : ptree)
    {
        const std::string& section_name = section_pair.first;
        const boost::property_tree::ptree& section_tree = section_pair.second;
        std::map<std::string, std::string> section_config;
        for (const auto& key_value_pair : section_tree)
        {
            const std::string& key = key_value_pair.first;
            const std::string& value = key_value_pair.second.get_value<std::string>();\
            section_config[key] = value;
        }

        SectionInfo section_info;
        section_info.sections_datas_ = std::move(section_config);
        config_map_[section_name] = std::move(section_info);
    }

    // 输出所有的section和key-value对
    for (const auto& section_entry : config_map_) {
        const std::string& section_name = section_entry.first;
        SectionInfo section_config = section_entry.second;
        std::cout << "[" << section_name << "]" << std::endl;
        for (const auto& key_value_pair : section_config.sections_datas_) {
            std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
        }
    }
}

ConfigMgr::~ConfigMgr()
{
    config_map_.clear();
}

ConfigMgr::ConfigMgr(const ConfigMgr& src)
{
    config_map_ = src.config_map_;
}

SectionInfo ConfigMgr::operator[](const std::string& section)
{
    if (!config_map_.contains(section))
        return SectionInfo();
    return config_map_[section];
}

ConfigMgr& ConfigMgr::operator=(const ConfigMgr &src)
{
    if (this == &src)
        return *this;
    config_map_ = src.config_map_;
    return *this;
}
