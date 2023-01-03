// File:	writeonceFS.c

// List all group members' names:
// Sanchay Kanade (sk2656)
// Tanya Sharma (tds104)
// iLab machine tested on: ilabs 4

#include "writeonceFS.h"

// used to see if disk is initialized
int init = 0;
struct stat file_stat;
//Creates the disk file for the file system
void createFS(){

        int X = CAPACITY*sizeof(char)-1;
        FILE *fp = fopen("diskfile","w");
        fseek(fp, X, SEEK_SET);
        fputc('\0', fp);
        
        
        fseek(fp, 0, SEEK_SET); 
        fclose(fp);
        printf("Disk File created.......\n");

}

int checkDisk(void* memoryAddress){
    int inode_count=((sb*)memoryAddress)->numberOfinodesFilled;
    int count=0;
    memoryAddress+=1024;
    for(int i=1;i<=50;i++){
        if(((inode*)memoryAddress)->id>0){
            count++;
            printf("Inode id:%d\n",((inode*)memoryAddress)->id);
        }
        else 
        break;
        memoryAddress+=1024;
    }
    if(inode_count==count){
        return 0;
    }
    return -1;
}

//Writing from disk to memory
int wo_mount(char* fileName, void* memoryAddress){
    //Opening disk file
    FILE *f;
    if((f = fopen(fileName, "r+")) == NULL){
			printf("Unable to open file!\n");
            errno = ENOENT;
            fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
			//exit(1);
            return -1;
	}
    else{ 
        printf("Opened file successfully for reading.\n");
        
        
    }

    if(fread(memoryAddress, 4*1024*1024*sizeof(char), 1, f) != 1){
			printf( "Read error!\n" );
			
            return -1;
    }
    else {
        if(checkDisk(memoryAddress)==0){
            printf("Disk file not broken\n");
            printf( "Read was successful.\n" );
        }
        else{
            printf("Disk file broken\n");
            init == 0;
            return -1;
        }
    }

    // if we are opening the disk for the first time we need to set super block and inodes
    if(init ==0){
        //Since we are initializing no inode or data block is filled
        ((sb *)memoryAddress)->numberOfinodesFilled=0;

        //datablocks remaining after 1 sb and 50 inodes
        ((sb *)memoryAddress)->numberOfblocksFilled=0;

        //Assigning 1 block or 1KB for superblock
        memoryAddress += 1024;
        
        //Start storing inodes
        for(int i =1;i<=50;i++){
            ((inode *)memoryAddress)->id = -2;
            ((inode *)memoryAddress)->start_index = 0;
            ((inode *)memoryAddress)->numberOfDataBlocks = 0;
            ((inode *)memoryAddress)->isOpen=0;
            strcpy(((inode *)memoryAddress)->name,"");
            
            //Giving 1kb to each i node will change at last
            memoryAddress += 1024;
        }

        printf("DATA Region starts from %d\n",memoryAddress);
        //Start storing datablocks
        for(int i=0;i<4045;i++){
            
            ((datablock*)memoryAddress)->next=-1;
            memoryAddress +=1024;
        }
        
        //Write this structure to file as well to make it consistent
        //go to the start of the file
        fseek(f, 0, SEEK_SET);
        if( fwrite(memory_rep, CAPACITY*sizeof(char), 1,f) != 1){
			printf("Initial Write error!\n");
			return -1;
            
		}
        else{
            printf("Initial Write was successful.\n");
        }
        
        init = 1;
    }
    fstat(f,&file_stat);
    printf("Size of the file is = %d\n",file_stat.st_size);
    fclose(f);
    return 0;
}

//Writing memory to disk
int wo_unmount(void* memoryAddress){
    
    FILE *f;
    if((f = fopen("diskfile", "r+")) == NULL){
			printf("Unable to open file!\n");
            errno = ENOENT;
            fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
			return -1;
            
	}
    else{ 
        printf("Opened file successfully for writing.\n");
    }
    if( fwrite(memoryAddress, CAPACITY*sizeof(char), 1,f) != 1){
			printf("Write error!\n");
			return -1;
            
		}
    else{
        printf("Write was successful.\n");
    }
		
    fclose(f);
    return 0;
}

