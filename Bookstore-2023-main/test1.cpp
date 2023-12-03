#include<iostream>
#include"MemoryRiver.hpp"

int main()
{
    // 实例化一个 MemoryRiver 类，并指定文件名为 "data.bin"
    MemoryRiver<int> memoryRiver("data.bin");

    // 初始化 MemoryRiver，如果文件不存在，则创建一个新文件
    memoryRiver.initialise();

    // 示例数据写入和读取
    int data1 = 10;
    int data2 = 20;

    int index1 = memoryRiver.write(data1);
    int index2 = memoryRiver.write(data2);

    int readData1, readData2;

    memoryRiver.read(readData1, index1);
    memoryRiver.read(readData2, index2);

    std::cout << index1 << "Data 1: " << readData1 << std::endl; // 输出: Data 1: 10
    std::cout << index2 << "Data 2: " << readData2 << std::endl; // 输出: Data 2: 20

    return 0;
}
