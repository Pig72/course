#include "colony.h"

Colony::Colony(QVector<QVector<int>>dis, QVector<int> Hn,int ants_num, double _rou, double _increment,
                double _alpha_value, double _beta_value, double _alpha_bias, double _beta_bias)
{
    n = Hn.size();
    rou = _rou;
    increment = _increment;
    best = 0x7fffffff;
    H=Hn;
    dist.resize(n);
    pher.resize(n);
    for (int i = 0; i < n; ++i) {
        dist[i].resize(n);
        pher[i].resize(n);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            dist[i][j] = dist[j][i] = dis[i][j];
            pher[i][j] = pher[j][i] = 1.0;
        }
    }
    for (int i = 0; i < ants_num; ++i) {
        double alpha = _alpha_value + (rand() % RAND_MAX * _alpha_bias / RAND_MAX);
        double beta = _beta_value + (rand() % RAND_MAX * _beta_bias / RAND_MAX);
        ants.append(new Ant(i + 1, (alpha >= 0 ? alpha : 0), (beta >= 0 ? beta : 0),0));
    }
}

Colony::~Colony()
{
    for (Ant * ant : ants) {
        delete ant;
    }
}

void Colony::update()
{
    int n = dist.size();
    for (Ant *ant : ants) {
        ant->reset();
        while(ant->curpos!=1) {
            if(ant->curpos==-1){
                ant->reset();
            }
            ant->update(dist,H,pher);
        }
    }
    // 将ants全部sort，方便其他部件访问
    std::sort(ants.begin(), ants.end(), [](const Ant *a1, const Ant *a2){ return a1->get_length() < a2->get_length(); });
    update_pher();      //更新路径上信息素浓度
    for (int i = 0; i < ants.size(); ++i) {
        ants[i]->set_rank(i+1);
    }
}

void Colony::update_pher()
{
    // 所有有向边的pher都先衰减
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pher[i][j] *= (1 - rou);
        }
    }
    // 所有蚂蚁根据自己的表现留下pher
    for (int i = 0; i < ants.size(); ++i) {
        //增量=总信息素/长度
        double delta = increment / ants[i]->get_length();  // 策略：优胜者多留，进步者多留
        if (ants[i]->get_length() < best) { //更新记录
            best = ants[i]->get_length();
            best_path = ants[i]->get_path();
        }
        int pathnum=ants[i]->get_path().size();
        for (int j = 0; j < pathnum-1; ++j) {
            pher[ants[i]->get_path()[j]][ants[i]->get_path()[j+1]] += delta;        //依次更新选中路径的信息素浓度
        }
    }
}

