#include <stdio.h>
#include "unistd.h"
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define MAXLEN 1024

//密钥对应的扩展数组
int w[44];
/**
 * S盒
 */
int S[16][16] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
                 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

/**
 * 逆S盒
 */
int S2[16][16] = {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
                  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
                  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
                  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
                  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
                  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
                  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
                  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
                  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
                  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
                  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
                  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
                  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
                  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
                  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
                  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

/**
 * 常量轮值表
 */
int Rcon[10] = {0x01000000, 0x02000000,
                0x04000000, 0x08000000,
                0x10000000, 0x20000000,
                0x40000000, 0x80000000,
                0x1b000000, 0x36000000};
/**
 * 列混合要用到的矩阵
 */
int colM[4][4] = {2, 3, 1, 1,
                  1, 2, 3, 1,
                  1, 1, 2, 3,
                  3, 1, 1, 2};
/**
 * 逆列混合用到的矩阵
 */
int deColM[4][4] = {0xe, 0xb, 0xd, 0x9,
                    0x9, 0xe, 0xb, 0xd,
                    0xd, 0x9, 0xe, 0xb,
                    0xb, 0xd, 0x9, 0xe};
/**
 * 获取整形数据的低8位的左4个位
 */
int getLeft4Bit(int num)
{
    int left = num & 0x000000f0;
    return left >> 4;
}

/**
 * 获取整形数据的低8位的右4个位
 */
int getRight4Bit(int num)
{
    return num & 0x0000000f;
}
/**
 * 根据索引，从S盒中获得元素    S盒的字节代换 
 */
int getNumFromSBox(int index)
{
    int row = getLeft4Bit(index);
    int column = getRight4Bit(index);
    return S[row][column];
}

/**
 * 把一个字符转变成整型
 */
int getIntFromChar(char c)
{
    int result = (int)c;
    return result & 0x000000ff;
}

/**
 * 把16个字符转变成4X4的数组，
 * 该矩阵中字节的排列顺序为从上到下，
 * 从左到右依次排列。
 */
void convertToIntArray(char *str, int pa[4][4])
{
    int k = 0;
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            pa[j][i] = getIntFromChar(str[k]);
            k++;
        }
}

/**
 * 把连续的4个字符合并成一个4字节的整型
 */
int getWordFromStr(char *str)
{
    int one, two, three, four;
    one = getIntFromChar(str[0]);
    one = one << 24;
    two = getIntFromChar(str[1]);
    two = two << 16;
    three = getIntFromChar(str[2]);
    three = three << 8;
    four = getIntFromChar(str[3]);
    return one | two | three | four;
}

/**
 * 把一个4字节的数的第一、二、三、四个字节取出，
 * 入进一个4个元素的整型数组里面。
 */
void splitIntToArray(int num, int array[4])
{
    int one, two, three;
    one = num >> 24;
    array[0] = one & 0x000000ff;
    two = num >> 16;
    array[1] = two & 0x000000ff;
    three = num >> 8;
    array[2] = three & 0x000000ff;
    array[3] = num & 0x000000ff;
}

/**
 * 将数组中的元素循环左移step位
 */
void leftLoop4int(int array[4], int step)
{
    int i;
    int temp[4];
    for (i = 0; i < 4; i++)
        temp[i] = array[i];
    int index;
    if (step % 4 == 0)
    {
        index = 0;
    }
    else
    {
        index = step % 4;
    }
    for (i = 0; i < 4; i++)
    {
        array[i] = temp[index];
        index++;
        index = index % 4;
    }
}

/**
 * 把数组中的第一、二、三和四元素分别作为
 * 4字节整型的第一、二、三和四字节，合并成一个4字节整型
 */
int mergeArrayToInt(int array[4])
{
    int one = array[0] << 24;
    int two = array[1] << 16;
    int three = array[2] << 8;
    int four = array[3];
    return one | two | three | four;
}
/**
 * 密钥扩展中的T函数（字循环，字节代换，异或）
 */
int T(int num, int round)
{
    int numArray[4];
    int i;
    int result;
    splitIntToArray(num, numArray);
    leftLoop4int(numArray, 1); //字循环

    //字节代换
    for (i = 0; i < 4; i++)
        numArray[i] = getNumFromSBox(numArray[i]);

    result = mergeArrayToInt(numArray);
    return result ^ Rcon[round];
}

/**
 * 打印W数组
 */
