#include "dataMatrixGenerator.h"


#include <iostream>
#include <aspose_barcode_api_defs.h>
#include "BarCode.Generation/BarcodeGenerator.h"
#include "System/idisposable.h"
#include <BarCode.Generation/GenerationParameters/BarcodeParameters.h>
#include <BarCode.Generation/GenerationParameters/BaseGenerationParameters.h>
#include <BarCode.Generation/BarcodeGenerator.h>
#include <string>
#include <exception>
#include <stdexcept>

#include "version_info/version_info.h"

#include <system/aspose_cpp_library.h>

#include <system/exception.h>
#include <system/string.h>
#include <system/console.h>
#include <system/io/path.h>

#include <Licensing/License.h>

#include <BarCode.Common/Exceptions/BarCodeException.h>
#include <BarCodeRecognition/Recognition/RecognitionSession/BarCodeReader.h>
#include <BarCode.Generation/BarcodeGenerator.h>

#include <BarCode.Generation/EncodeTypes/BaseEncodeType.h>
#include <BarCode.Generation/EncodeTypes/EncodeTypes.h>
#include <BarCode.Generation/EncodeTypes/SymbologyEncodeType.h>

#include <BarCodeRecognition/Recognition/RecognitionSession/DecodeTypes/SingleDecodeType.h>
#include <BarCodeRecognition/Recognition/RecognitionSession/DecodeTypes/DecodeType.h>

#include <BarCodeRecognition/Recognition/RecognitionSession/BarcodeResults/BarCodeResult.h>
#include <fstream>

using namespace Aspose::BarCode::BarCodeRecognition;
using namespace Aspose::BarCode::Generation;
using namespace Aspose::BarCode;

void addBarcode(std::string*& arr, std::string entry, int& arrSize) {
    if (arrSize == 0) {
        arrSize++;
        arr = new std::string[arrSize];
        arr[arrSize - 1] = entry;
    }
    else {
        arrSize++;
        std::string* temp = new std::string[arrSize];
        for (int i = 0; i < arrSize - 1; i++) {
            temp[i] = arr[i];
        }
        delete[] arr;
        arr = NULL;
        temp[arrSize - 1] = entry;
        arr = new std::string[arrSize];
        for (int i = 0; i < arrSize; i++) {
            arr[i] = temp[i];
        }
        delete[] temp;
        temp = NULL;
    }
}

void removeBarcode(std::string*& arr, int& arrSize) {
    if (arrSize == 0) {
        std::cout << "Array is empty" << std::endl;
    }
    else if (arrSize == 1) {
        delete[] arr;
        arr = NULL;
    }
    else {
        std::string* temp = new std::string[arrSize - 1];
        for (int i = 1; i < arrSize; i++) {
            temp[i - 1] = arr[i];
        }
        delete[] arr;
        arr = NULL;
        arrSize--;
        arr = new std::string[arrSize];
        for (int i = 0; i < arrSize; i++) {
            arr[i] = temp[i];
        }
        delete[] temp;
        temp = NULL;
    }
}

void textIn(System::String codeText, System::String fileName) {
    System::SharedPtr<BarcodeGenerator> barcodeGenerator = System::MakeObject<BarcodeGenerator>(EncodeTypes::GS1DataMatrix, codeText);
    barcodeGenerator->get_Parameters()->set_Resolution(800);
    barcodeGenerator->Save(fileName);
}

void readFromFile(std::ifstream& inFile, std::string fileNa, std::string*& barCodeArr, int& count) {
    inFile.open(fileNa);
    if (!inFile) {
        std::cout << "Unable to open file" << std::endl;
    }
    std::string barCode;
    int arrSize = 0;
    while (!inFile.eof()) {
        getline(inFile, barCode);
        addBarcode(barCodeArr, barCode, arrSize);
    }
    while (barCodeArr != NULL) {
        System::String codeText = (System::String)barCodeArr[0];
        removeBarcode(barCodeArr, arrSize);
        System::String fileName = (System::String)std::to_string(count++) + u".jpg";
        textIn(codeText, fileName);
    }
    inFile.close();
}

void readManually(int& count) {
    std::string in;
    std::cout << "Type \"b\" whenever you want to go back to menu" << std::endl;
    for (int i = 1;; i++) {
        System::String fileName = (System::String)std::to_string(count++) + u".jpg";
        std::cout << "Enter GS1 Barcode: ";
        std::cin >> in;
        std::cout << in << std::endl;
        if (in == "b") {
            break;
        }
        else if (in == "q") {
            exit(0);
        }
        else {
            textIn((System::String)in, fileName);
        }
    }
}


int main() {
    std::ifstream inFile;
    std::string* arr;
    int arrSize = 0;
    char in;
    std::cout << "Hi! Select an option how you wish to generate data matrix of your barcode" << std::endl;
    int count = 1;
    for (int i = 1;; i++) {
        std::cout << "1- Type Manually\n2- Read From File\nq- Exit\nType only the option precisely(ex: 1,2,q)" << std::endl;
        std::cout << "Option: ";
        std::cin >> in;
        if (in == '1') {
            readManually(count);
        }
        else if (in == '2') {
            std::string fileName = "code.txt";
            std::cout << "Type file name without its extension: ";
            std::cin >> fileName;
            fileName += ".txt";
            readFromFile(inFile, fileName, arr, count);
            std::cout << "Data matrix of each GS1 barcode in file has been created and placed in folder" << std::endl;
        }
        else if (in == 'q') {
            exit(0);
        }
        else {
            std::cout << "False input has encountered. Program was terminated." << std::endl;
            exit(0);
        }
    }
    //std::string in;
    //std::cout << "Enter GS1 Barcode: ";
    //std::cin >> in;
    //System::String str = (System::String)in;
    // (7008)05
    // (13)121212
    // (11)121212
    // (7007)101010121212
    // (3554)123456
    // (3555)123456
    // (21)02134685
    //textIn((System::String)in);
    //const System::String codeText = u"(13)121212(11)121212";
    //const System::String fileName = u"Code.jpg";
    //System::SharedPtr<BarcodeGenerator> barcodeGenerator = System::MakeObject<BarcodeGenerator>(EncodeTypes::GS1DataMatrix, codeText);
    //barcodeGenerator->get_Parameters()->set_Resolution(800);
    //barcodeGenerator->Save(fileName);
    return -1;
}
