#pragma once

#include<Annotation_Tool_login.h>
#include<Annotation_Tool_Main.h>
#include"globals.h"
#include<QObject>
#include<QMessageBox>

Annotation_Tool_login::Annotation_Tool_login(QWidget* parent)
	: QDialog(parent) 
{
	ui.setupUi(this);
	QPixmap logo;
	logo.load("./imgs/caimi_logo.png");
	ui.lbl_logo->setPixmap(logo);
	ui.lbl_logo->setScaledContents(true);
	connect(ui.btn_login, &QPushButton::clicked, this, &Annotation_Tool_login::on_loginButton_clicked);
}

Annotation_Tool_login::~Annotation_Tool_login() {

}

void Annotation_Tool_login::on_loginButton_clicked() {
	QString login_name = ui.line_name->text();
	QString login_institude = ui.cb_institude->currentText();
	QString login_career = ui.cb_career->currentText();

	if (!login_name.isEmpty()) {
		global_login_name = login_name;
		global_login_institude = login_institude;
		global_login_career = login_career;

		mainWindow = new Annotation_Tool_Main();
		mainWindow->show();
		this->close();
	}
	else {
		QMessageBox::warning(nullptr, "Warning", "Please check the User Information.");
	}

	//mainWindow = new Annotation_Tool_Main();
	//mainWindow->show();
}