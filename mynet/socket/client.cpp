/*************************************************************************
    > File Name: server.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Mon 29 Apr 2024 11:46:25 PM PDT
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <map>
#include <vector>
using namespace std;
int main(){

    // 1.创建通信的套接字
 int cfd=socket(AF_INET,SOCK_STREAM,0);
 if(cfd==-1){
 perror("socket");
 exit(0);
 }

 //2.connect连接
 struct sockaddr_in addr;
 addr.sin_family =AF_INET;  //IPV4
 addr.sin_port=htons(8989);//端口，转换大端
 //将192.168.88.130 ->大端整型
 inet_pton(AF_INET,"192.168.88.130",&addr.sin_addr.s_addr);
 int ret =connect(cfd,(struct sockaddr*)&addr,sizeof(addr));//绑定
if(ret==-1){//如果绑定失败
perror("connect");
exit(0);
}


//3. 通信
 int num=0;
while (1){
 //发送数据
 char buf[1024];
  sprintf(buf,"hello,world,%d,.......",num++);
  send(cfd,buf,strlen(buf)+1,0);
  //接受数据
  memset(buf,0,sizeof(buf));
int len= recv(cfd,buf,sizeof(buf),0);
 if(len==0){
cout<<"服务器断开连接"<<endl;
break;
 }
else if(len>0){
printf("recv buf: %s\n",buf);
}
else{
perror("recv");
break;
}

sleep(2);

}
// 4.断开链接
close(cfd);
  



    return 0;
}

