#include <iostream>
#include <fstream>
#include <string>

const int Block_Size=320;
const int unit_len=64+sizeof(int);
template<typename index_type, typename value_type>
class block_link{
    std::fstream io;
public:
    struct body_node{
        index_type index;
        value_type value;
        body_node(index_type index_,value_type value_){
            index=index_;
            value=value_;
        }
        bool operator<(const body_node &rhs) const {
            return index < rhs.index;
        }
    };
public:
    int sum=0;
    struct Blocknode{
        index_type left_end;
        index_type right_end;
        int size=0;
        Blocknode *prev= nullptr;
        Blocknode *next= nullptr;
        int order;//0base
        body_node *body[Block_Size];
    };
    Blocknode *head= nullptr;
    void insert(index_type index_,value_type value_){
        ++sum;
        body_node *temp(index_,value_);
        std::fstream in;
        in.open("store");
        if(sum==0){
            Blocknode *tmp=new Blocknode;
            tmp->left_end=index_;
            tmp->right_end=index_;
            tmp->order=0;
            tmp->body[0]=temp;
            ++tmp->size;
            head=tmp;
            in.write(reinterpret_cast<char *>(&index_),64);
            in.write(reinterpret_cast<char *>(&value_),sizeof(int));
        }
        Blocknode *find=head;
        while(find->next!=nullptr){
            if (index_<find->next) break;
            find=find->next;
        }
        if(index_>find->right_end) {
            find->right_end=index_;
            in.seekp(find->order*unit_len*Block_Size+find->num*unit_len);
            in.write(reinterpret_cast<char *>(&index_),64);
            in.write(reinterpret_cast<char *>(&value_),sizeof(int));
        }else{
            int order=0;
            body_node *sub_find=find->body[order];
            while(sub_find->next<index_){
                ++order;
                *sub_find=find->body[order];
            }
            for(int i=find->size-1;i>=order;--i){
                find->body[i+1]=find->body[i];
            }
            find->body[order]=temp;
            in.seekp(find->order*unit_len*Block_Size+order*unit_len);
            in.write(reinterpret_cast<char *>(&index_),64);
            in.write(reinterpret_cast<char *>(&value_),sizeof(int));
        }
    }
};