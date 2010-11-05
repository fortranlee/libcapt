#include "stdafx.h"
#include "captUtil_FontFile.h"


//--------------------------------------------------------------------------
FontFilePlus::FontFilePlus()
{
}

//--------------------------------------------------------------------------
FontFilePlus::~FontFilePlus()
{
}

//--------------------------------------------------------------------------
bool FontFilePlus::createEmptyFile(unsigned int codeCounts)
{
	if(codeCounts<=0 || codeCounts>=0xFFFF) return false;

	//�ͷž�����
	release();

	//��ʼ������
	m_fileHead.dwMagicCode = MAGIC_CODE;
	m_fileHead.nCodeCounts = codeCounts;

	//����Code�ռ�
	m_pCodeBuf = new Code[codeCounts];
	memset(m_pCodeBuf, 0, sizeof(Code)*codeCounts);
	m_nCodeTailOffset = 0;

	//Ԥ����1k�ڴ�
	m_fileHead.nGlyphSize = 1024;
	m_pGlyphBuf = new unsigned char[m_fileHead.nGlyphSize];
	memset(m_pGlyphBuf, 0, m_fileHead.nGlyphSize);
	m_nGlyphTailOffset = GLYPHBUF_EMPTY_SIZE; //�ڴ�ǰ4���ֽڲ�ʹ��,����ͼ������ƫ�Ƴ���0

	return true;
}

//--------------------------------------------------------------------------
void FontFilePlus::_increseGlyphBuf(unsigned int increseSize)
{
	//ʣ���ڴ��Ƿ��㹻
	unsigned int glyphSize = m_fileHead.nGlyphSize;
	if(glyphSize!=0 && m_nGlyphTailOffset+increseSize<glyphSize) return;

	//�����ڴ�ӱ���ʽ�����µ��ڴ�
	unsigned int nNewLength = glyphSize*2;
	while(nNewLength<m_nGlyphTailOffset+increseSize) nNewLength*=2;
	unsigned char* pGlyphBuf = new unsigned char[nNewLength];

	//ָ�����ݾ�̬��
	_pointSerial();

	//��������
	memset(pGlyphBuf, 0, nNewLength);
	if(glyphSize > 0) memcpy(pGlyphBuf, m_pGlyphBuf, glyphSize);

	//�ͷž��ڴ�
	if(m_pGlyphBuf) delete[] m_pGlyphBuf;
	m_pGlyphBuf = pGlyphBuf;
	m_fileHead.nGlyphSize = nNewLength;

	//ָ����������ʱ��
	_pointRuntime();
}


//--------------------------------------------------------------------------
void FontFilePlus::_pointSerial(void)
{
	for(unsigned int i=0; i<m_fileHead.nCodeCounts; i++)
	{
		Code& code = m_pCodeBuf[i];
		if(code.wCode==0 || code.nGlyphCounts==0) continue;  //��δʹ��

		CodeGlyph* codeGlyph = code.pFirst;
		while(codeGlyph->pNext !=0)
		{
			CodeGlyph* next = codeGlyph->pNext;
			codeGlyph->nNextOffset = (unsigned long long)((unsigned char*)next - m_pGlyphBuf);
			codeGlyph = next;
		}

		code.nFirstOffset = (unsigned long long)((unsigned char*)code.pFirst - m_pGlyphBuf);
	}
}

