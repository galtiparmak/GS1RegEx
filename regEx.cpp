#include "regEx.h"

regEx::regEx() {
    barcodes = NULL;
    barcodeNumber = 0;
}

void regEx::addBarcode(int ai, string code, string description = "", string iso = "", string title = "", string value = "", string type = "") {
    barcodeNumber++;
    barcode* newBarcode = new barcode(ai, code, description, iso, title, value, type);
    if (barcodes == NULL) {
        barcodes = new barcode[barcodeNumber];
        barcodes[0] = *newBarcode;
    }
    else {
        barcode* temp = new barcode[barcodeNumber];
        for (int i = 0; i < barcodeNumber - 1; i++) {
            temp[i] = barcodes[i];
        }
        temp[barcodeNumber - 1] = *newBarcode;
        delete[] barcodes;
        barcodes = NULL;
        barcodes = new barcode[barcodeNumber];
        for (int i = 0; i < barcodeNumber; i++) {
            barcodes[i] = temp[i];
        }
        delete[] temp;
        temp = NULL;
    }
}

void regEx::removeBarcode() {
    if (barcodes == NULL) {
        cout << "No barcode to remove" << endl;
    }
    else {
        if (barcodeNumber == 1) {
            delete[] barcodes;
            barcodes = NULL;
        }
        else {
            barcode* temp = new barcode[barcodeNumber];
            for (int i = 0; i < barcodeNumber; i++) {
                temp[i] = barcodes[i];
            }
            barcodeNumber--;
            delete[] barcodes;
            barcodes = NULL;
            barcodes = new barcode[barcodeNumber];
            for (int j = 1; j < barcodeNumber + 1; j++) {
                barcodes[j - 1] = temp[j];
            }
            delete[] temp;
            temp = NULL;
        }
    }
}

void regEx::readCSV(ifstream& inFile, string fileName) {
    inFile.open(fileName);
    if (!inFile) {
        cout << "unable to open file" << endl;
    }
    string aiLoc, description, title, extra, result = "";
    while (!inFile.eof()) {
        getline(inFile, aiLoc, ';');
        getline(inFile, description, ';');
        getline(inFile, title, ';');
        if (title == "ISO" || title == "UIC") {
            getline(inFile, extra, ';');
            if (strToInt(aiLoc) == barcodes[0].getAi()) {
                barcodes[0].setDescription(description);
                barcodes[0].setISO(title);
                barcodes[0].setTitle(extra);
                inFile.close();
                return;
            }
        }
        if (strToInt(aiLoc) == barcodes[0].getAi()) {
            barcodes[0].setDescription(description);
            barcodes[0].setTitle(title);
            inFile.close();
            return;
        }
    }
    inFile.close();
}

void regEx::readData(const string data) {
    queue<string> q;
    string delimiter1 = "(";
    string delimiter2 = ")";
    string str = data;
    bool done = false;
    int size = 0;
    while (!done) {
        if (str[0] == '(') {
            q.push(str.substr(str.find(delimiter1) + 1, str.find(delimiter2) - 1));
            str = str.substr(str.find(delimiter2) + 1, str.size() - (str.find(delimiter2) + 1));
            size++;
        }
        if (str.find(delimiter1) < str.size()) {
            q.push(str.substr(0, str.find(delimiter1)));
            str = str.substr(str.find(delimiter1), str.size() - str.find(delimiter1));
            size++;
        }
        else {
            q.push(str);
            done = true;
            size++;
        }
    }
    string var1;
    string var2;
    int num;
    for (int count = 0; count < size; count += 2) {
        var1 = q.front();
        num = strToInt(var1);
        q.pop();
        var2 = q.front();
        q.pop();
        addBarcode(num, var2);
    }
}

void regEx::evaluate() {
    string isoValue;
    string titleValue;
    int ai = barcodes[0].getAi();
    if (barcodes[0].getISO() == "ISO") {
        titleValue = info(ai, barcodes[0].getCode());
        barcodes[0].setISO(barcodes[0].getCode().substr(0, 3));
    }
    else if (barcodes[0].getISO() == "UIC") {
        titleValue = info(ai, barcodes[0].getCode());
        isoValue = barcodes[0].getCode().substr(0, 1);
        barcodes[0].setISO(barcodes[0].getISO() + ": " + isoValue);
    }
    else {
        titleValue = info(ai, barcodes[0].getCode());
    }
    barcodes[0].setValue(titleValue);
}

