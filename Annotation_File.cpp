#include <QRect>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include<QStringList>
#include<QDebug>
#include<QPixmap>
#include<QPainter>
//#include<direct.h>

#include"Annotation_Tool_Main.h"

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

void make_info_txt(std::string info_path, QString filename, int width, int height, QStringList input_label_list) {
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

    std::string string_filename = filename.toStdString();

    if (std::filesystem::exists(save_path)) {
        //annotation_info.open(save_path, std::ios::app);
        //annotation_info << save_sentence.toStdString() << "\n";
        //annotation_info.close();

        replaceLine_annotation(save_path, string_filename, save_sentence.toStdString());
    }
    else {
        annotation_info.open(save_path, std::ios::out);
        annotation_info << save_sentence.toStdString() << "\n";
        annotation_info.close();
    }
}

void replaceLine_annotation(const std::string& fileName, std::string searchChar, const std::string& newContent) {
    // 파일 열기
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;
        return;
    }

    // 임시 파일 생성
    std::string tempFileName = fileName + ".tmp";
    std::ofstream outFile(tempFileName);
    if (!outFile) {
        std::cerr << "Failed to open temporary file for writing: " << tempFileName << std::endl;
        inFile.close(); // 원본 파일 닫기
        return;
    }

    // 파일의 각 줄을 읽으면서 수정
    std::string line;
    bool found = false;
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
        }
    }

    // 파일 닫기
    inFile.close();
    outFile.close();

    // 임시 파일을 원본 파일로 대체
    if (found) {
        if (std::remove(fileName.c_str()) != 0) {
            std::cerr << "Error removing original file: " << fileName << std::endl;
            return;
        }
        if (std::rename(tempFileName.c_str(), fileName.c_str()) != 0) {
            std::cerr << "Error renaming temporary file: " << tempFileName << std::endl;
            return;
        }
        std::cout << "Line updated successfully." << std::endl;
    }
    else {
        std::cerr << "The search character was not found in the file." << std::endl;
        // 임시 파일 삭제
        std::remove(tempFileName.c_str());
    }
}

//fstream search_line(fstream input_file, std::string find_string) {
//    std::vector<std::string> lines;
//    std::string line;
//    while (std::getline(input_file, line)) {
//        lines.push_back(line);
//    }
//
//    auto it = std::find_if(lines.begin(), lines.end(), [find_string](const std::string& line) {
//        return line.find(find_string) != std::string::npos;
//        });
//
//    if (it != lines.end()) {
//        // 찾은 줄의 내용을 새로운 내용으로 대체
//        *it = newContent;
//
//        // 파일을 쓰기 모드로 열어서 수정된 내용 쓰기
//        file.close();
//        file.open(fileName, std::ios::out | std::ios::trunc); // 기존 내용 삭제하고 새로 쓰기
//        for (const auto& updatedLine : lines) {
//            file << updatedLine << '\n';
//        }
//        std::cout << "Line updated successfully." << std::endl;
//    }
//    else {
//        std::cerr << "The search character was not found in the file." << std::endl;
//    }
//}


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
        pen.setWidth(3);
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