#ifndef CODE_LOG_HPP
#define CODE_LOG_HPP

#include <iomanip>
#include "account.hpp"
#include "../Database/Database.hpp"
#include "../Database/MemoryRiver.hpp"

class Log
{
public:
    char ID[31];
    std::string op;
    std::vector<double> finance;
    Log() : finance(), ID{}, op{} {}

    bool operator==(const Log &obj) const
    {
        if (strcmp(ID, obj.ID) == 0 && op == obj.op)
        {
            return true;
        }
        return false;
    }
    bool operator<(const Log &obj) const
    {
        if (strcmp(ID, obj.ID) != 0)
        {
            return op < obj.op;
        }
        return (strcmp(ID, obj.ID) < 0);
    }
    friend std::ostream &operator<<(std::ostream &out, const Log &obj)
    {
        std::cout << std::fixed << std::setprecision(2);
        out << obj.ID << "\t" << obj.op;
        return out;
    }

    void show_finance(int count);
    void report_finance();
    void report_employee();
    void log();
};

static MemoryRiver<double, 1> File_finance("File_finance");
static Database<Log> File_log("File_Log");
static Database<Log> File_employee("File_employee");

// extern Log logg;
extern std::vector<Account> login_stack;
#endif