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
#include <sys/select.h>
#include <sys/epoll.h>
using namespace std;
int main()
{

    // 1.创建监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(0);
    }
    // 2.绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;                                   // IPV4
    addr.sin_port = htons(8989);                                 // 端口，转换大端
    addr.sin_addr.s_addr = INADDR_ANY;                           // ip地址为0
    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr)); // 绑定
    if (ret == -1)
    { // 如果绑定失败
        perror("bind");
        exit(0);
    }
    // 3. 设置监听
    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(0);
    }

    // 4.创建epoll模型
    int epfd = epoll_create(100);
    if (epfd == -1)
    {
        perror("epoll_create");
        exit(0);
    }
    // 5.将要检测的节点添加到epoll模型中
    struct epoll_event ev;
    ev.data.fd = lfd;    // 将lfd添加到结构体数组中初始化
    ev.events = EPOLLIN; // 检测lfd的读缓冲区
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
    if (ret == -1)
    {
        perror("epoll_ctl");
        exit(0);
    }
    // 6.不停的委托内核检测epoll模型中的文件描述符的状态
    struct epoll_event evs[1024]; // evs 是传出方
    int size = sizeof(evs) / sizeof(evs[0]);
    while (1)
    {
        int num = epoll_wait(epfd, evs, size, -1); //-1 就是一直阻塞检测
        cout << num << endl;
        // 遍历evs
        for (int i = 0; i < num; i++)
        {
            // 取出数组元素的文件描述符
            int curfd = evs[i].data.fd;
            if (lfd == curfd)
            {
                int cfd = accept(curfd, NULL, NULL);
                // 添加结构体数组到检测的原始集合
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else
            {
                // 通信
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                int len = recv(curfd, buf, sizeof(buf), 0);
                if (len == 0)
                {
                    cout << "客户端断开链接" << endl;
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL); // 从epoll树删除此节点
                    close(curfd);
                }
                else if (len > 0)
                {
                    cout << "recv data :" << buf << endl;
                    send(curfd, buf, len, 0);
                }
                else
                {
                    perror("recv ");
                    exit(0);
                }
            }
        }
    }
    return 0;
}
