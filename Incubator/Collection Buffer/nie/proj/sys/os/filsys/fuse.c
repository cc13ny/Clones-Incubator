#include <fuse.h>
#include <fuse_common.h>//fuse_file_info
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //strcat
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>


#define MAX_NUM_OF_BLOCKS 10000
#define MAX_BLOCK_SIZE 4096
#define INIT_DIR_NLINKS 2
#define INIT_FIL_NLINKS 1
#define MAX_FIL_SIZE 1638400

struct item {
	char type;
	char* loc;
	int inode;
	struct item* next;
};


struct llist{
	
	struct llist* next;
};

struct strls{
	char* str;
	struct strls* next;
};

struct superblock {
	int creationTime;
	int mounted;
	int devId;
	int freeStart;
	int freeEnd;
	int root;
	int maxBlocks;
	
} superblock_info;

struct dir {
	off_t size;
	uid_t uid;
	gid_t gid;
	mode_t mode;
	time_t atime;
	time_t ctime;
	time_t mtime;
	nlinkt linkcount;
	struct item* ftid;
	
};

struct fil {
	off_t size;
	uid_t uid;
	gid_t gid;
	mode_t mode;
	time_t atime;
	time_t ctime;
	time_t mtime;
	nlinkt linkcount;
	int location;
	
};

/********************** Helper Function ************************************/

static void setfree(int inode)
{
	char allzeros[MAX_BLOCK_SIZE];
	memset (allzeros,'\0', MAX_BLOCK_SIZE);
	
	char filename[50], idx[50];
		
	strcpy(filename,  "fusedata.");
   	sprintf(idx, "%d", inode);

   	strcat(filename, idx);
   	
	FILE *fp = fopen(filename, "w");
	fwrite(allzeros, 1, sizeof(allzeros), fp);
	fclose(fp);
	
}

static void del_item(int inode, char* loc)
{
	
}


static bool isdir(int inode)
{
	
	bool = flag;
	struct dir* info;
	read_dir_info(inode, info);
	if(S_IFDIR&info->mode){
		flag = true;
	}
	return 1;
}


static char* get_abs_path(char* path)
{
	//should raise an ENONENT error:
	//				A component of path does not name an existing file or path is an empty string.
	if(strcmp(path, "")){
		return path;
	}
	
	//if it's a relative path
	char cwd[1024];
	if(path[0] != '/'){		
		if (getcwd(cwd, sizeof(cwd)) != NULL){
			char pat[32];
			strcpy(pat, path);
			strcat(cwd, "/");
			strcat(cwd, pat);
		}
		else{
			perror("getcwd() error");
			//exit(-1);
		}
	}
	else{
		strcpy(cwd, path);
	}
	
	
	///////////////////////////split absolute path into strings//////////////////////////////
    
    
  	char * pch;
  	struct strls* head;
  	struct strls* p;
  	char *sep = "/";
  	
  	
  	pch = strtok (cwd, sep);
  	head->str = pch;
  	head->next = NULL;
  	p = head;
  	
  	while (pch != NULL){
    	pch = strtok (NULL, sep);
    	if(pch != NULL){
	    	struct strls tmp;
	    	tmp.str = pch;
	    	tmp.next = NULL;
	    	p->next = &tmp;
	    	p = p->next;
    	}
  	}
  	
  	
	///////////////////////////////Remove "."////////////////////////////////////
	p = head;
	struct strls* pre = p;
	while(p != NULL){
		if(strcmp(p->str, ".") == 0){
			if(p == pre){
				head = head->next;
				p = head;
				pre = p;
			}
			else{
				p = p->next;
			}
		}
		else {
			if(p == pre){
				p = p->next;
			}
			else{
				pre = p;
				p = p->next;
				
			}
		}
		
	}
	
	/* Also should Remove ".." */

	return abs;	
}


static char* get_abs_par_path(char* path)
{
		char* abs = get_abs_path(path);
		
		if(abs == NULL){
			return abs;
		}
		
	
		
		int last = 0;
		int num = strlen(path);
		
		int i;
		for(i = 0; i < num;i++){
			char t[1];
			sprintf(t, "%c", path[i]);
			if(strcmp(t, "/") == 0){
				//printf("%c\n", path[i]);
				last = i;
			}
			
		}
	
	    //printf("%d", last);
		char* subbuff;
		subbuff = (char*)malloc(50*sizeof(char));
		memcpy( subbuff, path , last );
	    subbuff[last] = '\0';
	  	return subbuff;
	
}

