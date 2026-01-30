#ifndef FOODDATABASE_H_
#define FOODDATABASE_H_

#include <string>
#include <unordered_map>
#include <set>

#include "FoodItem.h"

#include <nlohmann/json.hpp>



class FoodDatabase {
public:
    // FoodDatabase();
    // ~FoodDatabase();
    bool addOrUpdate(const FoodItem& item);
    bool contains(const std::string& barcode) const;
    void remove(const FoodItem& item);

    std::optional<FoodItem> get(const std::string& barcode) const;

    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

    void printSummary() const;

private:
    std::unordered_map<std::string, FoodItem> items;
};




#endif