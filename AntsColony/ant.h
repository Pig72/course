#ifndef ANT_H
#define ANT_H
#include <QVector>
#include <QSet>

class Ant
{
public:
    int id; // 蚂蚁的唯一标识
    int curpos; //当前位置
    double alpha;   // 控制蚂蚁的信息素影响因子
    double beta;    // 控制蚂蚁的启发式影响因子
    int length;    // path的总长度
    int rank;   // 上一次迭代该蚂蚁的排名
    QVector<int> path;   //记录蚂蚁走的路径
    QSet<int> visited;

public:
    Ant(int _id, double _alpha, double _beta,int pos);
    void update(const QVector<QVector<int>>&dist, QVector<int> h,const QVector<QVector<double>>&pher);  // 控制蚂蚁走到下一个地点
    int select_next(const QVector<QVector<int>>&dist, QVector<int> h,const QVector<QVector<double>>&pher);  // 选择蚂蚁下一次要走的城市
    void reset();
    int get_length() const{return length;};
    int get_rank() const{return rank;};
    int get_id() const{return id;};
    double get_alpha() const{return alpha;};
    double get_beta() const{return beta;};
    void set_rank(int _rank){rank=_rank;};
    const QVector<int> &get_path() const{return path;};

};

#endif // ANT_H
