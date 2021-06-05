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
int algo = 0; 
FILE * fp;

void *updatelog(char *command){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fp = fopen ("/home/nor/log.txt", "a");
    // fprintf(fp, "punten \n");
    // fprintf(fp, "INFO::28052021-10:00:00:%s:: \n", command);
    fprintf(fp, "INFO::%02d%02d%d-%02d:%02d:%02d:%s:: \n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec, command);
    fclose(fp);
}

char* atbash(char name[]) {
    for (unsigned int i=0;i<strlen(name);i++){
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
	long long int decimal_num = 0, base = 1, rem;  
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
    printf("name1: %s\n",name);
	atbash(name);
    printf("name2: %s\n",name);
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

void checkName(char filedir[]){
    if (strcmp(filedir,"/") == 0 ) {
        filedir = "/home/bayu/Downloads";
    }
	else {
        if (strstr(filedir,"RX_")!=NULL){
            char* ptr = strstr( filedir, "RX_" );
            ptr = strchr(ptr, '/');
            ptr++;
            atoz(ptr);
            rx(ptr);
        }
        else if (strstr(filedir,"AtoZ_")!=NULL){
            char* ptr = strstr( filedir, "AtoZ_" );
            ptr = strchr(ptr, '/');
            if(ptr==NULL) {
                return;
            }
            ptr++;
            atoz(ptr);
        }
        else if (strstr(filedir,"A_is_a_")!=NULL){
            char* ptr = strstr( filedir, "A_is_a_" );
            ptr = strchr(ptr, '/');
            ptr++;
            aisa(ptr);
        }
        // else if (strstr(filedir,"abc")!=NULL){
        //     char* ptr = strstr( filedir, "abc" );
        //     ptr = strchr(ptr, '/');
        //     ptr++;
        //     printf("kONTOL 4\n");
        //     rxrename(ptr);
        //}
    }
}

int rename_dirs_atoz(const char *path)
{
    struct dirent *dp;
    DIR *dfd;


    if ((dfd = opendir(path)) == NULL) {
        fprintf(stderr, "Can't open %s\n", path);
        return 0;
    }
    char filename_qfd[1024] ;
    char new_name_qfd[1024] ;
    while ((dp = readdir(dfd)) != NULL) {
        struct stat stbuf ;
        sprintf( filename_qfd , "%s/%s",path,dp->d_name) ;
        printf("filename_qfd: %s\n",filename_qfd);
        if( stat(filename_qfd,&stbuf ) == -1 ) {
            printf("Unable to stat file: %s\n",filename_qfd) ;
            continue ;
        }
        if ( ( stbuf.st_mode & S_IFMT ) == S_IFDIR ) {
            continue;
            // Skip directories
        }
        else {
            char new_name[1024] = {"0"};
            strcpy(new_name,dp->d_name);
            atoz(new_name);
            sprintf(new_name_qfd,"%s/%s",path,new_name) ;
            printf("From file:%s \t To File:%s\n",filename_qfd,new_name_qfd);
            rename( filename_qfd , new_name_qfd ) ;
        }
    }
    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    unsigned int path_len = strlen(path);  

    /* Open directory */
    dirp = opendir(path);

    if (dirp == NULL)
        return -1;

    while ((direntp = readdir(dirp)) != NULL)
    {
        /* For every directory entry... */
        struct stat fstat;
        char full_name[1024];

        /* Calculate full name, check we are in file length limts */

        strcpy(full_name, path);
        if (full_name[path_len - 1] != '/')
            strcat(full_name, "/");
        strcat(full_name, direntp->d_name);

        /* Ignore special directories. */
        if ((strcmp(direntp->d_name, ".") == 0) ||
            (strcmp(direntp->d_name, "..") == 0))
            continue;

        /* Print only if it is really directory. */
        if (stat(full_name, &fstat) < 0)
            continue;
        if (S_ISDIR(fstat.st_mode))
        {
            printf("FULL NAME: %s\n", full_name);
            char oldname[1024] = {0};
            strcpy(oldname,full_name);
            char * slash = strrchr(full_name, '/');
            printf("Slash: %s\n", slash);
            if(slash==NULL) {
                return 0;
            }
            slash++;
            atoz(slash);
            printf("Rename: %s\n", slash);
            rename(oldname,full_name);
            rename_dirs_atoz(full_name);
        }
    }

    /* Finalize resources. */
    (void)closedir(dirp);
    return 0;
}


static int xmp_rename(const char *from, const char *to)
{
	int res;
    char fpath[1024] = {0};
    char tpath[1024] = {0};
    strcat(fpath,dirpath);
    strcat(fpath, from);
    strcat(tpath,dirpath);
    strcat(tpath, to);
    printf("from: %s\nto: %s\n",fpath,tpath);
    if(strstr(fpath,"AtoZ_")!=NULL | strstr(tpath,"AtoZ_")!=NULL) {
        rename_dirs_atoz(fpath);
        FILE* file = fopen("activity.log", "a");
        char msg[4096] = {0};
        sprintf(msg,"%s -> %s\n",fpath,tpath);
        fputs(msg,file);
        fclose(file);
    }
	res = rename(fpath, tpath);
	if (res == -1)
		return -errno;
    printf("selesai rename\n");
    char cmd[20] = "RENAME"; 
    updatelog(cmd);
	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{

    printf("mulai mkdir\n");
    printf("%s\n", path);
    char fpath[1024] = {0};
    strcat(fpath,dirpath);
    strcat(fpath, path);
    checkName(fpath);
    char* ptr = strchr(fpath, '/');
    if (strstr(ptr, "/AtoZ_")) {
        FILE* file = fopen("activity.log", "a");
        char msg[4096] = {0};
        char before[1024] = {0};
        strcpy(before,fpath);
        char *p = strrchr(before, '/');
        *p = 0;
        sprintf(msg,"%s -> %s\n",before,fpath);
        fputs(msg,file);
        fclose(file);
    }
    int res;
    printf("%s\n",fpath);
    res = mkdir(fpath, mode);
    if (res == -1)
        return -errno;

    printf("selesai mkdir\n");
    char cmd[20] = "MKDIR"; 
    updatelog(cmd);
    return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char fpath[1024] = {0};
    strcat(fpath,dirpath);
    strcat(fpath, path);
    int res;
    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;
    return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi)
{
    char fpath[1024] = {0};
    strcat(fpath,dirpath);
    strcat(fpath, path);
    int res;
    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));
        if(res!=0) break;
    }
    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi)
{

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
