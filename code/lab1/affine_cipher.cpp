#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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


int main()
{
    char text[50]="";
	char result[50]="";
    printf("请输入文字\n");
    scanf("%[^\n]",text);
	int mul[12] = {1,3,5,7,9,11,15,17,19,21,23,25};
	for(int i = 0 ; i < 10 ; i++){//枚举加法密钥 
		for(int j = 0 ; j < 12; j++){ //枚举乘法密钥 
			decrypt(text,result,mul[j],i);
			printf("破解:%s\n",result);
		}
	}

    return 0;
}
