#include "mod/MyMod.h"

#include <dlfcn.h>
#include <filesystem>

#include <pl/Mod.hpp>

namespace durability_hud {

DurabilityHudMod &DurabilityHudMod::instance() {
    static DurabilityHudMod instance;
    return instance;
}

DurabilityHudMod::DurabilityHudMod() : mSelf(*ll::mod::NativeMod::current()) {}

bool DurabilityHudMod::load() {
    auto &self = getSelf();
    self.getLogger().debug("Loading...");

    std::error_code ec;
    std::filesystem::create_directories(self.getDataDir(), ec);
    if (ec) {
        self.getLogger().error("Failed to create data directory {}: {}", self.getDataDir().string(),
                               ec.message());
        return false;
    }

    std::filesystem::create_directories(self.getConfigDir(), ec);
    if (ec) {
        self.getLogger().error("Failed to create config directory {}: {}",
                               self.getConfigDir().string(), ec.message());
        return false;
    }

    mConfigFile.emplace();
    if (!mConfigFile->load()) {
        self.getLogger().warn("Failed to load typed config");
        return false;
    }
    mConfig = mConfigFile->value();

    self.getLogger().info("Loaded {} from {}", self.getName(), self.getModDir().string());
    return true;
}

bool DurabilityHudMod::enable() {
    auto &self = getSelf();
    if (!mConfig.enabled) {
        self.getLogger().info("durability_hud is disabled by config");
        return true;
    }

    // Probe libminecraftpe.so symbols safely (read-only) before hooking in a later step.
    void *mc = dlopen("libminecraftpe.so", RTLD_NOLOAD);
    if (!mc) {
        self.getLogger().warn("libminecraftpe.so not loaded yet");
        return true;
    }

    const char *probes[] = {
        "_ZNK13ItemStackBase14getDamageValueEv",
        "_ZNK13ItemStackBase12getMaxDamageEv",
        "_ZNK6Player15getSelectedItemEv",
        "_ZNK5Actor8getArmorE9ArmorSlot",
    };
    for (const char *sym : probes) {
        void *addr = dlsym(mc, sym);
        self.getLogger().info("probe {} -> {}", sym, addr ? "FOUND" : "missing");
    }
    return true;
}

bool DurabilityHudMod::disable() {
    getSelf().getLogger().debug("Disabling...");
    return true;
}

bool DurabilityHudMod::unload() {
    getSelf().getLogger().debug("Unloading...");
    mConfigFile.reset();
    return true;
}

} // namespace durability_hud
