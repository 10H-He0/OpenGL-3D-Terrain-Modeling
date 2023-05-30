#ifndef CONFIG_H
#define CONFIG_H
//存放常量
class Config
{
public:
    Config(void);
    ~Config(void);
    static const int MAX_POINT_NUM = 100000;//最多顶点数
    static const int MAX_EDGE_NUM = 20000;//最多边数
    static const double EPS;//极小，当做0
    static const double INF;//极大
};
#endif