/******************************************************
 * wo_create fucntion
*******************************************************/
int wo_create(char* fileName, flags f){
    if(((sb*)memory_rep)->numberOfinodesFilled == 50){
        printf("Inode limit reached. Cannot create a new file. Exiting !!!\n");
        errno = ENFILE;
        fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
        return -1;

    }
    //At the begining of the i node region
    char * cursor = memory_rep+1024;
    int fileDescriptor = -1;

    //Check each inode if the file name exist or not
    for(int i=0;i<50;i++){
        if(strcmp((((inode*)cursor)->name), fileName)==0){
            printf("File already exits with the same filename\n");
            errno = EEXIST;
            fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
            return -1;
        }
        
        cursor += 1024;
    }
    //restting cursor
    cursor = memory_rep+1024;
    //Creating a new file and open it as well
    for(int i=1;i<=50;i++){
        if(((inode*)cursor)->id==-2){
            ((inode*)cursor)->id = i;
            ((inode*)cursor)->isOpen=1;
            strcpy(((inode *)cursor)->name, fileName);
            ((inode*)cursor)->rwFlag = f;
            fileDescriptor = ((inode *)cursor)->id;
            ((sb*)memory_rep)->numberOfinodesFilled++;
            break;
        }
        cursor +=1024;
    }
    return fileDescriptor;


}


/******************************************************
 * wo_open fucntion
*******************************************************/
int wo_open(char* fileName, flags f){
    
    int fileDescriptor = -1;

    //Start cursor after super block
    char * cursor = memory_rep+1024;
        
    //Check each inode if the file name exist or not
    for(int i=0;i<50;i++){
        if(strcmp((((inode*)cursor)->name), fileName)==0){
            printf("File exits !!!\n");
            if((((inode*)cursor)->isOpen)==0){
                ((inode*)cursor)->isOpen = 1;
                ((inode*)cursor)->rwFlag = f;
                printf("File opened !!\n");
                fileDescriptor = ((inode*)cursor)->id;
                return fileDescriptor;
                
            }
            else{
                printf("File already opened!!!\n");
                fileDescriptor = ((inode*)cursor)->id;
                return fileDescriptor;
            }
            
        }
        /*printf("%d\n",i*((inode*)cursor)->id);*/
        cursor += 1024;
    }

    printf("File not found with this name. First create this file.\n");
    errno = ENOENT;
    fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
    return -1;
}

/************************************************************
wo_read
fd: a valid filedscriptor for your filesystem
buffer: a memory location to either read bytes in to or from
bytes: the number of bytes to read or write
**************************************************************/

