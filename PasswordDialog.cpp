#include "PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnOK, SIGNAL(clicked()), this, SLOT(OnBtnOk()));
}

PasswordDialog::~PasswordDialog()
{
}

int PasswordDialog::OnBtnOk()
{
	this->pwdKey = ui.passwordText->text();
	this->accept();
	return 0;
}