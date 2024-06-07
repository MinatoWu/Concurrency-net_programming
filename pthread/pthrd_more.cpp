/*************************************************************************
    > File Name: pthrd_more.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Wed 01 May 2024 02:21:58 AM PDT
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include<pthread.h>
#include<string.h>
using namespace std;
int i;
void *tfn(void*arg){
    
   cout<<"i im "<<i++<<" thread:pid: "<<pthread_self()<<" pid :"<<getpid()<<endl;
   return NULL;
}
int main()
{
     
    pthread_t tid;
    for(i=0;i<5;i++){
    int ret = pthread_create(&tid,NULL,tfn,NULL);
     if(ret!=0){
         printf("pthread error",strerror(ret));
    }
    }
  sleep(2);
    return 0;
}
