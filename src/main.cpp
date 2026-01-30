#include <opencv2/opencv.hpp>

#include <ZXing/ReadBarcode.h>
#include <ZXing/TextUtfEncoding.h>
#include <ZXing/ReaderOptions.h>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <iostream>

#include "BarcodeScanner.h"
#include "OpenFoodFactsClient.h"
#include "FoodDatabase.h"
#include "config.h"


int main()
{
    curl_global_init(CURL_GLOBAL_ALL);

    BarcodeScanner scanner("http://admin:admin@100.110.182.18:8081/video");
    OpenFoodFactsClient client;
    
    FoodDatabase db;
    db.load(Config::FoodDatabasePath);


    /* video reception */

    std::string barcode = scanner.scan();
    if (barcode.empty()) return 0;
    
    std::optional<FoodItem> item(barcode);
    item = client.lookup(barcode);
    if (item)
    {
        std::cout << item->get_field_string(FoodField::Name) << std::endl;
        item->print();
        item->printMissing();
    }


    curl_global_cleanup();

    
    return 0;
}
