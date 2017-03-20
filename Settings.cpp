#include "Settings.h"
#include "tinyxml/AfTinyXml.h"

int Settings::loadSettings()
{
	int size;
	//解析xml
	TiXmlDocument xml_doc;
	if (!xml_doc.LoadFile("UserSettings.xml"))
	{
		return -1;
	}

	//获取根节点
	TiXmlElement *xml_root = xml_doc.RootElement();
	if (xml_root == NULL)
	{
		return -1;
	}

	//获取字体大小
	TiXmlElement *xml_child = xml_root->FirstChildElement("Settings");
	if (xml_child)
	{
		size = atoi(xml_child->Attribute("FontSize"));
	}
	else
	{
		return -1;
	}

	return size;
}

int Settings::SaveSettings(int size)
{
	//解析xml
	TiXmlDocument xml_doc;
	if (!xml_doc.LoadFile("UserSettings.xml"))
	{
		return -1;
	}

	//获取根节点
	TiXmlElement *xml_root = xml_doc.RootElement();
	if (xml_root == NULL)
	{
		return -1;
	}

	//设置字体大小
	TiXmlElement *xml_child = xml_root->FirstChildElement("Settings");
	if (xml_child)
	{
		xml_child->SetAttribute("FontSize", size);
		xml_doc.SaveFile();
	}
	else
	{
		return -1;
	}

	return 0;
}