int wo_read(int fileDescriptor, void* buffer, int Bytes){

    //check if file desciptor is valid
    if(fileDescriptor<0 || fileDescriptor>50){
        printf("No such file exists\n");
        errno = EBADF;
        fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
        return -1;
    }
    

    //start from inodes
    char* cursor=memory_rep+1024;
    int found_flag=0;


    for(int i =1;i<=50;i++){
        if(((inode*)cursor)->id==fileDescriptor){
            if(((inode*)cursor)->isOpen ==1){
                //For both WO_WRONLY and WO_RDWR cases
                if(((inode*)cursor)->rwFlag==0 || ((inode*)cursor)->rwFlag==2){
                    printf("File found and ready for reading\n");
                    found_flag=1;
                    break;
                }
                else{
                    printf("Requested file is not opened in read mode\n");
                    return -1;
                }
            }
            else{
                printf("Requested file is not opened\n");
                return -1;
            }
        }
        cursor +=1024;
    }
    if(found_flag==0){
        printf("Requested file not found\n");
        errno = ENOENT;
        fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
        return -1;
    }
    //To store number of bytes which we will copy to buffer
    char Buffer[Bytes];
    int numberOfBlocksNeededtoBeRead = Bytes/1020;
    int extraCharactersNeededtoBeRead = Bytes%1020;
    //Cursor at file's inode

    if(numberOfBlocksNeededtoBeRead<=((inode*)cursor)->numberOfDataBlocks){
            //start of the first block of file
            char *cursorData = memory_rep + 1024 + 50*1024 + ((inode*)cursor)->start_index;
            for(int i=0;i<numberOfBlocksNeededtoBeRead;i++){
                printf("We are at = %d\n",cursorData);
                
                printf("Address of block %s\n\n",((datablock*)cursorData)->data);
                strcat(Buffer,((datablock*)cursorData)->data); 
                cursorData=((datablock*)cursorData)->next + memory_rep;
                printf("%d th block printed\n",i);
            }
            //cursorData is at the last block
            if(extraCharactersNeededtoBeRead!=0){
                //write whatever is minimum of the bytes remaining in the file or bytes requested
                strncat(Buffer,(((datablock*)cursorData)->data),MIN(((inode*)cursor)->bytesFilled,extraCharactersNeededtoBeRead));
            }


    }
    //If the byte requested are more than in the file then read every block
    else{
        printf("Bytes request to be read are more than int he file. Reading entire file\n");
        char *cursorData = memory_rep + 1024 + 50*1024 + ((inode*)cursor)->start_index;
            for(int i=0;i<(((inode*)cursor)->numberOfDataBlocks);i++){
                printf("We are at = %d\n",cursorData);
                
                strcat(Buffer,((datablock*)cursorData)->data);
                printf("BUffer is = %s\n",Buffer);
                printf("%d th block copied\n",i); 
                cursorData=((datablock*)cursorData)->next + memory_rep;
                
            }
    }
    //Copying our string to buffer
    strcpy(buffer, Buffer);
    printf("String Copied to buffer\n");
    printf("___Printing buffer___\n\n");
    printf("%s\n",buffer);

    return 0;

}