bool regEx::containsWoISO(int num) {
    for (const int value : listWoISO)
        if (value == num) return true;
    return false;
}

bool regEx::containsWISO(int num) {
    for (const int value : listWISO)
        if (value == num) return true;
    return false;
}

bool regEx::containsDateSpecified(int num) {

    for (const int value : datesWoTime)
        if (value == num) return true;
    return false;
}

bool regEx::containsTimeSpecified(int num) {
    for (const int value : datesWTime)
        if (value == num) return true;
    return false;
}

string regEx::info(const int ai, const string data) {
    string aiStr = to_string(ai);
    string str = data;
    if (aiStr == "7040") {
        return str.substr(1, str.size() - 1);
    }
    else if (aiStr == "7007") {
        return "20" + str.substr(0, 2) + "." + str.substr(2, 2) + "." + str.substr(4, 2) + " - " + "20" + str.substr(6, 2) + "." + str.substr(8, 2) + "." + str.substr(10, 2);
    }
    else if (containsWISO(ai)) {
        return str.substr(3, str.size() - 3);
    }
    else if (containsWoISO(ai)) {
        return str;
    }
    else if (containsTimeSpecified(ai)) {
        return "20" + str.substr(0, 2) + "." + str.substr(2, 2) + "." + str.substr(4, 2) + ", " + str.substr(6, 2) + ":" + str.substr(8, 2);
    }
    else if (containsDateSpecified(ai)) {
        barcodes[0].setType("Date");
        return "20" + str.substr(0, 2) + "." + str.substr(2, 2) + "." + str.substr(4, 2);
    }
    else if (aiStr == "4321" || aiStr == "4322" || aiStr == "4323") {
        if (data == "1") barcodes[0].setValue("true");
        else if (data == "0") barcodes[0].setValue("false");
    }
    else if ((aiStr.size() == 4 && (aiStr.substr(0, 3) == "391")) || (aiStr.size() == 4 && aiStr.substr(0.3) == "393")) {
        barcodes[0].setType("Measurement");
        int decider = aiStr[aiStr.size() - 1] - 48;
        if (decider == 0) return str + "\n";
        else {
            return str.substr(3, data.size() - decider - 3) + "." + str.substr(data.size() - decider, decider);
        }
    }
    else if (aiStr.size() == 4 && aiStr.substr(0, 3) == "395") {
        barcodes[0].setType("Measurement");
        int decider = aiStr[3] - 48;
        if (decider == 0) {
            return str;
        }
        if (decider > 5 && decider < 10) {
            string result = "0.";
            for (int i = 0; i < decider - 6; i++) {
                result += "0";
            }
            return result + str;
        }
        else {
            return str.substr(0, data.size() - decider) + "." + str.substr(data.size() - decider, decider);
        }
    }
    else if (aiStr.size() == 4 && aiStr[0] == '3') {
        barcodes[0].setType("Measurement");
        int decider = aiStr[3] - 48;
        if (decider == 0) {
            return str;
        }
        else {
            return str.substr(0, data.size() - decider) + "." + str.substr(data.size() - decider, decider);
        }
    }
    else {
        return "error";
    }
}

string regEx::toString() {
    ifstream inFile;
    string csv = "C:\\Users\\gekol\\Desktop\\regEx\\csvFile.csv";
    string result = "{\n";
    result += "\t\"barcodes\": [\n";
    while (barcodes != NULL) {
        readCSV(inFile, csv);
        evaluate();
        result += "\n\t\t{\n";
        result += "\t\t\t\"ai\":" + to_string(barcodes[0].getAi()) + ",\n";
        result += "\t\t\t\"description\":\"" + barcodes[0].getDescription() + "\",\n";
        if (barcodes[0].getISO() != "") {
            result += "\t\t\t\"iso\":\"" + barcodes[0].getISO() + "\",\n";
        }
        result += "\t\t\t\"title\":\"" + barcodes[0].getTitle() + "\",\n";
        result += "\t\t\t\"" + barcodes[0].getTitle() + "\":" + barcodes[0].getValue() + ",\n";
        result += "\t\t\t\"type\":\"" + barcodes[0].getType() + "\"" + "\n";
        result += "\t\t}";
        if (barcodeNumber != 1) {
            result += ",";
        }
        result += "\n";
        removeBarcode();
    }
    result += "\t]\n}";
    return result;
}

