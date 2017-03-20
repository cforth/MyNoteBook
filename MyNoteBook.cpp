#include "MyNoteBook.h"
#include <QFileDialog>
#include <QMessageBox>
#include "GBK.h"

#include "Encryption.h"
#include "PasswordDialog.h"
#include "SettingsDialog.h"
#include "Settings.h"

MyNoteBook::MyNoteBook(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_labelName = new QLabel();
	m_labelName->setMinimumWidth(300);
	ui.statusBar->addWidget(m_labelName);
	setPlainTextSize();
	encryptFlag = GBK::ToUnicode("{cf}"); //设置加密标志

	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(OnMenuNew()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(OnMenuSave()));
	connect(ui.actionSaveAs, SIGNAL(triggered()), this, SLOT(OnMenuSaveAs()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(OnMenuOpen()));
	connect(ui.actionFontSizeUp, SIGNAL(triggered()), this, SLOT(OnMenuFontSizeUp()));
	connect(ui.actionFontSizeDown, SIGNAL(triggered()), this, SLOT(OnMenuFontSizeDown()));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(OnMenuHelp()));
	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(OnMenuSettings()));
	connect(ui.actionEncrypt, SIGNAL(triggered()), this, SLOT(OnMenuEncrypt()));
	connect(ui.actionDecrypt, SIGNAL(triggered()), this, SLOT(OnMenuDecrypt()));
}

void MyNoteBook::setPlainTextSize()
{
	textFontSize = Settings::loadSettings(); //从配置文件中读取字体大小设置
	ui.plainTextEdit->setFont(QFont("sans-serif", textFontSize));
}

int MyNoteBook::OnMenuNew()
{
	m_filepath = "";
	ui.plainTextEdit->clear(); //清空文本
	this->setWindowTitle(m_filepath); //修改窗口标题
	m_labelName->setText(GBK::ToUnicode("新建文件，未命名..."));
	return 0;
}

int MyNoteBook::OnMenuSave()
{
	QString filepath;
	if (m_filepath == "")
	{
		filepath = QFileDialog::getSaveFileName(this,
			GBK::ToUnicode("保存文件"));
	}
	else
	{
		filepath = m_filepath;
	}

	if (filepath.length() == 0) return -1;

	//保存文件
	string text = GBK::FromUnicode(ui.plainTextEdit->toPlainText());
	string filename = GBK::FromUnicode(filepath);

	FILE *fp = fopen(filename.c_str(), "wb");
	fwrite(text.c_str(), 1, text.length(), fp);
	fclose(fp);

	m_filepath = filepath;
	this->setWindowTitle(m_filepath); //修改窗口标题

	//状态栏显示
	ui.statusBar->showMessage(GBK::ToUnicode("已保存"), 3000);
	m_labelName->setText(filepath);
	return 0;
}

int MyNoteBook::OnMenuSaveAs()
{
	m_filepath = "";
	OnMenuSave();
	return 0;
}

int MyNoteBook::OnMenuOpen()
{
	QString filepath = QFileDialog::getOpenFileName(this,
		GBK::ToUnicode("打开文件"));
	if (filepath.length() == 0) return -1;

	//打开文件
	string filename = GBK::FromUnicode(filepath);
	FILE *fp = fopen(filename.c_str(), "rb");
	//文件的大小
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);

	//读取内容
	fseek(fp, 0, SEEK_SET);
	char* buf = new char[filesize + 1];
	int n = fread(buf, 1, filesize, fp);
	if (n > 0)
	{
		buf[n] = 0;

		//显示到文本框中
		ui.plainTextEdit->setPlainText(GBK::ToUnicode(buf));
		this->setWindowTitle(GBK::ToUnicode(filename)); //修改窗口标题
	}
	delete[] buf; //释放内存

	m_filepath = filepath;
	fclose(fp);
	//状态栏显示
	ui.statusBar->showMessage(GBK::ToUnicode("已打开"), 3000);
	m_labelName->setText(filepath);
	return 0;
}

int MyNoteBook::OnMenuFontSizeUp()
{
	textFontSize = textFontSize >= 50 ? 50 : textFontSize+2;
	ui.plainTextEdit->setFont(QFont("sans-serif", textFontSize));
	return 0;
}

int MyNoteBook::OnMenuFontSizeDown()
{
	textFontSize = textFontSize <= 10 ? 10 : textFontSize - 2;
	ui.plainTextEdit->setFont(QFont("sans-serif", textFontSize));
	return 0;
}

int MyNoteBook::OnMenuHelp()
{
	QMessageBox::information(this, GBK::ToUnicode("帮助"), GBK::ToUnicode("MyNoteBook 0.1版"));
	return 0;
}

int MyNoteBook::OnMenuSettings()
{
	SettingsDialog setDialog(this,textFontSize);
	setDialog.exec();
	return 0;
}

void MyNoteBook::closeEvent(QCloseEvent *event)
{
	int saveFlag = QMessageBox::question(this, 
		GBK::ToUnicode("关闭"),
		GBK::ToUnicode("是否关闭"),
		QMessageBox::Yes,
		QMessageBox::No);

	if (saveFlag == QMessageBox::Yes) {
//		Settings::SaveSettings(textFontSize);
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

int MyNoteBook::OnMenuEncrypt()
{
	if (isEncrypt())
	{
		QMessageBox::information(this, GBK::ToUnicode("错误提示"), GBK::ToUnicode("已经加密了啊，亲！"));
		return -1;
	}

	PasswordDialog pwdDialog(this);
	pwdDialog.exec();
	QString pwdKey = pwdDialog.pwdKey;
	if (pwdKey.size() != 0)
	{
		QString text = ui.plainTextEdit->toPlainText();
		ui.plainTextEdit->setPlainText(Encryption::Encrypt(pwdKey, text));
		ui.plainTextEdit->appendPlainText(encryptFlag); //添加尾部的加密标志字符串
	}
	return 0;
}

int MyNoteBook::OnMenuDecrypt()
{
	if (!isEncrypt())
	{
		QMessageBox::information(this, GBK::ToUnicode("错误提示"), GBK::ToUnicode("并没有加密啊，亲！"));
		return -1;
	}

	PasswordDialog pwdDialog(this);
	pwdDialog.exec();
	QString pwdKey = pwdDialog.pwdKey;
	if (pwdKey.size() != 0)
	{
		int encryptFlagLen = encryptFlag.size();
		QString oldText = ui.plainTextEdit->toPlainText();
		QString text = oldText.left(oldText.size() - encryptFlagLen); //删除尾部的加密标志字符串
		ui.plainTextEdit->setPlainText(Encryption::Decrypt(pwdKey, text));
	}
	return 0;
}

bool MyNoteBook::isEncrypt()
{
	QString text = ui.plainTextEdit->toPlainText();

	if (text.size() < encryptFlag.size())
		return false;

	if (text.endsWith(encryptFlag))
		return true;
	else
		return false;
}