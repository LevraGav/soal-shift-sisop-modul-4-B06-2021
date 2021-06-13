# soal-shift-sisop-modul-3-B06-2021
Laporan Penyelesaian Pengerjaan Soal Praktikum SISOP Modul 4 - Kelompok B06

# Anggota Kelompok
Anggota  | NRP
---------|-------
Faisal Reza Maulana | 05111940000009
Arvel Gavrilla R. | 05111940000040
Bayu Adjie Sidharta | 05111940000172

## Link-link
- [Soal](https://docs.google.com/document/d/1KlAG2thAMm42so8BZMylw216EETVGu_DAM5PfwJzl5U/edit)
- [Jawaban Soal](https://github.com/LevraGav/soal-shift-sisop-modul-4-B06-2021/blob/main/SinSeiFS_B06.c)

Kendala:
- Belum memahami materi modul 4 terutama fuse dengan baik
- Salah satu anggota kami memiliki kesulitan dalam penginstalan fuse, diduga berasal dari ubuntunya

# --- No 1 ---
Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei :

### Note
```
Semua file yang berada pada direktori harus ter-encode menggunakan Atbash cipher(mirror).
Misalkan terdapat file bernama kucinglucu123.jpg pada direktori DATA_PENTING
“AtoZ_folder/DATA_PENTING/kucinglucu123.jpg” → “AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg”
Note : filesystem berfungsi normal layaknya linux pada umumnya, Mount source (root) filesystem adalah directory /home/[USER]/Downloads, dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di-encode.
Referensi : https://www.dcode.fr/atbash-cipher
```

## 1A
Jika sebuah direktori dibuat dengan awalan ```AtoZ_```, maka direktori tersebut akan menjadi direktori ter-encode.

## 1B 
Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

## 1C
Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.

## 1D
Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : <b>/home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]</b>

## 1E 
Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

## Pembahasan No 1
Untuk menyelesaikan no 1A, 1B, 1C, dan 1E, kami menggunakan fungsi getDirFile(). Fungsi ini sendiri digunakan untuk melakukan ```encode/decode``` ketika melakukan fuse.
```c
char* getDirFile(char* path)
{
    char fullPath[2048];
    char rpath[2048];
    char tipe[2048];
    char namaFile[2048];
    char namaFolder[2048];
    char buf[2048];
    char temp[2048];
    
    memset(fullPath, 0, sizeof(fullPath));
    memset(rpath, 0, sizeof(rpath));
    memset(namaFile, 0, sizeof(namaFile));
    memset(namaFolder, 0, sizeof(namaFolder));
    memset(tipe, 0, sizeof(tipe));

    int i;
    bool isEncrypted = false;

    char *ketemu;
    if (strcmp(path, "/"))
    {
        ketemu = strstr(path, en1);
        if(ketemu)
	{
            isEncrypted = true;
            flagGlobal = 1;
            ketemu++;
        }
    }

    if(!strcmp(path, "/"))
    {
        path = dirpath;
        strcpy(fullPath, path);
    }

    else if (isEncrypted)
    {
        strncpy(rpath, path, strlen(path) - strlen(ketemu));

        strcpy(buf, ketemu);
        char *a; 
        char *b = buf;

        i = 0;
        a=strtok_r(b, "/", &b);
        for(;a!=NULL;a=strtok_r(NULL, "/", &b))
	{
            memset(temp, 0, sizeof(temp));

            if (!i)
	    {
                strcpy(temp, a);
                strcat(rpath, temp);
                i = 1;
                continue;
            }

            strcpy(tipe, rpath);
            strcat(tipe, "/");
            strcat(tipe, a);

            int lengthTipe = strlen(tipe);
            int lengthPath = strlen(path);

            if (lengthTipe != lengthPath)
	    {
                strcpy(namaFolder, a);
                int j = 0;
                for(; j<strlen(namaFolder); j++)
		{
                    if(namaFolder[j] >= 'A' && namaFolder[j] <= 'Z')
		    {
                        namaFolder[j] = 'Z' + 'A' - namaFolder[j];
                    }

                    else if (namaFolder[j] >= 'a' && namaFolder[j] <= 'z')
		    {
                        namaFolder[j] = 'z' + 'a' - namaFolder[j];
                    }
                }
                strcat(rpath, namaFolder);
            }

            else if(lengthTipe == lengthPath)
	    {
                char Folder[2048];
                strcpy(Folder, dirpath);
                strcat(rpath, "/");
                strcat(Folder, rpath);
                strcat(Folder, a);
                DIR *dp = opendir(Folder);

                if(dp){
                    strcpy(namaFolder, a);
                    int j = 0;
                    for(; j<strlen(namaFolder); j++)
		    {
                        if(namaFolder[j] >= 'A' && namaFolder[j] <= 'Z')
			{
                            namaFolder[j] = 'Z' + 'A' - namaFolder[j];
                        }

                        else if (namaFolder[j] >= 'a' && namaFolder[j] <= 'z')
			{
                            namaFolder[j] = 'z' + 'a' - namaFolder[j];
                        }
                    }
                    strcat(rpath, namaFolder);
                }

                else if(dp == 0)
		{
                    char *titik = strchr(a, '.');

                    if(titik != 0)
		    {
                        int minus2 = strlen(a) - strlen(titik);
                        strncpy(namaFile, a, minus2);

                        int j = 0;
                        for(; j<strlen(namaFile); j++)
			{
                            if(namaFile[j] >= 'A' && namaFile[j] <= 'Z')
			    {
                               namaFile[j] = 'Z' + 'A' - namaFile[j];
                            }

                            else if (namaFile[j] >= 'a' && namaFile[j] <= 'z')
			    {
                                namaFile[j] = 'z' + 'a' - namaFile[j];
                            }
                     }
                     strcat(namaFile, titik) ;
                 }

                    else
		    {
                        strcpy(namaFile, a);
                        int j = 0;
                        for(; j<strlen(namaFile); j++)
			{
                            if(namaFile[j] >= 'A' && namaFile[j] <= 'Z')
			    {
                               namaFile[j] = 'Z' + 'A' - namaFile[j];
                            }

                            else if (namaFile[j] >= 'a' && namaFile[j] <= 'z')
			    {
                                namaFile[j] = 'z' + 'a' - namaFile[j];
                            }
                        }
                    }
		strcat(rpath, namaFile);
                }
            }
        }
        strcpy(fullPath, dirpath);
        strcat(fullPath, rpath);
    }
	
    else
    {
        strcpy(fullPath, dirpath);
        strcat(fullPath, path);

    }
    char* result = fullPath;
    return result;
}
```
### Bagian 1
```c
    char fullPath[2048];
    char rpath[2048];
    char tipe[2048];
    char namaFile[2048];
    char namaFolder[2048];
    char buf[2048];
    char temp[2048];
    
    memset(fullPath, 0, sizeof(fullPath));
    memset(rpath, 0, sizeof(rpath));
    memset(namaFile, 0, sizeof(namaFile));
    memset(namaFolder, 0, sizeof(namaFolder));
    memset(tipe, 0, sizeof(tipe));

    int i;
    bool isEncrypted = false;
```
Bagian ini berfungsi untuk melakukan pengecekan, jika path yang dimiliki terdapat kata ```AtoZ_``` maka isi dari direktori tersebut harus diencode. Untuk melakukan pengecekan dapat dilakukan dengan menggunakan salah satu fungsi string yaitu strstr. Apabila terdapat substring ```AtoZ_```, bool ```isEncrypted``` dan ```flagGlobal``` di-mark terlebih dahulu untuk melakukan encode file pada bagian setelah ini. 

### Bagian 2
```c
    char *ketemu;
    if (strcmp(path, "/"))
    {
        ketemu = strstr(path, en1);
        if(ketemu)
	{
            isEncrypted = true;
            flagGlobal = 1;
            ketemu++;
        }
    }

    if(!strcmp(path, "/"))
    {
        path = dirpath;
        strcpy(fullPath, path);
    }
```
Apabila flag ```isEncrypted``` bernilai true, maka akan dilakukan pengecekan isi dari direktori tersebut. 

### Bagian 3
```c
    else if (isEncrypted)
    {
        strncpy(rpath, path, strlen(path) - strlen(ketemu));

        strcpy(buf, ketemu);
        char *a; 
        char *b = buf;

        i = 0;
        a=strtok_r(b, "/", &b);
        for(;a!=NULL;a=strtok_r(NULL, "/", &b))
	{
            memset(temp, 0, sizeof(temp));

            if (!i)
	    {
                strcpy(temp, a);
                strcat(rpath, temp);

                i = 1;
                
                continue;
            }

            strcpy(tipe, rpath);
            strcat(tipe, "/");
            strcat(tipe, a);
  ```
Ketika isi dari sebuah direktori dilakukan pengecekan, tentu juga perlu dicek apakah yang ada di dalam direktori tersebut adalah folder atau hanya file biasa. Pengecekan isi dari direktori dapat dilakukan dengan mengecek panjang variabel tipe dan path. Jika panjangnya tidak sama berarti hanya file biasa, sehingga dapat langsung dilakukan encode menggunakan Atbash Cipher yang ada di dalam fungsi. Setelah itu maka akan dimasukkan ke dalam ```rpath```. 

### Bagian 4  
```c
int lengthTipe = strlen(tipe);
int lengthPath = strlen(path);

if (lengthTipe != lengthPath)
{
   strcpy(namaFolder, a);
   int j = 0;
   for(; j<strlen(namaFolder); j++)
   {
      if(namaFolder[j] >= 'A' && namaFolder[j] <= 'Z'){
      namaFolder[j] = 'Z' + 'A' - namaFolder[j];
   }

   else if (namaFolder[j] >= 'a' && namaFolder[j] <= 'z')
   {
      namaFolder[j] = 'z' + 'a' - namaFolder[j];
   }
   strcat(rpath, namaFolder);
}
```
Jika panjangnya sama, maka dua kemungkinan yaitu path berupa Folder atau berupa file biasa tanpa ektensi. Oleh karena itu dilakukan pengecekan untuk mengetahui apakah path tersebut dapat dibuka menggunakan perintah ```opendir```. Apabila tidak bisa dibuka dengan menggunakan perintah ```opendir```, berarti path merupakan path file biasa tanpa ekstensi. 

### Bagian 5
```c
 else if(lengthTipe == lengthPath)
	    {
                char Folder[2048];
                strcpy(Folder, dirpath);
                strcat(rpath, "/");
                strcat(Folder, rpath);
                strcat(Folder, a);
                DIR *dp = opendir(Folder);

                if(dp){
                    strcpy(namaFolder, a);
                    int j = 0;
                    for(; j<strlen(namaFolder); j++)
		    {
                        if(namaFolder[j] >= 'A' && namaFolder[j] <= 'Z')
			{
                            namaFolder[j] = 'Z' + 'A' - namaFolder[j];
                        }

                        else if (namaFolder[j] >= 'a' && namaFolder[j] <= 'z')
			{
                            namaFolder[j] = 'z' + 'a' - namaFolder[j];
                        }
                    }
                    strcat(rpath, namaFolder);
                }

                else if(dp == 0)
		{
                    char *titik = strchr(a, '.');

                    if(titik != 0)
		    {
                        int minus2 = strlen(a) - strlen(titik);
                        strncpy(namaFile, a, minus2);

                        int j = 0;
                        for(; j<strlen(namaFile); j++)
			{
                            if(namaFile[j] >= 'A' && namaFile[j] <= 'Z')
			    {
                               namaFile[j] = 'Z' + 'A' - namaFile[j];
                            }

                            else if (namaFile[j] >= 'a' && namaFile[j] <= 'z')
			    {
                                namaFile[j] = 'z' + 'a' - namaFile[j];
                            }
                     }
                     strcat(namaFile, titik) ;
                 }

                    else
		    {
                        strcpy(namaFile, a);
                        int j = 0;
                        for(; j<strlen(namaFile); j++)
			{
                            if(namaFile[j] >= 'A' && namaFile[j] <= 'Z')
			    {
                               namaFile[j] = 'Z' + 'A' - namaFile[j];
                            }

                            else if (namaFile[j] >= 'a' && namaFile[j] <= 'z')
			    {
                                namaFile[j] = 'z' + 'a' - namaFile[j];
                            }
                        }
                    }
		strcat(rpath, namaFile);
                }
            }
        }
        strcpy(fullPath, dirpath);
        strcat(fullPath, rpath);
    }
	
    else
    {
        strcpy(fullPath, dirpath);
        strcat(fullPath, path);

    }
    char* result = fullPath;
    return result;
```
Hasil dari fungsi ini kemudian akan di-return kepada fungsi yang melakukan pemanggilan kepadanya. Fungsi ini juga akan digunakan di beberapa fungsi - fungsi Fuse seperti :
### Getattr
```c
static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    char fullPath[2048];
    memset(fullPath, 0, sizeof(fullPath));

    strcpy(fullPath, getDirFile(path));

    int res;
    res = lstat(fullPath, stbuf);

    if (res == -1) 
    	return -errno;
    return 0;
}
```

### Rename
```c
static int xmp_rename(const char *from, const char *to)
{

    log_rename(from, to);
    char f_from[2048]; 
    char f_to[2048];
    memset(f_from, 0, sizeof(f_from)) ; 
    memset(f_to, 0, sizeof(f_to)) ;
    strcpy(f_from, getDirFile(from)) ;
    strcpy(f_to, getDirFile(to)) ;

    int res;
    res = rename(f_from, f_to);
    if (res == -1)
	return -errno;
	
    return 0;
}
```

### Mkdir
```c
static int xmp_mkdir(const char *path, mode_t mode)
{
    log_mkdir(path);
    char fullPath[2048] ;
    memset(fullPath, 0, sizeof(fullPath)) ;
    strcpy(fullPath, getDirFile(path)) ;
    
	int res;

	res = mkdir(fullPath, mode);
	if (res == -1)
		return -errno;

	return 0;
}
```

### Bagian 6
Untuk menyelesaikan soal 1D (pembuatan log), menggunakan fungsi :
```c
void logRecord(char old_dir[], char new_dir[], int mode) 
{
    FILE* file = fopen("encode.log", "a") ;

    char str[2048] ;
    if (mode == 1) 
    {
        sprintf(str, "%s --> %s", old_dir, new_dir) ;

        fprintf(file, "%s\n", str) ;
    }
    
    else if (mode == 2) 
    {
        sprintf(str, "%s", new_dir) ;
        fprintf(file, "%s\n", str) ;
    }
    fclose(file) ;
}
```

### Output

# --- No 4 ---
Untuk memudahkan dalam memonitor kegiatan pada filesystem mereka Sin dan Sei membuat sebuah log system dengan spesifikasi sebagai berikut.
## 4A 
Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log).

