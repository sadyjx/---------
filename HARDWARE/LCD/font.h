#ifndef __FONT_H__
#define __FONT_H__	 

//����ASCII��
//ƫ����32 
//ASCII�ַ���: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
//PC2LCD2002ȡģ��ʽ���ã�����+����ʽ+˳��+C51��ʽ
//�ܹ���2���ַ�����16*16��24*24�����û������������������ֱ��ʵ��ַ�����
//ÿ���ַ���ռ�õ��ֽ���Ϊ:(size/8+((size%8)?1:0))*(size/2),����size:���ֿ�����ʱ�ĵ����С(12/16/24...)
//1608 ASCII�ַ�������
//extern const unsigned char asc2_1608[95][16]; 
//2412 ASICII�ַ�������
extern const unsigned char asc2_2412[30][36];
//3216 ASICII�ַ�������
extern const unsigned char asc2_3216[20][64];
//4824 ���������ASICII�ַ�������
extern const unsigned char asc2_4824[11][144];

/* �����ַ� */
extern const unsigned char pandora_ch[];

extern const unsigned char asc2_9648[11][576];

extern const unsigned char mode_4848[14][288];
extern const unsigned char tft_asc2_2412[95][36];	  

#endif

