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

struct Rect_info{
    QString img_name;
    QString img_shape;
    QString label_number;
    QString rect_coord;
    QString writer_name;
    QString user_institude;
    QString career;
};

class Annotation_Tool_Main : public QMainWindow
{
    Q_OBJECT

public:
    Annotation_Tool_Main(QWidget *parent = nullptr);
    ~Annotation_Tool_Main();
    //explicit RubberBand(Shape s, QWidget* p = 0);

    void change_list_info(QString label);
    void change_txt(vector<Rect_info> input_vec, QString select_label);
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::Annotation_Tool_MainClass ui;
    QPixmap m_image;
    QPixmap return_pixmap;
    QPixmap current_pixmap;
    QRect m_currentRect;
    QVector<QRect> m_rectangles;
    bool m_drawing;
    bool rect_result;
    QPoint startPoint;
    QStringList file_list;
    QStringList file_name_list;
    QStringList tmp;
    QStringList* ptr = &file_list;
    int file_list_len;
    int img_count = 0;
    int img_w;
    int img_h;
    string user_name;
    string user_career;
    string user_institution;

    string user_path;
    QString fileName;

    vector<Rect_info> rect_info;

    

protected:
    bool eventFilter(QObject* obj, QEvent* event);
    QPoint mapToImageCoordinates(const QPoint& pos);
    bool read_info();

    //void keyPressEvent(QKeyEvent* event);
    



private slots:
    //QStringList load_image();
    void load_image();
    void init_info();
    void show_img(QString path);
    void next_img();
    void prev_img();

    void onItemClicked(QListWidgetItem* item);
    
    void delete_label();

    void adjustBrightness(int value);
    void image_move();
};
