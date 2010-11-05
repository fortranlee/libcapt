#pragma once


/** �༭ģʽ�µ������ļ�
*/
class FontFilePlus : public libCapt::FontFile
{
public:
	/** �������ļ�, ������ǰ���뽫Ҫ������ַ����� */
	bool createEmptyFile(unsigned int codeCounts);
	/** ����һ���ַ�ͼƬ(rgb24bit) */
	bool insertGlyph_RGB(unsigned short code, const unsigned char* pCanvasBitbuf, int canvasSize, unsigned int lPitch);
	/** ���浽�ļ� */
	bool saveToFile(const TCHAR* szFileName);

private:
	/** ����ͼƬ�����ڴ� */
	void _increseGlyphBuf(unsigned int increseSize);
	/** ָ�����л� */
	void _pointSerial(void);

private:
	/// ͼƬ�����ڴ治ʹ�õ�һ���ڴ泤�ȣ���������ƫ�Ƴ���0
	enum { GLYPHBUF_EMPTY_SIZE = 4 };

	/// ���ڱ༭״̬���ʱ��code����β
	unsigned int m_nCodeTailOffset;
	/// ���ڱ༭״̬���ʱ��glyph����β
	unsigned int m_nGlyphTailOffset;

public:
	FontFilePlus();
	virtual ~FontFilePlus();
};