void printW()
{
    int i, j;
    for (i = 0, j = 1; i < 44; i++, j++)
    {
        printf("w[%d] = 0x%x ", i, w[i]);
        if (j % 4 == 0)
            printf("\n");
    }
    printf("\n");
}

/**
 * 扩展密钥，结果是把w[44]中的每个元素初始化
 */
void extendKey(char *key)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        w[i] = getWordFromStr(key + i * 4);
    }
    //请补充代码完成后面的w[4]-w[43]
    for (i = 4, j = 0; i < 44; i++)
    {
        if (i % 4 == 0)
        {
            w[i] = w[i - 4] ^ T(w[i - 1], j);
            j++;
        }
        else
        {
            w[i] = w[i - 4] ^ w[i - 1];
        }
    }
}

/**
 * 轮密钥加
 */
void addRoundKey(int array[4][4], int round)
{
    int warray[4];
    for (int i = 0; i < 4; i++)
    {
        splitIntToArray(w[round * 4 + i], warray);
        for (int j = 0; j < 4; j++)
        {
            array[j][i] = array[j][i] ^ warray[j];
        }
    }
}

/**
 * 字节代换
 */
void subBytes(int array[4][4])
{
    int i;
    int j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            array[i][j] = getNumFromSBox(array[i][j]);
        }
    }
}

/**
 * 行移位
 */
void shiftRows(int array[4][4])
{
    int temp2[4], temp3[4], temp4[4];
    int i;
    for (i = 0; i < 4; i++)
    {
        temp2[i] = array[1][i];
        temp3[i] = array[2][i];
        temp4[i] = array[3][i];
    }
    leftLoop4int(temp2, 1);
    leftLoop4int(temp3, 2);
    leftLoop4int(temp4, 3);
    for (i = 0; i < 4; i++)
    {
        array[1][i] = temp2[i];
        array[2][i] = temp3[i];
        array[3][i] = temp4[i];
    }
}

int GFMul2(int s)
{
    int result = s << 1;
    int a7 = result & 0x00000100;

    if (a7 != 0)
    {
        result = result & 0x000000ff;
        result = result ^ 0x1b;
    }

    return result;
}

int GFMul3(int s)
{
    return GFMul2(s) ^ s;
}

int GFMul4(int s)
{
    return GFMul2(GFMul2(s));
}

int GFMul8(int s)
{
    return GFMul2(GFMul4(s));
}

int GFMul9(int s)
{
    return GFMul8(s) ^ s;
}

int GFMul11(int s)
{
    return GFMul9(s) ^ GFMul2(s);
}

int GFMul12(int s)
{
    return GFMul8(s) ^ GFMul4(s);
}

int GFMul13(int s)
{
    return GFMul12(s) ^ s;
}

int GFMul14(int s)
{
    return GFMul12(s) ^ GFMul2(s);
}

/**
 * GF上的二元运算
 */
int GFMul(int n, int s)
{
    int result;

    if (n == 1)
        result = s;
    else if (n == 2)
        result = GFMul2(s);
    else if (n == 3)
        result = GFMul3(s);
    else if (n == 0x9)
        result = GFMul9(s);
    else if (n == 0xb) //11
        result = GFMul11(s);
    else if (n == 0xd) //13
        result = GFMul13(s);
    else if (n == 0xe) //14
        result = GFMul14(s);

    return result;
}
/**
 * 列混合
 */
void mixColumns(int array[4][4])
{
    int i;
    int j;
    int tempArray[4][4];

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            tempArray[i][j] = array[i][j];

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            array[i][j] = GFMul(colM[i][0], tempArray[0][j]) ^ GFMul(colM[i][1], tempArray[1][j]) ^ GFMul(colM[i][2], tempArray[2][j]) ^ GFMul(colM[i][3], tempArray[3][j]);
        }
}
/**
 * 把4X4数组转回字符串
 */
void convertArrayToStr(int array[4][4], char *str)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            *str++ = (char)array[j][i];
}
/**
 * 检查密钥长度
 */
int checkKeyLen(int len)
{
    if (len == 16)
        return 1;
    else
        return 0;
}

