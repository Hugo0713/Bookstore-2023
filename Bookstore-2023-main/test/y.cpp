#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unistd.h>

template <class T, int info_len = 2>
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
        file.open(file_name, std::fstream::in);
        if (access(file_name.c_str(), F_OK) == 0)
        {
            return;
        }
        file.open(file_name, std::fstream::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // 读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n)
    {
        if (n > info_len)
            return;
        file.open(file_name);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        return;
    }

    // 将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n)
    {
        if (n > info_len)
            return;
        file.open(file_name, std::fstream::in | std::fstream::out);
        file.seekp((n - 1) * sizeof(int), std::fstream::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        return;
    }

    // 在文件合适位置写入类对象t，并返回写入的位置索引index
    // 位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    // 位置索引index可以取为对象写入的起始位置
    void write(T &t, long long place, int size = 1)
    {
        file.open(file_name, std::fstream::in | std::fstream::out);
        file.seekp(place);
        file.write(reinterpret_cast<char *>(&t), sizeofT * size);
        file.close();
        return;
    }
    void read(T &t, const long long index, int size = 1)
    {
        file.open(file_name);
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT * size);
        file.close();
        return;
    }
};

class Node
{
public:
    int value;
    int size;
    int index_num;
    int next;
    char index[66] = {'\0'};
    Node() : value(0), size(0), next(0), index_num(0) {}
    Node(char *_index) : value(-2147483648)
    {
        strcpy(index, _index);
    }
    Node(char *_index, int value) : value(value)
    {
        strcpy(index, _index);
    }

    bool operator==(const Node &obj)
    {
        if (strcmp(index, obj.index) == 0 && value == obj.value)
            return true;
        else
            return false;
    }
    bool operator<(const Node &obj)
    {
        if (strcmp(index, obj.index) < 0)
            return true;
        if (strcmp(index, obj.index) > 0)
            return false;
        if (value < obj.value)
            return true;
        else
            return false;
    }
    bool operator>(const Node &obj) const
    {
        if (strcmp(index, obj.index) > 0)
            return true;
        if (strcmp(index, obj.index) < 0)
            return false;
        if (value > obj.value)
            return true;
        else
            return false;
    }
    bool operator!=(const Node &obj) const
    {
        if (strcmp(index, obj.index) == 0 && value == obj.value)
            return false;
        else
            return true;
    }
};

Node block_index[1000], block_num[1000]; // 索引块，数据块
MemoryRiver<Node, 3> File;               // 三个参数：总块数，开头，裂块位置。
// 前面的三个int类型数据：总共块数，开头索引，裂块新位置
// 注意每一次裂块将其添加到最后（实际）, 但我们通过next将其以链表的形式顺序储存，也就是实际储存并不是顺序的
const int MAX = 1000;
const int LIMIT = 500;

int IndexFind(Node &obj)
{
    int start, total;
    File.get_info(start, 2);
    File.get_info(total, 1);
    File.read(block_index[1], 3 * sizeof(int), total);
    int last = start;
    for (int i = start; i; i = block_index[i].next)
    {
        if (obj < block_index[i])
        {
            return last;
        }
        last = i;
    }
    return last;
}

void Divide(int num, int size)
{
    int mid = size / 2;
    Node preNode, newNode;
    File.read(preNode, 3 * sizeof(int) + (num - 1) * sizeof(Node));
    File.read(block_num[1], preNode.index_num * MAX * sizeof(Node) + 3 * sizeof(int), size);
    int total, div_pos;
    File.get_info(total, 1);
    File.get_info(div_pos, 3);
    total++;
    div_pos++;
    File.write_info(total, 1);
    File.write_info(div_pos, 3);
    File.write(block_num[mid + 1], div_pos * MAX * sizeof(Node) + 3 * sizeof(int), size - mid);
    newNode = block_num[mid + 1];
    newNode.size = size - mid;
    newNode.next = preNode.next;
    newNode.index_num = div_pos;
    File.write(newNode, (total - 1) * sizeof(Node) + 3 * sizeof(int));
    preNode.size = mid;
    preNode.next = total; // 以乱序储存，以顺序读取
    File.write(preNode, (num - 1) * sizeof(Node) + 3 * sizeof(int));
}

