#include <QRect>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include<QStringList>
#include<QDebug>
//#include<direct.h>

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
        label_info << x << "," << y << "," << width << "," << height << "\n";
        label_info.close();
    }
    else {
        label_info.open(label_txt_path, std::ios::out);

        std::string x = std::to_string(startP.x());
        std::string y = std::to_string(startP.y());
        std::string width = std::to_string(w);
        std::string height = std::to_string(h);
        //label_info << x + "," + y; //+ "," + width + "," + height + "\n";
        label_info << x << "," << y << "," << width << "," << height << "\n";
        label_info.close();
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

QList<int> each_label(QStringList label) {
    QList<int> return_label;

    for (auto iter = label.begin(); iter != label.end(); ++iter) {
        QString tmp =  *iter;
        return_label.append(tmp.toInt());
    }
    return return_label;
}