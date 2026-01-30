#include "FoodDatabase.h"
#include "FoodItem.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

bool FoodDatabase::addOrUpdate(const FoodItem& item)
{
    if (item.getBarcode().empty())
        return false;

    items[item.getBarcode()] = item;
    return true;
}

std::optional<FoodItem>
FoodDatabase::get(const std::string& barcode) const
{
    auto it = items.find(barcode);
    if (it == items.end())
        return std::nullopt;
    return it->second;
}

bool FoodDatabase::contains(const std::string& barcode) const
{
    return items.count(barcode) > 0;
}

bool FoodDatabase::save(const std::string& filename) const
{
    nlohmann::json j;

    for (const auto& [barcode, item] : items)
        j[barcode] = to_json(item);

    std::ofstream out(filename);
    if (!out) return false;

    out << j.dump(2);
    return true;
}

bool FoodDatabase::load(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in) return false;

    nlohmann::json j;
    in >> j;

    items.clear();
    for (auto& [barcode, val] : j.items())
        items[barcode] = FoodItem::from_json(val);

    return true;
}

void FoodDatabase::printSummary() const
{
    std::cout << "Food database (" << items.size() << " items):\n";
    for (const auto& [barcode, item] : items) {
        std::cout << "- " << barcode << ": "
                  << item.getName() << " (" << item.getBrand() << ")\n";
    }
}