## 4B 
Log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.

## 4C 
Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.

## 4D 
Sisanya, akan dicatat pada level INFO.

## 4E 
Format untuk logging yaitu : 
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]

- Level : Level logging
- dd : 2 digit tanggal
- mm : 2 digit bulan
- yyyy : 4 digit tahun 
- HH : 2 digit jam (format 24 Jam)
- MM : 2 digit menit
- SS : 2 digit detik
- CMD : System Call yang terpanggil
- DESC : informasi dan parameter tambahan

INFO::28052021-10:00:00:CREATE::/test.txt
INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt


## Pembahasan No 4
### Bagian 1
Mendefinisikan path dari file log yang akan diproses sekaligus menetapkan direktori tempat file log berada.
```
static  const  char *logpath = "/home/arvel/SinSeiFS.log"; 
```

### Bagian 2
Membuat fungsi untuk melakukan logging.
```
void mklog(char* level, char* cmd, int desctotal, const char* desc[])
{
    FILE* file = fopen(sysLog, "a");

    time_t now;
    time(&now);

    struct tm* t = localtime(&now);
    fprintf(file, "%s::%s::%02d%02d%04d-%02d:%02d:%02d", level, cmd, t->tm_mda>
    for (int i = 0; i < desctotal; i++)
    {
        fprintf(file, "::%s", desc[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}
```
Fungsi di atas akan dipanggil pada fungsi `mkdir`, `mknode`, `unlink`, `rmdir`, `rename`, dan `write`. Fungsi tersebut akan menuliskan info/warning, timestamp saat operasi dilakukan, beserta keterangannya. Untuk warning akan digunakan saat `unlink` dan `rmdir`, sedangkan yang lainnya merupakan info.

### Output
