#pragma once
#include <QRect>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include<QStringList>
#include<QDebug>
#include<QPixmap>
#include<QPainter>
#include<QBuffer>
#include<QFile>
#include<QIODevice>
//#include<QtXlsx>
//#include<direct.h>

#include"Annotation_Tool_Main.h"
#include"globals.h"
bool replaceLine_annotation(const std::string& fileName, std::string searchChar, const std::string& newContent) {
    vector<std::string> tmp_list;
    // 파일 열기
    bool found = false;
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;
        return found;
    }

    // 임시 파일 생성
    std::string tempFileName = fileName + ".tmp";
    std::ofstream outFile(tempFileName);
    if (!outFile) {
        std::cerr << "Failed to open temporary file for writing: " << tempFileName << std::endl;
        inFile.close(); // 원본 파일 닫기
        return found;
    }

    // 파일의 각 줄을 읽으면서 수정
    std::string line;
    
    while (std::getline(inFile, line)) {
        // 찾을 문자가 포함된 줄인지 확인
        if (line.find(searchChar) != std::string::npos) {
            // 해당 줄의 내용을 새로운 내용으로 대체
            outFile << newContent << '\n';
            found = true;
        }
        else {
            // 그 외의 경우는 그대로 복사
            outFile << line << '\n';
            //tmp_list.push_back(line);
        }
    }
    // 파일 닫기
    inFile.close();
    outFile.close();

    // 임시 파일을 원본 파일로 대체
    if (found) {
        if (std::remove(fileName.c_str()) != 0) {
            std::cerr << "Error removing original file: " << fileName << std::endl;
            return found;
        }
        if (std::rename(tempFileName.c_str(), fileName.c_str()) != 0) {
            std::cerr << "Error renaming temporary file: " << tempFileName << std::endl;
            return found;
        }
        std::cout << "Line updated successfully." << std::endl;
    }
    else {
        std::cerr << "The search character was not found in the file." << std::endl;
        // 임시 파일 삭제
        std::remove(tempFileName.c_str());
    }
    return found;
}
void make_directory(std::string path, std::string mode, std::string filename) {
    if (mode == "root") {
        std::filesystem::path p(path);

        if (std::filesystem::is_directory(p)) {
            return;
        }
        else {
            std::filesystem::create_directories(p);
        }
    }
    else if (mode == "image") {
        std::filesystem::path p(path + "\\" + filename);
        if (std::filesystem::is_directory(p)) {
            return;
        }
        else {
            std::filesystem::create_directories(p);
        }
    }

}

void make_label_txt(std::string txt_path, std::string filename, QPoint startP, int w, int h) {
    std::fstream label_info;
    std::string label_txt_path = txt_path + filename + "\\" + filename + ".txt";
    if (std::filesystem::exists(label_txt_path)) {
        label_info.open(label_txt_path, std::ios::app);
        std::string x = std::to_string(startP.x());
        std::string y = std::to_string(startP.y());
        std::string width = std::to_string(w);
        std::string height = std::to_string(h);
        //label_info << x + "," + y; //+ "," + width + "," + height + "\n";
        label_info << x << ";" << y << ";" << width << ";" << height << "\n";
        label_info.close();
    }
    else {
        label_info.open(label_txt_path, std::ios::out);

        std::string x = std::to_string(startP.x());
        std::string y = std::to_string(startP.y());
        std::string width = std::to_string(w);
        std::string height = std::to_string(h);
        //label_info << x + "," + y; //+ "," + width + "," + height + "\n";
        label_info << x << ";" << y << ";" << width << ";" << height << "\n";
        label_info.close();
    }
}

void make_info_txt(std::string info_path, QString filename, int width, int height, QStringList input_label_list, string user, string institude, string career) {
    std::fstream annotation_info;
    std::string save_path = info_path + "\\Annotation_info.txt";
    QString save_sentence = filename + "," + QString::number(width) + " " + QString::number(height) + ",";
    QString label_info;
    QString coord_info;

    int size = input_label_list.size();

    for (int i = 0; i < size; i++) {
        if (i <= size - 2) {
            label_info.append(input_label_list[i] + ":");
            coord_info.append("Label_" + QString::number(i) + ":");
        }
        else if (i == size - 1) {
            label_info.append(input_label_list[i] + ",");
            coord_info.append("Label_" + QString::number(i) + ",");
        }
    }

    save_sentence.append(coord_info);
    save_sentence.append(label_info);
    save_sentence.append(user + ",");
    save_sentence.append(institude + ",");
    save_sentence.append(career + ",");

    std::string string_filename = filename.toStdString();

    if (std::filesystem::exists(save_path)) {

        bool find_result = replaceLine_annotation(save_path, string_filename, save_sentence.toStdString());
        if (!find_result) {
            annotation_info.open(save_path, std::ios::app);
            annotation_info << save_sentence.toStdString() << "\n";
            annotation_info.close();
        }
    }
    else {
        annotation_info.open(save_path, std::ios::out);
        annotation_info << save_sentence.toStdString() << "\n";
        annotation_info.close();
    }
}

QStringList read_label_txt(std::string txt_path, std::string filename) {
    std::string label_txt_path = txt_path + filename + "\\" + filename + ".txt";
    std::string label;

    QStringList label_list;
    
    if (std::filesystem::exists(label_txt_path)) {
        std::ifstream label_info(label_txt_path);
        while (std::getline(label_info, label)) {
            label_list.append(QString::fromStdString(label));
        }
        return label_list;
    }
    else {
        return label_list;
    }
}


