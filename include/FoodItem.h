#ifndef FOODITEM_H_
#define FOODITEM_H_

#include <string>
#include <set>
#include <iostream>
#include <optional>
#include <nlohmann/json.hpp>

enum class FoodField {
    Name,
    Brand,
    Store,
    Barcode,
    Protein,
    Fat,
    Carbs,
    Fiber,
    SaturatedFat,
    AddedSugar,
    Calories,
    ServingSize,
    ServingsPerContainer,
    ServingUnit,
    Sodium,
    Price
};

// Helper to convert enum to string for missing fields / UI
inline std::string fieldToString(FoodField f) {
    switch (f) {
        case FoodField::Name: return "name";
        case FoodField::Brand: return "brand";
        case FoodField::Store: return "store";
        case FoodField::Barcode: return "barcode";
        case FoodField::Protein: return "protein";
        case FoodField::Fat: return "fat";
        case FoodField::Carbs: return "carbs";
        case FoodField::Fiber: return "fiber";
        case FoodField::SaturatedFat: return "saturated_fat";
        case FoodField::AddedSugar: return "added_sugar";
        case FoodField::Calories: return "calories";
        case FoodField::ServingSize: return "serving_size";
        case FoodField::ServingsPerContainer: return "servings_per_container";
        case FoodField::ServingUnit: return "serving_unit";
        case FoodField::Sodium: return "sodium";
        case FoodField::Price: return "price";
        default: return "unknown";
    }
}
inline FoodField fromString(std::string value) {
    if (value == "name") return FoodField::Name;
    else if (value == "brand") return FoodField::Brand;
    else if (value == "store") return FoodField::Store;
    else if (value == "barcode") return FoodField::Barcode;
    else if (value == "protein") return FoodField::Protein;
    else if (value == "fat") return FoodField::Fat;
    else if (value == "carbs") return FoodField::Carbs;
    else if (value == "fiber") return FoodField::Fiber;
    else if (value == "saturated_fat" || value == "saturated fat") return FoodField::SaturatedFat;
    else if (value == "added_sugar" || value == "added sugar") return FoodField::AddedSugar;
    else if (value == "calories") return FoodField::Calories;
    else if (value == "serving_size" || value == "serving size") return FoodField::ServingSize;
    else if (value == "servings_per_container" || value == "servings per container") return FoodField::ServingsPerContainer;
    else if (value == "serving_unit" || value == "serving unit") return FoodField::ServingUnit;
    else if (value == "sodium") return FoodField::Sodium;
    else if (value == "price") return FoodField::Price;
    else return FoodField::Name; // RETURN UNKNOWN?
}
inline bool isDoubleField(FoodField f)
{
    switch (f) {
        case FoodField::Protein:
        case FoodField::Fat:
        case FoodField::Carbs:
        case FoodField::SaturatedFat:
        case FoodField::AddedSugar: 
        case FoodField::Calories: 
        case FoodField::ServingSize:
        case FoodField::ServingsPerContainer: 
        case FoodField::Sodium:
        case FoodField::Price: return true;

        case FoodField::Name: 
        case FoodField::Brand: 
        case FoodField::Store: 
        case FoodField::Barcode: 
        case FoodField::ServingUnit: 
        default: return false;
    } 
}

class FoodItem {
public:
    FoodItem(std::string barcode = "") : barcode(std::move(barcode)) {}

    // getting functions
    const std::string& getBarcode() const;
    const std::string& getName() const;
    const std::string& getBrand() const;
    double get_field_double(FoodField field){
        double value;
        switch (field) {
            case FoodField::Protein: value = protein; break;
            case FoodField::Fat: value = fat; break;
            case FoodField::Carbs: value = carbs; break;
            case FoodField::SaturatedFat: value = saturated_fat; break;
            case FoodField::AddedSugar: value = added_sugar; break;
            case FoodField::Calories: value = calories; break;
            case FoodField::Sodium: value = sodium; break;
            case FoodField::ServingSize: value = serving_size; break;
            case FoodField::ServingsPerContainer: value = servings_per_container; break;
            default: return 0.0;
        }
        return value;
    }
    std::string get_field_string(FoodField field){
        std::string value;
        switch (field) {
            case FoodField::Name: value = name; break;
            case FoodField::Brand: value = brand; break;
            case FoodField::Store: value = store; break;
            case FoodField::ServingUnit: value = serving_unit; break;
            case FoodField::Barcode: value = barcode; break;
            default: return "";
        }
        return value;
    }

    // setting functions
    void setField(FoodField field, double value) { //doubles
        switch (field) {
            case FoodField::Protein: protein = value; break;
            case FoodField::Fat: fat = value; break;
            case FoodField::Carbs: carbs = value; break;
            case FoodField::Fiber: fiber = value; break;
            case FoodField::SaturatedFat: saturated_fat = value; break;
            case FoodField::AddedSugar: added_sugar = value; break;
            case FoodField::Calories: calories = value; break;
            case FoodField::ServingSize: serving_size = value; break;
            case FoodField::ServingsPerContainer: servings_per_container = value; break;
            case FoodField::Sodium: sodium = value; break;
            case FoodField::Price: price = value; break;
            default: return;
        }
        missing_fields.erase(field);
    }
    void setField(FoodField field, std::optional<double> value) { // optional doubles
        if (!value.has_value())
        {
            markMissing(field);
            return;
        }
        clearMissing(field);
        switch (field) {
            case FoodField::Protein: protein = *value; break;
            case FoodField::Fat: fat = *value; break;
            case FoodField::Carbs: carbs = *value; break;
            case FoodField::Fiber: fiber = *value; break;
            case FoodField::SaturatedFat: saturated_fat = *value; break;
            case FoodField::AddedSugar: added_sugar = *value; break;
            case FoodField::Calories: calories = *value; break;
            case FoodField::ServingSize: serving_size = *value; break;
            case FoodField::ServingsPerContainer: servings_per_container = *value; break;
            case FoodField::Sodium: sodium = *value; break;
            case FoodField::Price: price = *value; break;
            default: return;
        }
    }
    void setField(FoodField field, const std::string& value) { //strings
        switch (field) {
            case FoodField::Name: name = value; break;
            case FoodField::Brand: brand = value; break;
            case FoodField::Store: store = value; break;
            case FoodField::ServingUnit: serving_unit = value; break;
            case FoodField::Barcode: barcode = value; break;
            default: return;
        }
        missing_fields.erase(field);
    }

    // UI functions
    void fix_missing();
    void fix_fields();

    // Mark missing and clear missing
    void markMissing(FoodField field) { missing_fields.insert(field); }
    void clearMissing(FoodField field) { missing_fields.erase(field); }
    bool isMissing(FoodField field) const;  //IMPLEMENT
    bool hasMissingFields() const; //IMPLEMENT

    /* print functions */
    void print() const;
    void printMissing() const;

    /* for file save/load */
    friend nlohmann::json to_json(const FoodItem&);
    static FoodItem from_json(const nlohmann::json&);

private:
    std::string barcode;
    std::string name;
    std::string brand;
    std::string store;

    double fat = 0;
    double carbs = 0;
    double fiber = 0.0;
    double protein = 0;
    double saturated_fat = 0;
    double added_sugar = 0;
    double sodium;
    double calories = 0;
    double serving_size = 0;
    double servings_per_container = 0;

    double price = 0.0;

    std::string serving_unit;

    std::set<FoodField> missing_fields;

};



#endif