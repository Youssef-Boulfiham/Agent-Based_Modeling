#pragma once

#include <string>
#include <vector>

enum class Domain {
    Unknown = 0,
    Home,
    School,
    Friends,
    Leisure,
    Path,
    Public
};

inline std::string domainToString(Domain domain) {
    switch (domain) {
        case Domain::Home: return "home";
        case Domain::School: return "school";
        case Domain::Friends: return "friends";
        case Domain::Leisure: return "leisure";
        case Domain::Path: return "path";
        case Domain::Public: return "public";
        default: return "unknown";
    }
}

inline std::vector<Domain> allDomains() {
    return {Domain::Home, Domain::School, Domain::Friends, Domain::Leisure, Domain::Path, Domain::Public};
}
