/*
 * @FilePath: \src\utils.cpp
 * @Author: Fantety
 * @Descripttion: 
 * @Date: 2024-10-28 10:06:10
 * @LastEditors: Fantety
 * @LastEditTime: 2024-10-28 10:06:27
 */
#include "utils.h"

#include <iomanip>
#include <iostream>

std::optional<std::size_t> Utils::getUserInputInt(const std::string& line, std::size_t max) {
    std::size_t ret;

    while (!std::cin.eof()) {
        std::cout << line << " (0-" << max << "): ";
        std::cin >> ret;

        if (!std::cin) {
            return {};
        }

        if (ret <= max) {
            return ret;
        }
    }
    return {};
}

std::optional<SimpleBLE::Adapter> Utils::getAdapter() {
    if (!SimpleBLE::Adapter::bluetooth_enabled()) {
        std::cout << "Bluetooth is not enabled!" << std::endl;
        return {};
    }

    auto adapter_list = SimpleBLE::Adapter::get_adapters();

    // no adapter found
    if (adapter_list.empty()) {
        std::cerr << "No adapter was found." << std::endl;
        return {};
    }

    // only one found, returning directly
    if (adapter_list.size() == 1) {
        auto adapter = adapter_list.at(0);
        std::cout << "Using adapter: " << adapter.identifier() << " [" << adapter.address() << "]" << std::endl;
        return adapter;
    }

    // multiple adapters found, ask user to select one
    std::cout << "Available adapters:" << std::endl;
    int i = 0;
    for (auto& adapter : adapter_list) {
        std::cout << "[" << i++ << "] " << adapter.identifier() << " [" << adapter.address() << "]" << std::endl;
    }

    auto adapter_selection = Utils::getUserInputInt("Select an adapter", adapter_list.size() - 1);

    if (!adapter_selection.has_value()) {
        return {};
    }

    return adapter_list[adapter_selection.value()];
}