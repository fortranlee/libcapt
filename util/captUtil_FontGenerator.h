#pragma once

#include "captUtil_FontFile.h"

/** �������������ļ�
*/
class FontGenerator
{
public:
	/** ������Ҫ���ɵ��ַ�, ������Ч�ַ�����
	*/
	int setValiableCodeFromFile(const TCHAR* szFileName, String& strErrMsg);
	/** �����ʹ�õ�����, ����������Ѿ��ж��壬����false
	*/
	bool addFont(const LOGFONT& lf, String& strErrorMsg);
	/** �������
	*/
	void clearFont(void);
	/** ��ʼ��������
	*/
	bool generateFont(void);
	/** ����ļ�
	*/
	FontFilePlus& getFontFile(void) { return m_fontFile; }

private:
	/// �����Ĵ�С
	enum { CANVAS_SIZE = 48, };
	/// ʹ�õ�����
	std::vector< std::pair< LOGFONT, HFONT > > m_allFont;
	/// 16λUNICODE�ܹ����ַ�����
	enum { CODE_COUNTS = 0x10000, };
	/// ��Ҫ�������ַ�
	std::vector< unsigned short > m_allCode;
	/// �����ļ�
	FontFilePlus m_fontFile;

public:
	FontGenerator();
	~FontGenerator();
};