void Insert(Node key)
{
    int total;
    File.get_info(total, 1);
    // 如果是第一个元素
    if (total == 0)
    {
        total++;
        key.index_num = 1;
        key.size = 1;
        File.write_info(total, 1);
        File.write_info(total, 2);
        File.write_info(total, 3);
        File.write(key, 3 * sizeof(int), 1);
        File.write(key, 3 * sizeof(int) + MAX * sizeof(Node));
        return;
    }
    Node preNode, newNode;       // 先前与更改之后的索引块
    int target = IndexFind(key); // 索引块的位置
    File.read(preNode, 3 * sizeof(int) + (target - 1) * sizeof(Node), 1);
    // 在数据块中插入
    File.read(block_num[1], 3 * sizeof(int) + preNode.index_num * MAX * sizeof(Node), preNode.size);
    int num = std::lower_bound(block_num + 1, block_num + 1 + preNode.size, key) - block_num - 1;
    if (block_num[num + 1] == key && num < preNode.size)
    {
        return; // 重复元素
    }
    if (num == 0)
    {
        // 说明是这个块中最小的元素
        File.write(key, sizeof(int) * 3 + preNode.index_num * MAX * sizeof(Node), 1);
        File.write(block_num[1], 3 * sizeof(int) + (preNode.index_num * MAX + 1) * sizeof(Node), preNode.size);
        newNode = key; // 索引块为块中的第一个块
    }
    else
    {
        File.write(block_num[1], sizeof(int) * 3 + preNode.index_num * MAX * sizeof(Node), num);
        File.write(key, sizeof(int) * 3 + (preNode.index_num * MAX + num) * sizeof(Node), 1);
        if (num != preNode.size)
        {
            // key不是最大的
            File.write(block_num[num + 1], sizeof(int) * 3 + (preNode.index_num * MAX + num + 1) * sizeof(Node),
                       preNode.size - num);
        }
        newNode = block_num[1];
    }
    // 覆盖原有索引块
    newNode.size = preNode.size + 1;
    newNode.next = preNode.next;
    newNode.index_num = preNode.index_num;
    File.write(newNode, 3 * sizeof(int) + (target - 1) * sizeof(Node));
    if (newNode.size > LIMIT)
    {
        // 裂块
        Divide(target, newNode.size);
    }
}

void Find(Node &key)
{
    int total, start;
    int theIndex = IndexFind(key);
    File.get_info(total, 1);
    File.get_info(start, 2);
    if (total == 0)
    {
        std::cout << "null" << '\n';
        return;
    }
    Node tmp;
    bool nextOne = true;
    bool flag = false;
    while (nextOne)
    {
        File.read(tmp, 3 * sizeof(int) + (theIndex - 1) * sizeof(Node));
        File.read(block_num[1], 3 * sizeof(int) + tmp.index_num * MAX * sizeof(Node), tmp.size);
        int num = std::lower_bound(block_num + 1, block_num + tmp.size + 1, key) - block_num;
        for (int i = num; i <= tmp.size; i++)
        {
            if (strcmp(block_num[i].index, key.index) == 0)
            {
                flag = true;
                std::cout << block_num[i].value << ' ';
            }
            else
            {
                nextOne = false;
                break;
            }
        }
        theIndex = tmp.next; // 下一个块里面可能还有index
        if (theIndex == 0)
        { // 找到最后一个块了
            break;
        }
    }
    if (!flag)
    {
        std::cout << "null";
    }
    std::cout << '\n';
}

void Delete(Node key)
{
    int total, start;
    int target = IndexFind(key);
    File.get_info(total, 1);
    if (total == 0)
    {
        return;
    }
    Node tmp;
    File.read(tmp, 3 * sizeof(int) + (target - 1) * sizeof(Node));
    File.read(block_num[1], MAX * sizeof(Node) * tmp.index_num + 3 * sizeof(int), tmp.size);
    int num = std::lower_bound(block_num + 1, block_num + tmp.size + 1, key) - block_num - 1;
    if (key != block_num[num + 1])
        return;
    if (num == 0)
    {
        // 元素在第一个
        File.write(block_num[2], 3 * sizeof(int) + MAX * tmp.index_num * sizeof(Node), tmp.size - 1);
    }
    else
    {
        File.write(block_num[1], MAX * tmp.index_num * sizeof(Node) + sizeof(int) * 3, num);
        File.write(block_num[num + 2], (MAX * tmp.index_num + num) * sizeof(Node) + 3 * sizeof(int), tmp.size - num - 1);
    }
    // 处理索引块
    File.get_info(start, 2);
    tmp.size--;
    File.write(tmp, 3 * sizeof(int) + (target - 1) * sizeof(Node));
    if (tmp.size == 0)
    {
        if (start == target)
        {
            start = tmp.next; // 把开头的块删除
            File.write_info(start, 2);
        }
        else
        { // 类似于链表的构造
            for (int i = start; i; i = block_index[i].next)
            {
                if (block_index[i].next == target)
                {
                    block_index[i].next = tmp.next;
                    File.write(block_index[i], 3 * sizeof(int) + (i - 1) * sizeof(Node));
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
    File.initialise("file_name");
    int tmp;
    File.get_info(tmp, 1);
    for (int i = 1; i <= n; ++i)
    {
        std::string command;
        std::cin >> command;
        if (command == "insert")
        {
            char index[66];
            int value;
            std::cin >> index >> value;
            Node node(index, value);
            Insert(node);
        }
        else if (command == "delete")
        {
            char index[66];
            int value;
            std::cin >> index >> value;
            Node node(index, value);
            Delete(node);
        }
        else if (command == "find")
        {
            char index[66];
            std::cin >> index;
            Node node(index);
            Find(node);
        }
    }
    return 0;
}