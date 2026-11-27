#pragma once

#include <optional>

#include "mod/Config.h"

#include <pl/Mod.hpp>

namespace durability_hud {

class DurabilityHudMod {
  public:
    static DurabilityHudMod &instance();

    DurabilityHudMod();

    [[nodiscard]] ll::mod::NativeMod &getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

  private:
    ll::mod::NativeMod &mSelf;
    ModConfig mConfig;
    std::optional<pl::config::ConfigFile<ModConfig>> mConfigFile;
};

} // namespace durability_hud
