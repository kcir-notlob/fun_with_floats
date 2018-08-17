//fun with the IEEE754 spec (float)
#include <stdio.h>
#include <math.h>


typedef struct parts{
  char sign;
  char exp[8];    
  char fract[23];
} Parts;

typedef struct raw{
  char data[32];
} Raw;


typedef union uparts{
  Parts part;
  Raw  raw; 
} UParts;

typedef UParts* pUParts;


int exp2Int(UParts parts){
  int mask=0x80;
  int total=0;

  for(int i=0;i<8;i++){
    if(parts.part.exp[i]=='1'){
      total+=mask;
    }
    mask=mask >> 1;
  }
  return total;

}

double  fract2Int(UParts parts){

  double total=0.0;
  unsigned int mask=0x02;

  for(int i=0;i<23;i++){
    if(parts.part.fract[i]=='1'){
      double t=((double)1.0/(double)mask);
      total+=t;
      printf("fract:%f total:%f mask:%i\n",t,total,mask);
    }
    mask=mask << 1;
  }
  return total;

}

void convert(unsigned int x,pUParts parts){

  for(int i=0;i<32;i++){
    parts->raw.data[i]=(x & 0x80000000) >> 31;
    x=x << 1;
    if(parts->raw.data[i]==0){
      parts->raw.data[i]='0';
    }
    else{
      parts->raw.data[i]='1';
    }

  }
}


void print(pUParts parts){
  printf("[%c]",parts->part.sign);
  printf("[");
  for(int i=0;i<8;i++){
    printf("%c",parts->part.exp[i]);
  }
  printf("]");

  printf("[");
  for(int i=0;i<23;i++){
    printf("%c",parts->part.fract[i]);
  }
  printf("]\n");

}



int main(){
  float a=0.1;
  unsigned int *b;
  b=&a;
  unsigned int c=*b;

  UParts bits2;

  convert(c,&bits2);
  print(&bits2);

  int exp=exp2Int(bits2);
  printf("exp:%i\n",exp);
  printf("fixed bias:127\n");
  int realexp=exp-127;
  printf("real exp:%i\n",realexp);
  double  fract=fract2Int(bits2);
  printf("fract:%f\n",fract);
  double realFract=1.0 + fract;
  printf("real fract:%f\n",realFract);  
 
  double num=pow(2,realexp) * realFract;
  printf("number:%f\n",num);
 

  return 0;
}
