#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int putString(char** string, char* add, int len){
    *string = (char *)realloc(*string, sizeof(char) * (len + strlen(add)));
    strcat(*string, add);
    return len + strlen(add);
}

int main(int argc, char **argv){
    FILE *fp;
    char *buffer, *p, *source;
    char tmp[40];
    int size, i, count = 0, len = 0, count_ptr = 0, ptr = 0;

    if(argc != 2){
        printf("ex)bfc source.bf\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("File not found.\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    buffer = (char *)malloc(sizeof(char) * (size + 1));
    
    fseek(fp, 0, SEEK_SET);
    fread(buffer, size, 1, fp);
    p = buffer;
    
    source = NULL;
    len = putString(&source, "global _start\n", len);
    len = putString(&source, "section .text\n", len);
    len = putString(&source, "_start:\n", len);
    while(*p != '\0'){
        switch(*p){
            case '+':
                sprintf(tmp, "add dword [var%d], 1\n", ptr);
                len = putString(&source, tmp, len);
                break;
            case '-':
                sprintf(tmp, "sub dword [var%d], 1\n", ptr);
                len = putString(&source, tmp, len);
                break;
            case '>':
                if(count_ptr == ptr){
                    count_ptr++;
                    ptr++;
                }else{
                    ptr++;
                }
                break;
            case '<':
                ptr--;
                break;
            case '[':
                sprintf(tmp, "loop%d:\n", count);
                len = putString(&source, tmp, len);
                break;
            case ']':
                len = putString(&source, "cmp dword [var0], 0\n", len);
                sprintf(tmp, "jnz loop%d\n", count);
                len = putString(&source, tmp, len);
                count++;
                break;
            case '.':
                len = putString(&source, "mov edx, 1\n", len);
                sprintf(tmp, "mov ecx, var%d\n", ptr);
                len = putString(&source, tmp, len);
                len = putString(&source, "mov ebx, 1\n", len);
                len = putString(&source, "mov eax, 4\n", len);
                len = putString(&source, "int 0x80\n", len);

                break;
            case ',':
                len = putString(&source, "mov edx, 1\n", len);
                sprintf(tmp, "mov ecx, var%d\n", ptr);
                len = putString(&source, tmp, len);
                len = putString(&source, "mov ebx, 1\n", len);
                len = putString(&source, "mov eax, 3\n", len);
                len = putString(&source, "int 0x80\n", len);
                break;
        }
        p++;
    }

    len = putString(&source, "mov eax, 1\n", len);
    len = putString(&source, "mov ebx, 0\n", len);
    len = putString(&source, "int 0x80\n", len);
    len = putString(&source, "section .data\n", len);
    for(i = 0; i <= count_ptr; i++){
        sprintf(tmp, "var%d dd 0\n", i);
        len = putString(&source, tmp, len);
    }

    fclose(fp);
    sprintf(tmp, "%s.s", argv[1]);
    fp = fopen(tmp, "w");
    fprintf(fp, "%s", source);
    fclose(fp);
}
