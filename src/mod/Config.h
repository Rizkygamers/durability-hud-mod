#pragma once

#include <string>
#include <string_view>

#include <pl/Config.hpp>

namespace durability_hud {

struct ModConfig {
    int version = 1;
    bool enabled = true;
    bool showArmor = true;
    bool showHeldItem = true;
    bool showPercentage = false;
};

nlohmann::json makeDefaultConfigJson();
nlohmann::json makeConfigSchemaJson();

} // namespace durability_hud

template <> struct pl::config::Schema<durability_hud::ModConfig> {
    static constexpr std::string_view title = "Durability HUD Config";
    static constexpr std::string_view description = {};

    static constexpr FieldSchema field(std::string_view name) {
        if (name == "version")
            return {.title = "Version", .readOnly = true};
        if (name == "enabled")
            return {.title = "Enabled", .description = "Turns Durability HUD on or off."};
        if (name == "showArmor")
            return {.title = "Show Armor", .description = "Show durability for equipped armor."};
        if (name == "showHeldItem")
            return {.title = "Show Held Item", .description = "Show durability for held tool/weapon."};
        if (name == "showPercentage")
            return {.title = "Show Percentage", .description = "Display percentage instead of raw numbers."};
        return {};
    }
};
