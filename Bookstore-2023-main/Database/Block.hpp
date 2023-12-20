#ifndef CODE_BLOCK_HPP
#define CODE_BLOCK_HPP

#include <cstring>

template <typename valueType>
class Block // 序列化存储
{
public:
    valueType value; // 值
    int size;        // 索引块对应值块个数
    int nextBlock;   // 索引块后继索引->
    int idx;         // 索引块：值块位置
    char index[31];  // 键 ID:31 ISBN:21

    Block() : value(), size(0), nextBlock(0), idx(0), index{} {}

    // Block(const char *key, const valueType &Value) : size(0), nextBlock(0), idx(0), index(key), value(Value) {}
    Block(const char *key, const valueType &Value) : size(0), nextBlock(0), idx(0)
    {
        strncpy(index, key, sizeof(index));
        value = Value;
    }
    Block(const char *key) : size(0), nextBlock(0), idx(0), value()
    {
        strncpy(index, key, sizeof(index));
    }

    // void initialise() // 初始化读入
    // {

    // }

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

#endif