#include "filestream.hpp"
#include <fstream>
#include <iostream>
#include "common.hpp"
namespace gen {
    using namespace std;

    FileStream& FileStream::file(const string& name) {
        currentFile = name;
        return *this;
    }

    FileStream& FileStream::operator << (const string& s) {
        if (currentFile.empty())
            return *this;
        
        if (data.find(currentFile) == data.end())
            data[currentFile] = "";
            
        data[currentFile] += s;

        return *this;
    }

    static void syncIfNeeded(const string& fName, const string& data) {

        if (data != readFile(fName)) {
            cout << "updating " << fName << endl;
            ofstream of(fName.c_str(), ofstream::out | ofstream::trunc);
            of.write(data.c_str(), data.size());
            of.close();
        }
    }

    void FileStream::sync() const {
        for (Data::const_iterator i = data.begin(); i != data.end(); i++) {
            syncIfNeeded(i->first, i->second);                
        }
    }

}        
