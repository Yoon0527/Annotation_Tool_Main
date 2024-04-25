﻿#pragma once

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
    connect(ui.btn_delLabel, SIGNAL(clicked()), this, SLOT(delete_label()));
    //connect(ui.btn_quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui.list_lbl, &QListWidget::itemClicked, this, &Annotation_Tool_Main::onItemClicked);

    connect(ui.sl_imgBrightness, &QSlider::valueChanged, this, &Annotation_Tool_Main::adjustBrightness);

    ui.lbl_image->installEventFilter(this);
}

Annotation_Tool_Main::~Annotation_Tool_Main()
{}

void Annotation_Tool_Main::adjustBrightness(int value)
{
    QPixmap originalPixmap;

    if (tmp.size() != 0) {
        originalPixmap = return_pixmap.copy();
    }
    else {
        originalPixmap = m_image.copy();
    }

    QPixmap adjustedPixmap = originalPixmap;
    QImage adjustedImage = originalPixmap.toImage();
    qreal brightnessFactor = (100 + value) / 100.0;
    QColor color;
    for (int y = 0; y < adjustedImage.height(); ++y) {
        for (int x = 0; x < adjustedImage.width(); ++x) {
            color = adjustedImage.pixelColor(x, y);
            color.setRed(qMin(color.red() * brightnessFactor, qreal(255)));
            color.setGreen(qMin(color.green() * brightnessFactor, qreal(255)));
            color.setBlue(qMin(color.blue() * brightnessFactor, qreal(255)));
            adjustedImage.setPixelColor(x, y, color);
        }
    }
    adjustedPixmap = QPixmap::fromImage(adjustedImage);
    //current_pixmap = adjustedPixmap;
    string read_path = user_path + "\\Annotation_info.txt";
    QStringList tmp = read_label_txt(user_path + "\\", fileName.toStdString());
    bool annotationFile_bool = (std::filesystem::exists(read_path)) ? true : false;

    if (tmp.size() != 0) {
        QPixmap new_pixmap = make_pixmap(tmp, adjustedPixmap);
        ui.lbl_image->setPixmap(new_pixmap);
        ui.lbl_image->setScaledContents(true);
        current_pixmap = new_pixmap;
        //if (annotationFile_bool) {
        //    rect_result = read_info();
        //    if (!rect_result) {
        //        rect_info.clear();
        //    }
        //}
    }
    else {
        ui.lbl_image->setPixmap(adjustedPixmap);
        ui.lbl_image->setScaledContents(true);
        current_pixmap = adjustedPixmap;
        
    }

    // Update the label with adjusted pixmap
    //ui.lbl_image->setPixmap(adjustedPixmap);
}

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
    ui.list_lbl->clear();
    ui.line_inputfile->clear();
    ui.line_inputshape->clear();
    ui.line_inputcoord->clear();
    ui.line_inputname->clear();
    ui.line_inputinstitude->clear();
    ui.line_inputcareer->clear();
    string read_path = user_path + "\\Annotation_info.txt";
    QString file_path = path;
    QString fileName_pre = file_path.section("/", -1);
    fileName = fileName_pre.split(".")[0];
    make_directory(user_path, "image", fileName.toStdString());
    m_image.load(file_path);
    save_pixmap(user_path + "\\", fileName.toStdString(), m_image);
    //img.scaled(30, 30);
    //img.scaledToWidth(10);
    img_w = m_image.width();
    img_h = m_image.height();
    current_pixmap = m_image.copy();
    if (!m_image.isNull()) {
        ui.lbl_image->setPixmap(m_image);
        ui.lbl_image->setScaledContents(true);
        
    }
    else {
        qDebug() << "Failed to load image";
    }

    tmp = read_label_txt(user_path + "\\", fileName.toStdString());
    bool annotationFile_bool = (std::filesystem::exists(read_path)) ? true : false;

    if (tmp.size() != 0) {
        return_pixmap = make_pixmap(tmp, m_image);
        current_pixmap = return_pixmap.copy();
        ui.lbl_image->setPixmap(return_pixmap);
        ui.lbl_image->setScaledContents(true);
        if(annotationFile_bool){
            rect_result = read_info();
            if (!rect_result) {
                rect_info.clear();
            }
        }
    }
    else {
        ui.lbl_image->setPixmap(m_image);
        ui.lbl_image->setScaledContents(true);
        current_pixmap = m_image.copy();
    }

}

void Annotation_Tool_Main::next_img() {
    img_count += 1;
    if (img_count < file_list_len) {
        QString* first_path(&file_list[0]);
        Annotation_Tool_Main::show_img(*(first_path + img_count));
        ui.sl_imgBrightness->setValue(0);
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
        ui.sl_imgBrightness->setValue(0);
    }
    else {
        img_count = 0;
        QMessageBox::warning(this, "Warning", "This is the first image.");
    }
}


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

            QPixmap pixmap = current_pixmap;
            QPainter painter(&pixmap);
            QPen pen(Qt::green);
            pen.setWidth(1);

            painter.setPen(pen);
            painter.drawRect(m_currentRect);
            ui.lbl_image->setPixmap(pixmap);
            
            //QPixmap output_pixmap = draw_rectangles(m_image, tmp);

        }
        else if (event->type() == QEvent::MouseButtonRelease && m_drawing) {
            m_drawing = false;
            m_rectangles.append(m_currentRect);
            make_label_txt(label_path, fileName.toStdString(), startPoint, m_currentRect.width(), m_currentRect.height());
            QStringList tmp = read_label_txt(label_path, fileName.toStdString());
            QPixmap return_pixmap = make_pixmap(tmp, current_pixmap);
            ui.lbl_image->setPixmap(return_pixmap);
            make_info_txt(user_path, fileName, img_w, img_h, tmp, user_name, user_institution, user_career);
            ui.list_lbl->clear();
            bool result = read_info();
        }
        //QStringList tmp = read_label_txt(label_path, fileName.toStdString());
        //QList<int> return_label = each_label(tmp);
        //QPixmap output_pixmap = draw_rectangles(m_image, tmp);

    }
    
    //bool annotationFile_bool = (std::filesystem::exists(user_path + "\\Annotation_info.txt")) ? true : false;
    //if (annotationFile_bool) {
    //    rect_info = read_info();
    //}

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