/*********Write once write function*************
************************************************/
int wo_write(int fileDescriptor, void* buffer, int Bytes){
    
    //Check if there is spcae on file system
    if(((sb*)memory_rep)->numberOfblocksFilled==4045){
        errno = ENOSPC;
        fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
        return -1;

    }

    //Check if no of bytes to be writen are well within the remaining capacity if data region
    if((4045-((sb*)memory_rep)->numberOfblocksFilled)*1024 < Bytes){
        printf("Not enough space on the File System for requested allocation");
        errno = ENOSPC;
        fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
        return -1;
    }
    
    int numberOfBlocksNeeded = Bytes/1020;
    int extraCharactersNeeded = Bytes%1020;

    printf("Numbe of blocks needed = %d\n",numberOfBlocksNeeded);
    printf("Numbe of extra bytes needed = %d\n",extraCharactersNeeded);

    //printf("Size of buffer is = %d\n",sizeof(buffer));

    //char Buffer[2*1024*1024];
    char Buffer[Bytes];
    strcpy(Buffer,buffer);

    printf("Size of Buffer is = %d\n",sizeof(Buffer));

    printf("Copied to buffer for writing\n");

    if(fileDescriptor<0 || fileDescriptor>50){
        printf("No such file exist\n");
        errno = EBADF;
        fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
        return -1;
    }

    //starting from inodes
    char * cursor = memory_rep+1024;
    int found_flag=0;

    for(int i =1;i<=50;i++){
        if(((inode*)cursor)->id==fileDescriptor){
            if(((inode*)cursor)->isOpen ==1){
                //For both WO_WRONLY and WO_RDWR cases
                if(((inode*)cursor)->rwFlag==1 || ((inode*)cursor)->rwFlag==2){
                    printf("File found and ready for writing\n");
                    found_flag=1;
                    break;
                }
                else{
                    printf("Requested file is not opened in write mode\n");
                    errno = EROFS;
                    fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
                    
                    return -1;
                }
            }
            else{
                printf("Requested file is not open\n");
                return -1;
            }
        }
        cursor +=1024;
    }
    if(found_flag==0){
        printf("Requested file not found\n");
        errno = ENOENT;
        fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
        return -1;
    }
    //If file is found cursor is at the inode of the file 
    printf("\n_____File info_______\n");
    printf("File descriptor is = %d\n",((inode*)cursor)->id);
    printf("File starting index of data block is = %d\n",((inode*)cursor)->start_index);
    //If writing first data block of file with 0 previous data nodes
    if(((inode*)cursor)->numberOfDataBlocks==0){
        int x = (((sb*)memory_rep)->numberOfblocksFilled);
        ((inode*)cursor)->start_index = x*1024;
        
        // array address + superblock width + inode regin + previously filled datablocks
        printf("File starting index of data block NOW is = %d\n",((inode*)cursor)->start_index);
        int xs = (((inode*)cursor)->start_index);
        //char *cursorData = xs;
        char *cursorData = memory_rep + 1024 + 50*1024 + ((inode*)cursor)->start_index;
        printf("We are at = %d\n", cursorData);
        //If bytes less than or equal to 1020
        //printf("Next is accessible = ",((datablock*)cursorData)->next);
        if(Bytes<=1022){
            printf("Buffer data need to be written is %s\n",Buffer);
            printf("***Case 1 << 1020!!!!***\n");
 
            //Copy data into the data block
            strcpy((((datablock*)cursorData)->data),Buffer);
            
            printf("Copying done\n");
            //Since first block and no other blocks are there nex will be -1
            ((datablock*)cursorData)->next = -1;
            //Bytes filled in the last block of file
            ((inode*)cursor)->bytesFilled = Bytes;
            //Increase number of blocks in a file
            ((inode*)cursor)->numberOfDataBlocks++;
            //Increment total number of blocks
            ((sb*)memory_rep)->numberOfblocksFilled++;
            printf("Number of blocks filled in the file = %d\n", ((inode*)cursor)->numberOfDataBlocks);
            return 0;
        }

        //If bytes are larger than 1020 and multiple blocks needed
        else{
            printf("***Case1 >>1020***\n");
            
            printf("We are now at = %d\n", cursorData);
            int y=0;
            //For number of blocks needed to write data
            for(y=0;y<numberOfBlocksNeeded;y++){
                //copy 1k of buffer in data block
                strncpy((((datablock*)cursorData)->data),Buffer+(y*1020),1020);
                //Modify next and move cursor data ahead
                if(y==numberOfBlocksNeeded-1 && extraCharactersNeeded==0){
                    ((datablock*)cursorData)->next = -1;
                    break;
                }
                else{
                //Modify next and move cursor data ahead
                    ((datablock*)cursorData)->next = cursorData + 1024 - memory_rep;
                    //((datablock*)cursorData)->next = memory_rep+((sb*)memory_rep)->numberOfblocksFilled*1024+1024;
                    printf("Sum is =%d\n",cursorData+1024);
                    printf("Next is %d\n",((datablock*)cursorData)->next);
                    cursorData = cursorData +1024;
                    printf("Cursor Data is = %d\n",cursorData);
                }
                //Modifiying inode of that file
                ((inode*)cursor)->bytesFilled = 1020;
                ((inode*)cursor)->numberOfDataBlocks++;

                //Modifying sb
                ((sb*)memory_rep)->numberOfblocksFilled++;
            }
            if(extraCharactersNeeded!=0){
                ((datablock*)cursorData)->next = cursorData + 1024 - memory_rep;
                //((datablock*)cursorData)->next = memory_rep+((sb*)memory_rep)->numberOfblocksFilled*1024+1024;
                printf("Next is %d\n",((datablock*)cursorData)->next);
                //cursorData = cursorData +1024;
                cursorData = memory_rep+((datablock*)cursorData)->next;
                strncpy((((datablock*)cursorData)->data),Buffer+(y*1020),extraCharactersNeeded);
                
                //Modifiying inode of that file
                ((inode*)cursor)->bytesFilled = extraCharactersNeeded;
                ((inode*)cursor)->numberOfDataBlocks++;

                //Modifying sb
                ((sb*)memory_rep)->numberOfblocksFilled++;

            }
            //printf("printing memory 111\n");
            printf("Number of blocks filled in the file = %d\n", ((inode*)cursor)->numberOfDataBlocks);
            
            cursorData = memory_rep + 1024 + 50*1024 + ((inode*)cursor)->start_index;
            for(int i=0;i<((inode*)cursor)->numberOfDataBlocks;i++){
                printf("We are at = %d\n",cursorData);
                //printf("Next address should be %d\n",((datablock*)cursorData)->next);
                printf("Address of block %s\n\n",((datablock*)cursorData)->data);
                
                cursorData=((datablock*)cursorData)->next + memory_rep;
                printf("%d th block printed\n",i);
            }
            printf("We are at = %d\n",cursorData);
            printf("NExt is = %d",((datablock*)cursorData)->next);

        }
        printf("Write successful in file from buffer\n");
        printf("Number of blocks filled = %d\n", ((sb*)memory_rep)->numberOfblocksFilled);
        printf("Number of blocks filled in the file = %d\n", ((inode*)cursor)->numberOfDataBlocks);
        printf("Number of bytes in the last data block = %d\n", ((inode*)cursor)->bytesFilled);
        printf("Cursor start index is now = %d\n",((inode*)cursor)->start_index);
        return 0;
    }
    /*Opened file already has few datablocks written and last block can be partially filled
    cursor is at the inode of the file*/
    else{   
        //case when last data block has no space left need to create new data block
        //printf("***Case2 Initial Write***\n");
        if((((inode*)cursor)->bytesFilled)==1020){
            printf("***Case2 Initial Write***\n");
            //need to search the last data block of the file
            char *cursorData = memory_rep + 1024 + 50*1024 +((inode*)cursor)->start_index;
            for(int i=0;i<((inode*)cursor)->numberOfDataBlocks-1;i++){
                cursorData=((datablock*)cursorData)->next + memory_rep;
            }
            int x = (((sb*)memory_rep)->numberOfblocksFilled);
            printf("Number of blocks filled in the file initially = %d\n",((inode*)cursor)->numberOfDataBlocks);
            printf("This Number of blocks filled in the file = %d\n", ((sb*)memory_rep)->numberOfblocksFilled);
            //Now we are at new data block
            ((datablock*)cursorData)->next = memory_rep + 1024 + 50*1024 + x*1024-memory_rep;
            cursorData = ((datablock*)cursorData)->next+ memory_rep;
            
            int y=0;
            //For number of blocks needed to write data
            for(y=0;y<numberOfBlocksNeeded;y++){
                //copy 1k of buffer in data block
                printf("Here!!!\n");
                strncpy((((datablock*)cursorData)->data),Buffer+(y*1020),1020);
                //Modify next and move cursor data ahead
                if(y==numberOfBlocksNeeded-1 && extraCharactersNeeded==0){
                    ((datablock*)cursorData)->next = -1;
                    ((inode*)cursor)->bytesFilled = 1020;
                    (((inode*)cursor)->numberOfDataBlocks)++;
                    printf("%d\n",((inode*)cursor)->numberOfDataBlocks);
                    //Modifying sb
                    ((sb*)memory_rep)->numberOfblocksFilled++;
                    break;
                }
                else{
                //Modify next and move cursor data ahead
                    ((datablock*)cursorData)->next = cursorData +1024-memory_rep;
                    cursorData = cursorData +1024;
                }
                //Modifiying inode of that file
                ((inode*)cursor)->bytesFilled = 1020;
                (((inode*)cursor)->numberOfDataBlocks)++;
                printf("%d\n",((inode*)cursor)->numberOfDataBlocks);
                //Modifying sb
                ((sb*)memory_rep)->numberOfblocksFilled++;
            }
            if(extraCharactersNeeded!=0){
                ((datablock*)cursorData)->next = cursorData +1024;
                cursorData = cursorData +1024;
                strncpy((((datablock*)cursorData)->data),Buffer+(y*1020),extraCharactersNeeded);
                
                //Modifiying inode of that file
                ((inode*)cursor)->bytesFilled = extraCharactersNeeded;
                ((inode*)cursor)->numberOfDataBlocks++;

                //Modifying sb
                ((sb*)memory_rep)->numberOfblocksFilled++;

            }
            printf("Number of blocks filled in the file = %d\n", ((inode*)cursor)->numberOfDataBlocks);
            return 0;
        }
        /*Case when last data block is partially filled*/
        
        else{
            printf("***Case2 Writing new data blocks for remaining buffer***\n");
            int remainingCapInLastBlock = 1020 - ((inode*)cursor)->bytesFilled;
            
            //need to search the last data block of the file
            char *cursorData = memory_rep + 1024 + 50*1024 + ((inode*)cursor)->start_index;
            printf("We are at = %d\n",cursorData);
            printf("Next is = %d\n",((datablock*)cursorData)->next + memory_rep);
            for(int i=0;i<((inode*)cursor)->numberOfDataBlocks-1;i++){
                printf("First");
                cursorData = ((datablock*)cursorData)->next+ memory_rep;
                //printf("Address of block %d\n",cursorData);
            }
            
            int x = (((sb*)memory_rep)->numberOfblocksFilled);
            printf("Number of blocks filled in the file = %d\n", ((sb*)memory_rep)->numberOfblocksFilled);
            
            printf("We are Now at = %d\n",cursorData);
            //Now we are at new data block
            ((datablock*)cursorData)->next = memory_rep + 1024 + 50*1024 + x*1024-memory_rep;
            
            cursorData = ((datablock*)cursorData)->next + memory_rep;
            printf("We are Now at = %d\n",cursorData);
            //Concatenate data amounting to remaining capacity of the block from buffer so that
            //this block is fully filled upto 1020
            printf("Data in buffer is  =%s\n",Buffer);
            printf("Size of buffer is = %d\n",sizeof(Buffer));
            printf("Current data in data block is %s\n",((datablock*)cursorData)->data);
            strncat((((datablock*)cursorData)->data),Buffer,Bytes);
            printf("Here\n");
            //Only Modifiying bytes filled of the inode of that file as
            //No new block was added yet
            ((inode*)cursor)->bytesFilled += MIN(remainingCapInLastBlock,Bytes);

            //printf("Stop!!!!\n\n");

            //Bytes remaining in buffer to be written
            int bytesLeftForWriting = Bytes - remainingCapInLastBlock;
            numberOfBlocksNeeded = bytesLeftForWriting/1020;
            extraCharactersNeeded = bytesLeftForWriting%1020;

            //Copying previous logic from here
            
            x = (((sb*)memory_rep)->numberOfblocksFilled);
            ((inode*)cursor)->start_index = x*1024;
            // array address + superblock width + inode regin + previously filled datablocks
            printf("File starting index of data block NOW is = %d\n",((inode*)cursor)->start_index);
            int xs = (((inode*)cursor)->start_index);
            //char *cursorData = xs;
            cursorData = memory_rep + 1024 + 50*1024 + ((inode*)cursor)->start_index;
            printf("We are at = %d\n", cursorData);
            //If bytes less than or equal to 1020
            //printf("Next is accessible = ",((datablock*)cursorData)->next);
            if(bytesLeftForWriting<=1020){
                printf("Buffer data need to be written is %s\n",Buffer+remainingCapInLastBlock);
                
                printf("***Case2 remaing bytes <<1022****\n");
               
                //Copy data into the data block
                strcpy((((datablock*)cursorData)->data),Buffer+remainingCapInLastBlock);
                //strncat((((datablock*)cursorData)->data),Buffer,Bytes);

                printf("Copying done\n");
                //Since first block and no other blocks are there nex will be -1
                ((datablock*)cursorData)->next = -1;
                //Bytes filled in the last block of file
                ((inode*)cursor)->bytesFilled = bytesLeftForWriting;
                //Increase number of blocks in a file
                ((inode*)cursor)->numberOfDataBlocks++;
                //Increment total number of blocks
                ((sb*)memory_rep)->numberOfblocksFilled++;
                printf("Number of blocks filled in the file = %d\n", ((inode*)cursor)->numberOfDataBlocks);
                return 0;
            }

            //If bytes are larger than 1020 and multiple blocks needed
            else{
                printf("***Case2 remaing bytes >>1020****\n");
                //need to search the last data block of the file
                /*char *cursorData = memory_rep + 1024 + 50*1024 +((inode*)cursor)->start_index;
                for(int i=0;i<((inode*)cursor)->numberOfDataBlocks-1;i++){
                 cursorData=((datablock*)cursorData)->next;
                }
                int x = (((sb*)memory_rep)->numberOfblocksFilled);
                //Now we are at new data block
                ((datablock*)cursorData)->next = memory_rep + 1024 + 50*1024 + x*1024;
                cursorData = ((datablock*)cursorData)->next;*/
                printf("We are now at = %d\n", cursorData);
                int y=0;
                //For number of blocks needed to write data
                for(y=0;y<numberOfBlocksNeeded;y++){
                    //copy 1k of buffer in data block
                    strncpy((((datablock*)cursorData)->data),Buffer+remainingCapInLastBlock+(y*1020),1020);
                    //Modify next and move cursor data ahead
                    if(y==numberOfBlocksNeeded-1 && extraCharactersNeeded==0){
                        ((datablock*)cursorData)->next = -1;
                        //Modifiying inode of that file
                        ((inode*)cursor)->bytesFilled = 1020;
                        ((inode*)cursor)->numberOfDataBlocks++;

                        //Modifying sb
                        ((sb*)memory_rep)->numberOfblocksFilled++;
                        break;
                    }
                    else{
                        //Modify next and move cursor data ahead
                        ((datablock*)cursorData)->next = cursorData +1024;
                        cursorData = cursorData +1024;
                    }
                    //Modifiying inode of that file
                    ((inode*)cursor)->bytesFilled = 1020;
                    ((inode*)cursor)->numberOfDataBlocks++;

                    //Modifying sb
                    ((sb*)memory_rep)->numberOfblocksFilled++;
                }
                if(extraCharactersNeeded!=0){
                    ((datablock*)cursorData)->next = cursorData +1024;
                    cursorData = cursorData +1024;
                    strncpy((((datablock*)cursorData)->data),Buffer+remainingCapInLastBlock+(y*1020),extraCharactersNeeded);
                
                    //Modifiying inode of that file
                    ((inode*)cursor)->bytesFilled = extraCharactersNeeded;
                    ((inode*)cursor)->numberOfDataBlocks++;

                    //Modifying sb
                    ((sb*)memory_rep)->numberOfblocksFilled++;

                }


            }
            printf("Write successful in file from buffer\n");
            printf("Number of blocks filled = %d\n", ((sb*)memory_rep)->numberOfblocksFilled);
            printf("Number of blocks filled in the file = %d\n", ((inode*)cursor)->numberOfDataBlocks);
            printf("Number of bytes in the last data block = %d\n", ((inode*)cursor)->bytesFilled);
            return 0;
            //To here

            }
    }
    printf("Number of blocks filled in the file = %d\n", ((inode*)cursor)->numberOfDataBlocks);
    return 0;
}