/**
 * 参数 p: 明文的字符串数组。
 * 参数 plen: 明文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void aes(char *p, int plen, char *key)
{

    int keylen = strlen(key);
    int pArray[4][4];
    int k, i;

    if (plen == 0 || plen % 16 != 0)
    {
        printf("明文字符长度必须为16的倍数！\n");
        exit(0);
    }

    if (!checkKeyLen(keylen))
    {
        printf("密钥字符长度错误！长度必须为16。当前长度为%d\n", keylen);
        exit(0);
    }

    extendKey(key); //扩展密钥
    printW();
    for (k = 0; k < plen; k += 16)
    {
        convertToIntArray(p + k, pArray);

        addRoundKey(pArray, 0); //一开始的轮密钥加

        for (i = 1; i < 10; i++)
        {

            subBytes(pArray); //字节代换

            shiftRows(pArray); //行移位

            mixColumns(pArray); //列混合

            addRoundKey(pArray, i);
        }

        subBytes(pArray); //字节代换

        shiftRows(pArray); //行移位

        addRoundKey(pArray, 10);

        convertArrayToStr(pArray, p + k);
    }
}
/**
 * 根据索引从逆S盒中获取值
 */
int getNumFromS1Box(int index)
{
    int row = getLeft4Bit(index);
    int column = getRight4Bit(index);
    return S2[row][column];
}
/**
 * 逆字节变换
 */
void deSubBytes(int array[4][4])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            array[i][j] = getNumFromS1Box(array[i][j]);
}
/**
 * 把4个元素的数组循环右移step位
 */
void rightLoop4int(int array[4], int step)
{
    int temp[4];
    for (int i = 0; i < 4; i++)
        temp[i] = array[i];

    int index = step % 4 == 0 ? 0 : step % 4;
    index = 3 - index;
    for (int i = 3; i >= 0; i--)
    {
        array[i] = temp[index];
        index--;
        index = index == -1 ? 3 : index;
    }
}

/**
 * 逆行移位
 */
void deShiftRows(int array[4][4])
{
    int i;
    int rowTwo[4], rowThree[4], rowFour[4];
    for (i = 0; i < 4; i++)
    {
        rowTwo[i] = array[1][i];
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }

    rightLoop4int(rowTwo, 1);
    rightLoop4int(rowThree, 2);
    rightLoop4int(rowFour, 3);

    for (i = 0; i < 4; i++)
    {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}
/**
 * 逆列混合
 */
void deMixColumns(int array[4][4])
{
    int i;
    int j;
    int tempArray[4][4];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            tempArray[i][j] = array[i][j];

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            array[i][j] = GFMul(deColM[i][0], tempArray[0][j]) ^ GFMul(deColM[i][1], tempArray[1][j]) ^ GFMul(deColM[i][2], tempArray[2][j]) ^ GFMul(deColM[i][3], tempArray[3][j]);
        }
}
/**
 * 把两个4X4数组进行异或
 */
void addRoundTowArray(int aArray[4][4], int bArray[4][4])
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            aArray[i][j] = aArray[i][j] ^ bArray[i][j];
}
/**
 * 从4个32位的密钥字中获得4X4数组，
 * 用于进行逆列混合
 */
void getArrayFrom4W(int i, int array[4][4])
{
    int index, j;
    int colOne[4], colTwo[4], colThree[4], colFour[4];
    index = i * 4;
    splitIntToArray(w[index], colOne);
    splitIntToArray(w[index + 1], colTwo);
    splitIntToArray(w[index + 2], colThree);
    splitIntToArray(w[index + 3], colFour);

    for (j = 0; j < 4; j++)
    {
        array[j][0] = colOne[j];
        array[j][1] = colTwo[j];
        array[j][2] = colThree[j];
        array[j][3] = colFour[j];
    }
}

/**
 * 参数 c: 密文的字符串数组。
 * 参数 clen: 密文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void deAes(char *c, int clen, char *key)
{
    int i;
    int cArray[4][4];
    int keylen, k;
    keylen = strlen(key);
    if (clen == 0 || clen % 16 != 0)
    {
        printf("密文字符长度必须为16的倍数！现在的长度为%d\n", clen);
        exit(0);
    }

    if (!checkKeyLen(keylen))
    {
        printf("密钥字符长度错误！长度必须为16、24和32。当前长度为%d\n", keylen);
        exit(0);
    }
    extendKey(key); //扩展密钥
    for (k = 0; k < clen; k += 16)
    {
        convertToIntArray(c + k, cArray); //字符转化为字节，并以矩阵形式排列
        addRoundKey(cArray, 10);          //轮密钥加
        int wArray[4][4];
        for (i = 9; i >= 1; i--)
        {
            deSubBytes(cArray);   //逆字节替换
            deShiftRows(cArray);  //逆行移位
            deMixColumns(cArray); //逆列混合
            getArrayFrom4W(i, wArray);
            deMixColumns(wArray);             //逆列混合
            addRoundTowArray(cArray, wArray); //逆轮密钥加
        }
        deSubBytes(cArray);     //逆字节替换
        deShiftRows(cArray);    //逆行移位
        addRoundKey(cArray, 0); //逆密钥加
        convertArrayToStr(cArray, c + k);
    }
}
/**
*读取字符串
*/
void getString(char *str, int len)
{
    int i;
    int slen = read(0, str, len);
    for (i = 0; i < slen; i++, str++)
    {
        if (*str == '\n')
        {
            *str = '\0';
            break;
        }
    }
}
/**
*打印ASCCI码
*/
void printASCII(char *str, int len)
{
    int i;
    int c;
    for (i = 0; i < len; i++)
    {
        c = (int)*str++;
        c = c & 0x000000ff;
        printf("0x%x ", c);
    }
    printf("\n");
}

