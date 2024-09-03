#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1500,800);
    setWindowTitle("蚁群算法解决罗马尼亚度假问题");
    readFile();         //读取文件
    initialMap();       //画图

    best_record = 0x7fffffff;
    colony = nullptr;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(auto_update()));
    info = new QTextBrowser(this);
    info->resize(300,500);info->move(1100,150);

    set_map = new QPushButton(this);        //重置地图
    set_map->resize(100,50);set_map->move(700,720);
    set_map->setText("重置地图");
    connect(set_map, SIGNAL(clicked(bool)), this, SLOT(on_setmapclicked()));

    simulate = new QPushButton(this);
    simulate->resize(100,50);simulate->move(300,720);
    simulate->setText("蚁群算法模拟");
    connect(simulate, SIGNAL(clicked(bool)), this, SLOT(on_simulateclicked()));

    // best_btn = new QPushButton(this);
    // best_btn->resize(100,50);best_btn->move(500,720);
    // best_btn->setText("展示最优路径");
    // connect(best_btn, SIGNAL(clicked(bool)), this, SLOT(on_show_best_clicked()));

    ants_label = new QLabel(this);ants_LineEdit=new QLineEdit(this);
    ants_label->setText("蚂蚁数量");ants_LineEdit->resize(100,30);
    ants_label->move(200,620);ants_LineEdit->move(250,620);
    ants_LineEdit->setText("9");

    iters_label = new QLabel(this);iters_LineEdit=new QLineEdit(this);
    iters_label->setText("最大迭代次数");iters_LineEdit->resize(100,30);
    iters_label->move(370,620);iters_LineEdit->move(450,620);
    iters_LineEdit->setText("20");

    alpha_value_label = new QLabel(this);alpha_value_LineEdit=new QLineEdit(this);
    alpha_value_label->setText("α值");alpha_value_LineEdit->resize(100,30);
    alpha_value_label->move(570,620);alpha_value_LineEdit->move(600,620);
    alpha_value_LineEdit->setText("4");

    beta_value_label = new QLabel(this);beta_value_LineEdit=new QLineEdit(this);
    beta_value_label->setText("β值");beta_value_LineEdit->resize(100,30);
    beta_value_label->move(720,620);beta_value_LineEdit->move(740,620);
    beta_value_LineEdit->setText("6");

    rou_value_label = new QLabel(this);rou_value_LineEdit=new QLineEdit(this);
    rou_value_label->setText("信息素消散速率");rou_value_LineEdit->resize(100,30);
    rou_value_label->move(350,670);rou_value_LineEdit->move(450,670);
    rou_value_LineEdit->setText("0.2");

    increment_label = new QLabel(this);increment_LineEdit=new QLineEdit(this);
    increment_label->setText("信息素总量");increment_LineEdit->resize(100,30);
    increment_label->move(570,670);increment_LineEdit->move(640,670);
    increment_LineEdit->setText("1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFile()
{
    QFile inFile("D:/AntsColony/Data.csv");
    QStringList lines;
    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream text(&inFile);
        while (!text.atEnd())
        {
            lines.push_back(text.readLine());
        }
        QString cities= lines.at(0); //读取第一行
        //以逗号分割当前行
        QStringList tokens = cities.split(",");
        tokens.removeFirst();
        for(int i=0;i<tokens.size();i++){
            Cityname.push_back(tokens[i]);
        }
        for (int j = 1; j < lines.size(); j++)
        {
            if(j<=20){
                QString disstring= lines.at(j);
                QStringList tokens = disstring.split(",");
                tokens.removeFirst();
                QVector<int> dis;
                for(int i=0;i<tokens.size();i++){
                    dis.push_back(tokens[i].toInt());
                }
                Distance.push_back(dis);            //赋值给Distance矩阵
            }
            else if(j>=22){
                QString num= lines.at(j);
                QStringList tokens = num.split(",");
                Hn.push_back(tokens[1].toInt());
            }

        }
        citynum=Cityname.size();
        inFile.close();
    }

}

void MainWindow::initialMap()
{
    pen_B.setColor(Qt::black);pen_B.setWidth(2);
    pen_R.setColor(Qt::red);pen_R.setWidth(2);
    pen_G.setColor(Qt::green);pen_G.setWidth(2);

    color_E.clear();
    color_N.clear();

    QVector<int> temp;
    for (int i = 0; i < Distance.size(); ++i) {
        color_N.push_back(0);
        for (int j = 0; j < Distance.size(); ++j) {
            if(Distance[i][j]!=1000&&Distance[i][j]!=0)
                temp.push_back(1);            //1,2,3代表黑，红，绿
            else
                temp.push_back(0);
        }
        color_E.push_back(temp);
        temp.clear();
    }
    update();       //会调用paint画图函数
}

