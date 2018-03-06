#include "dataframe.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Gimme an arff file!" << std::endl;
        return 1;
    }

    DataFrame *df = new DataFrame();

    df->ReadCSV(argv[1]);
    df->Print();

}