/*************************************************************************
    > File Name: semproducercomsumer.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Sun 12 May 2024 05:06:38 AM PDT
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include<pthread.h>
#include<semaphore.h>
using namespace std;
#define NUM 5
int queue[NUM];//全局数组实现环形队列
sem_t blank_number,product_number;//空格信号量，产品信号量

void *product(void *arg)
{
    int i=0;
    while(1){
        sem_wait(&blank_number);//生产者先让空格减少，如果已经为0就阻塞
        queue[i]=rand()%1000+1;
        cout<<"produce ---"<<queue[i]<<endl;
        sem_post(&product_number); //将物品++
        i=(i+1)%NUM;//环形
        sleep(rand()%1);
    }

}
void *consumer(void*arg){
    int i=0;
    while(1){
        sem_wait(&product_number);//产品数--，为0阻塞等待
        cout<<"comsumer ----"<<queue[i]<<endl;
        queue[i]=0;//消费一个产品
        sem_post(&blank_number);//消费掉之后，空格数++；
        i=(i+1)%NUM;
        sleep(rand()%3);
    }

}
int main(){
    pthread_t cid,pid;
    sem_init(&blank_number,0,NUM);
    sem_init(&product_number,0,0);
    pthread_create(&pid,NULL,product,NULL);
    pthread_create(&cid,NULL,consumer,NULL);
    pthread_join(pid,NULL);
    pthread_join(cid,NULL);

   


    return 0;
}
