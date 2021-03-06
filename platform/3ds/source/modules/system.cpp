#include <3ds.h>
#include "modules/system/system.h"

#include "common/results.h"

using namespace love;

int System::GetProcessorCount()
{
    if (this->systemInfo.processors != 0)
        return this->systemInfo.processors;

    uint8_t model = 0;
    CFGU_GetSystemModel(&model);

    int processorCount = 2;

    switch (model)
    {
        case CFG_MODEL_N3DS:
        case CFG_MODEL_N3DSXL:
        case CFG_MODEL_N2DSXL:
            processorCount = 4;
            break;
    }

    this->systemInfo.processors = processorCount;

    return this->systemInfo.processors;
}

const std::string & System::GetModel()
{
    if (!this->systemInfo.model.empty())
        return this->systemInfo.model;

    uint8_t model = 0;

    R_UNLESS(CFGU_GetSystemModel(&model), LOVE_STRING_EMPTY);

    const char * name = nullptr;
    if (!System::GetConstant(static_cast<CFG_SystemModel>(model), name))
        name = "Unknown";

    this->systemInfo.model = name;

    return this->systemInfo.model;
}

System::PowerInfo System::GetPowerInfo() const
{
    u8 batteryPercent = 100;
    u8 batteryState = 0;

    MCUHWC_GetBatteryLevel(&batteryPercent);
    PTMU_GetBatteryChargeState(&batteryState);

    PowerInfo info;

    info.percentage = batteryPercent;
    info.state = (batteryState == 1 && batteryPercent == 100) ? "charged" :
                 (batteryState == 1) ? "charging" : "battery";

    return info;
}

const std::string & System::GetUsername()
{
    if (!this->systemInfo.username.empty())
        return this->systemInfo.username;

    char username[USERNAME_LENGTH] = {0};

    R_UNLESS(FRD_GetMyScreenName(username, USERNAME_LENGTH), LOVE_STRING_EMPTY);

    this->systemInfo.username = username;

    return this->systemInfo.username;
}

const std::string & System::GetRegion()
{
    if (!this->systemInfo.region.empty())
        return this->systemInfo.region;

    uint8_t region = 0;

    R_UNLESS(CFGU_SecureInfoGetRegion(&region), LOVE_STRING_EMPTY);

    const char * name = nullptr;
    if (!System::GetConstant(static_cast<CFG_Region>(region), name))
        name = "Unknown";

    this->systemInfo.region = name;

    return this->systemInfo.region;
}

System::NetworkInfo System::GetNetworkInfo() const
{
    u32 status = 0;
    ACU_GetWifiStatus(&status);

    NetworkInfo info;

    info.signal = osGetWifiStrength();
    info.status = (status > 0) ? "connected" : "disconnected";

    return info;
}

const std::string & System::GetLanguage()
{
    if (!this->systemInfo.language.empty())
        return this->systemInfo.language;

    uint8_t language;

    R_UNLESS(CFGU_GetSystemLanguage(&language), LOVE_STRING_EMPTY);

    const char * name = nullptr;
    if (!System::GetConstant(static_cast<CFG_Language>(language), name))
        name = "Unknown";

    this->systemInfo.language = name;

    return this->systemInfo.language;
}

const std::string & System::GetVersion()
{
    if (!this->systemInfo.version.empty())
        return this->systemInfo.version;

    char out[256] = { 0 };

    R_UNLESS(osGetSystemVersionDataString(NULL, NULL, out, 256), LOVE_STRING_EMPTY);

    this->systemInfo.version = out;
    return this->systemInfo.version;
}

/* Friend Code stuff */

static inline std::string MAKE_FRIEND_CODE(uint64_t friendCode)
{
    std::string out = "####-####-####";

    sprintf(out.data(), "%04i-%04i-%04i",
            (int)(friendCode / 100000000) % 10000,
            (int)(friendCode / 10000) % 10000,
            (int)(friendCode % 10000));

    return out;
}

