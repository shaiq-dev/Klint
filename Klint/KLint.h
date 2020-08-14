#ifndef KLINT_H
#define KLINT_H

#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

#define KL_UCONSTANT 1
#define KL_STARTDATE 1990

namespace KL
{
    template <class T>

    std::string toString(const T &);

    struct dateTime
    {
        dateTime()
        {
            time_t ms;
            time(&ms);

            struct tm *sysTD = localtime(&ms);

            DAY     = sysTD -> tm_mday;
            MONTH   = sysTD -> tm_mon + KL_UCONSTANT;
            YEAR    = sysTD -> tm_year + KL_STARTDATE;
            MINUTES = sysTD -> tm_min;
            HOURS   = sysTD -> tm_hour;
            SECONDS = sysTD -> tm_sec;
        }

        dateTime(int DAY, int MONTH, int YEAR, int HOURS, int MINUTES, int SECONDS) : DAY(DAY), MONTH(MONTH), YEAR(YEAR), HOURS(HOURS), MINUTES(MINUTES), SECONDS(SECONDS)
        {}
        dateTime(int DAY, int MONTH, int YEAR) : DAY(DAY), MONTH(MONTH), YEAR(YEAR), HOURS(0), MINUTES(0), SECONDS(0)
        {}

        dateTime now() const
        {
            return dateTime();
        }

        int DAY, MONTH, YEAR, HOURS, MINUTES, SECONDS;

        std::string getDate() const
        {
            return std::string ( DAY < 10 ? "0" : "" )      + toString(DAY)   +
                   std::string ( MONTH < 10 ? ".0" : ".")   + toString(MONTH) + "." +
                   toString(YEAR);
        }

        std::string getTime(const std::string &sep = ":") const
        {
            return std::string ( HOURS < 10 ? "0" : "" )    + toString(HOURS)   + sep +
                   std::string ( MINUTES < 10 ? "0" : "" )  + toString(MINUTES) + sep +
                   std::string ( SECONDS < 10 ? sep : "" )  + toString(SECONDS);
        }

        std::string getDateTime(const std::string &sep = ":" ) const
        {
            return getDate() + " " + getTime(sep);
        }
    };

    template <class T>

    std::string toString(const T &e)
    {
        std::ostringstream s;
        s << e;
        return s.str();
    }

    void appendLogs(const std::string &s)
    {
        std::ofstream file("Klint_logs.txt", std::ios::app);

        file << "[" << KL::dateTime().getDateTime() << "]" <<
        "\n" << s << std::endl << "\n";

        file.close();
    }

}

#endif

