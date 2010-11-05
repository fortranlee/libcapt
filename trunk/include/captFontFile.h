#ifndef __LIB_CAPT_FONT_FILE_INCLUDE_H__
#define __LIB_CAPT_FONT_FILE_INCLUDE_H__

namespace libCapt
{
/** ��¼�����ļ�
*/
class FontFile
{
public:
	// �ļ���ʶ 'CAPF'
	enum { MAGIC_CODE = 0x46504143, };
	/** ���������ļ�
	*/
	struct Head
	{
		unsigned int dwMagicCode;		//'CAPF'
		unsigned int nCodeOffset;		//�ַ�����ƫ��
		unsigned int nCodeCounts;		//�������ַ���
		unsigned int nGlyphOffset;		//�ַ�ͼƬ����ƫ��
		unsigned int nGlyphCounts;		//�������ַ�ͼ����
		unsigned int nGlyphSize;		//�ַ�ͼƬ���ݳ���
	};

	/** һ���ַ�ͼ������ */
	struct CodeGlyph
	{
		union
		{
			unsigned long long	nNextOffset;	//!< ��һ��ͼƬ��ƫ��(�����m_pGlyphBuf),���ǵ�64λϵͳ�����ݸ�ʽ���ݣ�ʹ��64λ����
			CodeGlyph*			pNext;			//!< ��һ��ͼƬ��ָ��(����ʱ)
		};
		char				data[1];
	};

	/** һ���ַ����� */
	struct Code
	{
		unsigned short		wCode;			//!< UNICODE����
		unsigned short		nGlyphCounts;	//!< ͼƬ������
		union
		{
			unsigned long long	nFirstOffset;	//!< ��һ��ͼƬ��ƫ��(�����m_pGlyphBuf),���ǵ�64λϵͳ�����ݸ�ʽ���ݣ�ʹ��64λ����
			CodeGlyph*			pFirst;			//!< ��һ��ͼƬ��ָ��(����ʱ)
		};
	};

	/** �ַ����������ǣ����з��ͽ����� */
	enum { GLPYH_EOL=-128, GLPYH_EOF=-127 };

public:
	/** �ͷ� */
	void release(void);
	/** �������м����������� */
	bool loadFromDataStream(const unsigned char* pStream, unsigned int streamSize);
	/** �õ���֧�ֵ��ַ����� */
	unsigned int getCodeCounts(void) const { return m_fileHead.nCodeCounts; }
	/** �õ�����һ���ַ� */
	unsigned short getCodeFromIndex(unsigned int index) const;
	/** ͨ���������ĳ���ַ���ͼ������ */
	char* getCodeGlyphFromIndex(unsigned int index) const;

protected:
	/**  ָ������ʱ�� */
	void _pointRuntime(void);

private:
	/** �ڲ���������ȡ�ڴ��� */
	bool _readStrem(unsigned char* dest, unsigned int size, const unsigned char*& pStream, const unsigned char* pStreamEnd);

protected:
	/// �ļ�ͷ
	Head m_fileHead;
	/// �ַ�����
	Code* m_pCodeBuf;
	/// ͼƬ�����ڴ�
	unsigned char* m_pGlyphBuf;

public:
	FontFile();
	virtual ~FontFile();
};

}

#endif
