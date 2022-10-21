#include <bits/stdc++.h>
#include "../header_files/constants.hpp"
#include "../header_files/classes.hpp"
using namespace std;

void querie_options(){
    cout<<"************************************\n";
    cout<<"Enter queries:\n";
    cout<<"insert value     (value: integer)\n";
    cout<<"delete value     (value: integer)\n";
    cout<<"display show_all_buckets (show_all_buckets:0 for unique, 1 for all)\n";
    cout<<"exit\n";
    cout<<"************************************\n";
}

int Directory::hashing(int value){
    return value&((1<<global_depth) -1);
}

string Directory::bucket_id(int value){
    string ans="";
    int depth = directories[value]->local_depth;
    if(Debug){
        cout<<"Debug=> bucket_id(), depth of bucket: "<<depth<<"\n";
    }
    while(value>0 && depth>0){
        ans = (value%2==0?"0":"1")+ans;
        value /= 2;
        depth--;
    }
    while(depth>0){
        ans = "0"+ans;
        depth--;
    }
    return ans;
}

void Directory::expand(int localDepth){
    // new directories will be pointing to old buckets 
    // then after splitting pointers will also split

    // for(int i= directories.size(); i<(2* directories.size()); i++){
    //     directories.push_back(directories[i]);
    // }
    for(int i = 0; i < 1<<global_depth; i++){
        directories.push_back(directories[i]);
    }
    if(Debug){
        cout<<"Debug => expand(), Done\n";
    }
    global_depth++;
}

int Directory::pairedIndex(int directory_no, int local_depth){

    return directory_no^(1<<(local_depth-1)); // using local_depth before increased
}

void Directory::split(int directory_no){
    Bucket* cur_bucket = directories[directory_no];
    int local_depth = ++(cur_bucket->local_depth);
    if(Debug){
        cout<<"Debug => split(), local_depth: "<<local_depth<<"\n";
    }

    if(local_depth>global_depth) expand(local_depth);
    //let 4 directories pointing to 1 bucket
    // after split 2-2 will point to each bucket alternatively
    int next_dir_index = pairedIndex(directory_no, local_depth);
    directories[next_dir_index] = new Bucket(local_depth);
    // copy-
    set<int> temp_val = cur_bucket->stored_val;
    if(Debug){
        cout<<"Debug => split(), coping stored_val \n";
        for(auto i = temp_val.begin(); i!=temp_val.end();i++){
            cout<<(*i)<<" ";
        }
        cout<<"\n";
    }
    //clear-
    cur_bucket->stored_val.clear();

    int alternative_diff = 1<<local_depth;
    // we may need to move up or down depending on the directory_no we have
    for(int i= next_dir_index-alternative_diff; i>=0 ; i -= alternative_diff){
        directories[i] = directories[next_dir_index];
    }
    for(int i= next_dir_index+alternative_diff; i<(1<<global_depth) ; i+= alternative_diff){
        directories[i] = directories[next_dir_index];
    }

    for(auto i = temp_val.begin(); i!=temp_val.end();i++){
        insert_dir((*i),1);
    }
}

void Directory::insert_dir(int value, bool reinserted){
    //directory id -> pointer
    //bucket_no -> pointing to bucket
    int directory_no = hashing(value);
    int cases = directories[directory_no]->insert_bucket(value); 
    if(Debug){
        cout<<"Debug => insert_dir(), "<<"cases: "<<cases
        <<" directory_no "<<directory_no<<"\n";
    }
    if(cases == 1){
        if(!reinserted) cout<<value<<" inserted in bucket "<<bucket_id(directory_no)<<"\n";
        else cout<<value<<" moved to bucket "<<bucket_id(directory_no)<<"\n";
    }
    else if(cases == 0){
        // call split
        split(directory_no);
        insert_dir(value, reinserted);
    }
    else{
        cout<<"value "<<value<<" already exists in bucket "<<bucket_id(directory_no)<<"\n";
    }
}

void Directory::delete_dir(int value){
    int directory_no = hashing(value);
    auto temp_bucket = directories[directory_no];
    int local_dep = temp_bucket->local_depth;
    auto it = temp_bucket->stored_val.find(value);
    if(it!=temp_bucket->stored_val.end()){
        temp_bucket->stored_val.erase(it);
        cout<<"Deleted value: "<<value<<" from bucket "<<bucket_id(directory_no)<<"\n";
    }
    else{
        cout<<"This value doesn't exit, please enter the correct one.\n";
    }

    // we can merge if space is available
    // need to see only last k-1 bits

    if(local_dep == 0){
        return;
    }

    int pair_dir_index = pairedIndex(directory_no, local_dep);
    auto temp_bucket_2 = directories[pair_dir_index];
    if(temp_bucket->stored_val.size() 
        + temp_bucket_2->stored_val.size() 
        <= BucketSize
        && temp_bucket_2->local_depth == local_dep) 
    {
        for(auto i: temp_bucket_2->stored_val){
            temp_bucket->stored_val.insert(i);
        }
        delete(directories[pair_dir_index]);
        directories[pair_dir_index] = directories[directory_no];

        int alter_diff = 1<<local_dep;
        for(int i=pair_dir_index-alter_diff ; i>=0 ; i-=alter_diff){
            directories[i] = directories[directory_no];
        }
        for(int i=pair_dir_index+alter_diff ; i< (1<<global_depth) ; i+=alter_diff){
            directories[i] = directories[directory_no];
        } 
    }

    --temp_bucket->local_depth;
    // to half the directories we need to check all local_depth < global_depth
    
    int count = 0;
    for(int i=0;i<(1<<global_depth);i++){
        if(directories[i]->local_depth < global_depth){
            count++;
        }
    }
    if(count == (1 << global_depth)){
        global_depth--;
        directories.resize(1<<global_depth);
    }
}

void Directory::display(bool show_all_buckets){
    cout<<"Global depth : "<<global_depth<<"\n";
    string temp_id;
    set<string> bucket_list;
    for(int i=0;i<directories.size();i++){
        temp_id = bucket_id(i);
        if(show_all_buckets || bucket_list.find(temp_id)==bucket_list.end()){
            bucket_list.insert(temp_id);
            for(int a=directories[i]->local_depth; a<= global_depth;a++){
                cout<<" ";
            }
            cout<<temp_id<<" => ";
            for(auto j=directories[i]->stored_val.begin();j!=directories[i]->stored_val.end();j++){
                cout<<*j<<" ";
            }
            cout<<"\n";
        }
    }
}

// Bucket functions-
int Bucket::insert_bucket(int value){
    auto it = stored_val.find(value);
    if(it != stored_val.end()){
        return -1;
    }
    if(is_full()){
        return 0;
    }
    stored_val.insert(value);
    return 1;

}

int Bucket::is_full(){
    if(stored_val.size()==BucketSize){
        return 1;
    }
    else{
        return 0;
    }

}