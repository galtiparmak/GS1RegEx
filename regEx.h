#ifndef REGEX_H
#define REGEX_H
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <chrono>
#include <fstream>
#include <stack>
#include <list>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "barcode.h"
using namespace std;

class regEx {
public:
    regEx();
    void addBarcode(int, string, string, string, string, string, string);
    void removeBarcode();
    void readCSV(ifstream&, string);
    void readData(const string);
    void evaluate();
    string toString();
    string info(const int, const string);
    barcode* getBarcode();
    void perform(const string);
    void analyze(const string);
    void analyzeTable(const string);
    int strToInt(string);
    void aiWithISO();
    void aiWithoutISO();
    void dateSpecifiedAi();
    void timeSpecifiedAi();
    bool containsWoISO(int);
    bool containsWISO(int);
    bool containsTimeSpecified(int);
    bool containsDateSpecified(int);
    string getMonth(string);
    string getDescription(ifstream&, string, string);
    string seperate(ifstream&, string, string);
    void addDescription(string, string, string);
private:
    list<int>listWoISO;
    list<int>listWISO;
    list<int>datesWoTime;
    list<int>datesWTime;
    barcode* barcodes;
    int barcodeNumber;
};
#endif
