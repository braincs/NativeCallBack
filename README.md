## Android JNI 最佳实践

v1.x 

1. Demo native call java interface  C调用java 接口
2. Demo native  thread create/destory c++ 创建销毁线程
3. Demo native mutex lock under multiple thread 多线程下c++ 互斥锁的使用
4. Demo native queue, ring queue etc. 

v1.x bug

1. fatal signal 11 when release(onDestroy) native layer. 退出前释放资源会出现空指针异常。



