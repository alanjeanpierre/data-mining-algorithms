#include "dataframe.h"

#include <iostream>
#include <fstream>
#include <cstdio>

std::string getXthWord(std::string line, int n, char sep=' ', bool dup=true);

DataFrame::DataFrame() {
    df = new std::vector<std::vector<DType>* >;
    columns = new std::vector<std::string>;
    types = new std::vector<DataType>;
}

DataFrame::~DataFrame() {
    for (int i = 0; i < df->size(); i++) {
        delete df->at(i);
    }
    delete df;
    delete columns;
    delete types;
}

void DataFrame::ReadCSV(char* file) {
    std::ifstream f;
    f.open(file);
    std::string line;

    if (f.is_open()) {
        while (getline(f, line)) {
            if (line[0] == '%' || line.empty())
                continue;
            if (line.compare(0, 10, "@ATTRIBUTE") == 0) {
                columns->push_back(getXthWord(line, 2));
                types->push_back(DType::getType(getXthWord(line, 3)));
            } else if (line.compare(0, 5, "@DATA") == 0) {
                break;
            } else if (line.compare(0, 9, "@RELATION")) {
                name = getXthWord(line, 2);
            }
        }
        while (getline(f, line)) {
            std::vector<DType> *row = new std::vector<DType>;
            for (int i = 1; i <= columns->size(); i++) {
                row->push_back(DType(getXthWord(line, i, ',', false), (*types)[i-1]));
            }
            df->push_back(row);
        }

        f.close();
    }

}

void DataFrame::Print() {
    std::vector<std::string> s;

    printf("%s\n", name.c_str());
    // format string
    for (int i = 0; i < columns->size(); i++) {
        char buf[100];
        std::sprintf(buf, "%%-%ds ", (int)columns->at(i).length());
        s.push_back(std::string(buf));
    }

    for (int i = 0; i < columns->size(); i++) {
        printf("%s ", columns->at(i).c_str());
    }

    printf("\n");

    for (int i = 0; i < df->size(); i++) {
        for (int j = 0; j < df->at(i)->size(); j++) {
            printf(s[j].c_str(), df->at(i)->at(j).Print().c_str());
        }
        printf("\n");
    }
    printf("\n");
}

std::string getXthWord(std::string line, int n, char sep, bool dup) {
    std::size_t found = 0;
    for (int i = 1; i < n; i++) {
        found = line.find_first_of(sep, found) + 1;
        if (dup) while (line[found] == sep) { 
            found++;
        }
    }

    std::size_t found2 = line.find_first_of(sep, found+1);

    return line.substr(found, found2-found);

    
}