//--------------------------------------------------------------------------
bool FontFilePlus::insertGlyph_RGB(unsigned short code, const unsigned char* pCanvasBitbuf, int canvasSize, unsigned int lPitch)
{
	//����Code��Ϣ
	Code* codeInfo = 0;
	for(unsigned int i=0; i<m_nCodeTailOffset; i++)
	{
		if(m_pCodeBuf[i].wCode==code) 
		{
			codeInfo = m_pCodeBuf+i;
			break;
		}
	}
	if(codeInfo==0 && m_nCodeTailOffset<m_fileHead.nCodeCounts)
	{
		codeInfo = m_pCodeBuf+(m_nCodeTailOffset++);
		codeInfo->wCode = code;
	}
	if(codeInfo==0)
	{
		//code�Ѿ�����������
		return false;
	}

	//����ͼ����Ϣ��ʱ�ڴ�
	char tempMemory[1024*2]={0};
	int tempMemSize = 0;

	//��������
	for(int i=0; i<canvasSize; i++)
	{
		int blank_size = 0;
		const unsigned char* p = (const unsigned char*)pCanvasBitbuf + lPitch*i;
		for(int j=0; j<canvasSize; j++)
		{
			unsigned char c = *p;
			if(c==0) blank_size++;
			else
			{
				if(blank_size!=0)
				{
					tempMemory[tempMemSize++] = (char)(-blank_size);
					blank_size=0;
				}
				tempMemory[tempMemSize++] = ((unsigned char)(255-c))>>4;
			}
			p+=3;
		}
		tempMemory[tempMemSize++] = GLPYH_EOL;
	}

	//�����ļ�������(��λ���һ���н�����)
	while(tempMemSize>0 && tempMemory[tempMemSize-1]==GLPYH_EOL) tempMemSize--;
	tempMemory[tempMemSize++]=GLPYH_EOF;

	//�ڴ����
	_increseGlyphBuf(tempMemSize+sizeof(CodeGlyph));

	//����
	CodeGlyph codeGlyph;
	memset(&codeGlyph, 0, sizeof(codeGlyph));

	//������������
	if(codeInfo->nGlyphCounts == 0)
	{
		//��һ���ַ�
		codeGlyph.pNext=0;
		codeInfo->pFirst = (CodeGlyph*)(m_pGlyphBuf+m_nGlyphTailOffset);
	}
	else
	{
		//��������
		CodeGlyph* firstNow = codeInfo->pFirst;
		codeGlyph.pNext=firstNow;
		codeInfo->pFirst = (CodeGlyph*)(m_pGlyphBuf+m_nGlyphTailOffset);
	}

	int data_offset = (int)((unsigned char*)&(codeGlyph.data)-(unsigned char*)(&codeGlyph));
	memcpy(m_pGlyphBuf+m_nGlyphTailOffset, &codeGlyph, sizeof(codeGlyph)); m_nGlyphTailOffset+=sizeof(codeGlyph)-data_offset;
	memcpy(m_pGlyphBuf+m_nGlyphTailOffset, tempMemory, tempMemSize); m_nGlyphTailOffset+=tempMemSize;

	//ͼ���������
	codeInfo->nGlyphCounts += 1;
	m_fileHead.nGlyphCounts += 1;

	return true;
}

//--------------------------------------------------------------------------
bool FontFilePlus::saveToFile(const TCHAR* szFileName)
{
	//�޸��ļ�ͷ
	m_fileHead.nCodeOffset = sizeof(m_fileHead);
	m_fileHead.nCodeCounts = m_nCodeTailOffset;
	m_fileHead.nGlyphOffset = m_fileHead.nCodeOffset+sizeof(Code)*m_fileHead.nCodeCounts;
	m_fileHead.nGlyphSize = m_nGlyphTailOffset;
/*
	if(debug)
	{
		FILE* fp = fopen(szFileName, "w");
		fprintf(fp, "codeCounts = %d\n", m_fileHead.nCodeCounts);
		fprintf(fp, "glyphCounts = %d\n", m_fileHead.nGlyphCounts);

		for(unsigned int i=0; i<m_fileHead.nCodeCounts; i++)
		{
			Code& code = m_pCodeBuf[i];
			if(code.wCode==0 || code.nGlyphCounts==0) continue;  //��δʹ��

			fprintf(fp, "-------%d--------\n", code.wCode);
			CodeGlyph* glyph = code.pFirst;
			while(glyph!=0)
			{
				char* data = (char*)(glyph->data);
				for(;*data!=GLPYH_EOF;data++)
				{
					char c = *data;
					if(c<0) 
					{
						if(c == GLPYH_EOL) 
						{ 
							fprintf(fp, "\n");
						}
						else
						{
							for(char i=0; i<-c; i++) fprintf(fp, "_");
						}
					}
					else
					{
						fprintf(fp, "*");
					}
				};
				fprintf(fp, "\n===\n");
				glyph = glyph->pNext;
			}
		}
		fclose(fp);
	}
	else
//*/
	{
		//������ļ�
		FILE* fp = _tfopen(szFileName, _T("wb"));
		if(!fp) return false;

		//ָ�����л�
		_pointSerial();

		//������Ϣ
		fwrite(&m_fileHead, sizeof(m_fileHead), 1, fp);
		fwrite(m_pCodeBuf, sizeof(Code), m_fileHead.nCodeCounts, fp);
		fwrite(m_pGlyphBuf, 1, m_nGlyphTailOffset, fp);
		fclose(fp);

		//�ָ�
		_pointRuntime();
	}

	return true;
}