const std::string & System::GetFriendCode()
{
    if (!this->systemInfo.friendCode.empty())
        return this->systemInfo.friendCode;

    FriendKey key;
    uint64_t friendCode;

    /* Get the Friend Key for the user */
    R_UNLESS(FRD_GetMyFriendKey(&key), LOVE_STRING_EMPTY);

    /* Convert the principalId to friendCode */
    R_UNLESS(FRD_PrincipalIdToFriendCode(key.principalId, &friendCode), LOVE_STRING_EMPTY);

    this->systemInfo.friendCode = MAKE_FRIEND_CODE(friendCode);

    return this->systemInfo.friendCode;
}

/* LANGUAGE CONSTANTS */

bool System::GetConstant(const char * in, CFG_Language & out)
{
    return System::languages.Find(in, out);
}

bool System::GetConstant(CFG_Language in, const char *& out)
{
    return System::languages.Find(in, out);
}

std::vector<std::string> System::GetConstants(CFG_Language)
{
    return System::languages.GetNames();
}

StringMap<CFG_Language, System::MAX_LANGUAGES>::Entry System::languageEntries[] =
{
    { "Japanese",            CFG_LANGUAGE_JP },
    { "English",             CFG_LANGUAGE_EN },
    { "French",              CFG_LANGUAGE_FR },
    { "German",              CFG_LANGUAGE_DE },
    { "Italian",             CFG_LANGUAGE_IT },
    { "Spanish",             CFG_LANGUAGE_ES },
    { "Simplified Chinese",  CFG_LANGUAGE_ZH },
    { "Korean",              CFG_LANGUAGE_KO },
    { "Dutch",               CFG_LANGUAGE_NL },
    { "Portugese",           CFG_LANGUAGE_PT },
    { "Russian",             CFG_LANGUAGE_RU },
    { "Traditional Chinese", CFG_LANGUAGE_TW }
};

StringMap<CFG_Language, System::MAX_LANGUAGES> System::languages(System::languageEntries, sizeof(System::languageEntries));

/* MODEL CONSTANTS */

bool System::GetConstant(const char * in, CFG_SystemModel & out)
{
    return System::models.Find(in, out);
}

bool System::GetConstant(CFG_SystemModel in, const char *& out)
{
    return System::models.Find(in, out);
}

std::vector<std::string> System::GetConstants(CFG_SystemModel)
{
    return System::models.GetNames();
}

StringMap<CFG_SystemModel, System::MAX_MODELS>::Entry System::modelEntries[] =
{
    { "3DS",       CFG_MODEL_3DS    },
    { "3DSXL",     CFG_MODEL_3DSXL  },
    { "New 3DS",   CFG_MODEL_N3DS   },
    { "2DS",       CFG_MODEL_2DS    },
    { "New 3DSXL", CFG_MODEL_N3DSXL },
    { "New 2DSXL", CFG_MODEL_N2DSXL },
};

StringMap<CFG_SystemModel, System::MAX_MODELS> System::models(System::modelEntries, sizeof(System::modelEntries));

/* REGION CONSTANTS */

bool System::GetConstant(const char * in, CFG_Region & out)
{
    return System::regions.Find(in, out);
}

bool System::GetConstant(CFG_Region in, const char *& out)
{
    return System::regions.Find(in, out);
}

std::vector<std::string> System::GetConstants(CFG_Region)
{
    return System::regions.GetNames();
}

StringMap<CFG_Region, System::MAX_REGIONS>::Entry System::regionEntries[] =
{
    { "Japan",         CFG_REGION_JPN },
    { "United States", CFG_REGION_USA },
    { "Europe",        CFG_REGION_EUR },
    { "Australia",     CFG_REGION_AUS },
    { "China",         CFG_REGION_CHN },
    { "Korea",         CFG_REGION_KOR },
    { "Taiwan",        CFG_REGION_TWN },
};

StringMap<CFG_Region, System::MAX_REGIONS> System::regions(System::regionEntries, sizeof(System::regionEntries));
