/*************************************************************************
    > File Name: server.cpp
    > Author:Wux1aoyu
    >  
    > Created Time: Mon 29 Apr 2024 11:46:25 PM PDT
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <map>
#include <vector>
using namespace std;
int main(){

    // 1.创建监听的套接字
 int lfd=socket(AF_INET,SOCK_STREAM,0);
 if(lfd==-1){
 perror("socket");
 exit(0);
 }
 //2.绑定
 struct sockaddr_in addr;
 addr.sin_family =AF_INET;  //IPV4
 addr.sin_port=htons(8989);//端口，转换大端
 addr.sin_addr.s_addr=INADDR_ANY;//ip地址为0
 int ret =bind(lfd,(struct sockaddr*)&addr,sizeof(addr));//绑定
if(ret==-1){//如果绑定失败
perror("bind");
exit(0);
}
// 3. 设置监听 
ret =listen(lfd,128);
if(ret==-1){

    perror("listen");
    exit(0);
}
//4. 等待客户端链接
struct sockaddr_in cliaddr;
socklen_t clilen =sizeof(cliaddr);
int cfd =accept(lfd,(struct sockaddr*)&cliaddr,&clilen);//读入读出相同，//客户端
if(cfd==-1){
perror("accept");
exit(0);
}

// 5. 通信
while (1){
 //接受数据
 char buf[1024];
 memset(buf,0,sizeof(buf));
int len= recv(cfd,buf,sizeof(buf),0);
 if(len==0){
cout<<"客户端断开连接"<<endl;
break;
 }
else if(len>0){
printf("recv buf: %s\n",buf);
send(cfd,buf ,strlen(buf)+1,0);
}
else{
perror("recv");
break;
}


}
close(cfd);
close(lfd);
  



    return 0;
}
