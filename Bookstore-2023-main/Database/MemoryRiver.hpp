#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <unistd.h>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len>
class MemoryRiver
{
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {}

    void initialise(string FN = "")
    {
        if (FN != "")
            file_name = FN;
        if (access(file_name.c_str(), F_OK) == 0)
        {
            return;
        }
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void get_info(int &tmp, int n)
    {
        if (n > info_len)
            return;
        file.open(file_name);
        file.seekg((n - 1) * sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        return;
    }

    void write_info(int tmp, int n)
    {
        if (n > info_len)
            return;
        file.open(file_name);
        file.seekp((n - 1) * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        return;
    }

    void write(T &t, long long place, int size)
    {
        file.open(file_name);
        file.seekp(place);
        file.write(reinterpret_cast<char *>(&t), sizeofT * size);
        file.close();
    }

    void read(T &t, const int index, int size)
    {
        file.open(file_name);
        long long position = index;
        file.seekg(position, std::ios::beg);
        file.read(reinterpret_cast<char *>(&t), sizeofT * size);
        file.close();
    }
};


#endif //BPT_MEMORYRIVER_HPP