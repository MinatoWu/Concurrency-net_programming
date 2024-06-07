/*************************************************************************
    > File Name: pthrd_var.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Thu 02 May 2024 01:49:13 AM PDT
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

int var =200;
void*tfn(void*afg){
   var =100;
    cout<<"thread ,var :"<<pthread_self()<<"  "<<var<<endl;
    return NULL;
}
int main(){

    cout<<"at first var : "<<var<<endl;
    pthread_t tid;
    pthread_create(&tid,NULL,tfn,NULL);
    sleep(2);
    cout<<"after pthread_create ,var :"<<var<<endl;
    return 0;
}