bool Annotation_Tool_Main::read_info() {
    //vector<Rect_info> return_vec;
    string read_path = user_path +"\\Annotation_info.txt";
    QString target_sentence;
    string line;
    bool found = false;
    rect_info.clear();
    string img_name = fileName.toStdString();

    std::ifstream annotation_info(read_path);

    while (std::getline(annotation_info, line)) {
        if (line.find(img_name) != std::string::npos) {
            target_sentence = QString::fromStdString(line);
            found = true;
        }
    }
    if (found) {
        annotation_info.close();

        QStringList split_list = target_sentence.split(",");
        QStringList label_list = split_list[2].split(":");
        QStringList coord_list = split_list[3].split(":");
        int label_count = label_list.size();

        if (label_count >= 1) {
            for (int i = 0; i < label_count; i++) {
                Rect_info each_rect;

                each_rect.img_name = split_list[0];
                each_rect.img_shape = split_list[1];
                each_rect.label_number = label_list[i];
                each_rect.rect_coord = coord_list[i];
                each_rect.writer_name = split_list[4];
                each_rect.user_institude = split_list[5];
                each_rect.career = split_list[6];


                rect_info.push_back(each_rect);
            }
            ui.list_lbl->addItems(label_list);
        }




        return found;
    }
    else {
        return found;
    }
    
}

void Annotation_Tool_Main::change_list_info(QString label) {
    QString label_num = label.split("_")[1];
    int index = label_num.toInt();
    ui.line_inputfile->clear();
    ui.line_inputshape->clear();
    ui.line_inputcoord->clear();
    ui.line_inputname->clear();
    ui.line_inputinstitude->clear();
    ui.line_inputcareer->clear();


    ui.line_inputfile->setText(rect_info[index].img_name);
    ui.line_inputshape->setText(rect_info[index].img_shape);
    ui.line_inputcoord->setText(rect_info[index].rect_coord);
    ui.line_inputname->setText(rect_info[index].writer_name);
    ui.line_inputinstitude->setText(rect_info[index].user_institude);
    ui.line_inputcareer->setText(rect_info[index].career);


}

void Annotation_Tool_Main::onItemClicked(QListWidgetItem* item) {
    QString select_label = item->text();
    change_list_info(select_label);
}

void Annotation_Tool_Main::delete_label() {
    //list_lbl의 currentItem을 QString으로 받고,
    //해당 라벨을 annotation_info.txt에서 삭제
    //annotation_txt를 다시 읽기.

    QListWidgetItem* select_item = ui.list_lbl->currentItem();
    QString select = select_item->text();
    //ui.list_lbl->setCurrentItem(nullptr);
    int row = ui.list_lbl->currentRow();
    delete ui.list_lbl->takeItem(row);
    //select_item->setSelected(false);
    change_txt(rect_info, select);
}

void Annotation_Tool_Main::change_txt(vector<Rect_info> input_vec, QString select_label) {
    string read_path = user_path + "\\Annotation_info.txt";
    string label_txt = user_path + "\\" + fileName.toStdString() +"\\" + fileName.toStdString() +".txt";
    QString save_sentence;
    QString label_;
    QString coord_;
    
    int find_num = select_label.split("_")[1].toInt();

    input_vec.erase(input_vec.begin() + find_num);
    int vec_size = input_vec.size();
    if (vec_size > 0) {
        save_sentence.append(fileName + "," + QString::number(img_w) + " " + QString::number(img_h) + ",");

        std::ofstream label_coord(label_txt);

        for (int i = 0; i < vec_size; i++) {
            if (i == vec_size - 1) {
                label_.append(QString("label_%1,").arg(i));
                coord_.append(input_vec[i].rect_coord + ",");
            }
            else {
                label_.append(QString("label_%1:").arg(i));
                coord_.append(input_vec[i].rect_coord + ":");
            }
            label_coord << input_vec[i].rect_coord.toStdString() << "\n";
        }

        label_coord.close();

        save_sentence.append(label_);
        save_sentence.append(coord_);

        save_sentence.append(user_name + ",");
        save_sentence.append(user_institution + ",");
        save_sentence.append(user_career + ",");
    }
    else {
        //save_sentence.append(fileName + ",");
        std::ofstream label_coord(label_txt);
        label_coord << "";
        label_coord.close();
        save_sentence.append("");
    }

    bool change_result = replaceLine_annotation(read_path, fileName.toStdString(), save_sentence.toStdString());

    show_img(*(&file_list[0] + img_count));

    //ui.list_info->addItem(label_);
    //ui.list_info->addItem(coord_);
}

