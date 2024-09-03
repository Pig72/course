#ifndef COLONY_H
#define COLONY_H
#include "ant.h"
#include <QPoint>

class Colony
{
public:
    int n;              // 城市数目
    double rou;         // 信息素衰散速率
    double increment;   // 蚂蚁放置信息素的量
    int best;           // 该colony中至今为止发现的最优路径长度
    QVector<int> best_path;  // 该colony中至今为止发现的最优路径
    QVector<Ant *> ants; // 存储所有的蚂蚁指针
    QVector<QVector<int>> dist;   // 存储各个地点之间的距离
    QVector<QVector<double>> pher; // 存储各个地点之间的信息素浓度
    QVector<int> H;        // 启发式函数
public:
    Colony(QVector<QVector<int>>dis,QVector<int> Hn,
           int ants_num, double rou, double increment, double _alpha_value, double _beta_value,
           double _alpha_bias, double _beta_bias);  // 构造函数，需要生成所有蚂蚁并加载地图
    ~Colony();
    void update(); // 一次完整迭代，控制所有蚂蚁走完全部路程并更新信息素状态
    void update_pher();  // 更新信息素浓度
    int get_best() const{return best;};
    const QVector<int> get_best_path() const{return best_path;};
    const QVector<Ant *> &get_ants() const{return ants;};
    const QVector<QVector<double>> &get_pher() const{return pher;};

};

#endif // COLONY_H
