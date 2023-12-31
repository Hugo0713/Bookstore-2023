#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <ostream>
// #include "MemoryRiver.hpp"

// using std::fstream;
// using std::ifstream;
// using std::ofstream;
// using std::string;

const int MAX = 1024;
const int LIMIT = 1000;

template <class T, int info_len>
class MemoryRiver
{
private:
    std::fstream file;
    std::string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    MemoryRiver(const std::string &file_name) : file_name(file_name) {}

    void initialise(std::string FN = "")
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


class Block // 序列化存储
{
public:
    int value;        // 值
    int size;       // 索引块对应值块个数
    int nextBlock;  // 索引块后继索引->
    int idx;        // 索引块：值块位置
    char index[68]; // 键

    Block() : value(0), size(0), nextBlock(0), idx(0)
    {
        memset(index, '\0', sizeof(index));
    }

    void initialise(int x) // 初始化读入
    {
        std::cin >> index;
        if (x == 0)
        {
            std::cin >> value;
        }
        else
        {
            value = INT_MIN;
        }
        return;
    }

    bool operator==(const Block &obj) const
    {
        return strcmp(index, obj.index) == 0 && value == obj.value;
    }
    bool operator!=(const Block &obj) const
    {
        return !(*this == obj);
    }
    bool operator<(const Block &obj) const
    {
        return strcmp(index, obj.index) < 0   ? true
               : strcmp(index, obj.index) > 0 ? false
                                              : value < obj.value;
    }
    bool operator>(const Block &obj) const
    {
        return !(*this == obj) && !(*this < obj);
    }
};
    const int SIZE = sizeof(Block);
    MemoryRiver<Block, 3> File; // 前置参数：总块数，起始块，裂块位置
    Block block_index[1024];
    Block block_value[1024];
    // std::vector<Block> block_value(1024);
    // std::vector<Block> block_index(1024);

    int index_find(Block &blk)
    {
        int total, start;
        File.get_info(total, 1);
        File.get_info(start, 2);
        File.read(block_index[1], 12, total); // 全体读入
        int target = start;
        for (int i = start; i; i = block_index[i].nextBlock)
        {
            if (blk < block_index[i])
            {
                return target;
            }
            target = i;
        }
        return target;
    }

    void split_(int num, int size)
    {
        int mid = size / 2; // 分裂点

        Block pre, now;
        File.read(pre, 12 + (num - 1) * SIZE, 1); // 读取待分裂块
        File.read(block_value[1], 12 + pre.idx * MAX * SIZE, size);

        int total, cur; // 更新前置索引信息
        File.get_info(total, 1);
        File.get_info(cur, 3);
        ++total;
        ++cur;
        File.write_info(total, 1);
        File.write_info(cur, 3);

        File.write(block_value[mid + 1], 12 + cur * MAX * SIZE, size - mid); // 写入裂块
        now = block_value[mid + 1];                                          // 更新索引块并写入
        now.size = size - mid;
        now.nextBlock = pre.nextBlock;
        now.idx = cur;
        File.write(now, 12 + (total - 1) * SIZE, 1);

        pre.size = mid; // 更新原索引块并写入
        pre.nextBlock = total;
        File.write(pre, 12 + (num - 1) * SIZE, 1);
    }

    void insert_(Block &bll)
    {
        int total, start, cur;
        File.get_info(total, 1);
        File.get_info(start, 2);
        File.get_info(cur, 3);

        if (total == 0) // 外存为空初始化
        {
            ++total;
            bll.idx = 1;
            bll.size = 1;
            File.write_info(total, 1);
            File.write_info(total, 2);
            File.write_info(total, 3);
            File.write(bll, 12, 1);              // 索引块写入
            File.write(bll, 12 + MAX * SIZE, 1); // 值块写入
            return;
        }
        Block pre, now;
        int target = index_find(bll);
        File.read(pre, 12 + (target - 1) * SIZE, 1);                                                                            // 读取索引块
        File.read(block_value[1], 12 + pre.idx * MAX * SIZE, pre.size);                                                         // 读取值块
        int num = std::lower_bound(block_value + 1, block_value + 1 + pre.size, bll) - block_value - 1; // 计算值块中的插入位置
        if (block_value[num + 1] == bll && num < pre.size)                                                                      // 元素重复
        {
            return;
        }
        if (num == 0) // 值块内最小
        {
            File.write(bll, 12 + pre.idx * MAX * SIZE, 1);                          // 写入索引块
            File.write(block_value[1], 12 + pre.idx * MAX * SIZE + SIZE, pre.size); // 写入值块
            now = bll;
        }
        else
        {
            File.write(bll, 12 + pre.idx * MAX * SIZE + num * SIZE, 1);
            File.write(block_value[1], 12 + pre.idx * MAX * SIZE, num);
            if (num != pre.size) // 非最大
            {
                File.write(block_value[num + 1], 12 + pre.idx * MAX * SIZE + (num + 1) * SIZE, pre.size - num); // 插入位置后值块后移
            }
            now = block_value[1];
        }

        now.size = pre.size + 1;
        now.nextBlock = pre.nextBlock;
        now.idx = pre.idx;
        File.write(now, 12 + target * SIZE - SIZE, 1); // 更新索引块
        if (now.size > LIMIT)                          // 超出值块最大容量
        {
            split_(target, now.size);
        }
    }