int wo_close(int fileDescriptor){

    //check if file descriptor is valid
    if(fileDescriptor<0 || fileDescriptor>50){
        printf("Invalid file descriptor");
        errno = EBADF;
        fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
        return -1;
    }

    //check if file descriptor is present in current files
    char * cursor = memory_rep+1024;

    //check if file is present at all, if present we close it
    for(int i=1;i<=50;i++){
        if(((inode*)cursor)->id==fileDescriptor){
            if(((inode*)cursor)->isOpen==1){
                ((inode*)cursor)->isOpen=0;
                printf("File closed!!!\n");
                return 0;
            }
            else{
                printf("Attempted to close already closed file\n");
                return -1;
            }
            //break;
        }
        cursor+=1024;
    }
    printf("File not found for closing\n");
    errno = ENOENT;
    fprintf(stderr,"Error Number :%d %s\n",errno,strerror(errno));
    return -1;
}

void saturatingInodes(){
    //Saturation test Maximum number of files that we can create
    printf("Creating 50 files\n");
    int fileDesc[50];
    char * filename[50]={"a1","a2","a3","a4","a5","a6","a7","a8","a9","a10","a11","a12","a13","a14","a15","a16","a17","a18","a19","a20","a21","a22","a23","a24","a25","a26","a27","a28","a29","a30","a31","a32","a33","a34","a35","a36","a37","a38","a39","a40","a41","a42","a43","a44","a45","a46","a47","a48","a49","a50"};
    for(int i=0;i<50;i++){
        //strcpy(s,i);
        //filename = "x" + i;
        fileDesc[i] = wo_create(filename[i], WO_RDWR);
        if(fileDesc[i]==-1){
            printf("Test failed at = %d\n",i);
            break;
        }
    }
    printf("checking if all files are created\n");
    int i;
    for(i=0;i<50;i++){
        if(fileDesc[i]==-1){
            printf("Maximum allocatiosn failed at %d\n",i);
            break;
            
        }
    }
    if(i==50){
        printf("Created 50 files.test passed!!!!!!!!\n");
    }
}

