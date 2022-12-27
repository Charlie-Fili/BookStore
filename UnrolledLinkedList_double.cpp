#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include "UnrolledLinkedList_double.h"

template<typename index_type1, typename index_type2>
double_BlockList<index_type1,index_type2>::double_BlockList(const std::string &fileName1, const std::string &fileName2) {
    filename_of_body = fileName1;
    filename_of_head = fileName2;

    store_of_body.open(fileName1);
    if (!store_of_body.good()) { // 是否成功打开
        store_of_body.open(fileName1, std::fstream::out); // 新建
        store_of_body.close();
        store_of_body.open(fileName1); // 变为可读可写
    }
    store_of_head.open(fileName2);
    if (!store_of_head.good()) { // 是否成功打开
        store_of_head.open(fileName2, std::fstream::out); // 新建
        store_of_head.close();
        store_of_head.open(fileName2); // 变为可读可写
    } else { // 若已存在
        int total = 0; // 所有使用过的tag数
        int used = 0; // 正在使用的tag数

        store_of_head.seekg(0);
        store_of_head.read(reinterpret_cast <char *> (&total), sizeof(total));
        store_of_head.read(reinterpret_cast <char *> (&used), sizeof(used));

        HeadNode temp;
        int k = 0;
        while (k < total) { // 读入HeadNode链表
            ++k;
            store_of_head.read(reinterpret_cast <char *> (&temp), sizeof(temp));
            if (temp.size != 0) HeadList.push_back(temp);
        }

        bool *ifUsed = new bool[total]; // 读入整理storage
        storage.resize(total);
        for (auto &tmp: HeadList) { ifUsed[tmp.tag] = true; }
        int j = 0;
        for (int i = 0; i < total; ++i)
            if (!ifUsed[i]) { storage[j++] = i; }
        storage.resize(j);
        delete[] ifUsed;
    }
}

template<typename index_type1, typename index_type2>
double_BlockList<index_type1,index_type2>::~double_BlockList() {
    store_of_head.seekp(0);
    int total = HeadList.size() + storage.size();
    int used = HeadList.size();
    store_of_head.write(reinterpret_cast <const char *> (&total), sizeof(int));
    store_of_head.write(reinterpret_cast <const char *> (&used), sizeof(int));
    for (auto &it: HeadList) store_of_head.write(reinterpret_cast <char *> (&it), sizeof(it));
    HeadList.clear();
    storage.clear();
    store_of_head.close();
    store_of_body.close();
}

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::readFile(Block &read_, const int &location) {
    store_of_body.seekg(sizeof(Block) * location);
    store_of_body.read(reinterpret_cast<char *>(&read_), sizeof(Block));
}

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::writeFile(Block &write_, const int &location) {
    store_of_body.seekp(sizeof(Block) * location);
    store_of_body.write(reinterpret_cast<char *>(&write_), sizeof(Block));
}

template<typename index_type1, typename index_type2>
int double_BlockList<index_type1,index_type2>::find(Node &node_, typename std::list<HeadNode>::iterator &iter) { //找到节点插入数组的位置
    readFile(tmp_store, iter->tag);
    return std::lower_bound(tmp_store, tmp_store + iter->size, node_) - tmp_store;
}

template<typename index_type1, typename index_type2>
int double_BlockList<index_type1,index_type2>::assign_tag() { // 给出一个可用tag
    if (storage.empty()) return HeadList.size();
    else {
        int tag_ = storage.back();
        storage.pop_back();
        return tag_;
    }
}

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::restore_tag(int tag_) { // 回收一个可用tag
    storage.push_back(tag_);
}

