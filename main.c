#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#define nn 0

#define rb 1
#define rw 2
#define ib 3
#define iw 4
#define mb 5

#define equ 1
#define db  2

#define TEXTLEN 256

char *p,*auxp;
int numbytes=0;

char str[TEXTLEN];
char sdata[TEXTLEN];
char numero[17];
char cmdbin[35];

char codigo[1000];
char coddata[500];

int cdatabytes = 0;
int idata = 0;
int jmp;

bool zona1 = false;
bool zona2 = false;
bool zona3 = false;
bool zona4 = false;
bool zona5 = false;
bool zona6 = false;
bool zona7 = false;

//funcion que elimina espacios hacia la derecha
void rtrim(char *str)
{
    size_t n;
    n = strlen(str);
    while (n > 0 && (isspace((unsigned char)str[n - 1]) || (unsigned char)str[n - 1] == ' ')) {
        n--;
    }
    str[n] = '\0';
}

//funcion que elimina espacios hacia la izquierda
void ltrim(char *str)
{
    size_t n;
    n = 0;
    while (str[n] != '\0' && isspace((unsigned char)str[n])) {
        n++;
    }
    memmove(str, str + n, strlen(str) - n + 1);
}

//funcion que elimina espacios hacia la izquierda y derecha
void trim(char *str){
    rtrim(str);
    ltrim(str);
}

//funcion que convierte a mayusculas
void string_toupper(int num){
    int i;
    for(i=0;i<num;i++)
        p[i]=toupper(p[i]);
    auxp=p;
    p=&p[i];
}

typedef struct t_cmddata{
    char    *name;
    char    val1;
    char    val2;
    char    *opcode;
} t_cmddata;

t_cmddata *pd;

typedef struct t_asmoperand{
    char    name[20];
    char    val1;
    char    bin1[20];
    char    val2;
    char    bin2[20] ;
} t_asmoperand;

t_asmoperand op;

typedef struct t_data{
    char    data[20];
    int     type;
    int     value;
    char    var;
} t_data;

t_data datos[20];

//diccionario
t_cmddata cmddata[] = {
    {"MOV",rb,rb,"1000101011"},
    {"MOV",rb,mb,"1000101000"},
    {"MOV",rw,rw,"1000101111"},
    {"MOV",rb,ib,"10110"},
    {"MOV",rw,ib,"10111"},
    {"MOV",rw,iw,"10111"},
    {"MOV",mb,rb,"1000100000"},

    {"ADD",rb,rb,"0000001011"},
    {"ADD",rw,rw,"0000001111"},
    {"ADD",rb,ib,"1000000011000"},
    {"ADD",rw,ib,"000"},
    {"ADD",rw,iw,"000"},

    {"SUB",rb,rb,"0010101011"},
    {"SUB",rw,rw,"0010101111"},
    {"SUB",rb,ib,"1000000011101"},
    {"SUB",rw,ib,"101"},
    {"SUB",rw,iw,"101"},

    {"MUL",rb,nn,"1111011011100"},
    {"MUL",rw,nn,"1111011111100"},

    {"DIV",rb,nn,"1111011011110"},
    {"DIV",rw,nn,"1111011111110"},

    {"INT",ib,nn,"11001101"},

    {NULL,nn,nn,NULL}
};

char *regname[3][8] = {
  { "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"},
  { "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"},
  { "000", "001", "010", "011", "100", "101", "110", "111"} };

int scanasm(){
    int i=0;
    sdata[0]='\0';
    while(isblank(*p))p++;
    while(isalnum(*p))sdata[i++]=*p++;
    sdata[i]='\0';
    if(sdata[0]=='\0')return 1;
    while(isblank(*p))p++;
    strupr(sdata);
    return 0;
}

void strcp(char *s, char *t, int a){
    int i;
	for(i=0;i<a;i++)
		*s++ = *t++;
}

int digittobin(int num){
    if(num > 65535)return 1;

    int i=0;
    while(num!=0){
         numero[i++] = (num % 2)+48;
         num = num / 2;
    }
    numero[i]='\0';
    strrev(numero);
    return 0;
}

