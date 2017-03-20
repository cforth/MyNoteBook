#pragma once

class Settings
{
public:
	static int loadSettings(); //从配置文件UserSettings.xml中读取用户配置
	static int SaveSettings(int size); //从配置文件UserSettings.xml中保存用户配置
};