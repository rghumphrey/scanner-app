
#include "FoodItem.h"
#include <iostream>
#include <iomanip>

const std::string& FoodItem::getBarcode() const
{
    return barcode;
}
const std::string& FoodItem::getName() const
{
    return name;
}
const std::string& FoodItem::getBrand() const
{
    return brand;
}

bool FoodItem::isMissing(FoodField field) const
{
    if (missing_fields.count(field)) return true;
    return false;
}
bool FoodItem::hasMissingFields() const
{
    if (missing_fields.empty()) return false;
    return true;
}


void FoodItem::print() const
{
    std::cout << "\n===== Food Item =====\n";

    auto printField = [&](const std::string& label,
                          const std::string& value)
    {
        std::cout << std::left << std::setw(25) << label << ": ";
        if (value.empty()) {
            std::cout << "[MISSING]\n";
        } else {
            std::cout << value << "\n";
        }
    };

    auto printNum = [&](const std::string& label,
                        double value,
                        FoodField field,
                        const std::string& unit = "")
    {
        std::cout << std::left << std::setw(25) << label << ": ";
        if (missing_fields.count(field)) {
            std::cout << "[MISSING]\n";
        } else {
            std::cout << value;
            if (!unit.empty()) std::cout << " " << unit;
            std::cout << "\n";
        }
    };

    printField(fieldToString(FoodField::Barcode), barcode );
    printField("Name", name);
    printField("Brand", brand);
    printField("Store", store);

    printNum("Calories", calories, FoodField::Calories, "kcal");
    printNum("Protein", protein, FoodField::Protein, "g");
    printNum("Fat", fat, FoodField::Fat, "g");
    printNum("Saturated Fat", saturated_fat, FoodField::SaturatedFat, "g");
    printNum("Carbohydrates", carbs, FoodField::Carbs, "g");
    printNum("Added Sugar", added_sugar, FoodField::AddedSugar, "g");
    printNum("Sodium", sodium, FoodField::Sodium, "g");

    printNum("Serving Size", serving_size, FoodField::ServingSize);
    printNum("Servings / Container",
             servings_per_container,
             FoodField::ServingsPerContainer);

    printField("Serving Unit", serving_unit);

    std::cout << "=====================\n";
}
void FoodItem::printMissing() const
{
    if (missing_fields.empty()) {
        std::cout << "No missing fields\n";
        return;
    }

    std::cout << "Missing fields:\n";
    for (const auto& field : missing_fields) {
        std::cout << " - " << fieldToString(field) << "\n";
    }
}

nlohmann::json to_json(const FoodItem& item)
{
    nlohmann::json j;

    j["barcode"] = item.barcode;
    j["name"] = item.name;
    j["brand"] = item.brand;
    j["store"] = item.store;
    j["fat"] = item.fat;
    j["carbs"] = item.carbs;
    j["protein"] = item.protein;
    j["saturated_fat"] = item.saturated_fat;
    j["added_sugar"] = item.added_sugar;
    j["calories"] = item.calories;
    j["serving_size"] = item.serving_size;
    j["servings_per_container"] = item.servings_per_container;
    j["serving_unit"] = item.serving_unit;

    for (auto f : item.missing_fields)
        j["missing_fields"].push_back(fieldToString(f));

    return j;
}

FoodItem FoodItem::from_json(const nlohmann::json& j)
{
    FoodItem item;
    item.barcode = j.value("barcode", "");
    item.name = j.value("name", "");
    item.brand = j.value("brand", "");
    item.store = j.value("store", "");
    item.fat = j.value("fat", 0.0);
    item.carbs = j.value("carbs", 0.0);
    item.protein = j.value("protein", 0.0);
    item.saturated_fat = j.value("saturated_fat", 0.0);
    item.added_sugar = j.value("added_sugar", 0.0);
    item.calories = j.value("calories", 0.0);
    item.serving_size = j.value("serving_size", 0.0);
    item.servings_per_container = j.value("servings_per_container", 0.0);
    item.serving_unit = j.value("serving_unit", "");
    item.price = j.value("price", 0.0);

    if (j.contains("missing_fields")) {
        for (const auto& f : j["missing_fields"])
            item.missing_fields.insert(fromString(f.get<std::string>()));
    }

    return item;
}