static char* get_rel_from_abs(char* path)
{
		char* abs = get_abs_path(path);;
		
		if(abs == NULL){
			return abs;
		}
		
	
		
		int last = 0;
		int num = strlen(abs);
		
		
		int i;
		for(i = 0; i < num;i++){
			char t[1];
			sprintf(t, "%c", abs[i]);
			if(strcmp(t, "/") == 0){
				//printf("%c\n", path[i]);
				last = i;
			}
			
		}
	
	    
		char* subbuff;
		subbuff = (char*)malloc(50*sizeof(char));
		memcpy( subbuff, &abs[last+1] , num - last - 1 );
	    subbuff[num - last - 1] = '\0';
	  	
		return subbuff;
		
}

static int in_fctl(int pinode, char* rel){
		//pinode: parent inode
		struct dir* info;
		read_dir_info(pinode, info);
		int rel_inode = -1;
		struct item* p = info->ftid;
		while(p!= NULL){
			char* lc = p->loc;
			if(strcmp(lc, rel) == 0){
				rel_inode = p->inode;// found it;
				break;
			}
			
		}
		
		return rel_inode;
	
}

static int get_inode(char* path)
{
		char* abs = get_abs_path(path);
		int inode = -1;
		
		if(abs == NULL){
			return inode;
		}
		
	
		
		int start = 1;
		int num = strlen(abs);
		
		int pinode = 26;
		
		char *p = abs;
		int i;
		for(i = 1; i < num;i++){
			char t[1];
			sprintf(t, "%c", abs[i]);
			if(strcmp(t, "/") == 0){
				
				char* dirname;
				dirname = (char*)malloc(50*sizeof(char));
				memcpy( subbuff, &abs[start] , i - start );
	    		dirname[i - start] = '\0';
	    		int pinode = in_fctl(pinode, dirname);
	    		if (pinode < 0){
	    			return -1;
				}
				inode = pinode;
				start = i+1;
			}
			
		}
	
		return inode;

}




//static void read_fbls_info()
//{
//	
//}

static void write_fb_info(int inode, int fbnum)
{
		char filename[50], idx[50], num[10];
			
		strcpy(filename,  "/tmp/fusedata.");
	   	sprintf(idx, "%d", inode);
	
	   	strcat(filename, idx);
	   	
		FILE *fp = fopen(filename, "a");
        sprintf(num, "/%d", fbnum);
        fputs(num, fp);
		fclose(fp);
	
}

static void write_fbls_info()
{
		
		int i;
		for(i = 27; i < MAX_NUM_OF_BLOCKS; i++){
			int inode = i /400 + 1;
			write_fb_info(inode, i);
		}
		
		
}

static void write_sb_info(struct superblock* info)
{
		FILE *fp = fopen("/tmp/fusedata.0", "w");
		fwrite(info, 1, sizeof(superblock), fp);
		fclose(fp);
}

static void write_dir_info(int inode, struct dir* info)
{
		char filename[50], idx[50];
			
		strcpy(filename,  "/tmp/fusedata.");
	   	sprintf(idx, "%d", inode);
	
	   	strcat(filename, idx);
	   	
		FILE *fp = fopen(filename, "w");
		fwrite(info, 1, sizeof(dir), fp);
		fclose(fp);
}

static void write_fil_info(int inode, struct fil* info)
{
		char filename[50], idx[50];
			
		strcpy(filename,  "/tmp/fusedata.");
	   	sprintf(idx, "%d", inode);
	
	   	strcat(filename, idx);
	   	
		FILE *fp = fopen(filename, "w");
		fwrite(info, 1, sizeof(fil), fp);
		fclose(fp);
}

///////////////////////////////////////////////////////////////////////
static void read_dir_info(int inode, struct dir* info)
{
		char filename[50], idx[50];
			
		strcpy(filename,  "/tmp/fusedata.");
	   	sprintf(idx, "%d", inode);
	
	   	strcat(filename, idx);
	   	
		FILE *fp = fopen(filename, "w");
		fread(info, 1, sizeof(dir), fp);
		fclose(fp);
}


