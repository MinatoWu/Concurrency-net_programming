/*************************************************************************
    > File Name: pthrd_crat.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Wed 01 May 2024 01:18:34 AM PDT
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include<string.h>
#include<pthread.h>
using namespace std;
//子线程的主题函数
 void *tfn(void*arg){
    cout<<"tfn:pid :"<<getpid();
         cout<<"pthread id:"<<pthread_self()<<endl;
     return NULL;
 }
int main(){
    cout<<"main:pid :"<<getpid();
    cout<<"pthread id:"<<pthread_self()<<endl;//先打印主线程
    //创建子进程
    pthread_t tid;
     int ret=  pthread_create(&tid,NULL,tfn,NULL);//回调函数
    if(ret){
       printf("pthread_create err %s\n",strerror(ret));
    }
    sleep(2);//防止直接return 等子线程调用完了 2秒
    return 0;
    

}
