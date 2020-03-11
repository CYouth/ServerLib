# ServerLib
基于非阻塞IO和事件驱动的C++网络库
****
1）利用 Reactor 事件处理模式，动态配置 poll/epoll 模式作为 IO 复用技术，分别使用 timerfd 和 eventfd管理定时事件和信号事件，它适合开发Linux下的面向业务的多线程服务端网络应用程序  
2）支持 one loop per thread（IO 模型）+thread pool 编程模型，实现灵活的 IO 与 CPU 配置  
3）动态扩展且线程安全的应用层 buffer，实现数据有效传输；高效的多线程异步日志，采用双缓冲技术，根据文件大小和时间来主动滚动日志文件  
4）使用 timing wheel 剔除空闲连接，有效管理服务端并发连接数  
5）较先进的 C++编程思想，比如 RAII 防止内存泄露，PIMPL 分离接口和实现，function/bind 降低程序耦合度等  
****
one loop per thread + thread pool：  
![image](https://github.com/CYouth/ServerLib/blob/master/image/reactor.png)  
目前主流的服务器开发架构一般有两个线程池：1）IO线程池；2）业务线程池  
1）IO线程池遵守“one loop per thread”的原则，一个IO线程有且仅有一个EventLoop（比如epollfd实现loop），可以处理IO事件、定时事件和信号事件（EventLoop搭载socketfd，timerfd和eventfd）。IO线程中也可以有处理业务的逻辑（eventfd可以唤醒IO线程处理异步业务逻辑），但是这些业务不能太耗时，否则还是建议使用业务线程池，因为业务线程池主要处理的是业务事件  
2）多线程TcpServer：目前每个TcpServer有自己的EventLoopThreadPool，多个TcpServer之间不共享EventLoopThreadPool。在新建TcpConnection时从Event Loop Pool里挑选一个loop给TcpConnection用，也就是说多线程TcpServer自己的EventLoop只用来接收新连接，而新连接会用其他EventLoop来执行IO  
  
上述的架构在CPU核数有限的情况下，线程数目可能会多于CPU核数，此时线程不一定是真的在并行执行  
****
代码版本：  
v1.0：一个简单的echo服务器，分模块解耦处理服务端逻辑  
v2.0：重要的里程碑版本，加入User概念，现在开放网络库api给用户自己操控（关注的三个半事件），这时才是真正意义上的网络库  

