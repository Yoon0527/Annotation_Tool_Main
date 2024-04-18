#pragma once

#ifndef Annoatation_File_h

#endif // !make_file_h


#include<iostream>
#include<fstream>
#include<string>
#include<QStringList>

void make_label_txt(std::string txt_path, std::string filename, QPoint startP, int w, int h);
void make_directory(string path, string mode, string filename);

QStringList read_label_txt(std::string txt_path, std::string filename);
QList<int> each_label(QStringList label);