static void read_fil_info(int inode, struct fil* info)
{
		char filename[50], idx[50];
			
		strcpy(filename,  "/tmp/fusedata.");
	   	sprintf(idx, "%d", inode);
	
	   	strcat(filename, idx);
	   	
		FILE *fp = fopen(filename, "w");
		fwrite(info, 1, sizeof(fil), fp);
		fclose(fp);
}


static int extract_next_inode()
{
		int X;
		char* subbuff;
		for(X = 1; X < 26; X++){
			char filename[50], idx[50];
			
			strcpy(filename,  "/tmp/fusedata.");
	   		sprintf(idx, "%d", X);
	
	   		strcat(filename, idx);
	   	
			FILE *fp = fopen(filename, "r");
			
			char info[400];
			fread(info, 1, sizeof(info), fp);
			
			
			//////////////////////////////////
			
			int start = 1;
			int num = strlen(info);
			
			
			int i;
			int flag = 0;
			for(i = 1; i < num;i++){
				char t[1];
				sprintf(t, "%c", info[i]);
				if(strcmp(t, "/") == 0){
					
					subbuff = (char*)malloc(50*sizeof(char));
					memcpy( subbuff, &abs[start] , i - start );
		    		subbuff[i - start] = '\0';
		    		
		    		
		    		char tmp[1];
		    		sprintf(tmp, "%c", subbuff[0]);
		    		if(strcpy(tmp, "-")!=0){
		    			flag = 1;
					}
				}
				if(flag == 1){
					break;
				}
				
			}
			fclose(fp);
			if(flag == 1){
				break;
			}
			
			
		}
		
		inode = stoi(subbuff);
		
		
		//update freeblock list
		
		
		return inode;
}


static void* cchen_init(struct fuse_conn_info *conn)
{
	    /* 
			Initialize filesystem
			The return value will passed in the private_data field of fuse_context to 
			all file operations and as a parameter to the destroy() method.
		*/
		
	    /*Make directory to store block devices*/
	    system("mkdir /tmp/fuse");
		/* Initialize 10000 block devices */
		int X;
		
		char allzeros[MAX_BLOCK_SIZE];
		memset (allzeros,'\0', MAX_BLOCK_SIZE);
		
		for( X = 0; X < MAX_NUM_OF_BLOCKS; X++){
			char filename[50], idx[50];
			
			strcpy(filename,  "/tmp/fuse/fusedata.");
	   		sprintf(idx, "%d", X);
	   		strcat(filename, idx);
	   		
	   		FILE *fp = fopen(filename, "w");
	   		fwrite(allzeros, 1, sizeof(allzeros), fp);
			
		}
		
		//Make free block list, written in block 1~25
		
		write_fbls_info();

		/* fusedata.0: info of superblock */
		superblock_info = {
			.creationTime  = 1376483073,// can be set by the function in /time.h, however, not implemented here
			.mounted       = 1, // should be updated
			.devId         = 20,
			.freeStart     = 1,
			.freeEnd       = 25,
			.root          = 26,
			.maxBlocks     = MAX_NUM_OF_BLOCKS,
		}
		
		write_sb_info(&superblock_info);
		
		/* fusedata.26: info of rootdir*/
		struct item parentdir = {
			.inode  = 26,
			.loc    = "..",
			.type   = 'd',
			.next   = NULL,
		};
	
		
		struct item currentdir = {
			.inode  = 26,
			.loc    = ".",
			.type   ='d',
			.next   = &parentdir,
	    }
		
		struct dir rootdir_info = {
			.size         = 0,
			.uid          = 1,
			.gid          = 1,
			.mode         = 16877,
			.atime		  = 1323630836,
			.ctime		  = 1323630836,
			.mtime	      = 1323630836,
			.linkcount    = INIT_DIR_NLINKS, // should be updated
			.ftid         = &currentdir,
		};
		
		write_dir_info(&rootdir_info, 26);
}

static void cchen_destroy(void *)
{
		/* Destroy the File System*/
		
		//Not sure about if we should destroy all dirs recursively in the filesystem under the root directory
		//destroy all devices blocks
		int X;
		
	
		for( X = 0; X < MAX_NUM_OF_BLOCKS; X++){
			char filename[50], idx[50];
			
			strcpy(filename,  "/tmp/fuse/fusedata.");
	   		sprintf(idx, "%d", X);
	   		strcat(filename, idx);
	   		
	   		unlink(filename);
		}
}