int scannum(){
    int i,j;
    numero[0]= '\0';
    i=strlen(sdata);
    if(i>5)return 1;
    if(sdata[i-1] == 'H'){   //Es hexadecimal
        for(j=0;j<i-1;j++)
            if(!isxdigit(sdata[j]))
                break;
        if(j != i-1)return 1;

        for(j=0;j<i-1;j++){
            switch(sdata[j]){
                case '0': strcat(numero,"0000"); break;
                case '1': strcat(numero,"0001"); break;
                case '2': strcat(numero,"0010"); break;
                case '3': strcat(numero,"0011"); break;
                case '4': strcat(numero,"0100"); break;
                case '5': strcat(numero,"0101"); break;
                case '6': strcat(numero,"0110"); break;
                case '7': strcat(numero,"0111"); break;
                case '8': strcat(numero,"1000"); break;
                case '9': strcat(numero,"1001"); break;
                case 'A': strcat(numero,"1010"); break;
                case 'B': strcat(numero,"1011"); break;
                case 'C': strcat(numero,"1100"); break;
                case 'D': strcat(numero,"1101"); break;
                case 'E': strcat(numero,"1110"); break;
                case 'F': strcat(numero,"1111"); break;
                case 'a': strcat(numero,"1010"); break;
                case 'b': strcat(numero,"1011"); break;
                case 'c': strcat(numero,"1100"); break;
                case 'd': strcat(numero,"1101"); break;
                case 'e': strcat(numero,"1110"); break;
                case 'f': strcat(numero,"1111"); break;
            }
        }
    }else{                              //Es decimal
        for(j=0;j<i;j++)
            if(!isdigit(sdata[j]))
                break;
        if(j != i)return 1;

        int num =(int)atoi(sdata);
        if(digittobin(num))return 1;
    }
    return 0;
}

void swap16(){
    char auxnumero[8];
    strncpy(auxnumero,numero,8);
    memmove(numero,numero+8,8);
    memmove(numero+8,auxnumero,8);
}

int readdata(){
    int i;
    char comp = '\0';
    bool coma =true;
    while(isblank(*p))p++;
    if(*p == '\0')return 1;
    while(*p != '\0'){

        if(*p == ',' && comp == '\0'){
           p++;
           coma = true;
           while(isblank(*p))p++;
        }
        else if(isdigit(*p) && comp == '\0' && coma){
            //numero
            i=0;
            sdata[0]='\0';
            while(isdigit(*p))sdata[i++]=*p++;
            sdata[i]='\0';
            int num =atoi(sdata);
            if(num > 255)return 1;
            if(datos[idata].type == equ)
                datos[idata].var = num;
            else
                coddata[cdatabytes++] = num;
            coma = false;
        }else if(isalpha(*p) && comp == '\0' && coma){
            //dato
            i=0;
            sdata[0]='\0';
            while(isalnum(*p))sdata[i++]=*p++;
            sdata[i]='\0';

            for(i=0;i<idata;i++)
                if(!strcmp(datos[i].data,sdata))
                    break;

            if(i==idata || datos[i].type == db)return 1;

            coddata[cdatabytes++] = datos[i].var;
            coma = false;
        }else if(*p== '\'' || *p == '\"'){
            if(comp == '\0'){
                if(!coma) return 1;
                comp = *p;
            }else
                coddata[cdatabytes++] = *p;
            p++;
        }else if(comp == '\'' || comp == '\"'){
            //cadena
            while(*p != '\0'){
                if(*p == comp)break;
                coddata[cdatabytes++]=*p++;
            }

            if(*p == '\0') return 1;
            comp = '\0';
            coma=false;
            p++;
            while(isblank(*p))p++;
        }else return 1;
    }
    return 0;
}

