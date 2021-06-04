#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

char* atbash(char name[]) {
    for (unsigned int i=0;i<strlen(name);i++) {
        if(name[i]>='A' && name[i]<='Z') {
            name[i] = 'Z' - name[i] + 'A';
        }
        if(name[i]>='a' && name[i]<='z') {
            name[i] = 'z' - name[i] + 'a';
        }
    }
    return name;
}

char* rot13(char name[]) {
    for (unsigned int i = 0; i < strlen(name); i++)
    {
        if(name[i]>='A' && name[i]<='Z') {
            name[i] += 13;
            if (name[i]>'Z') {
                name[i]-=26;
            }
        }
        if(name[i]>='a' && name[i]<='z') {
            if (name[i] + 13 >'z') {
                name[i] -= 26;
            }
            name[i] += 13;
        }
    }
    return name;
}

char* rot13decrypt(char name[]) {
    for (unsigned int i = 0; i < strlen(name); i++)
    {
        if(name[i]>='A' && name[i]<='Z') {
            name[i] -= 13;
            if (name[i] < 'A') {
                name[i]+=26;
            }
        }
        if(name[i]>='a' && name[i]<='z') {
            if (name[i] - 13 < 'a') {
                name[i] += 26;
            }
            name[i] -= 13;
        }
    }
    return name;
}

long long int binarytodec (long long int biner) {
	long long int  binary_num, decimal_num = 0, base = 1, rem;  
	while ( biner > 0)  
    {  
        rem = biner % 10; 
        decimal_num = decimal_num + rem * base;  
        biner = biner / 10; 
        base = base * 2;  
    } 
    return decimal_num;
}

long long int casebinary(char name[]) {
    unsigned int len = strlen(name);
    char binary[len];
    for (unsigned int i = 0; i < strlen(name); i++)
    {
        if(name[i]>='A' && name[i]<='Z') {
            strcat(binary,"1");
        }
        else {
            strcat(binary,"0");
        }
    }
    long long int biner = atoi(binary);
    return binarytodec(biner);
}

char* lowercase(char name[]) {
    for (unsigned int i = 0; i < strlen(name); i++)
    {
        name[i] = tolower(name[i]);
    }
    return name;
}

char* vignere(char name[]){
    char key[5] = "SISOP";
    int Kcount = 0;
    for (unsigned int i=0;i<strlen(name);i++) {
        if(name[i]>='A' && name[i]<='Z') {
            name[i] += key[Kcount] - 'A';
            if (name[i] > 'Z') {
                name[i]-=26;
            }
        }
        if(name[i]>='a' && name[i]<='z') {
            if (name[i] + key[Kcount] - 'A' > 'z') {
                name[i] -= 26;
            }
            name[i] += key[Kcount] - 'A';
        }
        Kcount++;
        if(Kcount == 5) {
            Kcount=0;
        }
    }
    return name;
}

char* vigneredecrypt(char name[]){
    char key[5] = "SISOP";
    int Kcount = 0;
    for (unsigned int i=0;i<strlen(name);i++) {
        if(name[i]>='A' && name[i]<='Z') {
            name[i] -= key[Kcount] - 'A';
            if (name[i] < 'A') {
                name[i]+=26;
            }
        }
        if(name[i]>='a' && name[i]<='z') {
            if (name[i] - key[Kcount] + 'A' < 'a') {
                name[i] += 26;
            }
            name[i] -= key[Kcount] - 'A';
        }
        Kcount++;
        if(Kcount == 5) {
            Kcount=0;
        }
    }
    return name;
}

void rx(char name[]) {
    char name2[100];
    char *res;
	res = rot13(name);
	strcpy(name2,res);
    printf("%s\n",name2);
}

void atoz(char name[]){
	//pass
} 

void aisa(char name[]){
	//pass
} 

void checkName(char name[]){
	if (strstr(name,"RX_")!=NULL){
        char* ptr = strstr( name, "RX_" );
        ptr+=3;
        rx(ptr);
    }
    else if (strstr(name,"A_is_a_")!=NULL){
        char* ptr = strstr( name, "AtoZ_" );
        ptr+=5;
        atoz(ptr);
    }
     else if (strstr(name,"A_is_a_")!=NULL){
        char* ptr = strstr( name, "A_is_a_" );
        ptr+=7;
        aisa(ptr);
    }
} 

int main () {
    char name[100];
    char newname[100];
    scanf("%s",name);
    printf("%s\n",vignere(name));
    printf("%s\n",vigneredecrypt(name));
    printf("%s\n",atbash(name));
    printf("%s\n",atbash(name));
    printf("%s\n",rot13(name));
    printf("%s\n",rot13decrypt(name));
    printf("%d\n",casebinary(name));
    printf("%s\n",lowercase(name));
}
