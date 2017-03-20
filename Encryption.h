#pragma once
#include <QString>

//加密解密模块，支持字母数字混合密码！
class Encryption
{
public:
	static QString Encrypt(QString pwdKey, QString text); //加密文本
	static QString Decrypt(QString pwdKey, QString text); //解密文本
};