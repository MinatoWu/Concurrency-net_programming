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
void *working(void*arg);
//创建int 数组，为了储存通信文件描述符，因为线程共享内存，地址不能变，一变就会影响其他的连接，数组大小就意味着，最大的通信链接数
int fds[1024];
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
   memset(fds,-1,sizeof(fds));//-1代表无效描述符
   while(1){
    int cfd =accept(lfd,(struct sockaddr*)&cliaddr,&clilen);
    if(cfd==-1){
       perror("accept失败重新尝试链接\n");
        continue;
         //连接建立，通信流程；  
    }
    char myip[24];
    cout<<"客户端的端口"<<ntohs(cliaddr.sin_port)<<" 客户端的IP"<<inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,myip,sizeof(myip))<<endl;
     //找到一个数组中的位置储存文件描述符
     int *ptr=NULL;
     int len=sizeof(fds)/sizeof(int);
     for(int i=0;i<len;i++){
        if(fds[i]==-1){
            //找到了可用的
            fds[i]=cfd;
            ptr=&fds[i];//指向那个可用的地址
            break;
        }
            
     }
    //走到这里说明 连接建立完成，创键子线程
        pthread_t tid;
     pthread_create(&tid,NULL,working,ptr);//不同数据写到不同内存块内，使他们不影响,为什么不值传递，因为值传有局限性，万一需要解析ip，端口信息，值传递很局限
        //线程分离
        pthread_detach(tid);
   }  
return 0;
}
 
            

void *working(void*arg){
    //当前的服务器和连接成功的客户端通信
    //强制转换,然后解引用
    int cfd=*(int*)arg;
    while(1){
      char buf[1024];
      memset(buf,0,sizeof(buf));
      int len=read(cfd,buf,sizeof(buf));
      if(len==0){
        cout<<"客户端断开了连接"<<endl;
        break;
      }
      else if(len==-1){
          perror("read error");
          break;
      }
      else{
          cout<<"客户端say ："<<buf<<endl;
          write(cfd,buf,len);
      }

    }
    close(cfd);
    *(int *)arg=-1;//将其改为可用
    return  NULL;
   
}


  

