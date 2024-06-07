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
#include<sys/select.h>
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
//4.初始化检测集合、
 fd_set reads,temp;
 FD_ZERO(&reads);
 FD_SET(lfd,&reads);
 int nfds=lfd;
 //5.委托内核不停检测集合中的文件描述符
    while(1){
        temp=reads;
        int num=select(nfds+1,&temp,NULL,NULL,NULL);//最后一个是什么时候解除阻塞，NULL那就是不解除阻塞，知道检测出有状态变化；
        cout<<num<<endl;
        for(int i=lfd;i<=nfds;i++){
            if(i==lfd&&FD_ISSET(lfd,&temp)){
                //建立新连接，这调用绝对不阻塞
                int cfd=accept(lfd,NULL,NULL);
                //cfd 添加到检测的原始集合
                FD_SET(cfd,&reads);
                nfds=nfds<cfd?cfd:nfds;//取最大的哪一个
            }
            else{
                if(FD_ISSET(i,&temp)){
                    //接受数据
                    char buf[1024];
                    memset(buf,0,sizeof(buf));
                    int len=recv(i,buf,sizeof(buf),0);
                    if(len==0){
                        cout<<"客户端断开连接"<<endl;
                        //将i从原始集合中删除，下次不检查了
                        FD_CLR(i,&reads);
                        close(i);
                    }
                    else if(len>0){
                    cout<<"recv data :"<<buf<<endl;
                       send(i,buf,len,0);
                    }
                    else{
                        perror("recv");
                        break;
                    }
                }
            }
        }

    }

close(lfd);
    return 0;
}
