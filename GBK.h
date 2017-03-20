#pragma once

#include <QString>
#include <QTextCodec>
#include <string>
using std::string;

class GBK
{
public:
	// QString(Unicode) -> std::string (GBK)
	static string FromUnicode(const QString& qstr)
	{
		QTextCodec* pCodec = QTextCodec::codecForName("gb2312");

		if (!pCodec) return "";
		/* 注： 如果pCodec为NULL, 则说明你的Qt安装位置不是默认位置，
		具体解决方法看Qt篇的《常见问题汇总》! */

		QByteArray arr = pCodec->fromUnicode(qstr);
		string cstr = arr.data();
		return cstr;
	}

	// std::string (GBK) -> QString(Unicode)
	static QString ToUnicode(const string& cstr)
	{
		QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
		if (!pCodec) return "";
		/* 注： 如果pCodec为NULL, 则说明你的Qt安装位置不是默认位置，
		具体解决方法看Qt篇的《常见问题汇总》! */

		QString qstr = pCodec->toUnicode(cstr.c_str(), cstr.length());
		return qstr;
	}

	/* 取两个较短的函数名 : w: wide-char  a: ansi
	大家可以用这两个函数
	*/
	// QString(Unicode) -> std::string (GBK)
	static string w2a(const QString& qstr)
	{
		return FromUnicode(qstr);
	}

	// std::string (GBK) -> QString(Unicode)
	static QString a2w(const string& cstr)
	{
		return ToUnicode(cstr);
	}


};