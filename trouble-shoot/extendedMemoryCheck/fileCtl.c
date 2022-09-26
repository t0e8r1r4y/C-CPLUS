#include "fileCtl.h"


/**
 * @brief specific directory check by using access function. if read-only system, make dir in root only once.
 * 
 * @param dir 
 * @return int 
 */

int 
extenedMemoryCheckbyAccessDir( FILENAME* dir )
{
    int iRes = 0;
    time_t tm;
    struct tm* nTime;
    unsigned char buff[64] = {0,};

    /**
     * @brief EROFS : Write permission was requested for a file on a read-only file system. ( linux man page )
     *        but, when extended memory is removed at slot , linux kernel still recognize mounting file system until mounting status changed.
     *        so, I also checking extened memory by using file write function.
     * 
     */

    iRes = access(dir->name, (F_OK | R_OK | W_OK) );

    if( errno == EROFS && iRes == -1 )
    {
        time(&tm);
        nTime = localtime(&tm);
        sprintf(buff, "%s/%s_%d%d", DIR_ERROR_MKDIR, dir->name, nTime->tm_mon, nTime->tm_mday);
        mkdir( (const char *)buff );
    }

    return iRes;
}


/**
 * @brief check the specific extended memeory by using file write. if permission of writing file is denied, return -1.
 * 
 * @param minInverVal 
 * @param file 
 * @return int (On Success, return 0. but on Error, -1 is returned. first param is over than 50, return -1 )
 */

int 
extenedMemoryCheckbyWriteFunc( int minInverVal, FILEINFO* file )
{
    int iRes = 0;

    if( minInverVal > 50 ) return 1;

    time_t tm;
    time(&tm);
    struct tm* nTime = localtime(&tm);
    struct tm* bTime = localtime(&file->fileInfo.st_mtime);
    unsigned char buffer[128] = {0, };
    int writeLen = 0;

    if( abs( nTime->tm_min - bTime->tm_min) > minInverVal )
    {
        writeLen = writeFile(file, buffer, writeLen, 0);
        
        if( writeLen < 0 ) 
        {
            iRes = -1;
        }
        else
        {
            stat(file->name.name, &file->fileInfo); // update File Info
        }
    } 

    return iRes;
}


/**
 * @brief file read function.
 * 
 * @param file 
 * @param buf 
 * @param size 
 * @param offset 
 * @return int 
 */
int 
readFile(FILEINFO *file, unsigned char *buf, int size, off_t offset )
{
    int n = 0;

    if(file->sysOffset != offset)
    {
        if(lseek(file->fd, offset, SEEK_SET) == -1)
        {
            return -1;
        }

        file->sysOffset = offset;
    }

    n = read(file->fd, buf, size);

    file->sysOffset += n;
    file->offset    +=n;

    return n;
}



/**
 * @brief file write function
 * 
 * @param file 
 * @param buf 
 * @param size 
 * @param offset 
 * @return int 
 */
int 
writeFile(FILEINFO *file, unsigned char *buf, int  size, off_t offset )
{
    int n, written = 0;

    if(file->sysOffset != offset)
    {
        if(lseek(file->fd, offset, SEEK_SET) == -1)
        {
            return -1;
        }

        file->sysOffset = offset;
    }

    while (1)
    {
        n = write(file->fd, buf + written, size);

        if(n == -1)
        {
            if(errno == EINTR)
            {
                continue;
            }

            return -1;
        }

        file->sysOffset += n;
        file->offset += n;
        written += n;

        if((int) n == size)
        {
            return written;
        }

        size -= n;
    }
    
}
