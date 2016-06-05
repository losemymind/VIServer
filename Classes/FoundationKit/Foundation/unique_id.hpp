
#pragma once
#include <chrono>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include "FoundationKit/Base/Types.h"
#include "FoundationKit/Platform/Platform.h"

NS_FK_BEGIN

const uint64 INVALID_UNIQUE_ID = -1;
static const  uint64_t sequence_max = (std::numeric_limits<uint64_t>::max)();
struct unique_id
{
    static uint64_t sequence;
    static uint64_t machineid;
    static uint64 create()
    {
        int64 uniqueid = 0;
        int64 timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();

        if (machineid == 0)
        {
            std::vector<uint8> machineid_arr = Platform::getMacAddressRaw();
            std::stringstream ss;
            ss << std::hex << std::setfill('0');

            for (unsigned i = 0; i < 6; ++i)
                ss << std::setw(2) << int(machineid_arr[i]);

            ss >> machineid;

            if (machineid == 0)
                return INVALID_UNIQUE_ID;
        }

        uniqueid  = timestamp << 22;
        uniqueid |= (machineid & 0x3FF) << 12;
        uniqueid |= sequence++ & 0xFFF;
        if (sequence == sequence_max)
        {
            sequence = 0;
        }
        return uniqueid;
    }
};



NS_FK_END

