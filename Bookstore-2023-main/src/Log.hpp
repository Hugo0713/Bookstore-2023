#ifndef CODE_LOG_HPP
#define CODE_LOG_HPP

#include <iomanip>

class Log
{
public:
    MemoryRiver<double, 1> File_finance;
    std::vector<double> finance;
    Log(std::string file_name) : finance()
    {
        File_finance.initialise(file_name);
    }

    void show_finance(int count)
    {
        double income = 0.00, outcome = 0.00;
        int Count;
        File_finance.read(finance[0], 4 + sizeof(double) * finance.size(), finance.size());
        File_finance.get_info(Count, 1);
        
        if (count > Count)
        {
            throw std::runtime_error("Invalid\n");
        }
        if (count == 0)
        {
            count = finance.size();
        }

        
        for (int m = 0; m < count; ++m)
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
        std::cout << "+" << income << " "
                  << "-" << std::abs(outcome) << "\n";
    }
};

extern Log logg;

#endif