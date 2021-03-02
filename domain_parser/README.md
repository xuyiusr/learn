# 域名备案信息查询

输入一批域名，返回这批域名备案信息中的主办单位名称

比如，对于域名“www.sina.com”，返回“北京新浪互联信息服务有限公司”

可以通过第三方API来查询www.sina.com备案信息，如：
http://icp.chinaz.com/www.sina.com
https://www.beian88.com/sina.com
http://www.beianbeian.com/search/www.sina.com.html

第三方API可能不稳定，可能返回错误，可能返回输入验证码，你的代码需要尝试从多个第三方API获取信息来保证程序的输出。

其它要求：
1. 尽可能考虑过程中可能出现的异常
2. 合理地记录日志
3. 使用多线程来优化性能
4. 需单元测试

