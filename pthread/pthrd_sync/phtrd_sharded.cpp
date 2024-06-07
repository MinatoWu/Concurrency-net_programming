/*************************************************************************
    > File Name: phtrd_sharded.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Sun 05 May 2024 11:05:24 PM PDT
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include<pthread.h>
using namespace std;

void *tfn (void*afg){
   srand(time(NULL));
    while(1){
    
    cout<<"hello";
    sleep(rand()%3);
    cout<<"world"<<endl;
    sleep(rand()%3);

    }
    return NULL;
}


int main(){

    pthread_t tid;
    srand(time(NULL));
    pthread_create(&tid,NULL,tfn,NULL);
    while(1){
     cout<<"HELLO";
     sleep(rand()%3);
     cout<<"WORLD"<<endl;
     sleep(rand()%3);
    }
    pthread_join(tid,NULL);
    
    return 0;
}
