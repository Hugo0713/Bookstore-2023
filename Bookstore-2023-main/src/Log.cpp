#include "Log.hpp"

void Log::show_finance(int count)
{
    if (login_stack.back().getPrivilege() != 7)
    {
        throw std::runtime_error("Invalid\n");
    }
    double income = 0.00, outcome = 0.00;
    int Count;
    File_finance.get_info(Count, 1);
    finance.resize(Count);
    File_finance.read(finance[0], 4, Count); // 想要插入需要先使得vector有这么大

    if (count > Count)
    {
        throw std::runtime_error("Invalid\n");
    }
    if (count == 0)
    {
        count = finance.size();
    }

    for (int m = Count - 1; m > Count - count - 1; --m)
    {
        if (finance[m] < 0)
        {
            outcome += finance[m];
        }
        else
        {
            income += finance[m];
        }
    }
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "+"
              << " " << income << " "
              << "-"
              << " " << std::abs(outcome) << "\n";
}

void Log::report_finance()
{
    if (login_stack.back().getPrivilege() != 7)
    {
        throw std::runtime_error("Invalid\n");
    }

    int Count;
    File_finance.get_info(Count, 1);

    for (int i = 1; i <= Count; ++i)
    {
        show_finance(i);
    }
}

void Log::report_employee()
{
    if (login_stack.back().getPrivilege() != 7)
    {
        throw std::runtime_error("Invalid\n");
    }
    File_employee.showALL();
}

void Log::log()
{
    if (login_stack.back().getPrivilege() != 7)
    {
        throw std::runtime_error("Invalid\n");
    }
    File_log.showALL();
}