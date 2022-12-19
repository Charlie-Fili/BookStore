#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <list>


//template<typename index_type, typename value_type>
using index_type = char[64];
using value_type = int;

//    static const int unit_len = 64 + sizeof(int);
const int BlockSize = 320; // sqrt of (100000)
const int minSize = 160;

class BlockList {
public:
    struct Node {
        index_type index;
        value_type value = -1;

        Node() = default;

        Node(const index_type &index_, const value_type &value_) {
//            index = index_;
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
//            if (index != rhs.index)return index < rhs.index;
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
//            if (index != rhs.index)return false;
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
        int tag = -1; // mark difference and order;
        int size = 0;
        Node bound;

        HeadNode() = default;

        HeadNode(const int &tag_, const int &size_, Node bound_) : tag(tag_), size(size_),
                                                                   bound(std::move(bound_)) {}

//        HeadNode(const int &tag_, const int &size_, const index_type &index_, const value_type &value_) : tag(tag_),
//                                                                                                          size(size_) {
//            Node tmp(index_, value_);
//            bound = tmp;
//        }
    };

    std::list<HeadNode> HeadList; // store headnode
    std::vector<int> storage; // store unused tag;
    std::fstream store_of_body;
    std::string filename_of_body;
    std::fstream store_of_head;
    std::string filename_of_head;


    BlockList() = default;

    BlockList(const std::string &fileName1, const std::string &fileName2) {
        filename_of_body = fileName1;
        filename_of_head = fileName2;
        store_of_body.open(fileName1);
        if (!store_of_body.good()) {
            store_of_body.open(fileName1, std::fstream::out);
        }
        store_of_head.open(fileName2);
        if (!store_of_head.good()) {
            store_of_head.open(fileName2, std::fstream::out);
        } else {
            int total = 0;
            int used = 0;
            store_of_head.seekg(0);
            store_of_head.read(reinterpret_cast <char *> (&total), sizeof(total));
            store_of_head.read(reinterpret_cast <char *> (&used), sizeof(used));
//            std::vector<HeadNode> temp;
            HeadNode temp;
            int k = 0;
            while (k < total) {
                ++k;
                store_of_head.read(reinterpret_cast <char *> (&temp), sizeof(temp));
                if (temp.size != 0) HeadList.push_back(temp);
            }
//            store_of_head.read(reinterpret_cast <char *> (&HeadList.front()), sizeof(HeadNode) * used);
            bool *ifUsed = new bool[total];
            storage.resize(total);
            for (auto &tmp: HeadList) { ifUsed[tmp.tag] = true; }
            int j = 0;
            for (int i = 0; i < total; ++i)
                if (!ifUsed[i]) { storage[j++] = i; }
            storage.resize(j);
            delete[] ifUsed;
        }
    }

    ~BlockList() {
        store_of_head.seekp(0);
        int total = HeadList.size() + storage.size();
        int used = HeadList.size();
        store_of_head.write(reinterpret_cast <const char *> (&total), sizeof(int));
        store_of_head.write(reinterpret_cast <const char *> (&used), sizeof(int));
//        store_of_head.close();
//        store_of_head.open(filename_of_head);
        for (auto &it: HeadList) store_of_head.write(reinterpret_cast <char *> (&it), sizeof(it));
//        store_of_head.write(reinterpret_cast <char *> (&HeadList.front()), sizeof(HeadNode) * used);
        HeadList.clear();
        storage.clear();
        store_of_head.close();
        store_of_body.close();
    }

    void readFile(Block &read_, const int &location) {
        store_of_body.seekg(sizeof(Block) * location);
        store_of_body.read(reinterpret_cast<char *>(&read_), sizeof(Block));
    }

    void writeFile(Block &write_, const int &location) {
        store_of_body.seekp(sizeof(Block) * location);
        store_of_body.write(reinterpret_cast<char *>(&write_), sizeof(Block));
    }

    Block tmp_store;

    int find(Node &node_, std::list<HeadNode>::iterator &iter) {
        readFile(tmp_store, iter->tag);
        return std::lower_bound(tmp_store, tmp_store + iter->size, node_) - tmp_store;
    }

    int assign_tag() {
        if (storage.empty()) return HeadList.size();
        else {
            int tag_ = storage.back();
            storage.pop_back();
            return tag_;
        }
    }

    void restore_tag(int tag_) {
        storage.push_back(tag_);
    }