void regEx::perform(const string data) {
    readData(data);
    aiWithISO();
    aiWithoutISO();
    dateSpecifiedAi();
    timeSpecifiedAi();
    cout << toString();
}

void regEx::analyze(const string data) {
    readData(data);
    aiWithISO();
    aiWithoutISO();
    dateSpecifiedAi();
    timeSpecifiedAi();
}

void regEx::analyzeTable(const string data) {
    int i;
    chrono::time_point<chrono::system_clock> startTime;
    chrono::duration<double, milli> elapsedTime;
    for (int k = 1; k < 12; k++) {
        startTime = chrono::system_clock::now();
        for (i = 0; i < pow(2, k); i++)
            analyze(data);
        elapsedTime = chrono::system_clock::now() - startTime;
        cout << "| Execution of" << setw(10) << pow(2, k) << setw(15) << " barcodes took" << setw(20) << elapsedTime.count() << "\n";
    }
}

int regEx::strToInt(string str) {
    int num;
    stringstream ss;
    ss << str;
    ss >> num;
    return num;
}

void regEx::addDescription(string ai, string description, string type) {
    string filename("Descriptions.txt");
    ofstream file_out;
    file_out.open(filename, std::ios_base::app);
    file_out << ai << " Description: " << description << " (" << type << "): " << endl;
    cout << "Done !" << endl;
}

void regEx::aiWithoutISO() {
    listWoISO.push_back(0); listWoISO.push_back(243);
    listWoISO.push_back(1); listWoISO.push_back(250); listWoISO.push_back(424);
    listWoISO.push_back(2); listWoISO.push_back(251); listWoISO.push_back(425);
    listWoISO.push_back(10); listWoISO.push_back(252); listWoISO.push_back(426);
    listWoISO.push_back(20); listWoISO.push_back(253); listWoISO.push_back(427);
    listWoISO.push_back(21); listWoISO.push_back(254); listWoISO.push_back(701);
    listWoISO.push_back(22); listWoISO.push_back(255); listWoISO.push_back(702);
    listWoISO.push_back(30); listWoISO.push_back(400); listWoISO.push_back(703);
    listWoISO.push_back(37); listWoISO.push_back(401); listWoISO.push_back(704);
    listWoISO.push_back(235); listWoISO.push_back(402); listWoISO.push_back(705);
    listWoISO.push_back(240); listWoISO.push_back(403); listWoISO.push_back(422);
    listWoISO.push_back(241); listWoISO.push_back(420); listWoISO.push_back(242);
    listWoISO.push_back(4320); listWoISO.push_back(7010); listWoISO.push_back(712);
    listWoISO.push_back(7001); listWoISO.push_back(7020); listWoISO.push_back(713);
    listWoISO.push_back(7002); listWoISO.push_back(7021); listWoISO.push_back(714);
    listWoISO.push_back(7004); listWoISO.push_back(7022); listWoISO.push_back(715);
    listWoISO.push_back(7005); listWoISO.push_back(7023); listWoISO.push_back(7230);
    listWoISO.push_back(7008); listWoISO.push_back(710); listWoISO.push_back(7231);
    listWoISO.push_back(7009); listWoISO.push_back(711); listWoISO.push_back(7232);
    listWoISO.push_back(7233); listWoISO.push_back(7234); listWoISO.push_back(7235);
    listWoISO.push_back(7236); listWoISO.push_back(7237); listWoISO.push_back(7238);
    listWoISO.push_back(7239); listWoISO.push_back(7240); listWoISO.push_back(8001);
    listWoISO.push_back(8002); listWoISO.push_back(8003); listWoISO.push_back(8004);
    listWoISO.push_back(8005); listWoISO.push_back(8006); listWoISO.push_back(8007);
    listWoISO.push_back(8008); listWoISO.push_back(8009); listWoISO.push_back(8010);
    listWoISO.push_back(8011); listWoISO.push_back(8012); listWoISO.push_back(8013);
    listWoISO.push_back(8014); listWoISO.push_back(8015); listWoISO.push_back(8016);
    listWoISO.push_back(8017); listWoISO.push_back(8018); listWoISO.push_back(8019);
    listWoISO.push_back(8020); listWoISO.push_back(8026); listWoISO.push_back(8110);
    listWoISO.push_back(8111); listWoISO.push_back(8112); listWoISO.push_back(8200);
    listWoISO.push_back(90); listWoISO.push_back(91); listWoISO.push_back(92);
    listWoISO.push_back(93); listWoISO.push_back(94); listWoISO.push_back(95);
    listWoISO.push_back(96); listWoISO.push_back(97); listWoISO.push_back(98);
    listWoISO.push_back(99); listWoISO.push_back(410); listWoISO.push_back(411);
    listWoISO.push_back(412); listWoISO.push_back(413); listWoISO.push_back(414);
    listWoISO.push_back(415); listWoISO.push_back(416); listWoISO.push_back(417);
    listWoISO.push_back(418); listWoISO.push_back(419); listWoISO.push_back(4300);
    listWoISO.push_back(4301); listWoISO.push_back(4302); listWoISO.push_back(4303);
    listWoISO.push_back(4304); listWoISO.push_back(4305); listWoISO.push_back(4306);
    listWoISO.push_back(4307); listWoISO.push_back(4308); listWoISO.push_back(4309);
    listWoISO.push_back(4310); listWoISO.push_back(4311); listWoISO.push_back(4312);
    listWoISO.push_back(4313); listWoISO.push_back(4314); listWoISO.push_back(4315);
    listWoISO.push_back(4316); listWoISO.push_back(4317); listWoISO.push_back(4318);
    listWoISO.push_back(4319);
}

