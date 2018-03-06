#include <string>
#include <vector>
#include "dtype.h"

class DataFrame {

private:
    std::vector<std::vector<DType>* > *df;
    std::string name;
    std::vector<std::string> *columns;
    std::vector<DataType> *types;

public: 
    DataFrame();
    ~DataFrame();
    void ReadCSV(char* file);
    void Print();



};