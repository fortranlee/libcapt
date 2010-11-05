#ifndef __LIB_CAPT_RLE_INCLUDE_H__
#define __LIB_CAPT_RLE_INCLUDE_H__

/** �ṩ�򵥵�ͼ��RLEѹ��/��ѹ���㷨,4bit���
*/
namespace libCapt
{

/** ���4bit���ͼ���RLEѹ���㷨
@param imageBuf 4bit��ʽ��ͼ������
@param width ͼ����
@param height ͼ��߶�
@param rleBuf ���ڴ洢ѹ����ͼ��Ļ�����
@param rleBufSize rleBuf���ֽڳ���
@return 
	����ɹ�������true, rleBufSize�洢ѹ����Ĵ�С
	���ʧ��,����false
*/
bool rleCompress(const unsigned char* imageBuf, int width, int height, unsigned char* rleBuf, unsigned int& rleBufSize);

/** ���4bit���ͼ���RLE��ѹ���㷨
@param rleBuf ѹ�����rleͼ������
@param width ͼ����
@param height ͼ��߶�
@param rleBuf ���ڴ洢��ѹ����ͼ��Ļ�����
@param rleBufSize imageBuf���ֽڳ���
@return 
	����ɹ�������true, imageBufSize�洢ѹ����Ĵ�С
	���ʧ��,����false
*/
bool rleDecompress(const unsigned char* rleBuf, unsigned int rleBufSize, int width, int height, unsigned char* imageBuf, unsigned int& imageBufSize);

}

#endif

