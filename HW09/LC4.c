//routines for decoding instructions and a file
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

unsigned short hstoi(char* in){
    //int q;
    //int r;
    unsigned short h = 0;
    unsigned short o = 0;
    for(int i = 0; i<4; i++){
        switch(in[i]){
            case '0' : 
                h = 0 ;
                break;
            case '1' : 
                h=1;
                break;
            case '2' : 
                h=2;
                break;
            case '3' : 
                h=3;
                break;
            case '4' : 
                h=4;
                break;
            case '5' : 
                h=5;
                break;
            case '6' : 
                h=6;
                break;
            case '7' : 
                h=7;
                break;
            case '8' : 
                h=8;
                break;
            case '9' : 
                h=9;
                break;
            case 'A' : 
                h=10;
                break;
            case 'B' : 
                h=11;
                break;
            case 'C' : 
                h=12;
                break;
            case 'D' : 
                h=13;
                break;
            case 'E' : 
                h=14;
                break;
            case 'F' : 
                h=15;
                break;
        }
        o = o <<4;
        o = o + h;
    }
    return o;
}
void signEx(unsigned short *num, char* IM, int n){
    unsigned short IMM9code = (unsigned short)(*num <<(16-n)) >>(16-n);
        
   // IMM9code = IMM9code ;
        
    if((IMM9code>>(n-1))==1){
        
        IMM9code = (~IMM9code);
        //printf("%u\n",IMM9code);
        IMM9code =IMM9code+1;
        //printf("%u\n",IMM9code);
        IMM9code = (unsigned short)(IMM9code <<(16-n)) >>(16-n);
        //printf("%u\n",IMM9code);
        sprintf(IM, "#-%u",IMM9code);
    }else{
        sprintf(IM, "#%u",IMM9code);
    }
}
char* binary2ass(char* output, unsigned short num){ // make an 16 field array
    unsigned short opcode = num >>12; // read first 4
    
    
    //printf("%u",opcode);
    if(opcode== 0){// BR branch
        char* type =(char*)malloc(sizeof(char)*6);
        unsigned short typecode = (unsigned short)(num <<4) >>13;
        if(typecode == 0){ //NOP
            sprintf(type, "NOP");
        }else if (typecode == 1){
            sprintf(type, "BRp");
        }else if (typecode == 2){
            sprintf(type, "BRz");
        }else if (typecode == 3){
            sprintf(type, "BRzp");
        }else if (typecode == 4){
            sprintf(type, "BRn");
        }else if (typecode == 5){
            sprintf(type, "BRnp");
        }else if (typecode == 6){
            sprintf(type, "BRnz");
        }else if (typecode == 7){
            sprintf(type, "BRnzp");
        }

        char* IMM9 =(char*)malloc(sizeof(char)*8);
        signEx(&num, IMM9,9);
        
        
        sprintf(output, "%s %s",type,IMM9);
        //printf(output, "%s %s \n",type,IMM9);
        free(type);
        
        free(IMM9);
    }else if (opcode == 1){ //Arithmatics
        char* type =(char*)malloc(sizeof(char)*6);
        unsigned short typecode = (unsigned short)(num <<10) >>13;
        unsigned short rd;
        unsigned short rs;
        unsigned short rt;
        char* IMM5 =(char*)malloc(sizeof(char)*8);
        if(typecode == 0){ //NOP
            sprintf(type, "ADD");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(IMM5,"R%u",rt);
        }else if (typecode == 1){
            sprintf(type, "MUL");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(IMM5,"R%u",rt);
        }else if (typecode == 2){
            sprintf(type, "SUB");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(IMM5,"R%u",rt);
        }else if (typecode == 3){
            sprintf(type, "DIV");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(IMM5,"R%u",rt);
        }else {
            sprintf(type, "ADD");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<11) >>11;
            
            signEx(&rt, IMM5, 5);
        }
        
        
        sprintf(output, "%s R%u, R%u, %s",type,rd,rs,IMM5);
       // printf(output, "%s R%u, R%u, %s \n",type,rd,rs,IMM5);
       free(type);
       free(IMM5);
    }else if (opcode == 10){ //MOD, shifts
        char* type =(char*)malloc(sizeof(char)*6);
        unsigned short typecode = (unsigned short)(num <<10) >>13;
        unsigned short rd;
        unsigned short rs;
        unsigned short rt;
        char* UIMM =(char*)malloc(sizeof(char)*8);
        if(typecode <= 1){ 
            sprintf(type, "SLL");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<12) >>12;
            sprintf(UIMM,"#%u",rt);
        }else if (typecode <= 3){
            sprintf(type, "SRA");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<12) >>12;
            sprintf(UIMM,"#%u",rt);
        }else if (typecode == 5){
            sprintf(type, "SRL");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<12) >>12;
            sprintf(UIMM,"#%u",rt);
        }else {
            sprintf(type, "MOD");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(UIMM,"R%u",rt);
        }
        
        
        sprintf(output, "%s R%u, R%u, %s",type,rd,rs,UIMM);
       // printf(output, "%s R%u, R%u, %s \n",type,rd,rs,UIMM);
       free(type);
       free(UIMM);
    }else if (opcode == 5){ //Logicals
        char* type =(char*)malloc(sizeof(char)*6);
        unsigned short typecode = (unsigned short)(num <<10) >>13;
        unsigned short rd;
        unsigned short rs;
        unsigned short rt;
        char* IMM5 =(char*)malloc(sizeof(char)*8);
        if(typecode == 0){ //NOP
            sprintf(type, "AND");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(IMM5,", R%u",rt);
            sprintf(output, "%s R%u, R%u%s",type,rd,rs,IMM5);
        }else if (typecode == 1){
            sprintf(type, "NOT");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
           
            sprintf(IMM5,"");
            sprintf(output, "%s R%u, R%u%s",type,rd,rs,IMM5);
        }else if (typecode == 2){
            sprintf(type, "OR");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(IMM5,", R%u",rt);
            sprintf(output, "%s R%u, R%u%s",type,rd,rs,IMM5);
        }else if (typecode == 3){
            sprintf(type, "XOR");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(IMM5,", R%u",rt);
            sprintf(output, "%s R%u, R%u%s",type,rd,rs,IMM5);
        }else {
            sprintf(type, "AND");
            rd = (unsigned short)(num <<4) >>13;
            rs = (unsigned short)(num <<7) >>13;
            rt = (unsigned short)(num <<11) >>11;
            
            signEx(&rt, IMM5, 5);
            //sprintf(IMM5, "AND");
            sprintf(output, "%s R%u, R%u, %s",type,rd,rs,IMM5);
        }
        
        
        
        //printf(output, "%s R%u, R%u%s \n",type,rd,rs,IMM5);
        free(type);
        free(IMM5);
    }else if (opcode == 6){ // LDR
        
        
        unsigned short rd;
        unsigned short rs;
        unsigned short rt;
        char* IMM6 =(char*)malloc(sizeof(char)*9);
        
            
        rd = (unsigned short)(num <<4) >>13;
        rs = (unsigned short)(num <<7) >>13;
        rt = (unsigned short)(num <<10) >>10;
            
        signEx(&rt, IMM6, 6);
        
        
        
        sprintf(output, "LDR R%u, R%u, %s",rd,rs,IMM6);
       // printf(output, "LDR R%u, R%u, %s \n",rd,rs,IMM6);
       
       free(IMM6);
    }else if (opcode == 7){ // STR
        unsigned short rd;
        unsigned short rs;
        unsigned short rt;
        char* IMM6 =(char*)malloc(sizeof(char)*10);
        
            
        rd = (unsigned short)(num <<4) >>13;
        rs = (unsigned short)(num <<7) >>13;
        rt = (unsigned short)(num <<10) >>10;
            
        signEx(&rt, IMM6, 6);
        
        
        
        sprintf(output, "STR R%u, R%u, %s",rd,rs,IMM6);
       // printf(output, "STR R%u, R%u, %s \n",rd,rs,IMM6);
      
       free(IMM6);
    }else if (opcode == 9){ // CONST
        unsigned short rd;
        
        unsigned short rt;
        char* IMM9 =(char*)malloc(sizeof(char)*9);
        
            
        rd = (unsigned short)(num <<4) >>13;
        
        rt = (unsigned short)(num <<7) >>7;
            
        signEx(&rt, IMM9, 9);
        
        
        
        sprintf(output, "CONST R%u, %s",rd, IMM9);
        
        free(IMM9);
    }else if (opcode == 13){ // HICONST
        unsigned short rd;
        
        unsigned short rt;
        char* IMM9 =(char*)malloc(sizeof(char)*9);
        
            
        rd = (unsigned short)(num <<4) >>13;
        
        rt = (unsigned short)(num <<8) >>8;
        
        
        
        sprintf(output, "HICONST R%u, #%u",rd, rt);
      //  printf(output, "HICONST R%u, #%u \n",rd, rt);
        free(IMM9);
    }else if (opcode == 2){ // CMP
        char* type =(char*)malloc(sizeof(char)*6);
        unsigned short typecode = (unsigned short)(num <<7) >>14;
        unsigned short rd;
        
        unsigned short rt;
        char* IMM7 =(char*)malloc(sizeof(char)*8);
        if(typecode == 0){ //NOP
            sprintf(type, "CMP");
            rd = (unsigned short)(num <<4) >>13;
            rt = (unsigned short)(num <<13) >>13;
            sprintf(IMM7,"R%u",rt);
        }else if (typecode == 1){
            sprintf(type, "CMPU");
            rd = (unsigned short)(num <<4) >>13;
            rt = (unsigned short)(num <<13) >>13;
           
            sprintf(IMM7,"R%u",rt);
        }else if (typecode == 2){
            sprintf(type, "CMPI");
            rd = (unsigned short)(num <<4) >>13;
            
            rt = (unsigned short)(num <<9) >>9;
            signEx(&rt, IMM7, 7);
            
        
        }else {
            sprintf(type, "CMPIU");
            rd = (unsigned short)(num <<4) >>13;
            
            rt = (unsigned short)(num <<9) >>9;
            
            sprintf(IMM7,"#%u",rt);
        }
        
        
        sprintf(output, "%s R%u, %s",type,rd,IMM7);
      //  printf(output, "%s R%u, %s \n",type,rd,IMM7);
        free(type);
        free(IMM7);
    }else if (opcode == 4){ // JSR
        char* type =(char*)malloc(sizeof(char)*6);
        unsigned short typecode = (unsigned short)(num <<4) >>15;
        
        unsigned short rs;
        unsigned short rt;
        char* IMM11 =(char*)malloc(sizeof(char)*11);
        if(typecode == 0){ //NOP
            sprintf(type, "JSRR");
            
            rs = (unsigned short)(num <<7) >>13;
            
            sprintf(IMM11,"R%u",rs);
        }else if (typecode == 1){
            sprintf(type, "JSR");
            
            rt = (unsigned short)(num <<5) >>5;
        
            signEx(&rt, IMM11, 11);
            
        }
        
        
        sprintf(output, "%s %s",type,IMM11);
      //  printf(output, "%s %s \n",type,IMM11);
      free(type);
        free(IMM11);
    }else if (opcode == 12){ // JMP
        char* type =(char*)malloc(sizeof(char)*6);
        unsigned short typecode = (unsigned short)(num <<4) >>15;
        
        unsigned short rs;
        unsigned short rt;
        char* IMM11 =(char*)malloc(sizeof(char)*11);
        if(typecode == 0){ //NOP
            sprintf(type, "JMPR");
            
            rs = (unsigned short)(num <<7) >>13;
            
            sprintf(IMM11,"R%u",rs);
        }else if (typecode == 1){
            sprintf(type, "JMP");
            
            rt = (unsigned short)(num <<5) >>5;
        
            signEx(&rt, IMM11, 11);
            
        }
        
        
        sprintf(output, "%s %s",type,IMM11);
       // printf(output, "%s %s \n",type,IMM11);
       free(type);
        free(IMM11);
    }else if (opcode == 15){ // TRAP
        
       
        
       
        unsigned short rt;
       
        
          
            
        rt = (unsigned short)(num <<8) >>8;
            
           
        
        
        
        sprintf(output, "TRAP #%u",rt);
      //  printf(output, "TRAP #%u",rt);
    }else if (opcode == 8){ //RTI
        sprintf(output, "RTI");
      //  printf(output, "RTI");
    }
    
    return output;
}