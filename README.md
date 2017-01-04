# mini-search-engine
##爬虫
###利用多线程Thread模块爬取 [南京大学](http://www.nju.edu.cn)及其指向的网页共计9000个。

##分词
###使用了[jieba分词](https://github.com/fxsjy/jieba)

##倒排索引
倒排索引是从词语（term, words）到出现这个词语的网页的映射（map）。在倒排索引中，每个词语是一个关键码(key), 其对应的值（value）是这个词语的postings list。一个词语的postings list是包含出现这个词语的所有网页的列表。对于每一次问询，我们用倒排索引这一数据结构来返回所有相关的网页。
每个网页要用一个唯一的整数标识，我们称这个整数为文档编号(document ID)。
使用红黑树在内存中存储倒排索引。
##PageRank/DegreeRank
###利用稀疏矩阵迭代计算Pagerank

##服务器
- 使用JAVA搭建具有良好用户体验的搜索网页（html）
- 使用**JNI接口**调用C++函数