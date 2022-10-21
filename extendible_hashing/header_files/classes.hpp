#pragma once
#include <bits/stdc++.h>
#include "constants.hpp"

using namespace std;


class Bucket{


    int is_full();

public:
    int local_depth;
    set<int> stored_val;

    Bucket(int depth){
        this->local_depth = depth;
        // this->size = BucketSize;
    }

    int insert_bucket(int value);
        


};

// buckets are acting like directory, storing pointers
class Directory{


    int hashing(int val);
    string bucket_id(int val);

public:
    int global_depth;
    vector<Bucket *> directories;

    Directory(int depth){
        this->global_depth = depth;
        // Number of Directories = 2^Global Depth = 1<<global_depth.
        for(int i = 0; i < 1<<global_depth; i++){
            // keeping depth same since will start from 1 
            // and so on = global_depth then we will increase while spliting.
            directories.push_back(new Bucket(depth));
        }
    }

    void insert_dir(int value, bool reinserted);
    void split(int directory_no);
    void expand(int localDepth);
    int pairedIndex(int directory_no, int local_depth);

    void delete_dir(int value);
    void display(bool show_all_buckets);

};