QPixmap make_pixmap(QStringList input_label_list, QPixmap input_pixmap) {
    QPixmap return_pixmap = input_pixmap;

    for (int i = 0; i < input_label_list.size(); i++) {
        QRect tmp_rect;
        
        int x = input_label_list[i].split(";")[0].toInt();
        int y = input_label_list[i].split(";")[1].toInt();
        int width = input_label_list[i].split(";")[2].toInt();
        int height = input_label_list[i].split(";")[3].toInt();
        
        tmp_rect = QRect(QPoint(x, y), QSize(width, height));

        QPainter painter(&return_pixmap);
        QPen pen(Qt::green);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRect(tmp_rect);

        QString label_num = "Label_" + QString::number(i);

        QFont font("Arial", 12);
        QRect txt_rect = QRect(QPoint(x+5, y), QSize(width, height));
        painter.setFont(font);
        painter.drawText(txt_rect, Qt::AlignLeft|Qt::AlignTop, label_num);
    }

    return return_pixmap;
}

void save_pixmap(std::string txt_path, std::string filename, QPixmap save_pixmap) {
    //std::string pixmap_save_path = txt_path + filename + "\\" + filename + ".png";
    QString pixmap_save_path = QString::fromStdString(txt_path + filename + "\\" + filename + ".png");
    QFile file(pixmap_save_path);

    if (QFile::exists(pixmap_save_path)) {
        return;
    }
    else {
        QImage save_img = save_pixmap.toImage();
        save_img.save(pixmap_save_path, "PNG");
    }

}

void save_pixmap_rect(std::string txt_path, std::string filename, QPixmap save_pixmap) {
    QString pixmap_save_path = QString::fromStdString(txt_path + filename + "\\" + filename + "_result" + ".png");
    QFile file(pixmap_save_path);

    QImage save_img = save_pixmap.toImage();
    save_img.save(pixmap_save_path, "PNG");
   
}

void write_log(QString log_sentence) {
    std::fstream log_txt;
    std::string log_path = ".\\result\\" + global_login_name.toStdString() + "\\log.txt";
    if (std::filesystem::exists(log_path)) {
        log_txt.open(log_path, std::ios::app);
        log_txt << log_sentence.toStdString() << "\n";
        log_txt.close();
    }
    else {
        log_txt.open(log_path, std::ios::out);
        log_txt << log_sentence.toStdString() << "\n";
        log_txt.close();
    }
}

int save_xlsx() {
    std::string csv_path = ".\\result\\" + global_login_name.toStdString() + "\\Annotaion_Information.csv";
    std::string annotation_path = ".\\result\\" + global_login_name.toStdString() + "\\annotation_info.txt";
    std::string read_sentence;

    vector<string> label_list;
    vector<string> coord_list;

    std::string line;

    std::string img_name;
    std::string img_shape;
    QStringList label_list_str;
    QStringList coord_list_str;
    std::string user_name;
    std::string user_insti;
    std::string user_career;

    int total_label_num = 0;

    {
        std::ofstream csv_file_header(csv_path);
        csv_file_header << "\xEF\xBB\xBF";
        csv_file_header << "파일명," << "파일 크기," << "작성자," << "소속," << "경력," << "라벨," << "좌표" << std::endl;
    }

    if (std::filesystem::exists(annotation_path)) {
        std::ofstream csv_file_data(csv_path, std::ios::app);
        std::ifstream annotation_file(annotation_path);
        while (std::getline(annotation_file, line)) {
            QStringList line_q = QString::fromStdString(line).split(",");
            if (line_q.size() != 0){}

            img_name = line_q[0].toStdString();
            img_shape = line_q[1].toStdString();
            label_list_str = line_q[2].split(":");
            coord_list_str = line_q[3].split(":");
            user_name = line_q[4].toStdString();
            user_insti = line_q[5].toStdString();
            user_career = line_q[6].toStdString();

            for (int i = 0; i < label_list_str.size(); i++) {
                std::string input_coord = "\"" + coord_list_str[i].toStdString() + "\"";
                std::string input_label = "\"" + label_list_str[i].toStdString() + "\"";
                csv_file_data << img_name + "," + img_shape + "," + user_name + "," + user_insti + "," + user_career + "," + input_label + "," + input_coord << std::endl;
                total_label_num += 1;
            }
        }
    }

    return total_label_num;
}

void removeEmptyLines() {

    std::string annotation_path = ".\\result\\" + global_login_name.toStdString() + "\\annotation_info.txt";
    // 파일 읽기 모드로 열기
    std::ifstream inFile(annotation_path);

    // 파일이 제대로 열렸는지 확인
    if (!inFile) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return;
    }

    // 파일의 내용을 줄 단위로 읽어와 벡터에 저장
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }

    // 파일 닫기
    inFile.close();

    // 공백이 있는 줄을 제거
    lines.erase(std::remove_if(lines.begin(), lines.end(), [](const std::string& str) {
        return str.find_first_not_of(" \t\n\r\f\v") == std::string::npos;
        }), lines.end());

    // 파일 쓰기 모드로 열기
    std::ofstream outFile(annotation_path);

    // 파일이 제대로 열렸는지 확인
    if (!outFile) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return;
    }

    // 수정된 내용을 파일에 쓰기
    for (const auto& line : lines) {
        outFile << line << std::endl;
    }

    // 파일 닫기
    outFile.close();

    std::cout << "공백이 있는 줄이 제거되었습니다." << std::endl;
}