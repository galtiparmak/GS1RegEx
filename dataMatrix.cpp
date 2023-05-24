#include <iostream>
#include <system/string.h>
#include <system/shared_ptr.h>
#include <system/object.h>
#include <system/environment.h>
#include <system/console.h>
#include <Generation/EncodeTypes/SymbologyEncodeType.h>
#include <Generation/EncodeTypes/EncodeTypes.h>
#include <Generation/Caption.h>
#include <BarCode.Generation/BarcodeGenerator.h>
#include <BarCode.Generation/GenerationParameters/BaseGenerationParameters.h>
#include <BarCode.Generation/GenerationParameters/BarcodeParameters.h>
#include <BarCode.Generation/GenerationParameters/CaptionParameters.h>
#include <BarCode.Generation/Helpers/FontUnit.h>
#include <BarCode.Generation/Helpers/Unit.h>
#include <BarCode.Generation/GenerationParameters/TextAlignment.h>
#include <drawing/string_alignment.h>
#include <Generation/BarCodeImageFormat.h>
#include <drawing/font.h>
#include <drawing/color.h>
int main() {
    // Create barcode generator
System::SharedPtr<BarcodeGenerator> barcodeGenerator = System::MakeObject<BarcodeGenerator>(EncodeTypes::DataMatrix, u"1234567");
barcodeGenerator->get_Parameters()->set_Resolution(300);
// Generate barcode and save as image
barcodeGenerator->Save(u"output.jpg"); 
// Use the relevant EncodeTypes Data Matrix, GS1 Data Matrix
// The path to the documents directory.
System::String dataDir = RunExamples::GetDataDir_CreateAndManage2DBarCodes();

// Instantiate barcode object and set CodeText & Barcode Symbology
System::SharedPtr<BarcodeGenerator> generator = System::MakeObject<BarcodeGenerator>(EncodeTypes::DataMatrix, u"This is the data to be encoded");

// Set Data Matrix EncodeMode 
generator->get_Parameters()->get_Barcode()->get_DataMatrix()->set_DataMatrixEncodeMode(Aspose::BarCode::DataMatrixEncodeMode::ASCII);
generator->Save(dataDir + u"Datamatrixbarcode_out.bmp", Aspose::BarCode::BarCodeImageFormat::Bmp);

System::Console::WriteLine(System::Environment::get_NewLine() + u"Barcode saved at " + dataDir + u"Datamatrixbarcode_out.bmp");
return -1;
}