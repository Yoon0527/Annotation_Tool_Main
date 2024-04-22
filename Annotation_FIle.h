#pragma once

#ifndef Annoatation_File_h

#endif // !make_file_h


#include<iostream>
#include<fstream>
#include<string>
#include<QStringList>

void make_label_txt(std::string txt_path, std::string filename, QPoint startP, int w, int h);
void make_directory(string path, string mode, string filename);
void make_info_txt(std::string info_path, QString filename, int width, int height, QStringList input_label_list);
void replaceLine_annotation(const std::string& fileName, string searchChar, const std::string& newContent);
//fstream search_line(fstream input_file, std::string find_string);

QStringList read_label_txt(std::string txt_path, std::string filename);
QPixmap make_pixmap(QStringList input_label_list, QPixmap input_pixmap);