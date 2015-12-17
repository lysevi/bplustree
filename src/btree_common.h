#pragma once
#include <utility>
#include <iostream>
namespace trees{
    template<class T>
    void insert_to_array(T array[5], size_t sz,size_t cap,const T &value){ //sz => len(array), cap => capacity(array)
        assert(cap<=sz);
        auto to= cap<sz?cap:cap;
        size_t insert_pos=0;
        for(;insert_pos<to; insert_pos++){

            if ((insert_pos+1)>cap){
                break;
            }
            if (array[insert_pos+1]>value){
                break;
            }
        }
        insert_pos++;
        for(auto i=cap;i>insert_pos;i--){
//            for(size_t pos=0;pos<sz;pos++){
//                std::cout<<" "<<array[pos];
//            }   std::cout<<std::endl;
            std::swap(array[i],array[i-1]);

        }
        array[insert_pos]=value;
//        for(size_t pos=0;pos<sz;pos++){
//            std::cout<<" "<<array[pos];
//        }
//        std::cout<<std::endl;
    }
}