static int cchen_opendir(const char * path, struct fuse_file_info * fi)
{
		/* http://linux.die.net/man/3/opendir */
		
		// The permission for opendir on the path is allowed by default.
		char* abs = get_abs_path(path);
		int inode = get_inode(abs);
		if(inode < 0){
			return -1;// the dir doesn't exist
		}
		
		if(!isdir(inode)){
			return -1;// it's not a dir
		}
		
		//set the filehandler for other operations
		fi->fh = 0;
		
		return inode;
	
}


static int cchen_create(const char *path, mode_t mode, struct fuse_file_info * fi)
{
		char* abs = get_abs_path(path);
		int inode = get_inode(abs);
		if(inode > 0){
			return -1;// the path exists. 
		}
		
		char* abs_par = get_abs_par_path(abs);
		int pinode = get_inode(abs_par);
		if(inode < 0){
			return -1;// parent doesn't exist
		}
		
		int ninode = extract_next_inode();
		
		if(ninode < 0){
			return -1;// overflow
		}
		
		/*
			When a file is created, we must consider how large it is
			If the file is larger thant 4096 bytes, it should have indirect 1
		 */
		
		struct fil info = {
			.size        =  0,
			.uid         =  1,
			.gid         =  1,
			.mode        =  mode,
			.atime       =  1323630836,
			.ctime       =  1323630836,
			.mtime       =  1323630836,
			.linkcount   =  INIT_FIL_NLINKS,
			.indirect    =  0,//
			.location    =  ninode,//
		};
	
		write_fil_info(ninode, &info);
		
		struct fuse_file_info *fi;
		int fh = cchen_open(path, fi);
		fi->fh = fh;
		
		
		return fh;
	
}

static int cchen_release(const char * path, struct fuse_file_info * fi)
{
		int inode = fi->fh;
		
		//release resources based on the inode or the path
		return 0;
}

static int cchen_releasedir(const char * path, struct fuse_file_info * fi)
{
	    int inode = fi->fh;
		
		//release resources based on the inode or the path
		return 0;
}


static int cchen_getattr(const char *path, struct stat *stbuf)
{
        char* abs = get_abs_path(path);
        int inode = get_inode(abs);
        if(inode < 0){
        	return -1;//file doesn't exist
		}
		
		if(isdir(inode)){
			struct dir* info;
			read_dir_info(inode, info);
			stbuf->st_atime = info->atime;
			stbuf->st_ctime = info->ctime;
			stbuf->st_mtime = info->mtime;
			//stbuf->st_dev   = info->
			stbuf->st_gid   = info->gid;
			stbuf->st_ino   = inode;
			stbuf->st_mode  = info->mode;
			stbuf->st_nlink = info->linkcount;
			//stbuf->st_rdev
			stbuf->st_size  = info->size;
			stbuf->st_uid   = info->uid;
		}
		else{
			struct fil* info;
			read_fil_info(inode, info);
			stbuf->st_atime = info->atime;
			stbuf->st_ctime = info->ctime
			stbuf->st_mtime = info->mtime;
			//stbuf->st_dev   = info->
			stbuf->st_gid   = info->gid;
			stbuf->st_ino   = inode;
			stbuf->st_mode  = info->mode;
			stbuf->st_nlink = info->linkcount;
			//stbuf->st_rdev
			stbuf->st_size  = info->size;
			stbuf->st_uid   = info->uid;
		}
        
        return 0;
}


static int cchen_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
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
                if (filler(buf, de->d_name, &st, 0))
                        break;
        }
        closedir(dp);
        return 0;
}


