#ifndef OPENFOODFACTSCLIENT_H_
#define OPENFOODFACTSCLIENT_H_

#include <string>
#include <optional>
#include "FoodDatabase.h"



class OpenFoodFactsClient {
public:
    // OpenFoodFactsClient();
    std::optional<FoodItem> lookup(const std::string& barcode);
private:
    const std::string url_pref = "https://world.openfoodfacts.net/api/v2/product/";
    const std::string url_suff = "?fields=product_name,brands,stores,nutriments,serving_size,serving_quantity,serving_unit";
    std::string url;
    // staging = https://world.openfoodfacts.net

    static size_t WriteCallback(char *data, size_t size, size_t nmemb, void *resp);
};

#endif