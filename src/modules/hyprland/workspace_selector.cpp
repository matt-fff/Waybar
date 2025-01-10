#include "modules/hyprland/workspace_selector.hpp"
#include <sstream>
#include <regex>

namespace waybar::modules::hyprland {

WorkspaceSelector::WorkspaceSelector(const std::string& selector_string) {
    std::istringstream stream(selector_string);
    std::string selector;
    
    // Split on spaces and parse each selector
    while (stream >> selector) {
        parseSelector(selector);
    }
}

void WorkspaceSelector::parseSelector(const std::string& selector) {
    if (auto range = parseRangeSelector(selector)) {
        selectors_.push_back(*range);
    } else if (auto special = parseSpecialSelector(selector)) {
        selectors_.push_back(*special);
    } else if (auto name = parseNameSelector(selector)) {
        selectors_.push_back(*name);
    } else if (auto monitor = parseMonitorSelector(selector)) {
        selectors_.push_back(*monitor);
    } else if (auto window = parseWindowSelector(selector)) {
        selectors_.push_back(*window);
    } else if (auto fullscreen = parseFullscreenSelector(selector)) {
        selectors_.push_back(*fullscreen);
    }
    // If no match, selector is invalid and ignored
}

std::optional<WorkspaceSelector::RangeSelector> 
WorkspaceSelector::parseRangeSelector(const std::string& selector) {
    std::regex range_regex(R"(r\[(\d+)-(\d+)\])");
    std::smatch matches;
    
    if (std::regex_match(selector, matches, range_regex)) {
        return RangeSelector{
            .start = std::stoi(matches[1].str()),
            .end = std::stoi(matches[2].str())
        };
    }
    return std::nullopt;
}

std::optional<WorkspaceSelector::SpecialSelector>
WorkspaceSelector::parseSpecialSelector(const std::string& selector) {
    std::regex special_regex(R"(s\[(true|false)\])");
    std::smatch matches;
    
    if (std::regex_match(selector, matches, special_regex)) {
        return SpecialSelector{
            .isSpecial = matches[1].str() == "true"
        };
    }
    return std::nullopt;
}

std::optional<WorkspaceSelector::NameSelector>
WorkspaceSelector::parseNameSelector(const std::string& selector) {
    std::regex name_bool_regex(R"(n\[(true|false)\])");
    std::regex name_starts_regex(R"(n\[s:(.*)\])");
    std::regex name_ends_regex(R"(n\[e:(.*)\])");
    std::smatch matches;
    
    if (std::regex_match(selector, matches, name_bool_regex)) {
        return NameSelector{
            .type = NameSelector::Type::BOOL,
            .value = matches[1].str() == "true"
        };
    } else if (std::regex_match(selector, matches, name_starts_regex)) {
        return NameSelector{
            .type = NameSelector::Type::STARTS_WITH,
            .value = matches[1].str()
        };
    } else if (std::regex_match(selector, matches, name_ends_regex)) {
        return NameSelector{
            .type = NameSelector::Type::ENDS_WITH,
            .value = matches[1].str()
        };
    }
    return std::nullopt;
}

std::optional<WorkspaceSelector::WindowSelector>
WorkspaceSelector::parseWindowSelector(const std::string& selector) {
    std::regex window_regex(R"(w\[([tfgv]*)(?:(\d+)(?:-(\d+))?)?\])");
    std::smatch matches;
    
    if (std::regex_match(selector, matches, window_regex)) {
        WindowSelector ws;
        
        // Parse flags
        std::string flags = matches[1].str();
        ws.countTiledOnly = flags.find('t') != std::string::npos;
        ws.countFloatingOnly = flags.find('f') != std::string::npos;
        ws.countGroups = flags.find('g') != std::string::npos;
        ws.countVisible = flags.find('v') != std::string::npos;
        
        // Parse count/range
        if (matches[3].matched) {  // Range
            ws.range = RangeSelector{
                .start = std::stoi(matches[2].str()),
                .end = std::stoi(matches[3].str())
            };
        } else if (matches[2].matched) {  // Single number
            ws.exactCount = std::stoi(matches[2].str());
        }
        
        return ws;
    }
    return std::nullopt;
}

std::optional<WorkspaceSelector::FullscreenSelector>
WorkspaceSelector::parseFullscreenSelector(const std::string& selector) {
    std::regex fullscreen_regex(R"(f\[(-?\d+)\])");
    std::smatch matches;
    
    if (std::regex_match(selector, matches, fullscreen_regex)) {
        return FullscreenSelector{
            .state = std::stoi(matches[1].str())
        };
    }
    return std::nullopt;
}

std::optional<WorkspaceSelector::MonitorSelector>
WorkspaceSelector::parseMonitorSelector(const std::string& selector) {
    std::regex monitor_regex(R"(m\[(.*)\])");
    std::smatch matches;
    
    if (std::regex_match(selector, matches, monitor_regex)) {
        return MonitorSelector{
            .monitor = matches[1].str()
        };
    }
    return std::nullopt;
}

// The matches function will need to be implemented based on how workspace info is provided
bool WorkspaceSelector::matches(const Workspace& workspace) const {
    // TODO: Implement matching logic based on workspace info structure
    return false;
}

} // namespace waybar::modules::hyprland
