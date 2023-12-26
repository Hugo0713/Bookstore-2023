#ifndef CODE_DATABASE_HPP
#define CODE_DATABASE_HPP

#include <cstring>
#include <vector>
#include <climits>
#include <iostream>
#include <fstream>
#include "MemoryRiver.hpp"
#include "Block.hpp"

template <typename valueType>
class Database
{
public:
    const int MAX = 4096;
    const int LIMIT = 4000;
    const int SIZE = sizeof(Block<valueType>);

    MemoryRiver<Block<valueType>, 3> File; // 前置参数：总块数，起始块，裂块位置
    std::vector<Block<valueType>> block_value;
    std::vector<Block<valueType>> block_index;

    Database(std::string file_name) : block_value(MAX), block_index(MAX)
    {
        File.initialise(file_name);
    }

    int index_find(Block<valueType> &blk);

    void Split(int num, int size);

    void Insert(Block<valueType> &blk);

    valueType &Find(Block<valueType> &blk);

    bool ifFind(Block<valueType> &blk);

    void showFind(Block<valueType> &blk);

    void showAll();

    void showALL();

    void Delete(Block<valueType> &blk);
};

template <typename valueType>
int Database<valueType>::index_find(Block<valueType> &blk)
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

template <typename valueType>
void Database<valueType>::Split(int num, int size)
{
    int mid = size / 2; // 分裂点

    Block<valueType> pre, now;
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

template <typename valueType>
void Database<valueType>::Insert(Block<valueType> &blk)
{
    int total, start, cur;
    File.get_info(total, 1);
    File.get_info(start, 2);
    File.get_info(cur, 3);

    if (total == 0) // 外存为空初始化
    {
        ++total;
        blk.idx = 1;
        blk.size = 1;
        File.write_info(total, 1);
        File.write_info(total, 2);
        File.write_info(total, 3);
        File.write(blk, 12, 1);              // 索引块写入
        File.write(blk, 12 + MAX * SIZE, 1); // 值块写入
        return;
    }
    if (start == 0)
    {
        ++start;
        File.write_info(start, 2);
    }
    Block<valueType> pre, now;
    int target = index_find(blk);
    File.read(pre, 12 + (target - 1) * SIZE, 1);                                                                            // 读取索引块
    File.read(block_value[1], 12 + pre.idx * MAX * SIZE, pre.size);                                                         // 读取值块
    int num = std::lower_bound(block_value.begin() + 1, block_value.begin() + 1 + pre.size, blk) - block_value.begin() - 1; // 计算值块中的插入位置
    if (block_value[num + 1] == blk && num < pre.size)                                                                      // 元素重复
    {
        return;
    }
    if (num == 0) // 值块内最小
    {
        File.write(blk, 12 + pre.idx * MAX * SIZE, 1);                          // 写入索引块
        File.write(block_value[1], 12 + pre.idx * MAX * SIZE + SIZE, pre.size); // 写入值块
        now = blk;
    }
    else
    {
        File.write(blk, 12 + pre.idx * MAX * SIZE + num * SIZE, 1);
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
        Split(target, now.size);
    }
}

template <typename valueType>
bool Database<valueType>::ifFind(Block<valueType> &blk)
{
    int total, start;
    int target = index_find(blk);
    File.get_info(total, 1);
    File.get_info(start, 2);
    if (total == 0)
    {
        return false;
    }

    Block<valueType> tmp;
    bool nextOne = true; // 循环控制
    bool flag = false;   // 查找判断
    while (nextOne)
    {
        File.read(tmp, 12 + (target - 1) * SIZE, 1);                    // 读取目标索引块
        File.read(block_value[1], 12 + tmp.idx * MAX * SIZE, tmp.size); // 读取目标值块
        int num = std::lower_bound(block_value.begin() + 1, block_value.begin() + tmp.size + 1, blk) - block_value.begin();

        for (int i = num; i <= tmp.size; ++i)
        {
            if (strcmp(block_value[i].index, blk.index) == 0) // 匹配
            {
                flag = true;
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
        return false;
    }
    return true;
}

template <typename valueType>
valueType &Database<valueType>::Find(Block<valueType> &blk)
{
    int total, start;
    int target = index_find(blk);
    File.get_info(total, 1);
    File.get_info(start, 2);
    if (total == 0)
    {
        throw std::runtime_error("Invalidgg\n");
    }

    Block<valueType> tmp;
    bool nextOne = true; // 循环控制
    bool flag = false;   // 查找判断
    while (true)
    {
        File.read(tmp, 12 + (target - 1) * SIZE, 1);                    // 读取目标索引块
        File.read(block_value[1], 12 + tmp.idx * MAX * SIZE, tmp.size); // 读取目标值块
        int num = std::lower_bound(block_value.begin() + 1, block_value.begin() + tmp.size + 1, blk) - block_value.begin();

        for (int i = num; i <= tmp.size; ++i)
        {
            if (strcmp(block_value[i].index, blk.index) == 0) // 匹配
            {
                flag = true;
                return block_value[i].value;
            }
            // else
            // {
            //     nextOne = false;
            //     break;
            // }
        }
        target = tmp.nextBlock; // 更新为下一索引
        if (target == 0)
        {
            break;
        }
    }
    if (!flag) // 查找失败
    {
        throw std::runtime_error("Invalid???\n");
    }
    throw std::logic_error("Invalidkbh\n");
}

template <typename valueType>
void Database<valueType>::showFind(Block<valueType> &blk)
{
    int total, start;
    int target = index_find(blk);
    File.get_info(total, 1);
    File.get_info(start, 2);
    if (total == 0)
    {
        throw std::runtime_error("Invalid\n");
    }

    Block<valueType> tmp;
    bool nextOne = true; // 循环控制
    bool flag = false;   // 查找判断
    while (nextOne)
    {
        File.read(tmp, 12 + (target - 1) * SIZE, 1);                    // 读取目标索引块
        File.read(block_value[1], 12 + tmp.idx * MAX * SIZE, tmp.size); // 读取目标值块
        int num = std::lower_bound(block_value.begin() + 1, block_value.begin() + tmp.size + 1, blk) - block_value.begin();

        for (int i = num; i <= tmp.size; ++i)
        {
            if (strcmp(block_value[i].index, blk.index) == 0) // 匹配
            {
                flag = true;
                std::cout << block_value[i].value << "\n";
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
        throw std::runtime_error("Invalid\n");
    }
}

template <typename valueType>
void Database<valueType>::showAll()
{
    int total, start;
    File.get_info(total, 1);
    File.get_info(start, 2);

    // 循环遍历每个索引块
    for (int target = start; target != 0; target = block_index[target].nextBlock)
    {
        Block<valueType> tmp;
        File.read(tmp, 12 + (target - 1) * SIZE, 1);                    // 读取目标索引块
        File.read(block_value[1], 12 + tmp.idx * MAX * SIZE, tmp.size); // 读取目标值块

        // 打印值块中的每个记录
        for (int i = 1; i <= tmp.size; ++i)
        {
            std::cout << block_value[i].value << "\n";
        }
    }
}

template <typename valueType>
void Database<valueType>::showALL()
{
    int total, start;
    File.get_info(total, 1);
    File.get_info(start, 2);

    // 循环遍历每个索引块
    for (int target = start; target != 0; target = block_index[target].nextBlock)
    {
        Block<valueType> tmp;
        File.read(tmp, 12 + (target - 1) * SIZE, 1);                    // 读取目标索引块
        File.read(block_value[1], 12 + tmp.idx * MAX * SIZE, tmp.size); // 读取目标值块

        // 打印值块中的每个记录
        for (int i = 1; i <= tmp.size; ++i)
        {
            std::cout << block_value[i].index << " " << block_value[i].value << "\n";
        }
    }
}

template <typename valueType>
void Database<valueType>::Delete(Block<valueType> &blk)
{
    int total, start;
    File.get_info(total, 1);
    int target = index_find(blk);
    if (total == 0)
    {
        return;
    }
    Block<valueType> tmp;
    File.read(tmp, 12 + (target - 1) * SIZE, 1);                    // 读入索引块
    File.read(block_value[1], 12 + tmp.idx * MAX * SIZE, tmp.size); // 读入值块
    int num = std::lower_bound(block_value.begin() + 1, block_value.begin() + tmp.size + 1, blk) - block_value.begin() - 1;
    if (blk != block_value[num + 1]) // 无匹配值块
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

#endif