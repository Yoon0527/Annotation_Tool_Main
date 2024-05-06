#pragma once

#include "Annotation_Tool_Main.h"
#include"Annotation_Tool_login.h"
#include"Annotation_FIle.h"
#include"globals.h"
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QPaintEvent>
#include <iostream>
#include<QMessageBox>
#include<direct.h>
#include<cstdlib>
#include<QTimer>

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
    connect(ui.btn_saveInfo, SIGNAL(clicked()), this, SLOT(save_files()));
    //connect(ui.btn_quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui.list_lbl, &QListWidget::itemClicked, this, &Annotation_Tool_Main::onItemClicked);

    connect(ui.sl_imgBrightness, &QSlider::valueChanged, this, &Annotation_Tool_Main::adjustBrightness);
    connect(ui.list_image, &QListWidget::itemDoubleClicked, this, &Annotation_Tool_Main::image_move);

    ui.lbl_image->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Annotation_Tool_Main::updateTimer);
    timer->start(1000);

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
    }
    else {
        ui.lbl_image->setPixmap(adjustedPixmap);
        ui.lbl_image->setScaledContents(true);
        current_pixmap = adjustedPixmap;
        
    }

}

void Annotation_Tool_Main::init_info() {

    user_institution = global_login_institude.toStdString();
    user_career = global_login_career.toStdString();
    user_name = global_login_name.toStdString();

    //user_path = './' + "/result/" + user_name + '/';
    user_path = ".\\result\\" + user_name;
    make_directory(user_path, "root", "");

    ui.list_log->addItem("Log in Complete.");
    ui.list_log->addItem(QString("User: " + global_login_name));
    ui.list_log->addItem(QString("Institution: " + global_login_institude));
    ui.list_log->addItem(QString("Career: " + global_login_career));
    write_log(QString("User: " + global_login_name));
    write_log(QString("Institution: " + global_login_institude));
    write_log(QString("Career: " + global_login_career));
}

