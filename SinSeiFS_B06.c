#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

const char *dirpath = "/home/nor/Downloads";

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
    char binary[100];
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

char* rx(char name[]) {
	rot13(name);
	char* ptr = strchr(name,'.');
    if (ptr != NULL){
	    rot13decrypt(ptr);
    }
	return name;
}

char* atoz(char name[]){
	atbash(name);
    char* ptr = strchr(name,'.');
    if (ptr != NULL){
        atbash(ptr);
    }
    return name;
}

char* rxrename(char name[]) {
	atbash(name);
    char* ptr = strchr(name,'.');
    if (ptr != NULL){
        atbash(ptr);
    }
    char ext[20];
    strcpy(ext,ptr);
    if (ptr != NULL){
        vignere(name);
    }
    strcpy(ptr,ext);
	return name;
}

void aisa(char name[]){
	//pass
} 

char* checkName(char filedir[]){
    if (strcmp(filedir,"/") == 0 ) {
        filedir = "/home/nor/Downloads";
    }
	else {
        if (strstr(filedir,"RX_")!=NULL){
            char* ptr = strstr( filedir, "RX_" );
            ptr = strchr(ptr, '/');
            ptr++;
            atoz(ptr);
            return rx(ptr);
        }
        else if (strstr(filedir,"AtoZ_")!=NULL){
            char* ptr = strstr( filedir, "AtoZ_" );
            ptr = strchr(ptr, '/');
            ptr++;
            return atoz(ptr);
        }
        else if (strstr(filedir,"A_is_a_")!=NULL){
            char* ptr = strstr( filedir, "A_is_a_" );
            ptr = strchr(ptr, '/');
            ptr++;
            aisa(ptr);
        }
        else if (strstr(filedir,"abc")!=NULL){
            char* ptr = strstr( filedir, "abc" );
            ptr = strchr(ptr, '/');
            ptr++;
            rxrename(ptr);
        }
    }
    return filedir;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char path, mode_t mode)
{
    char ptr = strchr(path, '/') ;
    if (strstr(ptr, "/AtoZ_")) {
        char fullpath[1024] = {0};
        sprintf(fullpath, "%s%s", dirpath, path) ;
        // tbd log
    }
    // char array[1024] = {0};
    // strcpy(array,path)
    checkName(path); 
    int res;
 
    res = mkdir(path, mode);
    if (res == -1)
        return -errno;
 
    return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    checkName(path);
 
    int res;
    res = lstat(path, stbuf);
    if (res == -1)
        return -errno;
 
    return 0;
}



int algo = 0; 
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi)
{
    checkName(path);
    int res;
    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(path);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            res = (filler(buf, de->d_name, &st, 0)) ;
        }
        if (algo == 1) {
            atoz(de->d_name);
            res = (filler(buf, de->d_name, &st, 0));
        }
        else 
            res = (filler(buf, de->d_name, &st, 0));
        if(res!=0) break;
    }

    closedir(dp);
    return 0;
}



static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi)
{
    checkName(path);

    int fd;
    int res;
 
    (void) fi;
    fd = open(path, O_RDONLY);
    if (fd == -1)
        return -errno;
 
    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
 
    close(fd);
    return res;
}



static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .rename = xmp_rename,
};



int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
