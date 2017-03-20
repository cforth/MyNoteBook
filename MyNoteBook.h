#pragma once

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include "ui_MyNoteBook.h"

class MyNoteBook : public QMainWindow
{
	Q_OBJECT

public:
	MyNoteBook(QWidget *parent = Q_NULLPTR);
	void setPlainTextSize(); //设置文本窗口字体大小

private slots:
	int OnMenuNew();
	int OnMenuSave();
	int OnMenuSaveAs();
	int OnMenuOpen();
	int OnMenuFontSizeUp();
	int OnMenuFontSizeDown();
	int OnMenuHelp();
	int OnMenuSettings();
	int OnMenuEncrypt();
	int OnMenuDecrypt();

private:
	Ui::MyNoteBookClass ui;
	QString m_filepath; //文件名
	QLabel *m_labelName; //状态栏显示
	int textFontSize; //文本框内字体大小
	QString encryptFlag; //加密标志，如果文本第一行为加密标志，表示已经加密

	void closeEvent(QCloseEvent * event);
	bool isEncrypt(); //判断文本是否已经加密
};