void MainWindow::update_info()
{
    info->append(QString().asprintf("第[%d]此迭代结果:",iters_LineEdit->text().toInt() - count));
    const QVector<Ant *> &ants = colony->get_ants();
    for (int i = 0; i < ants.size(); ++i) {
        QString description;
        description="排名"+QString::number(i)+"  蚂蚁编号"+QString::number(ants[i]->get_id())+
                    " 寻找到的路径长度"+QString::number(ants[i]->get_length())+" ";
        info->append(description);
    }
}

void MainWindow::update_map()
{
    int n =citynum;
    if (colony) {
        for (int i=0;i<n;i++) {
            for (int j=i+1;j<n;j++)  {
                if (color_E[i][j]) {        //有边才画
                    color_E[i][j]=color_E[j][i]=3;        //设置成绿色
                }
            }
        }
        QVector<int> best=colony->best_path;
        for(int i=0;i<best.size()-1;i++){
            color_E[best[i]][best[i+1]]=color_E[best[i+1]][best[i]]=2;
        }
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    int n=citynum;
    if (n==0) return;
    std::vector<QPoint> v;
    v.push_back(QPoint(91*1.8,242*1.8));v.push_back(QPoint(400*1.8,77*1.8));
    v.push_back(QPoint(253*1.8,38*1.8));v.push_back(QPoint(165*1.8,49*1.8));
    v.push_back(QPoint(562*1.8,43*1.8));v.push_back(QPoint(305*1.8,199*1.8));
    v.push_back(QPoint(375*1.8, 20*1.8));v.push_back(QPoint(534*1.8, 100*1.8));
    v.push_back(QPoint(473*1.8, 256*1.8));v.push_back(QPoint(165*1.8, 129*1.8));
    v.push_back(QPoint(168*1.8, 89*1.8));v.push_back(QPoint(406*1.8, 287*1.8));
    v.push_back(QPoint(131*1.8, 321*1.8));v.push_back(QPoint(320*1.8, 118*1.8));
    v.push_back(QPoint(233*1.8, 160*1.8));v.push_back(QPoint(207*1.8, 207*1.8));
    v.push_back(QPoint(94*1.8, 160*1.8));v.push_back(QPoint(456*1.8, 100*1.8));
    v.push_back(QPoint(509*1.8, 194*1.8));v.push_back(QPoint(108*1.8, 281*1.8));
    for (int i=0;i<n;i++) {
        drawNode(v[i],Cityname[i],(!color_N[i])?pen_B:pen_R);
    }
    for (int i=0;i<n;i++) {
        for (int j=i+1;j<n;j++)  {
            if (color_E[i][j]) {        //有边才画
                drawEdge(v[i],v[j],10,QString::number(Distance[i][j]),pen(i,j));
            }
        }
    }
}

QPen MainWindow::pen(int i,int j)
{
    if (color_E[i][j]==2) {
        QColor pherColor("red");
        return pen_R;
    }
    if (color_E[i][j]==3){
        QColor pherColor("green");
        return pen_G;
    }
    return pen_B;
}

void MainWindow::drawNode(QPoint t,QString s,QPen pen)
{
    QPainter paint(this);
    paint.setPen(pen);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.drawEllipse(t,15,15);
    paint.drawText(t+QPoint(-10,5),s);
}

void MainWindow::drawEdge(QPoint t1, QPoint t2, int r,QString s,QPen pen)
{
    QPoint tmp=t2-t1;

    QPainter paint(this);
    paint.setPen(pen);
    paint.setRenderHint(QPainter::Antialiasing);

    paint.drawText((tmp*(1.0-(citynum+1)/2/double(citynum+1)))+t1,s);

    double angle=atan(fabs((double)tmp.y()/(double)tmp.x()));
    int x=double(r+r/2)*cos(angle);
    int y=double(r+r/2)*sin(angle);

    //生成对应边，其中注释内容为箭头标识，可表示有向边
    if  (tmp.x()>=0 && tmp.y()>0) {
        t1.rx()+=x;t1.ry()+=y;
        t2.rx()-=x;t2.ry()-=y;
        paint.drawLine(t1,t2);
        //         paint.drawLine(t2+QPoint(-r*cos(angle-acos(-1)/6),-r*sin(angle-acos(-1)/6)),t2);
        //         paint.drawLine(t2+QPoint(-r*cos(angle+acos(-1)/6),-r*sin(angle+acos(-1)/6)),t2);
    }
    if  (tmp.x()<0 && tmp.y()>=0) {
        t1.rx()-=x;t1.ry()+=y;
        t2.rx()+=x;t2.ry()-=y;
        paint.drawLine(t1,t2);
        //         paint.drawLine(t2+QPoint(+r*cos(angle-acos(-1)/6),-r*sin(angle-acos(-1)/6)),t2);
        //         paint.drawLine(t2+QPoint(+r*cos(angle+acos(-1)/6),-r*sin(angle+acos(-1)/6)),t2);
    }
    if  (tmp.x()<=0 && tmp.y()<0) {
        t1.rx()-=x;t1.ry()-=y;
        t2.rx()+=x;t2.ry()+=y;
        paint.drawLine(t1,t2);
        //        paint.drawLine(t2+QPoint(+r*cos(angle-acos(-1)/6),+r*sin(angle-acos(-1)/6)),t2);
        //        paint.drawLine(t2+QPoint(+r*cos(angle+acos(-1)/6),+r*sin(angle+acos(-1)/6)),t2);
    }
    if  (tmp.x()>0 && tmp.y()<=0) {
        t1.rx()+=x;t1.ry()-=y;
        t2.rx()-=x;t2.ry()+=y;
        paint.drawLine(t1,t2);
        //        paint.drawLine(t2+QPoint(-r*cos(angle-acos(-1)/6),+r*sin(angle-acos(-1)/6)),t2);
        //        paint.drawLine(t2+QPoint(-r*cos(angle+acos(-1)/6),+r*sin(angle+acos(-1)/6)),t2);
    }
    qDebug() <<  tmp/2 << s;
}

void MainWindow::on_showbestclicked()
{
    std::vector<QPoint> v;
    v.push_back(QPoint(91*1.8,242*1.8));v.push_back(QPoint(400*1.8,77*1.8));
    v.push_back(QPoint(253*1.8,38*1.8));v.push_back(QPoint(165*1.8,49*1.8));
    v.push_back(QPoint(562*1.8,43*1.8));v.push_back(QPoint(305*1.8,199*1.8));
    v.push_back(QPoint(375*1.8, 20*1.8));v.push_back(QPoint(534*1.8, 100*1.8));
    v.push_back(QPoint(473*1.8, 256*1.8));v.push_back(QPoint(165*1.8, 129*1.8));
    v.push_back(QPoint(168*1.8, 89*1.8));v.push_back(QPoint(406*1.8, 287*1.8));
    v.push_back(QPoint(131*1.8, 321*1.8));v.push_back(QPoint(320*1.8, 118*1.8));
    v.push_back(QPoint(233*1.8, 160*1.8));v.push_back(QPoint(207*1.8, 207*1.8));
    v.push_back(QPoint(94*1.8, 160*1.8));v.push_back(QPoint(456*1.8, 100*1.8));
    v.push_back(QPoint(509*1.8, 194*1.8));v.push_back(QPoint(108*1.8, 281*1.8));
    for(int i=0;i<best_record_path.size()-1;i++){
        int x=best_record_path[i];
        int y=best_record_path[i+1];
        QPen hua;hua.setColor(Qt::blue);hua.setWidth(2);
        drawEdge(v[x],v[y],10,QString::number(Distance[x][y]),hua);
    }

}

void MainWindow::simulate_end()
{
    timer->stop();
    int best = colony->get_best();
    if (best< best_record) {
        best_record = colony->get_best();
        best_record_path = colony->get_best_path();
    }
    QString description="模拟结束，结果如下：\n";
    QString path;
    for(int i=0;i<best_record_path.size()-1;i++){
        path+=Cityname[best_record_path[i]]+"--->";
    }
    path+=Cityname[best_record_path.last()];
    description="寻找到的最短路径为"+path+" 寻找到的路径长度"+QString::number(best_record)+" ";
    info->append(description);
}

void MainWindow::auto_update()
{
    if (count <= 0) {
        simulate_end();
        return;
    }
    count -= 1;
    colony->update();
    update_info();
    update_map();
}

void MainWindow::on_simulateclicked()
{
    if (colony) {
        delete colony;
    }
    info->append("开始蚁群算法模拟");
    int antsnum=ants_LineEdit->text().toInt();
    int iternum=iters_LineEdit->text().toInt();
    double rou=rou_value_LineEdit->text().toDouble();
    double incre=increment_LineEdit->text().toDouble();
    double a=alpha_value_LineEdit->text().toDouble();
    double b=beta_value_LineEdit->text().toDouble();
    colony = new Colony(Distance,Hn,antsnum, rou, incre, a, b,0,0);
    count = iternum;
    timer->start(1000.0 / 1);
}

void MainWindow::on_setmapclicked()
{
    initialMap();
    best_record_path.clear();
    best_record=0x7fffffff;
}
