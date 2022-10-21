#include <bits/stdc++.h>
#include "../header_files/constants.hpp"
#include "../header_files/functions.hpp"
#include "../header_files/classes.hpp"
using namespace std;

int BucketSize = 1;
int InitialGlobalDepth = 1;

int main(){
    bool show_all_buckets = 0; // including duplicate once
    string choice;
    int value;



    cout<<"Enter Bucket Size: ";
    cin>>BucketSize;
    cout<<"Enter Initial global depth : ";
    cin>>InitialGlobalDepth;

    Directory d(InitialGlobalDepth);
    if(Debug){
        cout<<"Debug => "<<"initialized directory\n";
    }
    querie_options();
    bool conti = true;
    while(conti){
        cout<<"==> ";
        cin>>choice;
        if(Debug){
            cout<<"Debug => main(), "<<"user choice: "<<choice<<"\n";
        }

        if(choice=="insert"){
            cin>>value;
            // call insert
            d.insert_dir(value, 0);
        }
        else if(choice=="delete"){
            cin>>value;
            //call delete
            d.delete_dir(value);
        }
        else if(choice=="display"){
            // call display
            cin>>show_all_buckets;
            d.display(show_all_buckets);
        }
        else if(choice=="exit"){
            conti = false;
        }
    }

    return 0;
}