    void find_(Block &bll)
    {
        int total, start;
        int target = index_find(bll);
        File.get_info(total, 1);
        File.get_info(start, 2);
        if (total == 0)
        {
            std::cout << "null"
                      << "\n";
            return;
        }

        Block tmp;
        bool nextOne = true; // 循环控制
        bool flag = false;   // 查找判断
        while (nextOne)
        {
            File.read(tmp, 12 + (target - 1) * SIZE, 1);                    // 读取目标索引块
            File.read(block_value[1], 12 + tmp.idx * MAX * SIZE, tmp.size); // 读取目标值块
            int num = std::lower_bound(block_value + 1, block_value + tmp.size + 1, bll) - block_value;

            for (int i = num; i <= tmp.size; ++i)
            {
                if (strcmp(block_value[i].index, bll.index) == 0) // 匹配
                {
                    flag = true;
                    std::cout << block_value[i].value << " ";
                }
                else
                {
                    nextOne = false;
                    break;
                }
            }
            target = tmp.nextBlock; // 更新为下一索引
            if (target == 0)
            {
                break;
            }
        }
        if (!flag) // 查找失败
        {
            std::cout << "null";
        }
        std::cout << "\n";
    }

    void delete_(Block &bll)
    {
        int total, start;
        File.get_info(total, 1);
        int target = index_find(bll);
        if (total == 0)
        {
            return;
        }
        Block tmp;
        File.read(tmp, 12 + (target - 1) * SIZE, 1);                    // 读入索引块
        File.read(block_value[1], 12 + tmp.idx * MAX * SIZE, tmp.size); // 读入值块
        int num = std::lower_bound(block_value + 1, block_value + tmp.size + 1, bll) - block_value - 1;
        if (bll != block_value[num + 1]) // 无匹配值块
        {
            return;
        }
        if (num == 0) // 删除第一个值块
        {
            File.write(block_value[2], 12 + tmp.idx * MAX * SIZE, tmp.size - 1); // 其余值块前移
        }
        else
        {
            File.write(block_value[1], 12 + tmp.idx * MAX * SIZE, num);                              // 删除位置前值块写入
            File.write(block_value[num + 2], 12 + (tmp.idx * MAX + num) * SIZE, tmp.size - num - 1); // 删除位置后值块写入
        }

        File.get_info(start, 2); // 更新目标索引块
        --tmp.size;
        File.write(tmp, 12 + (target - 1) * SIZE, 1);
        if (tmp.size == 0) // 若目标索引块无值块
        {
            if (start == target) // 起始块更换
            {
                start = tmp.nextBlock;
                File.write_info(start, 2);
            }
            else
            { // 在索引块中查找上一个块，修改其 nextBlock 为目标索引块的 nextBlock
                for (int i = start; i; i = block_index[i].nextBlock)
                {
                    if (block_index[i].nextBlock == target)
                    {
                        block_index[i].nextBlock = tmp.nextBlock;
                        File.write(block_index[i], 12 + (i - 1) * SIZE, 1); // 写入更新后的上一个索引块的信息
                        break;
                    }
                }
            }
        }
    }



int main()
{
    int n;
    std::cin >> n;
    Block blk;
    File.initialise("File.txt");

    

    for (int i = 0; i < n; ++i)
    {
        std::string op;
        std::cin >> op;
        if (op == "insert")
        {
            blk.initialise(0);
            insert_(blk);
        }
        if (op == "find")
        {
            blk.initialise(1);
            find_(blk);
        }
        if (op == "delete")
        {
            blk.initialise(0);
            delete_(blk);
        }
    }
    return 0;
}