void Annotation_Tool_Main::load_image() {
    if (file_list.size() != 0) {
        QStringList file_list_tmp = file_list;
        QStringList differ_list;

        QFileDialog dlg;
        file_list = dlg.getOpenFileNames(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");

        for (const QString& path : file_list) {
            if (!file_list_tmp.contains(path)) {
                differ_list.append(path);
                file_list_tmp.append(path);
            }
        }

        for (int i = 0; i < differ_list.size(); i++) {
            QString fileName_pre = differ_list[i].section("/", -1);
            file_name_list.append(fileName_pre.split(".")[0]);
        }
        std::sort(file_name_list.begin(), file_name_list.end());
        ui.list_image->clear();
        ui.list_image->addItems(file_name_list);
        write_log(QString(QString::number(differ_list.size()) + " additional images have been loaded"));
        ui.list_log->addItem(QString(QString::number(differ_list.size()) + " additional images have been loaded"));

        std::sort(file_list_tmp.begin(), file_list_tmp.end());

        file_list = file_list_tmp;
        file_list_len = file_list.size();
        img_count = file_list.indexOf(current_image_path);
    }
    else {
        file_list.clear();
        ui.list_image->clear();

        QFileDialog dlg;
        file_list = dlg.getOpenFileNames(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");

        file_name_list.clear();

        for (int i = 0; i < file_list.size(); i++) {
            QString fileName_pre = file_list[i].section("/", -1);
            file_name_list.append(fileName_pre.split(".")[0]);
        }

        ui.list_image->addItems(file_name_list);

        file_list_len = file_list.size();
        QString file_size = QString::number(file_list_len);

        if (file_list.size() > 0) {
            Annotation_Tool_Main::show_img(file_list[0]);
        }
        write_log(QString(file_size + " Images Load"));
        ui.list_log->addItem(QString(file_size + " Images Load"));
    }
    //ui.list_image->sortItems();
    ui.lbl_image->setFocus();
}

void Annotation_Tool_Main::show_img(QString path) {
    current_image_path = path;
    ui.lbl_image->setFocus();
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
    ui.lbl_current_imgName->setText(fileName);
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
        save_pixmap_rect(user_path + "\\", fileName.toStdString(), return_pixmap);
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
        save_pixmap_rect(user_path + "\\", fileName.toStdString(), m_image);
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
                rect_w = 0;
                rect_h = 0;
            }
        }
        else if (event->type() == QEvent::MouseMove && m_drawing) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint endPos = mapToImageCoordinates(mouseEvent->pos());
            rect_w = endPos.x() - startPoint.x();
            rect_h = endPos.y() - startPoint.y();
            if (rect_w > 15 && rect_h > 15) {
                m_currentRect = QRect(startPoint, QSize(rect_w, rect_h));

                QPixmap pixmap = current_pixmap;
                QPainter painter(&pixmap);
                QPen pen(Qt::green);
                pen.setWidth(1);

                painter.setPen(pen);
                painter.drawRect(m_currentRect);
                ui.lbl_image->setPixmap(pixmap);
            }

        }
        else if (event->type() == QEvent::MouseButtonRelease && m_drawing) {
            if (rect_w > 15 && rect_h > 15) {
                m_drawing = false;
                m_rectangles.append(m_currentRect);
                make_label_txt(label_path, fileName.toStdString(), startPoint, m_currentRect.width(), m_currentRect.height());
                QStringList tmp = read_label_txt(label_path, fileName.toStdString());
                QPixmap return_pixmap = make_pixmap(tmp, current_pixmap);
                ui.lbl_image->setPixmap(return_pixmap);
                make_info_txt(user_path, fileName, img_w, img_h, tmp, user_name, user_institution, user_career);
                ui.list_lbl->clear();
                bool result = read_info();
                write_log(QString("Draw Rect, " + fileName + ":" + QString::number(startPoint.x()) + "," + QString::number(startPoint.y()) + "," + QString::number(m_currentRect.width()) + "," + QString::number(m_currentRect.height())));
                save_xlsx();
                save_pixmap_rect(user_path + "\\", fileName.toStdString(), return_pixmap);
            }
            else {
                show_img(*(&file_list[0] + img_count));
            }

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
    if (ui.list_lbl->count() != 0 && select_item != NULL) {
        
        QString select = select_item->text();
        //ui.list_lbl->setCurrentItem(nullptr);
        int row = ui.list_lbl->currentRow();
        delete ui.list_lbl->takeItem(row);
        //select_item->setSelected(false);
        
        change_txt(rect_info, select);
        removeEmptyLines();
        save_xlsx();
    }
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

void Annotation_Tool_Main::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Right:
    case Qt::Key_Space:
        next_img();
        break;
    case Qt::Key_Left:
        prev_img();
        break;
    default:
        Annotation_Tool_Main::keyPressEvent(event);
        break;
    }
}

void Annotation_Tool_Main::image_move() {
    int find_txt = ui.list_image->currentIndex().row();
    img_count = find_txt;
    QString* first_path(&file_list[0]);
    Annotation_Tool_Main::show_img(*(first_path + find_txt));
}

void Annotation_Tool_Main::receiveLoginInfo(const QString& login_name, const QString& login_institude, const QString login_career) {
    user_name = login_name.toStdString();
    user_institution = login_institude.toStdString();
    user_career = login_career.toStdString();
}

void Annotation_Tool_Main::save_files() {
    QString total_label_num = QString::number(save_xlsx());

    if (total_label_num == "0") {
        ui.list_log->addItem(QString("No label information to save."));
    }
    else {
        ui.list_log->addItem(QString(total_label_num + " Labels Information Saved"));
        write_log(QString(QString(total_label_num + " Labels Information Saved")));
    }

    devideFile();
}

void Annotation_Tool_Main::updateTimer() {
    
    if (second < 60) {
        ui.lbl_second->setText(QString::number(second));
        ui.lbl_minute->setText(QString::number(minute));
        second += 1;
    }
    else if (second == 60) {
        second = 0;
        minute += 1;
        ui.lbl_second->setText(QString::number(second));
        ui.lbl_minute->setText(QString::number(minute));
        second += 1;
    }
}