// 暂时未实现并块

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::insert(Node &node_, typename std::list<HeadNode>::iterator &iter) { // 将node插入数组中
    int location = find(node_, iter); // 位置
    readFile(tmp_store, iter->tag); // 数组
    if (node_ == tmp_store[location]) return; // 重复

    if (location == iter->size) {
        iter->bound = node_;
        tmp_store[location] = node_;
    } else {
        for (auto i = iter->size - 1; i >= location; --i) {
            tmp_store[i + 1] = tmp_store[i];
        }
        tmp_store[location] = node_;
    }
    ++(iter->size);
    if (iter->size < BlockSize) writeFile(tmp_store, iter->tag);
    else split(tmp_store, iter); // 裂块
}

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::erase(Node &node_, typename std::list<HeadNode>::iterator &iter) { // 将node从数组中删除
    if (iter->size == 1) { // 只有一个则清理并回收
        if (node_ != iter->bound) return;
        HeadList.erase(iter);
        restore_tag(iter->tag);
        Block tmp;
        writeFile(tmp, iter->tag);
        return;
    }

    int location = find(node_, iter); // 位置
    readFile(tmp_store, iter->tag); // 数组
    if (node_ != tmp_store[location]) return; // 删除不存在的节点
    Node node;
    if (location == iter->size - 1) {
        iter->bound = tmp_store[iter->size - 2];
        tmp_store[location] = node;
    } else {
        for (auto i = location; i < iter->size - 1; ++i) {
            tmp_store[i] = tmp_store[i + 1];
        }
        memset(tmp_store[iter->size - 1].index1, 0, sizeof(tmp_store[iter->size - 1].index1));
        memset(tmp_store[iter->size - 1].index2, 0, sizeof(tmp_store[iter->size - 1].index2));
        tmp_store[iter->size - 1].value = -1;
    }
    --(iter->size);
//        if (iter->size <= BlockSize)
    writeFile(tmp_store, iter->tag);
//        else bind(node_, iter);
}

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::split(Block &tmp_store1, typename std::list<HeadNode>::iterator iter) { // 裂块
    Block tmp_store2;
    for (auto i = minSize; i < BlockSize; ++i) {
        tmp_store2[i - minSize] = tmp_store1[i];
        tmp_store1[i] = tmp_store2[minSize];
    }
    iter->size = minSize;
    HeadNode new_head(assign_tag(), minSize, iter->bound);
    iter->bound = tmp_store1[minSize - 1];
    writeFile(tmp_store1, iter->tag);
    ++iter;
    HeadList.emplace(iter, new_head);
    writeFile(tmp_store2, new_head.tag);
}

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::insert(const index_type1 &index1_,const index_type2 &index2_, value_type value_) { // 找到插入对应的节点

    Node tmp(index1_,index2_, value_);

    if (HeadList.empty()) { // 第一个插入的节点
        tmp_store[0] = tmp;
        HeadList.emplace_back(assign_tag(), 1, tmp);
        writeFile(tmp_store, HeadList.front().tag);
        return;
    }

    typename std::list<HeadNode>::iterator iter = HeadList.begin();

    while (iter != HeadList.end()) {
        if (iter->bound >= tmp) {
            insert(tmp, iter);
            return;
        }
        ++iter;
    }
    insert(tmp, --iter);
}

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::erase(const index_type1 &index1_,const index_type2 &index2_, value_type value_) { // 找到删除对应的节点
    Node tmp(index1_,index2_, value_);

    auto iter = HeadList.begin();

    while (iter != HeadList.end()) {
        if (iter->bound >= tmp) {
            erase(tmp, iter);
            return;
        }
        ++iter;
    }
}

template<typename index_type1, typename index_type2>
void double_BlockList<index_type1,index_type2>::search(const index_type1 &index_) { // 查询位置
    indexes.clear();
    index_type2 sub;
    memset(sub,0, sizeof(sub));
    Node tmp(index_,sub, -1);
    if (HeadList.empty()) {
        return;
    }
    auto iter = HeadList.begin();
    while (iter != HeadList.end()) {
        if (iter->bound >= tmp) {
            int location = find(tmp, iter);
            readFile(tmp_store, iter->tag);
            bool flag = true;
            if (strcmp(index_, tmp_store[location].index1) != 0) {
                return;
            } else {
                indexes.push_back(tmp_store[location].value); // 找到第一个后依次往后寻找
                while (location < iter->size) {
                    ++location;
                    if (location == iter->size) break;
                    if (strcmp(index_, tmp_store[location].index1) == 0) {
                        indexes.push_back(tmp_store[location].value);
                    } else {
                        flag = false;
                        break;
                    }
                }
                while (flag and iter != HeadList.end()) {
                    ++iter;
                    if (iter != HeadList.end()) {
                        readFile(tmp_store, iter->tag);
                        location = -1;
                        while (location < iter->size) {
                            ++location;
                            if (location == iter->size) break;
                            if (strcmp(index_, tmp_store[location].index1) == 0) {
                                indexes.push_back(tmp_store[location].value);
                            } else {
                                flag = false;
                                break;
                            }
                        }
                    }
                }
            }
            return;
        }
        ++iter;
    }
}

