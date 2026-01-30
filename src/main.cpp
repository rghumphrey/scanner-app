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

// #define TEST


int main()
{

    // std::string s = "HeLLo";
    // std::cout << "str = " << s << std::endl;
    // std::string c = to_lower(s);
    // std::cout << "lower str = " << c << std::endl;



    #ifndef TEST
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
        std::cout << item->getName() << std::endl;
        item->print();
        item->printMissing();
        item->fix_missing();
        item->fix_fields();
        item->print();
    }


    curl_global_cleanup();
    #endif

    
    return 0;
}
