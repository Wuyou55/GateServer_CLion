//
// Created by wuyou on 25-4-16.
//

#ifndef CONFIGMGR_H
#define CONFIGMGR_H
#include "Const.h"

struct SectionInfo
{
    SectionInfo()
    {
    };
    ~SectionInfo() { sections_datas_.clear(); };

    SectionInfo(const SectionInfo &src);
    SectionInfo& operator=(const SectionInfo &src);

    std::map<std::string, std::string> sections_datas_;
    std::string operator[](std::string key);
};

class ConfigMgr
{
public:
    ConfigMgr();
    ~ConfigMgr();

    ConfigMgr(const ConfigMgr &src);
    ConfigMgr& operator=(const ConfigMgr &src);

    SectionInfo operator[](const std::string& key);
private:
    std::map<std::string, SectionInfo> config_map_;
};


#endif //CONFIGMGR_H
