#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <string>
#include "openglcanvaswidget.h"




class FileHandler
{
public:
    FileHandler();
private:
    //todo jakie extension types chce dac
    bool haveitbeensaved();
    enum class ExtensionType{JSON,costamxd};
    void SaveToFile();
    void ReadFromFile();

    //moze dodac zeby byl empty z bomby
    std::string FilePath;
};

#endif // FILEHANDLER_H
