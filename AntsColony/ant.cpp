#include "ant.h"

Ant::Ant(int _id, double _alpha, double _beta,int pos)
{
    id = _id;
    alpha = _alpha;
    beta = _beta;
    rank = -1;
    curpos=pos;
    reset();
}

void Ant::reset()
{
    path.clear();
    path.append(0);
    visited.clear();
    visited.insert(0);
    length = 0;
    curpos=0;
}


void Ant::update(const QVector<QVector<int> > &dist, QVector<int> h ,const QVector<QVector<double>>&pher)
{
    int next = select_next(dist,h,pher);
    curpos=next;
    if(curpos==-1){
        return;
    }
    length += dist[path.last()][next];
    visited.insert(next);
    path.append(next);
}

int Ant::select_next(const QVector<QVector<int> > &dist, QVector<int> h,const QVector<QVector<double>>&pher)
{
    int n = dist.size();
    QVector<int> unvis;//未访问节点
    for (int j = 0; j < n; ++j) {//遍历节点
        if(dist[curpos][j]!=0&&dist[curpos][j]!=1000&&visited.find(j)==visited.end()){ //没访问过j，并且有边
            unvis.push_back(j);
        }
    }
    QVector<double> probs;//存放概率
    double sum = 0.0;
    for (int c : unvis) {
        double tao = pow(pher[path.last()][c], alpha);//信息素的影响
        double yita = pow(1.0 / (dist[path.last()][c] + 1+h[c]), beta);//距离的影响，实际距离加上启发式距离
        sum += tao * yita;              //总和
        probs.push_back(tao * yita);        //计算每条选择的概率
    }
    double r = (rand() % RAND_MAX) * 1.0 / RAND_MAX * sum;      //按照概率选择要走的节点
    if(unvis.size()>0){
        for (int i = 0; i < unvis.size(); ++i) {
            r -= probs[i];//遍历未访问节点，减去对应的概率值，当减到小于等于0时，选择该节点
            if (r <= 0) {
                return unvis[i];
            }
        }
        return unvis.last();
    }else
    {
        return -1;
    }

}