static int cchen_mkdir(const char *path, mode_t mode)
{
		
        int res;
        
        if(strcmp(path, "") == 0){
        	return -1; // path not allowed
		}
		
		char* abs = get_abs_path(path);
		char* abs_par = get_abs_par_path(abs);
		
		int par_inode = get_inode(abs_par);
		if(par_inode < 0){
			return -1;// parent of path doesn't exist
		}
		
		
		int inode = get_inode(abs);
		if(inode > 0){
			return -1;// path exists before
		}
		
		if(!isdir(par_inode)){
			return -1;// parent is not a dir
		}
		
		
        /* mkdir */
        res = mkdir(path, mode);
        if (res == -1)
        	return -errno;
        /********/
        
        
        char* dirname = get_rel_from_abs(abs); 
        int next_inode = extract_next_inode();
        
        /* Update parent's inode */
        struct dir par_info;
        read_dir_info(par_inode, &par_info);
        par_info.linkcount += 1; //why, should be deleted
        struct item tmp = {
        	.type       = 'd',
        	.inode      = next_inode,
        	.loc        = dirname,
        	.next       = par_info.ftid,
		};
		par_info.ftid = &tmp;
		write_dir_info(par_inode, &par_info);
		
		
		/* Create the inode for the new directory */
		struct item tmp2 = {
			.type       = 'd',
        	.inode      = par_inode,
        	.loc        = "..",
        	.next       = NULL,
		};
				
		struct item tmp1 = {
			.type       = 'd',
        	.inode      = next_inode,
        	.loc        = ".",
        	.next       = &tmp2,
		};
		
		struct dir cur_info = {
			.size        =  4096,
			.uid         =  1,
			.gid         =  1,
			.mode        =  33261, //mode | S_IFDIR,
			.atime       =  1323630836,
			.ctime       =  1323630836,
			.mtime       =  1323630836,
			.linkcount   =  INIT_DIR_NLINKS, //. and the realpath?
			.ftid        =  &tmp1,
		};
        write_dir_info(next_inode, &cur_info);
        
        
        return 0;
}

/////////////////////////////////////////////////////////////// 
static int cchen_unlink(const char *path)
{
		//when the last link is removed, the space is considered free.
		
		
		char* abs = get_abs_path(path);
		int inode = get_inode(abs);
		if(inode != 0){
			return -1; // path doesn't exist
		}
		
		if(isdir(inode)){
			return -1; // it's a dir
		}
		
		
		char* par_abs = get_abs_par_path(abs);
		int par_inode = get_inode(par_abs);
		
		char* filename = get_rel_from_abs(abs);
		del_item(par_inode, filename);
		
		struct dir par_info;
		read_dir_info(par_inode, &par_info);
		par_info.linkcount -= 1;
		write_dir_info(par_inode, &par_info);
		
		struct fil cur_info;
		read_fil_info(inode, &cur_info);
		cur_info.linkcount -= 1;
		if(cur_info.linkcount == 0){
			setfree(inode);
		}
		else{
			write_fil_info(inode, &cur_info);
		}
		
		

		
		/* unlink */
        int res;
        res = unlink(path);
        if (res == -1)
                return -errno;
        /*********/
        
        return 0;
}
/////////////////////////////////////////////////////////////// 


static int cchen_rename(const char *from, const char *to)
{
		char* abs = get_abs_path(from);
		int inode = get_inode(abs);
		
		if(inode < 0){
			return -1;// file doesn't exit
		}
		
		if(isdir(inode)){
			return -1;// it's a dir
		}
        
        char* abs_par = get_abs_par_path(abs);
        int pinode = get_inode(abs_par);
        
        struct dir* info;
        read_dir_info(pinode, info);
        struct item *p = info->ftid;
        while(p != NULL){
        	if(strcmp(p->loc, from)){
        		p->loc = to;
        		break;
			}
        	p = p->next;
		}
		
        write_dir_info(pinode, info);
        
        return 0;
}

