#pragma once
#include<QDialog>
#include"ui_Annotation_Tool_login.h"

class Annotation_Tool_login :public QDialog {
	Q_OBJECT

public:
	Annotation_Tool_login(QWidget* parent = nullptr);
	~Annotation_Tool_login();
private:
	Ui::Dialog ui;
};