#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <list>

//template<typename index_type, typename value_type>
using index_type = char[64];
using value_type = int;

const int BlockSize = 320; // sqrt of (100000)
const int minSize = 160;

class BlockList {
public:
    struct Node { // 最小的一个节点
        index_type index;
        value_type value = -1;

        Node() {
            memset(index, 0, sizeof(index));
        };

        Node(const index_type &index_, const value_type &value_) {
            memset(index, 0, sizeof(index));
            strcpy(index, index_);
            value = value_;
        }

        Node &operator=(const Node &rhs) {
            memset(index, 0, sizeof(index));
            strcpy(index, rhs.index);
            value = rhs.value;
            return *this;
        }

        bool operator<(const Node &rhs) const {
            if (strcmp(index, rhs.index) != 0) {
                if (strcmp(index, rhs.index) < 0) return true;
                return false;
            }
            return value < rhs.value;
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
            if (strcmp(index, rhs.index) != 0)return false;
            if (value != rhs.value)return false;
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

    BlockList() = default;

    BlockList(const std::string &fileName1, const std::string &fileName2);

    ~BlockList();

    void readFile(Block &read_, const int &location);

    void writeFile(Block &write_, const int &location);

    int find(Node &node_, std::list<HeadNode>::iterator &iter);

    int assign_tag();

    void restore_tag(int tag_);

    // 暂时未实现并块

    void insert(Node &node_, std::list<HeadNode>::iterator &iter);

    void erase(Node &node_, std::list<HeadNode>::iterator &iter);

    void split(Block &tmp_store1, std::list<HeadNode>::iterator iter);

    void insert(const index_type &index_, value_type value_);

    void erase(const index_type &index_, value_type value_);

    void find(const index_type &index_);
};

