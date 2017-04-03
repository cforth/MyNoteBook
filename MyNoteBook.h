#pragma once

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include "ui_MyNoteBook.h"
#include <string>

class MyNoteBook : public QMainWindow
{
	Q_OBJECT

public:
	MyNoteBook(QWidget *parent = Q_NULLPTR);
	void setPlainTextSize(); //�����ı����������С

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

protected:
	//��д�Ϸ��¼�����֧������txt�ļ�������
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);

private:
	Ui::MyNoteBookClass ui;
	QString m_filepath; //�ļ���
	QLabel *m_labelName; //״̬����ʾ
	int textFontSize; //�ı����������С
	QString encryptFlag; //���ܱ�־������ı���һ��Ϊ���ܱ�־����ʾ�Ѿ�����

	void closeEvent(QCloseEvent * event);
	int readFile(QString filepath); //��ȡ�ļ����ݵ��ı�����
	bool isEncrypt(); //�ж��ı��Ƿ��Ѿ�����
};
