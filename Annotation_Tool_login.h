#pragma once
#include<QDialog>
#include"ui_Annotation_Tool_login.h"
#include"Annotation_Tool_Main.h"

class Annotation_Tool_login :public QDialog {
	Q_OBJECT

public:
	Annotation_Tool_login(QWidget* parent = nullptr);
	~Annotation_Tool_login();

	//QString login_name;
	//QString login_institude;
	//QString login_career;

private:
	Ui::Dialog ui;
	Annotation_Tool_Main* mainWindow;

private slots:
	void on_loginButton_clicked();
};