/**
 * 从标准输入中读取用户输入的字符串
 */
void readPlainText(char *str, int *len)
{
    int plen;
    while (1)
    {
        getString(str, MAXLEN);
        plen = strlen(str);
        if (plen != 0 && plen % 16 == 0)
        {
            printf("你输入的明文为：%s\n", str);
            break;
        }
        else
        {
            printf("明文字符长度必须为16的倍数,现在的长度为%d\n", plen);
        }
    }
    *len = plen;
}
/**
 * 把字符串写进文件
 */
void writeStrToFile(char *str, int len, char *fileName)
{
    int i;
    FILE *fp;
    fp = fopen(fileName, "wb");
    for (i = 0; i < len; i++)
        putc(str[i], fp);
    fclose(fp);
}

/**
*加密后的密文字符写入文件
*/
void aesStrToFile(char *key)
{

    char p[MAXLEN];
    int plen;

    printf("请输入你的明文，明文字符长度必须为16的倍数\n");
    readPlainText(p, &plen);

    printf("轮密钥..................\n");
    aes(p, plen, key); //AES加密
    printf("进行AES加密..................\n");
    printf("加密完后的密文的ASCII为：\n");
    printASCII(p, plen);
    char fileName[64];
    printf("请输入你想要写进的文件名，比如'test.txt':\n");
    if (scanf("%s", fileName) == 1)
    {
        writeStrToFile(p, plen, fileName);
        printf("已经将密文写进%s中了,可以在运行该程序的当前目录中找到它。\n", fileName);
    }
}
/**
 * 从文件中读取字符串
 */
int readStrFromFile(char *fileName, char *str)
{
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL)
    {
        printf("打开文件出错，请确认文件存在当前目录下！\n");
        exit(0);
    }

    int i;
    for (i = 0; i < MAXLEN && (str[i] = getc(fp)) != EOF; i++)
        ;

    if (i >= MAXLEN)
    {
        printf("解密文件过大！\n");
        exit(0);
    }

    str[i] = '\0';
    fclose(fp);
    return i;
}

void deAesFile(char *key)
{
    char fileName[64];
    char c[MAXLEN]; //密文字符串
    printf("请输入要解密的文件名，该文件必须和本程序在同一个目录\n");
    if (scanf("%s", fileName) == 1)
    {
        int clen = readStrFromFile(fileName, c);
        printf("开始解密.........\n");
        deAes(c, clen, key);
        printf("解密后的明文ASCII为：\n");
        printASCII(c, clen);
        printf("明文为：%s\n", c);
        writeStrToFile(c, clen, fileName);
        printf("现在可以打开%s来查看解密后的密文了！\n", fileName);
    }
}

int main(int argc, char const *argv[])
{

    char key[17];
    int klen;

    int cos = 0;
    printf("************************$声明信息$****************************\n");
    printf("版权声明：未经授权，禁止传播、使用和用于商业用途\n");
    printf("使用说明：本程序是AES密码演示程序。\n");
    printf("**********************$声明信息$******************************\n");
    printf("================AES密码算法程序演示================\n\n");

    while (1)
    {
        printf("请输入16个字符的密钥：\n");
        getString(key, 17);
        klen = strlen(key);
        if (klen != 16)
        {
            printf("请输入16个字符的密钥,当前密钥的长度为%d\n", klen);
        }
        else
        {
            printf("你输入的密钥为：%s\n", key);
            break;
        }
    }

    aesStrToFile(key);
    printf("是否开始解密,1解密，2退出\n");
    scanf("%d", &cos);
    if (cos == 1)
    {
        deAesFile(key);
    }
    else
    {
        return 0;
    }
    system("pause");

    return 0;
}