int chekoffset(){
    int i,j,tam;
    for(i=0;i<idata;i++)
        if(!strcmp(datos[i].data,sdata)){
            if(datos[i].type == db){
                if(digittobin(datos[i].value+jmp))return 1;
            }else{
                if(digittobin(datos[idata].var))return 1;
            }

            op.val2=iw;
            tam=strlen(numero);
            memmove(numero+(16-tam),numero,tam);
            for(j=0;j<16-tam;j++)
            numero[j]='0';
            numero[16]='\0';
            swap16();
            strcpy(op.bin2,numero);
            break;
        }
    if(i==idata)return 1;

    return 0;
}

int chekval(int opc){
    int i,j;
    for(i=0;i<idata;i++)
        if(!strcmp(datos[i].data,sdata)){
            if(datos[i].type == db){
                if(digittobin(datos[i].value+jmp))return 1;
                i=strlen(numero);
                if(i>8 && i <16){
                    memmove(numero+(16-i),numero,i);
                    for(j=0;j<16-i;j++)
                        numero[j]='0';
                    numero[16]='\0';
                }
                swap16();
                if(opc == 1){
                    op.val1 = mb;
                    strcpy(op.bin1,"110");
                    strcat(op.bin1,numero);
                }else{
                    op.val2 = mb;
                    strcpy(op.bin2,"110");
                    strcat(op.bin2,numero);
                }

            }else{
                if(digittobin(datos[i].var))return 1;
                i=strlen(numero);
                if(i <8){
                    memmove(numero+(8-i),numero,i);
                    for(j=0;j<8-i;j++)
                        numero[j]='0';
                    numero[8]='\0';
                }
                op.val2 = ib;
                strcat(op.bin2,numero);
            }
            break;
        }
    if(i==idata)return 1;

    return 0;
}

void bin(){
    int i,j;
    int tamcad= strlen(p);
    int numb = tamcad/8;
    int num;

    char codebyte[9];
    char code[numb];

    for(i=0;i<tamcad;i += 8){
        num=0;
        memmove(codebyte,p+i,8);
        codebyte[8]='\0';
        strrev(codebyte);
        for(j=0;j<8;j++){
            if(codebyte[j]== '1'){
                num += pow(2,j);
            }
        }
        code[i/8] = num;
    }
    strcp(codigo+numbytes,code,numb);
    numbytes += numb;
    for(i=0;i<numb;i++)
        printf("%02X", ((unsigned char *)code)[i]);

    if(zona5){
        if(numb<4)
            printf("\t\t");
        else
            printf("\t");
    }
    else
        printf("\n\n");
}

