#ifndef __FONT_H__
#define __FONT_H__	 

//常用ASCII表
//偏移量32 
//ASCII字符集: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
//PC2LCD2002取模方式设置：阴码+逐列式+顺向+C51格式
//总共：2个字符集（16*16和24*24），用户可以自行新增其他分辨率的字符集。
//每个字符所占用的字节数为:(size/8+((size%8)?1:0))*(size/2),其中size:是字库生成时的点阵大小(12/16/24...)
//1608 ASCII字符集点阵
//extern const unsigned char asc2_1608[95][16]; 
//2412 ASICII字符集点阵
extern const unsigned char asc2_2412[30][36];
//3216 ASICII字符集点阵
extern const unsigned char asc2_3216[20][64];
//4824 数码管字体ASICII字符集点阵
extern const unsigned char asc2_4824[11][144];

/* 中文字符 */
extern const unsigned char pandora_ch[];

extern const unsigned char asc2_9648[11][576];

extern const unsigned char mode_4848[14][288];
extern const unsigned char tft_asc2_2412[95][36];	  

#endif

