#include "barcode.h"

barcode::barcode() {
    ai = INT_MAX;
    code = "";
    description = "";
    iso = "";
    title = "";
}

barcode::barcode(int ai, string code, string description, string iso, string title, string value, string type) {
    this->ai = ai;
    this->code = code;
    this->description = description;
    this->iso = iso;
    this->title = title;
    this->value = value;
    this->type = type;
}

int barcode::getAi() {
    return ai;
}

string barcode::getCode() {
    return code;
}

string barcode::getDescription() {
    return description;
}

string barcode::getISO() {
    return iso;
}

string barcode::getTitle() {
    return title;
}

string barcode::getValue() {
    return value;
}

string barcode::getType() {
    return type;
}

void barcode::setAi(int ai) {
    this->ai = ai;
}

void barcode::setCode(string code) {
    this->code = code;
}

void barcode::setDescription(string description) {
    this->description = description;
}

void barcode::setISO(string iso) {
    this->iso = iso;
}

void barcode::setTitle(string title) {
    this->title = title;
}

void barcode::setValue(string value) {
    this->value = value;
}

void barcode::setType(string type) {
    this->type = type;
}

