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

//在全局上，定义一个互斥锁
pthread_mutex_t mutex;

void *tfn (void*afg){
   srand(time(NULL));
    while(1){
  pthread_mutex_lock(&mutex);   
    cout<<"hello";
    sleep(rand()%3);
    cout<<"world"<<endl;
   pthread_mutex_unlock(&mutex);
    sleep(rand()%3);

    }
    return NULL;
}


int main(){

    pthread_t tid;//创建线程tid
    srand(time(NULL));//定义随机数

    pthread_mutex_init(&mutex,NULL);//初始化锁

    pthread_create(&tid,NULL,tfn,NULL);
    while(1){
     pthread_mutex_lock(&mutex);//加锁

     cout<<"HELLO";
     sleep(rand()%3);
     cout<<"WORLD"<<endl;
     pthread_mutex_unlock(&mutex);//程序执行全过程一变之后才解锁,唤醒 在锁上的子线程。

     sleep(rand()%3);
    }
    pthread_join(tid,NULL);
      
    return 0;
}
