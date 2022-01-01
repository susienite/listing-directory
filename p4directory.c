#include <stdio.h>
#include "apue.h"
#include <dirent.h>

/* Name: Susan Tan 
    DOC: A C program that displays the complete contents of the directory.
    The program will list each file, subdirectory + / , files in subdirectory, 
    and size of file.
    The program will indicate the directory it is in, a message when it is completed, 
    the space, and total files.
*/

// Global variables
int total_size = 0;
int total_files = 0;

//prints the size of the fiile
long long int file_size(const char *name) {
    struct stat buf;
    if (stat(name, &buf) < 0) return 0;
    
    long long int size = buf.st_size; 
    return size; 
}

// list out the contents of a directory without tree and without the spacing
void dir_contents(const char *name) {
    DIR             *dp;
    struct dirent   *dirp;
        
    // open directory, else error 
    dp = opendir(name);
    if (dp == NULL) { 
        return;
    }
    
        
    // read directory       
    while ((dirp = readdir(dp)) != NULL) { 
        
        // skip .. and .
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
            continue;
        }
        
        printf("  "); 
        
        // the contained file is a directory
        if (dirp->d_type == DT_DIR) {
            
            printf("%s /\n", dirp->d_name);
        
            // create pathname for file and recursively call to find its contents
            char path[1000];
            strcpy(path, name);
            strcat(path, "/");  
            strcat(path, dirp->d_name); 
            dir_contents(path);  	// recursion
        
        
        // otherwise, the file is not a directory   
        } else { 
            long long int sz = file_size(dirp->d_name); 
            total_size += sz; 
            total_files += 1;  
     
            printf("%s  %lld\n", dirp->d_name, sz);  // if file, print file name and size         
        }

    }
    
    closedir(dp); 
}

// lists the contents of the directory using a tree
void dir_tree(const char *name, const int root)
{
    
    DIR             *dp;
    struct dirent   *dirp;

    // open directory, else error 
    dp = opendir(name);
    if (dp == NULL) { 
        return;
    }

    // read directory 
    while ((dirp = readdir(dp)) != NULL){ 
    
        // skip .. and . 
        if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
            // add two spaces if it is a subdirectory 
            for (int i = 0; i < root; i++) {
                if (i%2 == 0 || i == 0) printf("  ");
            }

            // print for  subdirectory          
            if (dirp->d_type == DT_DIR) {
                printf("  %s /\n", dirp->d_name);

            // otherwise, not a subdirectory 
            } else {
                long long int sz = file_size(dirp->d_name); 
                total_size += sz; 
                total_files += 1;  
                printf("  %s  %lld\n", dirp->d_name, sz); 
            }

            // create path and recursively call 
            char path[1000]; 
            strcpy(path, name);
            strcat(path, "/");
            strcat(path, dirp->d_name);
            dir_tree(path, root + 2);
        }
      
   } 

    closedir(dp);
}


// get the input directory 
char* getenddir(const char *source) {
    char* predest; 
    char* dest; 
    int STR_SIZE; 

    if (source == ".") {
        return ".";
    }
        
    // find the string with the last '/' 
    predest = (char*)malloc(strlen(predest)*sizeof(char));
    predest = strrchr(source, '/');
    
    dest = (char*)malloc(strlen(dest)*sizeof(char));
    memset(dest, '\0', sizeof(dest));
    STR_SIZE = strlen(predest)-1;
    strncpy(dest, &predest[1], STR_SIZE);

    return dest;
}

// main
int main(int argc, char *argv[]) {
    
    // argument is 2, else error 
    if (argc != 2) {
        printf("usage: ls directory_name");
        return 1;
    }
    DIR *dp;

    // open directory, else error 
    dp = opendir(argv[1]);
    if (dp == NULL) {
        printf("can't open %s\n", argv[1]); 
        return 1;
    }

    // indicate directory     
    printf("Directory listing of %s\n", argv[1]);
    printf("\n%s\n", getenddir(argv[1])); 
    
    /* // list out contents of directory without the spacing
    dir_contents(argv[1]);
    */

    //list out contents of the directory 
    dir_tree(argv[1], 0); 
    
    // print the total space and total files
    printf("\nTotal space usage: %d bytes\n", total_size);
    printf("Total number of files: %d\n", total_files);
 
    printf("\nDone.\n");
    
    exit(0); 
}
