#pragma once

#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace waybar::modules::hyprland {

class WorkspaceSelector {
public:
    // Main selector types
    struct RangeSelector {
        int start;
        int end;
    };

    struct SpecialSelector {
        bool isSpecial;
    };

    struct NameSelector {
        enum class Type {
            BOOL,       // n[bool]
            STARTS_WITH,// n[s:string]
            ENDS_WITH   // n[e:string]
        };
        Type type;
        std::variant<bool, std::string> value;
    };

    struct MonitorSelector {
        std::string monitor;
    };

    struct WindowSelector {
        bool countTiledOnly = false;    // t flag
        bool countFloatingOnly = false; // f flag
        bool countGroups = false;       // g flag
        bool countVisible = false;      // v flag
        std::optional<int> exactCount;  // single number
        std::optional<RangeSelector> range; // range A-B
    };

    struct FullscreenSelector {
        int state; // -1: no fullscreen, 0: fullscreen, 1: maximized, 2: fullscreen without state
    };

    // Constructor takes space-delimited selector string
    explicit WorkspaceSelector(const std::string& selector_string);

    // Returns true if ALL selectors match the workspace
    bool matches(const Workspace& workspace) const;

private:
    // Parse individual selector strings
    void parseSelector(const std::string& selector);
    
    // Individual selector parsers
    std::optional<RangeSelector> parseRangeSelector(const std::string& selector);
    std::optional<SpecialSelector> parseSpecialSelector(const std::string& selector);
    std::optional<NameSelector> parseNameSelector(const std::string& selector);
    std::optional<MonitorSelector> parseMonitorSelector(const std::string& selector);
    std::optional<WindowSelector> parseWindowSelector(const std::string& selector);
    std::optional<FullscreenSelector> parseFullscreenSelector(const std::string& selector);

    // Store parsed selectors
    std::vector<std::variant<
        RangeSelector,
        SpecialSelector,
        NameSelector,
        MonitorSelector,
        WindowSelector,
        FullscreenSelector
    >> selectors_;
};

} // namespace waybar::modules::hyprland
