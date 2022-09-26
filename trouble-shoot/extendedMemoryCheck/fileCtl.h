#ifndef FILECTL_H_
#define FILECTL_H_

#include "core.h"

/**
 * @brief 외장 메모리는 SD와 MSD 두 종류를 사용합니다.
 * 
 */
#define DEV_TYPE_SD         1
#define DEV_TYPE_MSD        2

#define DIR_ERROR_MKDIR     "/root"

typedef struct str_fileName_t
{
    unsigned char *name;
    int           len;
} FILENAME;


typedef struct dir_t
{
    DIR                 *dir;
    struct dirent       *de;
    struct stat         info;

} DIRINFO;

typedef struct  file_t
{
    int                 fd;
    FILENAME            name;
    struct stat         fileInfo;

    off_t               sysOffset;
    off_t               offset;
    
} FILEINFO;


int extenedMemoryCheckbyAccessDir( FILENAME* dir );
int extenedMemoryCheckTimer( int minInverVal, FILENAME* file );
int readFile(FILEINFO *file, unsigned char *buf, int  size, off_t offset );
int writeFile(FILEINFO *file, unsigned char *buf, int  size, off_t offset );


#endif
