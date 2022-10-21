#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
void querie_options();
// void Directory::insert_dir(int value); wrong syntax
void insert_dir(int value, bool reinserted);
int insert_bucket(int value);
void delete_dir(int value);
void display(bool show_all_buckets);
#endif 