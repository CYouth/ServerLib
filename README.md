# ServerLib
基于C++11的非阻塞异步网络库，参考原型为陈硕的muduo网络库  
****
代码版本：  
v1.0：一个简单的echo服务器，分模块解耦处理逻辑  
v2.0：重要的里程碑版本，单线程reactor网络库  
加入User概念，之前事件发生之后的处理逻辑是写在网络库中的，现在开放api给用户自己操控（关注的三个半事件），这时操作就由用户自己实现的，这时才是真正意义上的网络库  
****  
todo：  
1）input output buffer需要重新设计，现在只是一个简陋的std::string版本  
2）完善异步任务接口的设计  
3）完善用户接口的设计  
4）完善时间事件runat/runafter/runevery接口  
5）代码规范问题，比如类型转换之类  
6）利用C++新特性重写代码(参考boost::binder)  
****  
coming soon...  
v3.0：多线程版本，任务线程池，one loop per thread设计理念  
v4.0：加入日志库
