#include "MyNoteBook.h"
#include <QFileDialog>
#include <QMessageBox>
#include "GBK.h"

#include "Encryption.h"
#include "PasswordDialog.h"
#include "SettingsDialog.h"
#include "Settings.h"
#include <QMimeData>
using std::string;

MyNoteBook::MyNoteBook(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//����״̬��
	m_labelName = new QLabel();
	m_labelName->setMinimumWidth(300);
	ui.statusBar->addWidget(m_labelName);
	
	//�������ļ����������С
	setPlainTextSize();

	//���ü��ܱ�־
	encryptFlag = GBK::ToUnicode("{cf}");

	//��ֹ�ӿؼ�plainTextEdit�Ϸ��¼������Ϸ��¼��������ڴ���
	ui.plainTextEdit->setAcceptDrops(false);
	setAcceptDrops(true);

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
	textFontSize = Settings::loadSettings(); //�������ļ��ж�ȡ�����С����
	ui.plainTextEdit->setFont(QFont("sans-serif", textFontSize));
}

int MyNoteBook::OnMenuNew()
{
	m_filepath = "";
	ui.plainTextEdit->clear(); //����ı�
	this->setWindowTitle(m_filepath); //�޸Ĵ��ڱ���
	m_labelName->setText(GBK::ToUnicode("�½��ļ���δ����..."));
	return 0;
}

int MyNoteBook::OnMenuSave()
{
	QString filepath;
	if (m_filepath == "")
	{
		filepath = QFileDialog::getSaveFileName(this,
			GBK::ToUnicode("�����ļ�"));
	}
	else
	{
		filepath = m_filepath;
	}

	if (filepath.length() == 0) return -1;

	//�����ļ�
	string text = GBK::FromUnicode(ui.plainTextEdit->toPlainText());
	string filename = GBK::FromUnicode(filepath);

	FILE *fp = fopen(filename.c_str(), "wb");
	fwrite(text.c_str(), 1, text.length(), fp);
	fclose(fp);

	m_filepath = filepath;
	this->setWindowTitle(m_filepath); //�޸Ĵ��ڱ���

	//״̬����ʾ
	ui.statusBar->showMessage(GBK::ToUnicode("�ѱ���"), 3000);
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
		GBK::ToUnicode("���ļ�"));

	if (filepath.length() == 0) return -1;

	//���ļ�
	readFile(filepath);


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
	QMessageBox::information(this, GBK::ToUnicode("����"), GBK::ToUnicode("MyNoteBook 0.1��"));
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
		GBK::ToUnicode("�ر�"),
		GBK::ToUnicode("�Ƿ�ر�"),
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
		QMessageBox::information(this, GBK::ToUnicode("������ʾ"), GBK::ToUnicode("�Ѿ������˰����ף�"));
		return -1;
	}

	PasswordDialog pwdDialog(this);
	pwdDialog.exec();
	QString pwdKey = pwdDialog.pwdKey;
	if (pwdKey.size() != 0)
	{
		QString text = ui.plainTextEdit->toPlainText();
		ui.plainTextEdit->setPlainText(Encryption::Encrypt(pwdKey, text));
		ui.plainTextEdit->appendPlainText(encryptFlag); //���β���ļ��ܱ�־�ַ���
	}
	return 0;
}

int MyNoteBook::OnMenuDecrypt()
{
	if (!isEncrypt())
	{
		QMessageBox::information(this, GBK::ToUnicode("������ʾ"), GBK::ToUnicode("��û�м��ܰ����ף�"));
		return -1;
	}

	PasswordDialog pwdDialog(this);
	pwdDialog.exec();
	QString pwdKey = pwdDialog.pwdKey;
	if (pwdKey.size() != 0)
	{
		int encryptFlagLen = encryptFlag.size();
		QString oldText = ui.plainTextEdit->toPlainText();
		QString text = oldText.left(oldText.size() - encryptFlagLen); //ɾ��β���ļ��ܱ�־�ַ���
		ui.plainTextEdit->setPlainText(Encryption::Decrypt(pwdKey, text));
	}
	return 0;
}

void MyNoteBook::dragEnterEvent(QDragEnterEvent * event)
{
	//�����ļ�����
	if (event->mimeData()->hasFormat("text/uri-list"))
	{
		QString falepath = event->mimeData()->text();
		//ֻ��txt�ļ�������drop
		if (falepath.endsWith("txt"))
		{
			event->acceptProposedAction();
		}
	}
	//�����ı���������
	else if (event->mimeData()->hasFormat("text/plain"))
	{
		event->acceptProposedAction();
	}
	return;
}

void MyNoteBook::dropEvent(QDropEvent * event)
{
	QString text = event->mimeData()->text();
	if (event->mimeData()->hasFormat("text/uri-list"))
	{
		//ͨ���ϷŴ��ļ�ʱ���뽫������ļ�·��ǰ��"file:\\\"ɾ��
		string t = GBK::FromUnicode(text).substr(8);
		QString filepath(GBK::ToUnicode(t));
		readFile(filepath);
	}
	else if (event->mimeData()->hasFormat("text/plain"))
	{
		ui.plainTextEdit->appendPlainText(text);
	}
	return;
}

int MyNoteBook::readFile(QString filepath)
{
	string filename = GBK::FromUnicode(filepath);
	if (filename.length() == 0) return -1;
	FILE *fp = fopen(filename.c_str(), "rb");
	//�ļ��Ĵ�С
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);

	//��ȡ����
	fseek(fp, 0, SEEK_SET);
	char* buf = new char[filesize + 1];
	int n = fread(buf, 1, filesize, fp);

	if (n > 0)
	{
		buf[n] = 0;

		//��ʾ���ı�����
		ui.plainTextEdit->setPlainText(GBK::ToUnicode(buf));
		this->setWindowTitle(GBK::ToUnicode(filename)); //�޸Ĵ��ڱ���
	}
	delete[] buf; //�ͷ��ڴ�
	fclose(fp);
	m_filepath = filepath;

	//״̬����ʾ
	ui.statusBar->showMessage(GBK::ToUnicode("�Ѵ�"), 3000);
	m_labelName->setText(filepath);

	//�ж��ı��Ƿ񾭹��˼���
	if (isEncrypt())
	{
		OnMenuDecrypt();
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