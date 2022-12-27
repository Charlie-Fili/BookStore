#ifndef INC_1_6BOOKSTORE_UNROLLEDLINKEDLIST_DOUBLE_H
#define INC_1_6BOOKSTORE_UNROLLEDLINKEDLIST_DOUBLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <list>

//template<typename index_type, typename value_type>
//using index_type = char[64];


template<typename index_type1, typename index_type2>
class double_BlockList {
public:
    using value_type = int;
    static const int BlockSize = 320; // sqrt of (100000)
    const int minSize = 160;

    struct Node { // 最小的一个节点
        index_type1 index1;
        index_type2 index2;
        value_type value = -1;

        Node() {
            memset(index1, 0, sizeof(index1));
            memset(index2, 0, sizeof(index2));
        };

        Node(const index_type1 &index1_,const index_type2 &index2_, const value_type &value_) {
            memset(index1, 0, sizeof(index1));
            memset(index2, 0, sizeof(index2));
            strcpy(index1, index1_);
            strcpy(index2, index2_);
            value = value_;
        }

        Node &operator=(const Node &rhs) {
            if (this == &rhs) return *this;
            memset(index1, 0, sizeof(index1));
            memset(index2, 0, sizeof(index2));
            strcpy(index1, rhs.index1);
            strcpy(index2, rhs.index2);
            value = rhs.value;
            return *this;
        }

        bool operator<(const Node &rhs) const {
            if (strcmp(index1, rhs.index1) != 0) {
                if (strcmp(index1, rhs.index1) < 0) return true;
                return false;
            }
            if (strcmp(index2, rhs.index2) < 0) return true;
            return false;
        }

        bool operator>(const Node &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const Node &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const Node &rhs) const {
            return !(*this < rhs);
        }

        bool operator==(const Node &rhs) const {
            if (strcmp(index1, rhs.index1) != 0)return false;
            if (strcmp(index2, rhs.index2) != 0)return false;
            return true;
        }

        bool operator!=(const Node &rhs) const {
            return !(*this == rhs);
        }
    };

    using Block = Node[BlockSize];

public:
    struct HeadNode {
        int tag = -1; // 记录从HeadNode到body的索引（注意重复利用）
        int size = 0; // 对应body的长度
        Node bound; // 记录body中最大node

        HeadNode() = default;

        HeadNode(const int &tag_, const int &size_, Node bound_) : tag(tag_), size(size_),
                                                                   bound(bound_) {}
    };

    std::list<HeadNode> HeadList; // store headnode
    std::vector<int> storage; // store unused tag;

    std::fstream store_of_body;
    std::string filename_of_body;
    std::fstream store_of_head;
    std::string filename_of_head;

    Block tmp_store;
    std::vector<int> indexes;

    double_BlockList() = default;

    double_BlockList(const std::string &fileName1, const std::string &fileName2);

    ~double_BlockList();

    void readFile(Block &read_, const int &location);

    void writeFile(Block &write_, const int &location);

    int find(Node &node_, typename std::list<HeadNode>::iterator &iter);

    int assign_tag();

    void restore_tag(int tag_);

    // 暂时未实现并块

    void insert(Node &node_, typename std::list<HeadNode>::iterator &iter);

    void erase(Node &node_, typename std::list<HeadNode>::iterator &iter);

    void split(Block &tmp_store1, typename std::list<HeadNode>::iterator iter);

    void insert(const index_type1 &index1_, const index_type2 &index2_, value_type value_);

    void erase(const index_type1 &index1_, const index_type2 &index2_, value_type value_);

    void search(const index_type1 &index1_);
};

template class double_BlockList<char[61], char[21]>;

#endif //INC_1_6BOOKSTORE_UNROLLEDLINKEDLIST_DOUBLE_H
