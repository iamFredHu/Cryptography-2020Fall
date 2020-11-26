#include <stdio.h>

long long plainText[100];  //明文
long long cipherText[100]; //密文
int n, e = 0, d;
int length = 0;

//足够大的素数的生成函数
int PrimeNumberCreate(int prime[])
{
    int num = 0, array[5001];
    memset(array, 0, sizeof(array));
    for (int i = 2; i <= 5000; i++)
        if (array[i] != 1)
        {
            prime[num++] = i;
            for (int j = i * i; j <= 5000; j += i)
                array[j] = 1;
        }

    return num;
}

//欧几里得扩展算法
int ExGCD(int m, int n, int &x)
{
    int x1, y1, x0, y0, y;
    x0 = 1;
    y0 = 0;
    x1 = 0;
    y1 = 1;
    x = 0;
    y = 1;
    int r = m % n;
    int q = (m - r) / n;
    while (r)
    {
        x = x0 - q * x1;
        y = y0 - q * y1;
        x0 = x1;
        y0 = y1;
        x1 = x;
        y1 = y;
        m = n;
        n = r;
        r = m % n;
        q = (m - r) / n;
    }
    return n;
}

//密钥生成函数
void KeysCreate()
{
    //取5000内素数保存在prime[]数组中
    int prime[5000];
    int count_Prime = PrimeNumberCreate(prime);

    //随机取两个素数p,q
    srand((unsigned)time(NULL));
    int ranNum1 = rand() % count_Prime;
    int ranNum2 = rand() % count_Prime;
    int p = prime[ranNum1];
    int q = prime[ranNum2];

    n = p * q;

    int Phin = (p - 1) * (q - 1);

    //用欧几里德扩展算法求e,d
    for (int j = 3; j < Phin; j += 1331)
    {
        int gcd = ExGCD(j, Phin, d);
        if (gcd == 1 && d > 0)
        {
            e = j;
            break;
        }
    }
}

//十进制转换为二进制
int DectoBin(int num, int bin_num[])
{

    int i = 0, mod = 0;

    //转换为二进制
    while (num != 0)
    {
        mod = num % 2;
        bin_num[i] = mod;
        num = num / 2;
        i++;
    }

    //返回二进制数的位数
    return i;
}

//蒙哥马利幂模运算
long long Modular_Exponentiation(long long a, int b, int n)
{
    int c = 0, bin_num[1000];
    long long d = 1;
    int k = DectoBin(b, bin_num) - 1;

    for (int i = k; i >= 0; i--)
    {
        c = 2 * c;
        d = (d * d) % n;
        if (bin_num[i] == 1)
        {
            c = c + 1;
            d = (d * a) % n;
        }
    }
    return d;
}

//RSA加密
void RSA_Encrypt()
{
    int i = 0;
    for (i = 0; i < length; i++)
        cipherText[i] = Modular_Exponentiation(plainText[i], e, n);

    printf("Use the public key (e, n) to encrypt:\n");
    for (i = 0; i < length; i++)
        printf("%c", cipherText[i]);
    printf("\n");
}

//RSA解密
void RSA_Decrypt()
{
    int i = 0;
    for (i = 0; i < length; i++)
        cipherText[i] = Modular_Exponentiation(cipherText[i], d, n);

    printf("Use the public key (d, n) to decrypt:\n");
    for (i = 0; i < length; i++)
        printf("%c", cipherText[i]);
    printf("\n");
}