void regEx::aiWithISO() {
    listWISO.push_back(3910); listWISO.push_back(3911); listWISO.push_back(3912);
    listWISO.push_back(3913); listWISO.push_back(3914); listWISO.push_back(3915);
    listWISO.push_back(3916); listWISO.push_back(3917); listWISO.push_back(3918);
    listWISO.push_back(3919); listWISO.push_back(3930); listWISO.push_back(3931);
    listWISO.push_back(3932); listWISO.push_back(3933); listWISO.push_back(3934);
    listWISO.push_back(3935); listWISO.push_back(3936); listWISO.push_back(3937);
    listWISO.push_back(3938); listWISO.push_back(3939); listWISO.push_back(421);
    listWISO.push_back(7030); listWISO.push_back(7031); listWISO.push_back(7032);
    listWISO.push_back(7033); listWISO.push_back(7034); listWISO.push_back(7035);
    listWISO.push_back(7036); listWISO.push_back(7037); listWISO.push_back(7038);
    listWISO.push_back(7039);
}

void regEx::dateSpecifiedAi() {
    datesWoTime.push_back(11); datesWoTime.push_back(12); datesWoTime.push_back(13);
    datesWoTime.push_back(15); datesWoTime.push_back(16); datesWoTime.push_back(17);
    datesWoTime.push_back(4326); datesWoTime.push_back(7006);
}

void regEx::timeSpecifiedAi() {
    datesWTime.push_back(4324); datesWTime.push_back(4325); datesWTime.push_back(7003);
}

string regEx::getMonth(const string str) {
    if (str == "01") return "January";
    else if (str == "02") return "Feburary";
    else if (str == "03") return "March";
    else if (str == "04") return "April";
    else if (str == "05") return "May";
    else if (str == "06") return "June";
    else if (str == "07") return "July";
    else if (str == "08") return "Agustus";
    else if (str == "09") return "September";
    else if (str == "00") return "October";
    else if (str == "11") return "November";
    else if (str == "12") return "December";
    else return "Incorrect Date";
}

int main() {
    regEx r;
    string str = "(11)121212";
    std::chrono::time_point<std::chrono::system_clock> startTime;
    std::chrono::duration<double, milli> elapsedTime;
    startTime = std::chrono::system_clock::now();
    r.perform(str);
    elapsedTime = std::chrono::system_clock::now() - startTime;
    cout << endl;
    cout << "Execution of 1 barcode took " << elapsedTime.count() << " milliseconds." << endl;
    //r.analyzeTable(str);
    return -1;
}
