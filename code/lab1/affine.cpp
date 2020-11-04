#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//加密
int encrypt(char *text,char *result,int a,int b)
{
    int i,z=0;
    int l = strlen(text); //长度
    for(i=0;i<l;i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z'){
            result[z]=(a*(text[i]-'A')+b)%26+'A';
        } 
        else if (text[i] >= 'a' && text[i] <= 'z'){
            result[z]=(a*(text[i]-'a')+b)%26+'a';
        } //大小写
        else{  
            result[z] = text[i];//空格
        }
        z++;
    }
    return 0;
}

//解密
int decrypt(char *text,char *result,int k3,int b)
{
    int i,z=0;
    int l = strlen(text); //长度
    for(i=0;i<l;i++)
    {
        //大小写
        if (text[i] >= 'A' && text[i] <= 'Z'){
            result[z]=(k3*((text[i]-'A')-b))%26+'A';
            if(k3*((text[i]-'A')-b) < 0){
                result[z] = result[z] + 26;
            }
        } else if (text[i] >= 'a' && text[i] <= 'z'){
            result[z]=(k3*((text[i]-'a')-b))%26+'a';
            if(k3*((text[i]-'a')-b) < 0){  //负数
                result[z] = result[z] + 26;
            }
        } else {  //空格
            result[z] = text[i];
        }
        z++;
    }
    return 0;
}

int ex_gcd(int a,int b,int &x,int &y) //扩展欧几里得 
{
    if(b==0)
    {
        x=1;
        y=0;
        return a;
    }
    int r=ex_gcd(b,a%b,x,y);
    int t=x;
    x=y;
    y=t-a/b*y;
    return r;
}
int mod_reverse(int a,int b)//求a的模b逆元
{
    int d,x,y;
    d=ex_gcd(a,b,x,y);
    if(d==1)
        return (x%b+b)%b;
    else
        return -1;
}

int main()
{
    int z = 0;
    char text1[50]="";
    char text2[50]="";
    char result[50]="";
    int a,b,k3;
    int type;
    printf("1加密，2解密\n");
    scanf("%d",&type);
    getchar();
    if(type == 1){
        printf("请输入文字\n");
        scanf("%[^\n]",text1);
        printf("输入a\n");
        scanf("%d",&a);
        printf("输入b\n");
        scanf("%d",&b);
        encrypt(text1,result,a,b);
        printf("密文:%s\n",result);
    }else if(type == 2){
        printf("请输入文字\n");
        scanf("%[^\n]",text2);
        printf("输入a\n");
        scanf("%d",&k3);
        a = mod_reverse(k3,26);
        printf("求得a的逆元a^-1为：%d\n",a);
        printf("输入b\n");
        scanf("%d",&b);
        decrypt(text2,result,a,b);
        printf("明文:%s\n",result);
    }
    return 0;
}
