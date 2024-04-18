#pragma once

#include <QtWidgets/QMainWindow>
#include <QRubberband>
#include <QPaintEvent>
#include "ui_Annotation_Tool_Main.h"
#include <qfiledialog.h>
#include <iostream>
#include <QPainter>
#include <QPen>
#include <vector>
#include <QMouseEvent>
#include <filesystem>

using namespace std;

struct {
    string label_num;

};

class Annotation_Tool_Main : public QMainWindow
{
    Q_OBJECT

public:
    Annotation_Tool_Main(QWidget *parent = nullptr);
    ~Annotation_Tool_Main();
    //explicit RubberBand(Shape s, QWidget* p = 0);

private:
    Ui::Annotation_Tool_MainClass ui;
    QPixmap m_image;
    QRect m_currentRect;
    QVector<QRect> m_rectangles;
    bool m_drawing;
    QPoint startPoint;
    QStringList file_list;
    QStringList* ptr = &file_list;
    int file_list_len;
    int img_count = 0;

    string user_name;
    string user_career;
    string user_institution;

    string user_path;
    QString fileName;

protected:
    //void mousePressEvent(QMouseEvent* event) override;
    //void mouseMoveEvent(QMouseEvent* event) override;
    //void mouseReleaseEvent(QMouseEvent* event) override;
    //void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event);
    QPoint mapToImageCoordinates(const QPoint& pos);
    QPixmap draw_rectangles(QPixmap pixmap, QStringList label_list);


private slots:
    //QStringList load_image();
    void load_image();
    void init_info();
    void show_img(QString path);
    void next_img();
    void prev_img();

};
