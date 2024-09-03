#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QPen>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QLineEdit>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include "colony.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readFile();
    void initialMap();
    void update_info();  // 更新info面板
    void update_map();
    QVector<QString> Cityname;          //读取城市名称和对应编号
    QVector<QVector<int>> Distance;     //存储各城市之间距离
    QVector<int> Hn;        // 启发式函数


    //画图函数
    QPen pen(int i,int j);QPen pen_B,pen_R,pen_G;
    void paintEvent(QPaintEvent *event);
    void drawNode(QPoint t,QString s,QPen pen);
    void drawEdge(QPoint t1,QPoint t2,int r,QString s,QPen pen);
    void setText(QString s);
    QVector<QVector<int>> color_E;
    QVector<int> color_N;


    int citynum;
    int count;          //目前迭代次数
    Colony *colony;     //蚁群
    QTimer *timer;
    int best_record;
    QVector<int> best_record_path;
    QTextBrowser *info;
    QPushButton *set_map;
    QPushButton *simulate;
    QPushButton *best_btn;

    QLabel *ants_label;
    QLabel *iters_label;
    QLabel *alpha_value_label;
    QLabel *beta_value_label;
    QLabel *rou_value_label;
    QLabel *increment_label;
    QLineEdit *ants_LineEdit;
    QLineEdit *iters_LineEdit;
    QLineEdit *alpha_value_LineEdit;
    QLineEdit *beta_value_LineEdit;
    QLineEdit *rou_value_LineEdit;      //信息衰散数据
    QLineEdit *increment_LineEdit;
    // some new
    QLabel *record_label; // 记录当前突破的目标
    QLineEdit *record_LineEdit;
    QLabel *push_label;     // 记录push的力度
    QLineEdit *push_LineEdit;

private slots:
    void on_showbestclicked();    // 展示最佳路线
    void simulate_end();   //停止计时器，设置record
    void auto_update();    //更新数据并刷新输出
    void on_simulateclicked();  //当开始按钮被按下，启动模拟
    void on_setmapclicked();    //当设置地图按钮被按下，重设地图

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
