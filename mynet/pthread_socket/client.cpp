/*************************************************************************
    > File Name: process_server.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Fri 03 May 2024 11:17:08 PM PDT
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include<sys/wait.h>
#include <string.h>
#include <cstring>
#include<pthread.h>
using namespace std;
void working(int cfd);
int main(){
//创建套接字 
int lfd=socket(AF_INET,SOCK_STREAM,0);
    if(lfd==-1){
    perror("socket");
    exit(0);
    }
    //2.绑定
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;//ipv4
    addr.sin_port=htons(9999);//端口大一点
    addr.sin_addr.s_addr=INADDR_ANY;//地址为0地址

    int ret=bind(lfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret==-1){
        perror("bind");
        exit(0);
    }

   //3. 监听
   ret =listen(lfd,128);
   if(ret == -1){
    perror("listen");
    exit(0);
   }
   //4 .等待连接，因为要接受多客户端的连接因此需要循环调用accept
   struct sockaddr_in cliaddr ;
   socklen_t clilen=sizeof(cliaddr);
   while(1){
    int cfd =accept(lfd,(struct sockaddr*)&cliaddr,&clilen);
    if(cfd==-1){
       perror("accept失败重新尝试链接\n");
        continue;
         //连接建立，通信流程，在子进程里处理通信了；  
    }
    char myip[24];
    cout<<"客户端的端口"<<ntohs(cliaddr.sin_port)<<" 客户端的IP"<<inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,myip,sizeof(myip))<<endl;
        //走到这里说明 连接建立完成，创建子进程
        pid_t pid =fork();
        if(pid==0){
            //当前为子进程，与客户端实现通信，通过cfd
            //关闭监听的lfd
            close(lfd);
            working(cfd);
            //退出
            exit(0);
           
        }
        else{
            close(cfd);//父进程关闭通信描述符
        }


 
   }
return 0;
}
void working(int cfd){
    while(1){
         char buf[1024];
        memset(buf,0,sizeof(buf));
        //read 阻塞函数，当客户端发来数据 ，代表阻塞解除
       int len= read(cfd,buf,sizeof(buf));
       if(len==0){
           cout<<"客户端关闭连接....... "<<endl;
           break;
       }
       else if(len==-1){
           perror("read");
           break;
       }
       else{
        printf("客户端说：%s\n",buf);
        //发送
        write(cfd,buf,len);
       }
    }
    close(cfd);//关闭通信描述符
}
    
    



  

