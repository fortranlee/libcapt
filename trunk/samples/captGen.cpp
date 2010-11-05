#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <captGenerator.h>
#include <captFontFile.h>
#include <captRLE.h>


#pragma pack(push, 1)
struct TGA_HEAD				// TGAͼ���ļ�ͷ
{
	unsigned char  IdentSize;			// ��־�򳤶�		00 �ޱ�־��
	unsigned char  ColorMapType;		// ɫ�ʱ�����		00 ��ɫ�ʱ�
	unsigned char  ImageType;			// ͼ������			02 ��ѹ��RGBͼ��
	unsigned short ColorMapStart;		// ��һ��ɫ�ʱ����	00 00
	unsigned short ColorMapLength;		// ɫ�ʱ���		00 00
	unsigned char  ColorMapBits;		// ɫ�ʱ���ڴ�С	00
	unsigned short XStart;				// ͼ��Xԭ��		00 00
	unsigned short YStart;				// ͼ��Yԭ��		00 00
	unsigned short Width;				// ͼ����			80 02 ͼ����640 ****
	unsigned short Height;				// ͼ��߶�			E0 01 ͼ��߶�480 ****
	unsigned char  Bits;				// ÿ������λ��		20 ����λ��RGBA 32λ ****
	unsigned char  Descriptor;			// ͼ��������		08 Alphaͨ��λ��8
										// ͼ���һ���������½�
};
#pragma pack(pop)

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Usage: captGen [FontFile] [OutputFile]\n");
		return 1;
	}
	
	FILE* fp = fopen(argv[1], "rb");
	if(fp==0)
	{
		printf("Error, Open font file %s error!\n", argv[1]);
		return 1;
	}
	
	fseek(fp, 0, SEEK_END);
	unsigned int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* streamBuf = new unsigned char[fileSize];
	fread(streamBuf, 1, fileSize, fp);
	fclose(fp); fp=0;

	//�������ļ�
	libCapt::FontFile fontFile;
	if(!fontFile.loadFromDataStream(streamBuf, fileSize))
	{
		printf("Error, Load font file error!\n");
		delete[] streamBuf;
		return 0;
	}
	delete[] streamBuf; streamBuf=0;
	
	//����һ��ͼ���������
	srand((unsigned int)time(0));
	libCapt::Question question;
	libCapt::Generator generator(&fontFile);
	generator.generateQuestion(question);
		
	
	//------------------------------------------------------------


	//��ͼ�����ݽ�ѹ��ת����ͼƬ
	unsigned char bitBuf[libCapt::Question::IMAGE_BUF_LENGTH];
	if(question.isCompressed())
	{
		unsigned int rleSize = question.getSize();
		unsigned int bufSize = libCapt::Question::IMAGE_BUF_LENGTH;
		rleDecompress(question.imageBuf, rleSize, libCapt::Question::IMAGE_WIDTH, libCapt::Question::IMAGE_HEIGHT, (unsigned char*)bitBuf, bufSize); 
	}
	else
	{
		memcpy(bitBuf, question.imageBuf, libCapt::Question::IMAGE_BUF_LENGTH);
	}

	//����PBMͼ��4bit���
	fp = fopen(argv[2], "wb");
	if(fp==0)
	{
		printf("Open output file %s error!\n", argv[2]);
		return 1;
	}

	TGA_HEAD head;
	memset(&head, 0, sizeof(head));
	head.ImageType = 2;
	head.Width = libCapt::Question::IMAGE_WIDTH;
	head.Height = libCapt::Question::IMAGE_HEIGHT;
	head.Bits = 24;
	fwrite(&head, sizeof(head), 1, fp);

	for(int y=0; y<libCapt::Question::IMAGE_HEIGHT; y++)
	{
		for(int x=0; x<libCapt::Question::IMAGE_WIDTH; x++)
		{
			unsigned char c = bitBuf[(libCapt::Question::IMAGE_HEIGHT-1-y)*libCapt::Question::IMAGE_PITCH + (x>>1)];
			c = (x&1) ? ((c&0xf)<<4) : (c&0xf0);
			unsigned char pixel[4]={c,c,c,c};
			fwrite(pixel, 1, 3, fp);
		}
		
	}
	fclose(fp); fp=0;
	
	return 0;
}