void maximization(){
        int fd1 = wo_create("t2", WO_RDWR);
        if(fd1 <0){
            printf("Create test failed :(\n");
        }
        else
        printf("File created and opened!! File Handle is = %d\n",fd1);
       for(int i=0;i<4045;i++){
        if(wo_write(fd1, "wvumxwqhpr vjywxxzjws fvyfydbjdd umxduvnoyc glskrgqemx bkiogtrkca uxlypfdobw yelzyofmxl vysndgjwlb prpylsbxgr lvpcbildvp pmjhupxmqq nydkmaysbb wiwiefbjth lkuhjahvas xxbxivogen pdsdqvrztz vkgfxepbnl cfucbwrrvp zinujpwxvr wvjwmjegws euiigjklxx kbneondgfl idvlkliagn efqjdqkrrh ranhfsmzqq ezpxfnddpt wadqvltlrs sijoajindx zkodcryiyi iqufdzxmic sqcmkbtvej tvyngzfmfr asnfdbiimk ajinaavraw gprbanubxy emcjlwufgw ydpsypklui hbildwxrzs phmejtcyxc gnlxedgdfu iuytmyirkx dowdueensn hjfxdawzom zwleshnivb jymjqtqqcu yesastidhj tsdkatdnll puogcvawvs qugaopjwjx tjqzkqwvwb wcravlquuv mcrkuudcec iccufyqzrc ygtadfmsbl epamotrgqq khqksespsq mvuscypxgs zskogpzgfr gbbdvuvddx wfyhfwzcjc ifuejhqhqh zdfopydxqi wtqvtlmvhn unseguggap xxyzcjomwp troxoquvkd khmmlmzhnb onxkvwpsor mshpdynqtu cuhiggrnqk cbawnxwnfn agpztyflpo jbbvbvzlfy neyjzjoexd tkjffzbgwn qiyomjfxga uvoemerczx cqwxfwafwi btavrrvjmf pgnnrncqwb uahqsepoxg qkldsaljsr dugofbokxr vbpexyqysj kkppkakwlf nkpqeuhwhk zxxpmrthek ihsfhacsib peouxdqovf jtimcyfujw ekjwjyfplz kvibysqaX w", 1020)==0){
        printf("Maximization test Passed!!!\n");
        }
        else{
            printf("Maximization test failed!!!\n");
        }
    }
}