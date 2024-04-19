#include "Annotation_Tool_Main.h"
#include"Annotation_FIle.h"
//#include "CustomLabel.h"
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QPaintEvent>
#include <iostream>
#include<QMessageBox>
#include<direct.h>


Annotation_Tool_Main::Annotation_Tool_Main(QWidget* parent)
    : QMainWindow(parent)
{
    
    ui.setupUi(this);
    //setMouseTracking(true);
    Annotation_Tool_Main::init_info();

    //CustomLabel* lbl_image = new CustomLabel(this);

    connect(ui.btn_loadImg, SIGNAL(clicked()), this, SLOT(load_image()));
    connect(ui.btn_next, SIGNAL(clicked()), this, SLOT(next_img()));
    connect(ui.btn_prev, SIGNAL(clicked()), this, SLOT(prev_img()));

    ui.lbl_image->installEventFilter(this);
    //connect(ui.menuImage_Load, SIGNAL(clicked()), this, SLOT(load_image()));
    
    
}

Annotation_Tool_Main::~Annotation_Tool_Main()
{}

void Annotation_Tool_Main::init_info() {

    user_institution = "caimi";
    user_career = "3";
    user_name = "test2";

    //user_path = './' + "/result/" + user_name + '/';
    user_path = ".\\result\\" + user_name;
    make_directory(user_path, "root", "");

    ui.lbl_institution->setText(QString::fromStdString(user_institution));
    ui.lbl_career->setText(QString::fromStdString(user_career));
    ui.lbl_name->setText(QString::fromStdString(user_name));
}

void Annotation_Tool_Main::load_image() {
    QFileDialog dlg;
    file_list = dlg.getOpenFileNames(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");
    
    //int size = file_list.length();

    file_list_len = file_list.size();
    //file_list_len = QString::number(list_len);
    //ui.list_info->addItem("length: " + len);
    Annotation_Tool_Main::show_img(file_list[0]);
    
    //QString fileName = filepath.section("/", -1);
    //ui.lbl_image->setText(fileName);

    
    //img.load(filepath);

    //return file_list;
}

void Annotation_Tool_Main::show_img(QString path) {
    QString file_path = path;
    QString fileName_pre = file_path.section("/", -1);
    fileName = fileName_pre.split(".")[0];
    make_directory(user_path, "image", fileName.toStdString());
    m_image.load(file_path);
    //img.scaled(30, 30);
    //img.scaledToWidth(10);
    int w = m_image.width();
    int h = m_image.height();

    if (!m_image.isNull()) {
        ui.lbl_image->setPixmap(m_image);
        ui.lbl_image->setScaledContents(true);
        
    }
    else {
        qDebug() << "Failed to load image";
    }

    QStringList tmp = read_label_txt(user_path + "\\", fileName.toStdString());
    if (tmp.size() != 0) {
        QPixmap return_pixmap = make_pixmap(tmp, m_image);
        ui.lbl_image->setPixmap(return_pixmap);
        ui.lbl_image->setScaledContents(true);
    }
    else {
        ui.lbl_image->setPixmap(m_image);
        ui.lbl_image->setScaledContents(true);
    }

}

void Annotation_Tool_Main::next_img() {
    img_count += 1;
    if (img_count < file_list_len) {
        QString* first_path(&file_list[0]);
        Annotation_Tool_Main::show_img(*(first_path + img_count));
    }
    else {
        img_count = file_list_len;
        QMessageBox::warning(nullptr, "Warning", "This is the last image.");
    }
    
}

void Annotation_Tool_Main::prev_img() {
    img_count -= 1;
    if (img_count >= 0) {
        QString* first_path(&file_list[0]);
        Annotation_Tool_Main::show_img(*(first_path + img_count));
    }
    else {
        img_count = 0;
        QMessageBox::warning(this, "Warning", "This is the first image.");
    }
}
//마우스로 사각형 그리기 원본 코드
//bool Annotation_Tool_Main::eventFilter(QObject* obj, QEvent* event)
//{
//    string label_path = user_path + "\\";
//    if (obj == ui.lbl_image) {
//        if (event->type() == QEvent::MouseButtonPress) {
//            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
//            if (mouseEvent->button() == Qt::LeftButton) {
//                startPoint = mapToImageCoordinates(mouseEvent->pos());
//                m_drawing = true;
//            }
//        }
//        else if (event->type() == QEvent::MouseMove && m_drawing) {
//            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
//            QPoint endPos = mapToImageCoordinates(mouseEvent->pos());
//            int width = endPos.x() - startPoint.x();
//            int height = endPos.y() - startPoint.y();
//            m_currentRect = QRect(startPoint, QSize(width, height));
//
//            QPixmap pixmap = m_image;
//            QPainter painter(&pixmap);
//            painter.setPen(Qt::green);
//            painter.drawRect(m_currentRect);
//            ui.lbl_image->setPixmap(pixmap);
//            
//        }
//        else if (event->type() == QEvent::MouseButtonRelease && m_drawing) {
//            m_drawing = false;
//            m_rectangles.append(m_currentRect);
//            make_label_txt(label_path, fileName.toStdString(), startPoint, m_currentRect.width(), m_currentRect.height());
//        }
//        
//    }
//    QStringList tmp = read_label_txt(label_path, fileName.toStdString());
//    QList<int> return_label = each_label(tmp);
//    return QMainWindow::eventFilter(obj, event);
//}

bool Annotation_Tool_Main::eventFilter(QObject* obj, QEvent* event)
{
    string label_path = user_path + "\\";
    if (obj == ui.lbl_image) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                startPoint = mapToImageCoordinates(mouseEvent->pos());
                m_drawing = true;
            }
        }
        else if (event->type() == QEvent::MouseMove && m_drawing) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint endPos = mapToImageCoordinates(mouseEvent->pos());
            int width = endPos.x() - startPoint.x();
            int height = endPos.y() - startPoint.y();
            m_currentRect = QRect(startPoint, QSize(width, height));

            QPixmap pixmap = m_image;
            QPainter painter(&pixmap);
            painter.setPen(Qt::green);
            painter.drawRect(m_currentRect);
            ui.lbl_image->setPixmap(pixmap);
            
            //QPixmap output_pixmap = draw_rectangles(m_image, tmp);

        }
        else if (event->type() == QEvent::MouseButtonRelease && m_drawing) {
            m_drawing = false;
            m_rectangles.append(m_currentRect);
            make_label_txt(label_path, fileName.toStdString(), startPoint, m_currentRect.width(), m_currentRect.height());
            QStringList tmp = read_label_txt(label_path, fileName.toStdString());
            QPixmap return_pixmap = make_pixmap(tmp, m_image);
            ui.lbl_image->setPixmap(return_pixmap);
        }
        //QStringList tmp = read_label_txt(label_path, fileName.toStdString());
        //QList<int> return_label = each_label(tmp);
        //QPixmap output_pixmap = draw_rectangles(m_image, tmp);
    }
    
    
    return QMainWindow::eventFilter(obj, event);
}

QPoint Annotation_Tool_Main::mapToImageCoordinates(const QPoint& pos)
{
    if (!m_image.isNull()) {
        QSize imageSize = m_image.size();
        QSize labelSize = ui.lbl_image->size();
        int x = pos.x() * imageSize.width() / labelSize.width();
        int y = pos.y() * imageSize.height() / labelSize.height();
        return QPoint(x, y);
    }
    else {
        return QPoint();
    }
}