int main(int argc,char *argv[])
{
    int line=0;
    codigo[0] = '\0';
    int i,j;
    bool fin;

    FILE *fpOut;
    FILE *fpIn;

    if (argc==1){
        printf("\n               UNIVERSIDAD DE COLIMA\n\n");
        printf("     FACULTAD DE INGENIERIA MECANICA Y ELECTRICA\n");
        printf("           ING. EN SISTEMAS COMPUTACONALES\n\n");
        printf("          CURSO: COMPILADORES Y TRADUCTORES\n");
        printf("          MAESTRO: BENJAMIN IGLESIAS CORTES\n\n");
        printf("        ALUMNO Y AUTOR: EDUARDO AVELAR PANTOJA\n\n");
        printf("                                     FECHA: MAYO/2015\n");
        printf("-----------------------------------------------------\n");
        printf(" OpenASM 1.0\n");
        printf(" Instrucciones soportadas:\n MOV,ADD,SUB,MUL,DIV y INT\n");
        printf("-----------------------------------------------------\n");
        printf(" Para compilar \n");
        printf(" Entrada:\n");
        printf(" OpenASM NombreArchivo.asm\n\n");
        printf(" Archivo de salida:\n");
        printf(" NombreArchivo.com\n");

        return 0;
    }if (argc > 2){
        printf("Argumentos de mas \n");
        return 0;
    }

    if((fpIn  = fopen(argv[1],"r")) == NULL ){
        printf("No pudo abrirse el Archivo %s \n",argv[1]);
        return 0;
    }

    int tam = strlen(argv[1]);
    for(i=0;i<tam;i++)
        if(argv[1][i] == '.')break;

    if(argv[1][i] != '.'){
        printf("El Archivo %s no tiene extension\n",argv[1]);
        return 0;
    }

    char name[tam];
    strcpy(name,argv[1]);
    p = strrchr(name,'.');
    strcpy(p,".com");

    if((fpOut = fopen(name,"wb")) == NULL){
        printf("No pudo crearse el Archivo %s \n",argv[1]);
        return 0;
    }

    printf("\n        CODIGO MAQUINA\n\nInstruccion de salto: ");
    while (fgets(str, TEXTLEN, fpIn)) {
        line++;
        fin = false;

        char comp = '\0';
        int a = strlen(str);
        for(i=0;i<a;i++){        //Elimino comentario
            if(str[i] == ';' && comp == '\0'){
                str[i] = '\0';
                break;
            }else if(str[i] == '\'' || str[i] == '\"' ){
                if(comp == '\0')
                    comp = str[i];
                else if(comp == str[i])
                    comp = '\0';
            }
        }
        trim(str);              //Elimino espacios a los lados
        p=str;

        if(!*p);                    //si es un salto de linea no hace nada
        else if(*p == ';');         //si es un comentario no hace nada
        else if(!zona1){
            string_toupper(6);
            if (!strncmp(auxp,".MODEL",6)){
                if(!isblank(*p))break;
                ltrim(p);
                string_toupper(4);
                if (!strncmp(auxp,"TINY",4)){
                    ltrim(p);
                    if(!(*p==0 || *p==';'))
                        break;
                }else break;
            }else break;
            zona1=true;
        }
        else if(zona1 && !zona2){
            string_toupper(5);
            if(!strncmp(auxp,".DATA",5)){
                ltrim(p);
                if(!(*p==0 || *p==';'))
                    break;
            }else if(!strncmp(auxp,".CODE",5)){
                ltrim(p);
                if(!(*p==0 || *p==';'))
                    break;
                zona3=true;
                zona4=true;
            }else break;
            zona2=true;
        }
        else if(zona2 && !zona3){
            string_toupper(5);
            if(!strncmp(auxp,".CODE",5)){
                zona3=true;
                zona4=true;
                char inicio[25];
                inicio[0] = '\0';

                digittobin(cdatabytes);
                int tam=strlen(numero);

                if(cdatabytes>126){           //jmp code
                    strcat(inicio,"11101001");
                    if(tam<16){
                        memmove(numero+(16-tam),numero,tam);
                        for(i=0;i<16-tam;i++)
                        numero[i]='0';
                        numero[16]='\0';
                    }
                    jmp=3;
                    swap16();
                }else{
                    strcat(inicio,"11101011");
                    if(tam<8){
                        memmove(numero+(8-tam),numero,tam);
                        for(i=0;i<8-tam;i++)
                        numero[i]='0';
                        numero[8]='\0';
                    }
                    jmp=2;
                }

                strcat(inicio,numero);
                p=inicio;
                bin();

                strcp(codigo+jmp,coddata,cdatabytes);
                numbytes += cdatabytes;
            }else{
                //analizo datos
                p=auxp;
                if(isalpha(*p)){
                    scanasm();
                    if(!idata)
                        strcpy(datos[idata].data,sdata);
                    else{
                        for(i=0;i<idata;i++)
                            if(!strcmp(datos[i].data,sdata))
                                break;

                        if(i!=idata)break;

                        for (pd=cmddata; pd->name!=NULL; pd++){
                            if(!strcmp(pd->name,sdata))break;
                        }
                        if(pd->name) break;
                        strcpy(datos[idata].data,sdata);
                    }

                    ltrim(p);
                    if(isalpha(*p)){
                        scanasm();
                        if(!strcmp(sdata,"DB")){
                            datos[idata].type = db;
                        }else if(!strcmp(sdata,"EQU")){
                            datos[idata].type = equ;
                        }else break;

                        datos[idata].value=cdatabytes+256;

                        if(readdata())break;

//                        printf("%s\n",datos[idata].data);

//                        for(i=datos[idata].value-256;i<cdatabytes;i++)
//                            printf("%02X", ((unsigned char *)coddata)[i]);
//                         printf("\n");
                        idata++;
                    }else break;
                }else break;
            }
        }
        else if(zona4 && !zona5){
            string_toupper(3);
            if (!strncmp(auxp,"ORG",3)){
                if(!isblank(*p))break;
                ltrim(p);
                string_toupper(4);
                if (!strncmp(auxp,"100H",4)){
                    ltrim(p);
                    if(!(*p==0 || *p==';'))
                        break;
                }else break;
            }else break;
            zona5=true;
        }else if(zona5 && !zona6){
            string_toupper(7);
            if(!strncmp(auxp,"INICIO:",7)){
                ltrim(p);
                if(!(*p==0 || *p==';'))
                    break;
            }else break;
            zona6=true;
        }else if(zona6 && !zona7){
            string_toupper(3);
            if(!strncmp(auxp,"END",3)){
                if(!isblank(*p))break;
                ltrim(p);
                string_toupper(6);
                if (!strncmp(auxp,"INICIO",6)){
                    ltrim(p);
                    if(!(*p==0 || *p==';'))
                        break;
                    zona7=true;
                }else break;
            }else{
            //analizo valores
                op.val1 = nn;
                op.bin1[0] = '\0';
                op.val2 = nn;
                op.bin2[0] = '\0';
                p=auxp;

                if(scanasm())break;
                for (pd=cmddata; pd->name!=NULL; pd++){
                    if(!strcmp(pd->name,sdata))break;
                }
                if(!pd->name) break;
                strcpy(op.name,sdata);

                if(isalnum(*p)){            //Obtengo val1
                    if(scanasm())break;
                    for(i=0;i<8;i++)
                        if(!strcmp(sdata,regname[0][i])){
                            op.val1=rb;
                            strcpy(op.bin1,regname[2][i]);
                            break;
                        }
                    if(!op.val1)
                        for(i=0;i<8;i++)
                        if(!strcmp(sdata,regname[1][i])){
                            op.val1=rw;
                            strcpy(op.bin1,regname[2][i]);
                            break;
                        }
                    if(!op.val1){
                        if(!scannum()){
                            i=strlen(numero);
                            if(i <8){
                                memmove(numero+(8-i),numero,i);
                                for(j=0;j<8-i;j++)
                                    numero[j]='0';
                                numero[8]='\0';
                                op.val1 = ib;
                            }else if(i==8){
                                op.val1 = ib;
                            }else if(i>8 && i <16){
                                memmove(numero+(16-i),numero,i);
                                for(j=0;j<16-i;j++)
                                    numero[j]='0';
                                numero[16]='\0';

                                swap16();
                                op.val1 = iw;
                            }else{
                                op.val1 = iw;
                            }
                            strcpy(op.bin1,numero);
                        }
                    }

                    if(!op.val1)
                        chekval(1);

                    if(!op.val1)break;

                    if(*p == ','){          //Obtengo val2
                        p++;
                        while(isblank(*p))p++;
                        if(isalnum(*p)){
                            if(scanasm())break;
                            for(i=0;i<8;i++)
                                if(!strcmp(sdata,regname[0][i])){
                                    op.val2=rb;
                                    strcpy(op.bin2,regname[2][i]);
                                    break;
                                }
                            if(!op.val2)
                                for(i=0;i<8;i++)
                                if(!strcmp(sdata,regname[1][i])){
                                    op.val2=rw;
                                    strcpy(op.bin2,regname[2][i]);
                                    break;
                                }
                            if(!op.val2)
                                if(!strcmp(sdata,"OFFSET")){
                                    if(!scanasm()){
                                        if(chekoffset())break;
                                    }else break;
                                }
                            if(!op.val2){
                                chekval(2);
                            }
                            if(!op.val2){
                                if(!scannum()){
                                    i=strlen(numero);
                                    if(i <8){
                                        memmove(numero+(8-i),numero,i);
                                        for(j=0;j<8-i;j++)
                                            numero[j]='0';
                                        numero[8]='\0';
                                        op.val2 = ib;
                                    }else if(i==8){
                                        op.val2 = ib;
                                    }else if(i>8 && i <16){
                                        memmove(numero+(16-i),numero,i);
                                        for(j=0;j<16-i;j++)
                                            numero[j]='0';
                                        numero[16]='\0';

                                        op.val2 = iw;
                                        swap16();

                                    }else{
                                        swap16();
                                        op.val2 = iw;
                                    }
                                    strcpy(op.bin2,numero);
                                }
                            }
                            if(!op.val2 || *p != '\0') break;
                        }
                    }
                }

                //Analisis de instruciones
                for (pd=cmddata; pd->name!=NULL; pd++){
                    if(!strcmp(pd->name,op.name) && pd->val1 == op.val1 && pd->val2 == op.val2)break;
                }
                if(!pd->name) break;

                cmdbin[0]='\0';
                bool _opcode    = false;
                bool _bin1      = false;
                bool _bin2      = false;
                bool _rev       = false;

                //casos especiales
                if(!strcmp(op.name,"ADD") && !strcmp(op.bin1,"000") && op.val1 == rb && op.val2 == ib){
                	strcat(cmdbin,"00000100");
                	_opcode = true;
                	_bin1	= true;
				}else if(!strcmp(op.name,"SUB") && !strcmp(op.bin1,"000") && op.val1 == rb && op.val2 == ib){
                    strcat(cmdbin,"00101100");
                	_opcode = true;
                	_bin1	= true;
				}else if(op.val1 == rw && (!strcmp(op.name,"ADD") || !strcmp(op.name,"SUB")) &&
                        (op.val2 == ib || op.val2 == iw)){
                    if(!strcmp(op.bin1,"000") && !strcmp(op.name,"ADD")){
                        strcat(cmdbin,"00000101");
                        _opcode = true;
                        _bin1	= true;
                    }else if(!strcmp(op.bin1,"000") && !strcmp(op.name,"SUB")){
                        strcat(cmdbin,"00101101");
                        _opcode = true;
                        _bin1	= true;
                    }else{
                        int addnum = 0;
                        int addtam = strlen(op.bin2);

                        char auxadd[17];
                        char auxnumero[8];

                        strcpy(auxadd,op.bin2);

                        if(op.val2 == iw){
                            strncpy(auxnumero,auxadd,8);
                            memmove(auxadd,auxadd+8,8);
                            memmove(auxadd+8,auxnumero,8);
                        }

                        strrev(auxadd);

                        for(i=0;i<addtam;i++){
                            if(auxadd[i]== '1'){
                                addnum += pow(2,i);
                            }
                        }

                        if(addnum < 128){
                            _bin2 = true;
                            strcat(cmdbin,"1000001111");
                        }else if(addnum >= 128 && addnum < 65408)
                            strcat(cmdbin,"1000000111");
                        else if(addnum >= 65408){
                            op.bin2[8]='\0';
                            strcat(cmdbin,"1000001111");
                        }
                    }
                }else if(op.val1==mb)
                    _rev = true;

                if(!_opcode)
                    strcat(cmdbin,pd->opcode);

                if(_rev){
                    char temp[20];
                    strcpy(temp,op.bin2);
                    strcpy(op.bin2,op.bin1);
                    strcpy(op.bin1,temp);
                }

                if(op.val1 && !_bin1)
                    strcat(cmdbin,op.bin1);

                if(!_bin2 && op.val1 == rw && op.val2 == ib)
                    memmove(op.bin2+8,"00000000\0",9);

                if(op.val2)
                    strcat(cmdbin,op.bin2);

                p=cmdbin;
                bin();
                printf("%s\n",auxp);
            }
        }else break;

        fin = true;
    }

    fclose(fpIn);

    if(fin && zona7){

        fwrite(codigo,1,numbytes,fpOut);
        printf("\nCompilado\n");
    }else
        printf("Error en linea %d",line);

    fclose(fpOut);

    return 0;
}


