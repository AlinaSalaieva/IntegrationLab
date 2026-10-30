#pragma once
#include <iostream>
#include <string>
#include <utility>
class ScopeLogger {
    std::string name_;
public:
    explicit ScopeLogger(std::string name) : name_(std::move(name)) {
        std::cout << "[ВХІД] " << name_ << '\n';
    }
    ~ScopeLogger() noexcept {
        try { std::cout << "[ВИХІД] " << name_ << '\n'; }
        catch (...) {  }
    }
    ScopeLogger(const ScopeLogger&) = delete;
    ScopeLogger& operator=(const ScopeLogger&) = delete;
};
