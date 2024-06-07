/*************************************************************************
    > File Name: phtrd_producter_consumer.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Sun 12 May 2024 12:25:48 AM PDT
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
//链表作为共享数据，需要被互斥量保护
struct msg{
int num;
struct msg *next;
};
struct msg *head=NULL;

//静态初始化 一个条件变量，一个互斥锁
pthread_cond_t hasproduct=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
void *consumer(void *arg){
    struct msg *mp;
    while(1){
        //加锁
       pthread_mutex_lock(&mutex);
       while(head==NULL){//阻塞函数
        pthread_cond_wait(&hasproduct,&mutex);//阻塞等待共享区有没有数据
       }
       //头删法删掉节点
       mp=head;
       head=mp->next;
       pthread_mutex_unlock(&mutex);
       cout<<"consumer : "<<mp->num<<endl;
       cout<<"phtid : "<<pthread_self()<<endl;
       free(mp);
       sleep(rand()&3);

    }

}
void *product(void*arg){//生产者模型
    struct msg*mp;
    while(1){
     mp=(struct msg *)malloc(sizeof(struct msg));
     mp->num=rand()%1000+1;
     cout<<"product : "<<mp->num<<endl;
     pthread_mutex_lock(&mutex);
     mp->next=head;
     head=mp;
     pthread_mutex_unlock(&mutex);//解锁完之后唤醒等待的线程
     pthread_cond_signal(&hasproduct);//将等待该条件变量的一个线程唤醒
     sleep(rand()%3);
    }


}
int main(){

pthread_t pid,cid;
srand(time(NULL));
pthread_create(&pid,NULL,product,NULL);
pthread_create(&cid,NULL,consumer,NULL);

pthread_join(pid,NULL);
pthread_join(cid,NULL);


    return 0;

}
