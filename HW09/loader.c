//that contains the routines you need for opening 
//and loading the object files,
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "LC4.h"
char* itohs(char* out, unsigned short num){
    int q;
    int r;
    char h = 'E';
    for(int i = 0; i<4; i++){

        q = num/16;
        r = num%16;
        num = q;
        switch(r){
            case 0 : 
                h='0';
                break;
            case 1 : 
                h='1';
                break;
            case 2 : 
                h='2';
                break;
            case 3 : 
                h='3';
                break;
            case 4 : 
                h='4';
                break;
            case 5 : 
                h='5';
                break;
            case 6 : 
                h='6';
                break;
            case 7 : 
                h='7';
                break;
            case 8 : 
                h='8';
                break;
            case 9 : 
                h='9';
                break;
            case 10 : 
                h='A';
                break;
            case 11 : 
                h='B';
                break;
            case 12 : 
                h='C';
                break;
            case 13 : 
                h='D';
                break;
            case 14 : 
                h='E';
                break;
            case 15 : 
                h='F';
                break;
        }
        out[3-i]=h;
    }
    return out;
}
unsigned short makeWindow(FILE **fp, int n){
    int c;
    unsigned short w=0;
    for(int i=0;i<n;i++){       //<address>
        c = fgetc(*fp);
        if( feof(*fp) ) {
            break ;
        }
        w = w << 8;//bit shift 
        w = w + c;//add to window
    }
    return w;
}
void cadeParse(unsigned short * mem, char* kind, FILE **fp){

    unsigned short address = makeWindow(fp, 2);
    unsigned short n = makeWindow(fp, 2);
    
    for(int i=0;i<n;i++){ //n word body
        unsigned short w=0;

        w = makeWindow(fp, 2);
        mem[address+i] = w;
        kind[address+i] = 1;
        char *bruh = (char*)malloc(sizeof(char)*5);
        char *bruh2 = (char*)malloc(sizeof(char)*5);
        char *bruh3 = (char*)malloc(sizeof(char)*10);
        if(w==0){
           kind[address+i] = 0; 
        }
     //   printf("%s: (%d) %s[%u]---%d\n", itohs(bruh2,address+i),w, itohs(bruh,w), hstoi(bruh),kind[address+i]);
        //binary2ass(bruh3, hstoi(bruh));
        free(bruh);
        free(bruh2);
        free(bruh3);
    }
    
}
void dataParse(unsigned short * mem, char* kind, FILE **fp){
    
    unsigned short address = makeWindow(fp, 2);
    unsigned short n = makeWindow(fp, 2);
    
    for(int i=0;i<n;i++){ //n word body
        unsigned short w=0;

        w = makeWindow(fp, 2);
        mem[address+i] = w;
        kind[address+i] = 2;
        char *bruh = (char*)malloc(sizeof(char)*5);
        char *bruh2 = (char*)malloc(sizeof(char)*5);
        if(w==0){
           kind[address+i] = 0; 
        }
        //if(kind[address+i]!=0){
        //printf("%s: (%d) %s---%d\n", itohs(bruh2,address+i),w, itohs(bruh,w),kind[address+i]);}
        free(bruh);
        free(bruh2);
    }
    
}
void symbolParse(unsigned short * mem, char* kind, FILE **fp){
   
    unsigned short address = makeWindow(fp, 2);
    unsigned short n = makeWindow(fp, 2);
    
    for(int i=0;i<n;i++){ //n word body
        unsigned short w=0;
        unsigned short c = fgetc(*fp);
        w = c;

        char *bruh = (char*)malloc(sizeof(char)*5);
        char *bruh2 = (char*)malloc(sizeof(char)*5);  
        //printf("%s: (%d) %s---%d\n", itohs(bruh2,address+i),w, itohs(bruh,w),kind[address+i]);
        free(bruh);
        free(bruh2);
    }
    
}
void nameParse(unsigned short * mem, char* kind, FILE **fp){
    unsigned short n = makeWindow(fp, 2);
    
    for(int i=0;i<n;i++){ //n word body       
        
        fgetc(*fp);    
    }
    
}
void lineParse(unsigned short * mem, char* kind, FILE **fp){
      
    for(int i=0;i<6;i++){ //n word body
        
        fgetc(*fp);          
    }
    
}

void load(unsigned short * mem, char* kind, char* obj){

    
    FILE *fp;
    int c;
    unsigned short w=0;

    fp = fopen(obj,"r");
    if(fp == NULL) {
        perror("Error in opening file");
        //return(-1);
    } do {
        c = fgetc(fp);
        if( feof(fp) ) {
            break ;
        }
        
        w = w << 8;//bit shift 
        w = w + c;//add to window
        
       

        if (w == 51934){ //CADE

            cadeParse(mem,kind,&fp);
        } else if(w == 56026){ //DATA
            
            dataParse(mem,kind,&fp);
            printf("%u, %d\n",w,fgetc(fp));
        }else if(w == 50103){

            symbolParse(mem,kind,&fp);
        }else if(w == 61822){

            nameParse(mem,kind,&fp);
        }else if(w == 29022){

            lineParse(mem,kind,&fp);
        }
       
   } while(1);
   fclose (fp);
}