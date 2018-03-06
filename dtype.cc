#include "dtype.h"
#include <string>
#include <sstream>
#include <cstdio>

DType::DType(std::string data, DataType type) {
    
    std::istringstream str(data);
    t = type;
    switch(type) {
        case Integer:
            str >> d.i;
            break;
        case Double:
            str >> d.d;
            break;
        case String:
            d.s = new char[100];
            data.copy(d.s, 100);
            break;
    }
}

DType::DType(std::string data, std::string type) {
    DType(data, getType(type));
}

std::string DType::Print() {
    char buf[100];
    switch(t) {
        case Integer:
            sprintf(buf, "%d", d.i);
            break;
        case Double:
            sprintf(buf, "%f", d.d);
            break;
        case String:
            sprintf(buf, "%s", d.s);
            break;
        default:
            // idk
            break;
    }

    return std::string(buf);
}

DataType DType::getType(std::string type) {
    if (type.compare(0, 7, "integer") == 0) {
        return Integer;
    } else if (type.compare(0, 6, "double") == 0) {
        return Double;
    } else if (type.compare(0, 6, "string") == 0) {
        return String;
    } else {
        return String;
    }
}