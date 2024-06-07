/*************************************************************************
    > File Name: udp_server.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Fri 10 May 2024 01:09:35 AM PDT
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
//服务器启动后，不发数据，先接受数据
//需要绑定端口
int main(){
    //1.创建通信的套接字
    int fd=socket(AF_INET,SOCK_DGRAM,0);
    if(fd==-1){
    perror("socket");
     exit(0);
    }
#if 0
    //2.接受数据需要绑定一个固定的端口 IP
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(8989);
    addr.sin_addr.s_addr=INADDR_ANY;
    int ret =bind(fd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret==-1){
        perror("bind");
        exit(0);
    }
#endif

    //通信
    char buf[1024];
    char ip[24];
    struct sockaddr_in cliaddr;
    int clilen=sizeof(cliaddr);
    //服务器地址
    struct sockaddr_in seraddr;
    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(8989);
    inet_pton(AF_INET,"192.168.122.1",&seraddr.sin_addr.s_addr);
    
    int num=0;
    while(1){
        sprintf(buf,"hello world,..");
        cout<<num++<<endl;
        //发送数据
        memset(buf,0,sizeof(buf));
        sendto(fd,buf,strlen(buf)+1,0,(struct sockaddr*)&seraddr,sizeof(seraddr));
       int len =recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
        if(len==-1){
          break;
      }
        cout<<buf<<endl;
        sleep(1);
    }
    close(fd);

    return 0;
}