    void insert(Node &node_, std::list<HeadNode>::iterator &iter) {
        int location = find(node_, iter);
        readFile(tmp_store, iter->tag);
        if (node_ == tmp_store[location]) return;
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
        if (iter->size < BlockSize) {
            writeFile(tmp_store, iter->tag);
            Block omg;
            readFile(omg,iter->tag);
            int a;
            a=1;
        }
        else split(tmp_store, iter);
    }

    void erase(Node &node_, std::list<HeadNode>::iterator &iter) {
        if (iter->size == 1) {
            if (node_ != iter->bound) return;
            HeadList.erase(iter);
            restore_tag(iter->tag);
            Block tmp;
            writeFile(tmp, iter->tag);
            return;
        }
        int location = find(node_, iter);
        readFile(tmp_store, iter->tag);
        if (node_ != tmp_store[location]) return;
        Node node;
        if (location == iter->size - 1) {
            iter->bound = tmp_store[iter->size - 2];
            tmp_store[location] = node;
        } else {
            for (auto i = location; i < iter->size - 1; ++i) {
                tmp_store[i] = tmp_store[i + 1];
            }
            tmp_store[iter->size - 1] = node;
        }
        --(iter->size);
//        if (iter->size <= BlockSize)
        writeFile(tmp_store, iter->tag);
//        else bind(node_, iter);
    }

    void split(Block &tmp_store1, std::list<HeadNode>::iterator iter) {
        Block tmp_store2;
        for (auto i = minSize; i < BlockSize; ++i) {
            tmp_store2[i - minSize] = tmp_store1[i];
            tmp_store1[i] = tmp_store2[minSize + 1];
        }
        iter->size = minSize;
        HeadNode new_head(assign_tag(), minSize, iter->bound);
        iter->bound = tmp_store1[minSize - 1];
        HeadList.push_back(new_head);
        writeFile(tmp_store1, iter->tag);
        writeFile(tmp_store2, new_head.tag);
    }

    void insert(const index_type &index_, value_type value_) {
        Node tmp(index_, value_);
        if (HeadList.empty()) {
            tmp_store[0] = tmp;
            HeadList.emplace_back(assign_tag(), 1, tmp);
            writeFile(tmp_store, HeadList.front().tag);
            return;
        }

        auto iter = HeadList.begin();

        while (iter != HeadList.end()) {
            if (strcmp(iter->bound.index, index_) > 0) {
                insert(tmp, iter);
                return;
            }
            ++iter;
        }
        insert(tmp, --iter);
    }

    void erase(const index_type &index_, value_type value_) {
        Node tmp(index_, value_);

        auto iter = HeadList.begin();

        while (iter != HeadList.end()) {
            if (strcmp(iter->bound.index, index_) > 0) {
                erase(tmp, iter);
                return;
            }
            ++iter;
        }
    }

    void find(const index_type &index_) {
        Node tmp(index_, -1);
        if (HeadList.empty()) {
            std::cout << "null\n";
            return;
        }
        auto iter = HeadList.begin();
        while (iter != HeadList.end()) {
            if (strcmp(iter->bound.index, index_) >= 0) {
                int location = find(tmp, iter);
                readFile(tmp_store, iter->tag);
                bool flag = true;
                if (strcmp(index_, tmp_store[location].index) != 0) {
                    std::cout << "null\n";
                    return;
                } else {
                    std::cout << tmp_store[location].value << ' ';
                    while (location < iter->size) {
                        ++location;
                        if (strcmp(index_, tmp_store[location].index) == 0) {
                            std::cout << tmp_store[location].value << ' ';
                        } else {
                            flag = false;
                            break;
                        }
                    }
                    while (flag and iter != HeadList.end()) {
                        ++iter;
                        if (iter != HeadList.end()) {
                            location = -1;
                            while (location < iter->size) {
                                ++location;
                                if (strcmp(index_, tmp_store[location].index) == 0) {
                                    std::cout << tmp_store[location].value << ' ';
                                } else {
                                    flag = false;
                                    break;
                                }
                            }
                        }
                    }
                }
                std::cout << '\n';
                return;
            }
            ++iter;
        }
        std::cout << "null\n";
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    BlockList ull("body", "head");
    int n;
    std::string instruct;
    char index[64];
    memset(index, 0, sizeof(index));
    int value;
    std::cin >> n;
    while (n--) {
        std::cin >> instruct >> index;
        if (instruct == "find") {
            ull.find(index);
            continue;
        }
        std::cin >> value;
        if (instruct == "insert") {
            ull.insert(index, value);
        } else if (instruct == "delete") {
            ull.erase(index, value);
        }
    }
    return 0;
}

