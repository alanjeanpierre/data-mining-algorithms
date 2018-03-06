#include <string>
#include <iostream>

union Data {
    int i;
    double d;
    char *s;
};

enum DataType {
    Integer, Double, String
};


class DType {

private:
    Data d;
    DataType t;

public:
    DType(std::string data, std::string type);
    DType(std::string data, DataType type);
    DataType getType() { return t; }
    int getInt() { return d.i; }
    double getDouble() { return d.d; }
    const char* getString() { return d.s; }
    std::string Print();
    static DataType getType(std::string type);

};