
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "OpenFoodFactsClient.h"
#include "FoodDatabase.h"
#include "FoodItem.h"

#include <iostream>
#include <vector>
#include <optional>

using json = nlohmann::json;


// Return first valid field from JSON object (double)
static std::optional<double> getPreferredField(const nlohmann::json& obj, 
                                const std::vector<std::string>& keys )
{
    for (const auto& key : keys) {
        if (obj.contains(key) && !obj[key].is_null()) {
                return obj[key].get<double>();
        }
    }
    
    return std::nullopt;
}


size_t OpenFoodFactsClient::WriteCallback(char *data, size_t size, size_t nmemb, void *resp)
{
    size_t realsize = nmemb * size;
    std::string* str = static_cast<std::string*>(resp);
    str->append(static_cast<char*>(data), realsize);
    return realsize;
}


std::optional<FoodItem> OpenFoodFactsClient::lookup(const std::string& barcode)
{
    if (barcode.empty())
        return std::nullopt;

    /* perform http GET */
    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
        return std::nullopt;

    std::string response;
    std::string url = url_pref + barcode + url_suff;
    
    /* set URL to go to and capture output in response */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);   
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

    /* Perform the request, result gets the return code and cleanup */
    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    /* Check for errors */
    if(result != CURLE_OK){
        std::cerr << "curl_easy_perform() failed" << curl_easy_strerror(result) << "\n";
        return std::nullopt;
    }

    
    /* parse response */
    auto j = json::parse(response, nullptr, false);
    if (j.is_discarded() || j["status"] != 1)
        return std::nullopt;

    const auto& product = j["product"];
    // const auto& nutr = product["nutriments"];

    FoodItem item(barcode);

    // get product info
    std::string str = product.value("product_name", "");
    if (!str.empty()) 
    {
        item.setField(FoodField::Name, str);
    }else {
        item.markMissing(FoodField::Name);
    }

    str = product.value("brands", "");
    if (!str.empty()) 
    {
        item.setField(FoodField::Brand, str);
    }else {
        item.markMissing(FoodField::Brand);
    }

    str = product.value("store", "");
    if (!str.empty()) {
        item.setField(FoodField::Store, str);
    }else {
        item.markMissing(FoodField::Store);
    }

    // get nutrition info
    if (product.contains("nutriments")) {
    auto nutriments = product["nutriments"];

    item.setField(FoodField::Protein, getPreferredField(nutriments, {"proteins_serving", "proteins"}));
    item.setField(FoodField::Fat, getPreferredField(nutriments, {"fat_serving", "fat"}));
    item.setField(FoodField::SaturatedFat, getPreferredField(nutriments, {"saturated-fat_serving", "saturated-fat"}));
    item.setField(FoodField::Carbs, getPreferredField(nutriments, {"carbohydrates_serving", "carbohydrates", "carbs"}));
    item.setField(FoodField::AddedSugar, getPreferredField(nutriments, {"sugars_serving", "sugars", "added-sugars"}));
    item.setField(FoodField::Calories, getPreferredField(nutriments, {"energy-kcal_serving", "energy-kcal"}));
    item.setField(FoodField::ServingUnit, getPreferredField(nutriments, {"serving_unit"}));
    item.setField(FoodField::ServingsPerContainer, getPreferredField(nutriments, {"servings_per_container"}));
    item.setField(FoodField::Sodium, getPreferredField(nutriments, {"sodium"}));

    // finish up product
    // item.setField(FoodField::ServingSize, getPreferredField(product, {"serving_size"}));
    str = product.value("serving_size", "");
    if (!str.empty()) {
        item.setField(FoodField::ServingSize, str);
    }else {
        item.markMissing(FoodField::ServingSize);
    }
}

    return item;
}