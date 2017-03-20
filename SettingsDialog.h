#pragma once

#include <QDialog>
#include "ui_SettingsDialog.h"

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(QWidget *parent = Q_NULLPTR, int fontsize = 14);
	~SettingsDialog();
private slots:
	int onBtnSaveSettings();
private:
	Ui::SettingsDialog ui;
	int m_textFontSize; //文本框内字体大小
};
