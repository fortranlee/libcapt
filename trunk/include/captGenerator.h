#ifndef __LIB_CAPT_GENERATOR_INCLUDE_H__
#define __LIB_CAPT_GENERATOR_INCLUDE_H__

namespace libCapt
{
//Ԥ����
class FontFile;

/** һ��ͼ���������
*/
struct Question
{
	/// ��ѡ�𰸳���
	enum { ANASWER_LENGTH = 4 };			
	/// ͼƬ�ߴ�
	enum { IMAGE_WIDTH=256, IMAGE_HEIGHT=64, IMAGE_PITCH=IMAGE_WIDTH>>1 };
	/// ͼƬ��Ҫ���ڴ��ֽ�(δѹ��״̬)
	enum { IMAGE_BUF_LENGTH = IMAGE_PITCH*IMAGE_HEIGHT };
	/// ͼƬ����ѹ�����
	enum { FLAG_RLE = 1<<16, };

	/// �Ƿ��Ǿ���rleѹ��
	inline bool isCompressed(void) const { return (nFlags & FLAG_RLE) !=0; }
	/// ��������ݵĳ��ȣ����δ����ѹ������С��IMAGE_BUF_LENGTH
	inline unsigned int getSize(void) const { return nFlags&0xFFFF; }

	/// ����flag, ����λ��ͼƬ�ڴ泤�ȣ�����λ��ͼƬ��ʽ����(�Ƿ�ѹ��)
	unsigned int nFlags;
	/// ��ѡ���ĸ���
	unsigned short	wAnswer0[ANASWER_LENGTH];		//��ѡ��0
	unsigned short	wAnswer1[ANASWER_LENGTH];		//��ѡ��1
	unsigned short	wAnswer2[ANASWER_LENGTH];		//��ѡ��2
	unsigned short	wAnswer3[ANASWER_LENGTH];		//��ѡ��3
	/// ͼƬ����
	unsigned char imageBuf[IMAGE_BUF_LENGTH];

	//---------------------------
	//��ȷ�Ĵ𰸱��[0, 1, 2, 3]
	int	nCorrectAnswer;
};

/** ����һ��ͼ���������
*/
class Generator
{
public:
	/** ����һ������
	*/
	void generateQuestion(Question& question);

	/*****************************
		�ڲ�����
	******************************/
private:
	//�������
	enum { NOISE_POINT_COUNTS = 100, NOISE_CURVES_COUNTS = 10};
	//һ���Բ����㹻����������ӿ��ٶ�
	typedef unsigned char RandomBuf[Question::IMAGE_WIDTH];
	//ͼ�񻺳���,Ϊ�򻯴��룬ÿ������1�ֽ�
	typedef unsigned char IMAGE_BUF[Question::IMAGE_WIDTH*Question::IMAGE_HEIGHT];
	//�����������
	enum { CURVES_LENGTH = 256 };
	static const char CURVES_DATA[CURVES_LENGTH];

	int _drawCharacter(int codeIndex, int x_pos, int y_pos, IMAGE_BUF& imageBuf, int _sk1, int _sk2);
	void _drawLine(IMAGE_BUF& im);
	void _drawDots(IMAGE_BUF& im);
	void _blur(IMAGE_BUF& im);
	void _fillRandCurves(IMAGE_BUF& imageBuf);
	void _generatorAnswer(Question& question, unsigned int* answerIndex);
	

private:
	FontFile* m_fontFile;	//!< �����ļ�

public:
	Generator(FontFile* fontFile);
	~Generator();
};

}

#endif