static int cchen_link(const char *from, const char *to)
{
		//pc: lock the file system
		
		//Upon successful completion, link() shall mark for update the st_ctime field of the file. 
		//Also, the st_ctime and st_mtime fields of the directory that contains the new entry shall be marked for update.
		char* abs_from = get_abs_path(from);
		int finode = get_inode(abs_from);
		
		if(finode < 0){
			return -1;//path doesn't exist
		}
		
		if(isdir(finode)){
			return -1;// only a link to a file is allowed
		}
		
		//pc: check permissions
		
		/******************Check**********************/
		if(strcmp(to, "") == 0){
        	return -1; // path not allowed
		}
		
		char* abs_to = get_abs_path(to);
		char* abs_tpar = get_abs_par_path(abs_to);
		
		int tpar_inode = get_inode(abs_tpar);
		if(tpar_inode < 0){
			return -1;// parent of path doesn't exist
		}
		
		int tinode = get_inode(abs_to);
		if(tinode > 0){
			return -1;// path exists before
		}
		
		if(!isdir(tpar_inode)){
			return -1;// parent is not a dir
		}
		/*-----------------Check---------------*/
		
	    char* filename = get_rel_from_abs(abs_to); 
        
        /* Update parent's inode */
        struct dir tpar_info;
        read_dir_info(tpar_inode, &tpar_info);
        tpar_info.linkcount += 1; //why, should be deleted
        struct item tmp = {
        	.type       = 'f',
        	.inode      = finode,
        	.loc        = filename,
        	.next       = tpar_info.ftid,
		};
		tpar_info.ftid = &tmp;
		write_dir_info(tpar_inode, &tpar_info);
		
		
		/* Update the inode of the 'from' path*/
		struct fil from_info;
        read_fil_info(finode, &from_info);
        from_info.linkcount += 1; //
		write_fil_info(next_inode, &from_info);
		
        return 0;
}



static int cchen_open(const char *path, struct fuse_file_info *fi)
{
	    if(strcmp(path, "") == 0){
        	return -1; // path not allowed
		}
		
		char* abs = get_abs_path(path);		
		int inode = get_inode(abs);
		if(inode < 0){
			return -1;// the file doesn't exist
		}
	
		fi->fh = inode;
		
		
        return 0;
}
static int cchen_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi)
{
		char* abs = get_abs_path(path);
		int inode = get_inode(abs);
		
		if(inode < 0){
			return 0;//path doesn't exist
		}
		
		if(size == 0){
			return 0;
		}
		
		
		FILE* fp = fopen(abs, "r");
		

        fseek(fp, offset, SEEK_SET);
        fread(buf, size, offset, fp);
        close(fp);

        return size;
}
static int cchen_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
	    char* abs = get_abs_path(path);
		int inode = get_inode(abs);
		
		if(inode < 0){
			return 0;//path doesn't exist
		}
		
		if(size == 0){
			return 0;
		}
		
		
		FILE* fp = fopen(abs, "r");
		

        fseek(fp, offset, SEEK_SET);
        fwrite(buf, size, offset, fp);
        //update the size of the inode
        struct fil* info;
        read_fil_info(inode, info);
        info->size += size;
        write_fil_info(inode, info);
        close(fp);
        
        return size;
}

static int cchen_statfs(const char *path, struct statvfs *stbuf)
{
        stbuf->f_bsize    = MAX_BLOCK_SIZE;    /* file system block size */
        stbuf->f_files    = MAX_NUM_OF_BLOCKS;    /* # inodes */
        stbuf->f_bfree    = 25;    /* # free blocks */
        stbuf->f_namemax  = 50;
        
		//stbuf->f_ffree = ;    /* # free inodes */
        //unsigned long  f_frsize;   /* fragment size */
        //fsblkcnt_t     f_blocks;   /* size of fs in f_frsize units */
        //fsblkcnt_t     f_bavail;   /* # free blocks for unprivileged users */
        //fsfilcnt_t     f_favail;   /* # free inodes for unprivileged users */
        //unsigned long  f_fsid;     /* file system ID */
        //unsigned long  f_flag;     /* mount flags */
    
        
        return 0;
}



static struct fuse_operations cchen_oper = {
	
		.init			= cchen_init,//
		.destroy		= cchen_destroy,//
		.opendir		= cchen_opendir,//
		.create			= cchen_create,
		.release		= cchen_release,
		.releasedir		= cchen_releasedir,
		
        .getattr        = cchen_getattr,//
        .readdir        = cchen_readdir,
        .mkdir          = cchen_mkdir,//
        .unlink         = cchen_unlink,//later on, key: design for free block
        .rename         = cchen_rename,//
        .link           = cchen_link,//
        .open           = cchen_open,// later on 
        .read           = cchen_read,// take a look at py
        .write          = cchen_write,// take a look at py
        .statfs         = cchen_statfs,//.....
};

int main(int argc, char *argv[])
{
        //umask(0);
        return fuse_main(argc, argv, &cchen_oper, NULL);
}
