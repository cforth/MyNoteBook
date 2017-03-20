#pragma once

#include <QDialog>
#include "ui_PasswordDialog.h"

class PasswordDialog : public QDialog
{
	Q_OBJECT

public:
	PasswordDialog(QWidget *parent = Q_NULLPTR);
	~PasswordDialog();

private slots:
	int OnBtnOk();

public:
	QString pwdKey;

private:
	Ui::PasswordDialog ui;
};
