#include "SettingsDialog.h"
#include "Settings.h"
#include "MyNoteBook.h"

SettingsDialog::SettingsDialog(QWidget *parent, int fontsize)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.spinBoxFontSize->setValue(fontsize);
	connect(ui.btnSaveSettings, SIGNAL(clicked()), this, SLOT(onBtnSaveSettings()));
}

SettingsDialog::~SettingsDialog()
{
}

int SettingsDialog::onBtnSaveSettings()
{
	Settings::SaveSettings(ui.spinBoxFontSize->value());
	MyNoteBook *mainPtr = (MyNoteBook *)parentWidget(); //获得父窗口的指针
	mainPtr->setPlainTextSize(); //调用父窗口的方法
	return 0;
}
