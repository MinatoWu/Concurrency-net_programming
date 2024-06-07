//伪代码

//创建tcpclient对象
class Tcpsocked{
public:
    Tcpsocked(){
        m_connfd=socked(af_inet,sock_stream,0);
    }
    Tcpsocked(int fd){
    this->m_connfd=fd;
    }
    ~Tcpsocked();
    //客户端链接服务器
    int connectHost (string ip,unsigned short port,int connecttime);
    //客户端关闭连接
    int disconnect();
    //客户端发送数据
    int sendMsg(string sendmsg,int sendtime=1000);
    //客户端接收数据
    string recvMsg(int timeout);

private :
int m_connfd;
};

//这个类不能用 ，只能和一个客户端建立连接，而且两个类都重用了通信流程

// class Tcpserver{
// public:
//     Tcpserver();//初始化
//     ~Tcpserver();//释放
//     int sckserver_accpet(int timeout);

//     //服务器发送数据
//     int sendMsg(string sendmsg,int sendtime=1000);
//     //服务器接收数据
//     string recvMsg(int timeout);
//     //关闭
//     int closefd();
     
// private:
// int m_lfd;//监听
// int m_connfd;//通信
// };

//3.
//思想服务器端不负责通信，如果通信使用客户端的类
class Tcpserver{
public:
    Tcpserver();//初始化
    ~Tcpserver();//释放
    Tcpsocked* accpetconn(int timeout){
        int fd=accept(m_lfd,&address,&len);
        Tcpsocked*Tcp=new Tcpsocked(fd);
        if(Tcp!=NULL){
            return Tcp;
        }
        return NULL;
    }
private:
int m_lfd;

}