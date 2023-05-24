#pragma once
#ifndef BARCODE_H
#define BARCODE_H

#include <iostream>
#include <string>
using namespace std;

class barcode {
public:
    barcode();
    barcode(int, string, string, string, string, string, string);
    int getAi();
    string getCode();
    string getDescription();
    string getISO();
    string getTitle();
    string getValue();
    string getType();
    void setAi(int);
    void setCode(string);
    void setDescription(string);
    void setISO(string);
    void setTitle(string);
    void setValue(string);
    void setType(string);
private:
    int ai;
    string code;
    string description;
    string iso;
    string title;
    string value;
    string type;
};

#endif //BARCODE_H


