#pragma once

#ifndef Annoatation_File_h

#endif // !make_file_h


#include<iostream>
#include<fstream>
#include<string>
#include<QStringList>
#include<list>

bool replaceLine_annotation(const std::string& fileName, std::string searchChar, const std::string& newContent);
void make_label_txt(std::string txt_path, std::string filename, QPoint startP, int w, int h);
void make_directory(std::string path, std::string mode, std::string filename);
void make_info_txt(std::string info_path, QString filename, int width, int height, QStringList input_label_list, std::string user, std::string institude, std::string career);
void save_pixmap(std::string txt_path, std::string filename, QPixmap save_pixmap);

//fstream search_line(fstream input_file, std::string find_string);

QStringList read_label_txt(std::string txt_path, std::string filename);
QPixmap make_pixmap(QStringList input_label_list, QPixmap input_pixmap);
