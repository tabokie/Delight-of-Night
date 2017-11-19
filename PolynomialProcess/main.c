#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DOUBLE 0
#define COMPLEX 1
#define POLY 2
#define CHRLIST "ABCDEFGHIJ"
#define ADD 0
#define MUL 1
#define NUM 10

struct polyNode{
	struct polyUnit* coe;
	int pow;
	char var;
};
struct complex{
	double real;
	double imag;
};
struct polyUnit{
	int tag;
	struct polyNode* poly;
	struct complex* cpl;
	struct polyUnit* next;
};

struct treeNode{
	int value;
	struct treeNode* l;
	struct treeNode* r;
};

struct polyUnit* newPoly(); 
struct polyUnit* copyPolyUnit(struct polyUnit* plp);
struct polyUnit* copyPoly(struct polyUnit* head);
struct polyUnit* getPoly(char* str);
struct polyUnit* add(struct polyUnit* p1,struct polyUnit* p2);
struct polyUnit* mul(struct polyUnit* p1,struct polyUnit* p2);
struct polyUnit* simPoly(struct polyUnit* p);
void printPoly(struct polyUnit* p);

struct treeNode* getTree(char* str,struct polyUnit** numPoly,int* numLoc);
struct polyUnit* parseTree(struct treeNode* treeHead,struct polyUnit** polys,struct polyUnit** numPoly);

void main(){
	int polys;
	printf(">>Enter number of polynomials:[less than 10]\n");
	scanf("%d",&polys);
	
	char str1[100];
	gets(str1);
	struct polyUnit* poly[10];
	for(int i=0;i<polys;i++){
		for(int j=0;j<100;j++){
			str1[j]='\0';
		}
		gets(str1);
		poly[i]=getPoly(str1);
	}
	
	printf(">>Here are given polynomials:\n");
	printf("\n");
	for(int i=0;i<polys;i++){
		printf("%c = ",'A'+i);
		printPoly(poly[i]);
		printf("\n");
	}
	printf("\n");
	
	printf(">>Enter your reguest:[use upper-case char and + - * ^]\n");
	struct polyUnit* numPoly[20];
	int* numLoc=(int*)malloc(sizeof(int));
	*numLoc=0;
	char str3[100];
	gets(str3);
	struct polyUnit* p=parseTree(getTree(str3,numPoly,numLoc),poly,numPoly);
	printf(">>The result is:\n");
	printf("\n");
	printPoly(p);
	printf("\n");
}

struct polyUnit* newPoly(){
	struct polyUnit* newPoly=(struct polyUnit*)malloc(sizeof(struct polyUnit));
	newPoly->next=NULL;
	newPoly->poly=(struct polyNode*)malloc(sizeof(struct polyNode));
	newPoly->cpl=(struct complex*)malloc(sizeof(struct complex));
	return newPoly;
}

struct polyUnit* copyPolyUnit(struct polyUnit* plp){
	struct polyUnit* temp;
	if(plp->tag==POLY){
		temp=newPoly();
		temp->tag=POLY;
		temp->poly->pow=plp->poly->pow;
		temp->poly->var=plp->poly->var;
		temp->poly->coe=copyPoly(plp->poly->coe);
	}
	else if(plp->tag==COMPLEX){
		temp=newPoly();
		temp->tag=COMPLEX;
		temp->cpl->imag=plp->cpl->imag;
		temp->cpl->real=plp->cpl->real;
	}
	else printf("\nError\n");
	return temp;
}

struct polyUnit* copyPoly(struct polyUnit* head){
	struct polyUnit *cpyHead=newPoly(),*p;
	for(p=cpyHead;p!=NULL&&head!=NULL;p=p->next,head=head->next){
		p->tag=head->tag;
		if(head->tag==POLY){	
			p->poly->pow=head->poly->pow;
			p->poly->var=head->poly->var;
			p->poly->coe=copyPoly(head->poly->coe);
		}
		else if(head->tag==COMPLEX){
			p->cpl->imag=head->cpl->imag;
			p->cpl->real=head->cpl->real;
		}
		else printf("\nError\n");
		
		if(head->next!=NULL){
			p->next=newPoly();
		}
		else p->next=NULL;
	}
	return cpyHead;
}

struct polyUnit* getPoly(char* str){

	struct polyUnit* head=newPoly();
	struct polyUnit* plp=head;
	
	int len=strlen(str);
	char* p=str,*p2;
	char tempArray[100]={0},*temp=tempArray;
	char mark;

	if(strchr(p,':')!=NULL){
		sscanf(p,"%c:",&mark);
		p+=2;
		while(strchr(p,mark)!=NULL){
			plp->next=newPoly();
			plp=plp->next;
			p2=strchr(p,mark);
			plp->tag=POLY;
			plp->poly->var=mark;
			plp->poly->pow=atoi(p2+1);
			temp=strncpy(temp,p+1,(p2-p-2)/sizeof(char));
			plp->poly->coe=getPoly(temp);
			for(int i=0;i<=99;i++)temp[i]='\0';
			p=p2+3;
		}
		if(strchr(p,')')!=NULL){
			temp=strncpy(temp,p+1,strlen(p)-2);
			plp->next=getPoly(temp);
			for(int i=0;i<=99;i++)temp[i]=0;
		}
		return head->next;
	}	
	else{
		head->tag=COMPLEX;
		if(strchr(p,'+')!=NULL){
			head->cpl->real=(double)atof(p);
			p=strchr(p,'+')+1;
			head->cpl->imag=(double)atof(p);
		}
		else if(strchr(p,'i')!=NULL){
			head->cpl->real=0;
			head->cpl->imag=(double)atof(p);
		}
		else{
			head->cpl->real=(double)atof(p);
			head->cpl->imag=0;
		}
		return head;
	}
}

struct polyUnit* add(struct polyUnit* p1,struct polyUnit* p2){
	
	struct polyUnit* addPolyComplex(struct polyUnit* p1,struct polyUnit* p2){
		p2->tag=POLY;
		p2->poly->pow=0;
		p2->poly->var=p1->poly->var;
		p2->poly->coe=newPoly();
		p2->poly->coe->tag=COMPLEX;
		p2->poly->coe->cpl->imag=p2->cpl->imag;
		p2->poly->coe->cpl->real=p2->cpl->real;
		return add(p1,p2);
	}
	struct polyUnit* addComplexComplex(struct polyUnit* p1,struct polyUnit* p2){
		struct polyUnit* p=newPoly();
		p->tag=COMPLEX;
		p->cpl->imag=p1->cpl->imag+p2->cpl->imag;
		p->cpl->real=p1->cpl->real+p2->cpl->real;
		return p;
	}
	struct polyUnit* addPolyPolyUnit(struct polyUnit* p1,struct polyUnit* pIN){
		int pow=pIN->poly->pow;
		char var=pIN->poly->var;
		
		p1=copyPoly(p1);
		struct polyUnit* p2=pIN;
		
		struct polyUnit* p;
		if(var==p1->poly->var&&pow>p1->poly->pow){
			p2->next=p1;
			p1=p2;
		}
		else if(var==p1->poly->var&&pow==p1->poly->pow){
			p1->poly->coe=add(p1->poly->coe,p2->poly->coe);
		}
		else{
			for(p=p1;p!=NULL&&p->next!=NULL;p=p->next){
				if(p->poly->pow==pow&&p->poly->var==var){
					p->poly->coe=add(p->poly->coe,p2->poly->coe);
					break;
				}
				else if(p->poly->pow>pow&&p->next->poly->pow<pow&&p->poly->var==var){
					p2->next=p->next;
					p->next=p2;
					break;
				}
			}
			if(p->next==NULL){
				p->next=p2;
			}
		}
		return p1;
	}
	
	struct polyUnit* addUnit(struct polyUnit* p1,struct polyUnit* p2){
		int tag=p1->tag*10+p2->tag;
		switch(tag){
			case COMPLEX*10+COMPLEX:{
				return addComplexComplex(p1,p2);
				break;
			}
			case POLY*10+POLY:{
				return addPolyPolyUnit(p1,p2);
				break;
			}
			case POLY*10+COMPLEX:{
				return addPolyComplex(p1,p2);
				break;
			}
			case COMPLEX*10+POLY:{
				return addPolyComplex(p2,p1);
				break;
			}
			default:return NULL;
		}
	}
	
	struct polyUnit* head=copyPoly(p1);
	struct polyUnit* plp;
	struct polyUnit* temp;
	for(plp=p2;plp!=NULL;plp=plp->next){
		temp=copyPolyUnit(plp);
		head=addUnit(head,temp);
	}
	return head;
	
}

struct polyUnit* mul(struct polyUnit* p1,struct polyUnit* p2){
	
	struct polyUnit* mulPolyComplex(struct polyUnit* p1,struct polyUnit* p2){
		p1->poly->coe=mul(p1->poly->coe,p2);
		return p1;
	}
	struct polyUnit* mulComplexComplex(struct polyUnit* p1,struct polyUnit* p2){
		struct polyUnit* p=newPoly();
		p->tag=COMPLEX;
		p->cpl->real=(p1->cpl->real*p2->cpl->real)-(p1->cpl->imag*p2->cpl->imag);
		p->cpl->imag=(p1->cpl->real*p2->cpl->imag)+(p1->cpl->imag*p2->cpl->real);
		return p;
	}
	struct polyUnit* mulPolyPoly(struct polyUnit* p1,struct polyUnit* p2){
		if(p1->poly->var==p2->poly->var){
			p1->poly->pow+=p2->poly->pow;
			p1->poly->coe=mul(p1->poly->coe,p2->poly->coe);
		}
		else{
			p1->poly->coe=mul(p1->poly->coe,p2);
		}
		return p1;
	}
	
	struct polyUnit* mulUnit(struct polyUnit* p1,struct polyUnit* p2){
		int tag=p1->tag*10+p2->tag;
		switch(tag){
			case COMPLEX*10+COMPLEX:{
				return mulComplexComplex(p1,p2);
				break;
			}
			case POLY*10+POLY:{
				return mulPolyPoly(p1,p2);
				break;
			}
			case POLY*10+COMPLEX:{
				return mulPolyComplex(p1,p2);
				break;
			}
			case COMPLEX*10+POLY:{
				return mulPolyComplex(p2,p1);
				break;
			}
			default:return NULL;
		}
	}

	struct polyUnit* head=newPoly(),*temp1,*temp2;
	head->tag=COMPLEX;
	head->cpl->imag=0;
	head->cpl->real=0;
	struct polyUnit *plp1,*plp2;
	for(plp1=p1;plp1!=NULL;plp1=plp1->next){
		for(plp2=p2;plp2!=NULL;plp2=plp2->next){
			temp1=copyPolyUnit(plp1);
			temp2=copyPolyUnit(plp2);
			head=add(head,mulUnit(temp1,temp2));
			free(temp1);
			free(temp2);
		}
	}
	return head;	
}

struct polyUnit* simPoly(struct polyUnit* head){
	struct polyUnit* p,*temp,*temp2;

	while(head!=NULL){
		if(head->tag==COMPLEX){
			if(head->cpl->imag==0&&head->cpl->real==0)
				head=head->next;
			else break;
		}
		else if(head->tag==POLY){
			temp=simPoly(head->poly->coe);
			if(temp==NULL){
				head=head->next;
			}
			else if(head->poly->pow==0){
				temp2=head->next;
				p->next=temp;
				struct polyUnit* i;
				for(i=head;i!=NULL&&i->next!=NULL;i=i->next){
				}
				i->next=temp2;
			}
			else break;
		}
	}

	if(head!=NULL){
		for(p=head;p!=NULL&&p->next!=NULL;){
			if(p->next->tag==COMPLEX){
				if(p->next->cpl->imag==0&&p->next->cpl->real==0){
					p->next=p->next->next;
					continue;
				}
			}
			else if(p->next->tag==POLY){
				temp=simPoly(p->next->poly->coe);
				if(temp==NULL){
					p->next=p->next->next;
					continue;
				}
				else if(p->next->poly->pow==0){
					temp2=p->next->next;
					p->next=temp;
					struct polyUnit* i;
					for(i=p->next;i!=NULL&&i->next!=NULL;i=i->next){
					}
					i->next=temp2;
					p=i;
					continue;
				}
			}
			p=p->next;
		}
	}
	
	if(head!=NULL){
		struct polyUnit* complexSum=newPoly();
		complexSum->tag=COMPLEX;
		complexSum->cpl->imag=0;
		complexSum->cpl->real=0;
		while(head!=NULL&&head->tag==COMPLEX){
			complexSum->cpl->imag+=head->cpl->imag;
			complexSum->cpl->real+=head->cpl->real;
			head=head->next;
		} 
		if(head!=NULL){
			for(p=head;p!=NULL&&p->next!=NULL;){
				if(p->next->tag==COMPLEX){
					complexSum->cpl->imag+=p->next->cpl->imag;
					complexSum->cpl->real+=p->next->cpl->real;
					p->next=p->next->next;
					continue;
				}
				p=p->next;
			}
			if(complexSum->cpl->imag||complexSum->cpl->real)
				p->next=complexSum;
		}
		else{
			if(complexSum->cpl->imag||complexSum->cpl->real)
				head=complexSum;
		}
	}
	return head;
}

void printPoly(struct polyUnit* head){
	struct polyUnit* p=head;
	if(p->tag==POLY){
		printf("(");
		printPoly(p->poly->coe);
		printf(")");
		printf("%c^%d",p->poly->var,p->poly->pow);
	}
	else if(p->tag==COMPLEX){
		if(p->cpl->real)printf("%.1lf",p->cpl->real);
		if(p->cpl->real&&p->cpl->imag>0)printf("+");
		if(p->cpl->imag)printf("%.1lfi",p->cpl->imag);
		if(p->cpl->imag==0&&p->cpl->real==0)printf("0");
	}
	else printf("\nError\n");
	for(p=head->next;p!=NULL;p=p->next){
		printf("+");
		if(p->tag==POLY){
			printf("(");
			printPoly(p->poly->coe);
			printf(")");
			printf("%c^%d",p->poly->var,p->poly->pow);
		}
		else if(p->tag==COMPLEX){
			printf("(");
			if(p->cpl->real)printf("%.1lf",p->cpl->real);
			if(p->cpl->real&&p->cpl->imag>0)printf("+");
			if(p->cpl->imag)printf("%.1lfi",p->cpl->imag);
			if(p->cpl->imag==0&&p->cpl->real==0)printf("0");
			printf(")");
		}
		else printf("\nError!\n");
	}
}

struct treeNode* getTree(char* str,struct polyUnit** numPoly,int* numLoc){
	struct treeNode* head=(struct treeNode*)malloc(sizeof(struct treeNode));
	head->value=-1;
	head->l=NULL;
	head->r=NULL;
	
	int paren1=0,paren2=0;
	char tempArray[100]={0},*temp=tempArray;
	//ADD type check
	if(strcspn(str,CHRLIST)!=strlen(str))
		for(int i=0;i<strlen(str)&&str[i]!='\0';i++){
			if(str[i]=='(')paren1++;
			else if(str[i]==')')paren2++;
			else if(str[i]=='+'){
				if(paren1==paren2){
					head->value=ADD;
					
					for(int j=0;j<100;j++)tempArray[j]='\0';
					if(str[0]=='('&&str[i-1]==')')
						temp=strncpy(temp,str+1,i-2);
					else 
						temp=strncpy(temp,str,i);
					head->l=getTree(temp,numPoly,numLoc);
					
					for(int j=0;j<100;j++)tempArray[j]='\0';
					int isClosed=1;
					if(strlen(&str[i+1])>=3){
						int partParen1=0,partParen2=0;
						for(int j=i+1;j<strlen(str)-1;j++){
							if(str[j]=='(')partParen1++;
							else if(str[j]==')')partParen2++;
							if(partParen1==partParen2&&partParen1>0){
								isClosed=0;
								break;
							}
						}
					}
					else isClosed=0;
					if(isClosed)
						temp=strncpy(temp,&str[i+2],strlen(str)-i-3);
					else 
						temp=strncpy(temp,&str[i+1],strlen(str)-i-1);
					head->r=getTree(temp,numPoly,numLoc);
				}
			}
		}
	//MUL type check
	if(head->value==-1){
		if(strcspn(str,CHRLIST)!=strlen(str)){
			paren1=0;
			paren2=0;
			for(int i=0;i<strlen(str)&&str[i]!='\0';i++){
				if(str[i]=='(')paren1++;
				else if(str[i]==')')paren2++;
				else if(str[i]=='*'){
					if(paren1==paren2){
						head->value=MUL;
						
						for(int j=0;j<100;j++)tempArray[j]='\0';
						if(str[0]=='('&&str[i-1]==')')
							temp=strncpy(temp,str+1,i-2);
						else 
							temp=strncpy(temp,str,i);
						head->l=getTree(temp,numPoly,numLoc);
						
						for(int j=0;j<100;j++)tempArray[j]='\0';
						if(str[i+1]=='('&&str[strlen(str)-1]==')')
							temp=strncpy(temp,&str[i+2],strlen(str)-i-3);
						else 
							temp=strncpy(temp,&str[i+1],strlen(str)-i-1);
						head->r=getTree(temp,numPoly,numLoc);
					}
				}
				else if(str[i]=='^'){
					if(paren1==paren2){
						head->value=MUL;
						
						for(int j=0;j<100;j++)tempArray[j]='\0';
						strncpy(temp,str,i);
						int powerNum=atoi(&str[i+1]);
						for(int j=1;j<powerNum;j++){
							strcat(temp,"*");
							strncat(temp,str,i);	
						}
						if(strspn(&str[i+1],"0123456789")<strlen(&str[i+1]))
							strcat(temp,&str[i+strspn(&str[i+1],"0123456789")+1]);
						
						char tempArray2[100]={0},*temp2=tempArray2;
						if(temp[0]=='('&&temp[i-1]==')')
							temp2=strncpy(temp2,temp+1,i-2);
						else 
							temp2=strncpy(temp2,temp,i);
						head->l=getTree(temp2,numPoly,numLoc);
						
						for(int j=0;j<100;j++)tempArray2[j]='\0';
						temp2=strncpy(temp2,&temp[i+1],strlen(temp)-i-1);
						head->r=getTree(temp2,numPoly,numLoc);
					}	
				}	
			}	
		}
		//NUM or VAR type check
		if(head->value==-1){
			if(strcspn(str,CHRLIST)==strlen(str)){
				head->value=NUM+(*numLoc);
				head->l=NULL;
				head->r=NULL;
				numPoly[*numLoc]=newPoly();
				numPoly[*numLoc]->tag=COMPLEX;
				numPoly[*numLoc]->cpl->real=(double)atof(str);
				if(strcspn(str,"+-")!=strlen(str)){
					numPoly[*numLoc]->cpl->imag=(double)atof(str+strcspn(str,"+-"));
				}
				else{
					numPoly[*numLoc]->cpl->imag=0;
				}	
				*numLoc++;	
			}
			else{
				if(strlen(str)==1){
					if(str[0]-'A'+2<10){
						head->value=str[0]-'A'+2;
						head->l=NULL;
						head->r=NULL;	
					}
					else{
						printf("\nInvalid Char Error!\n");
						head=NULL;
					}
				}
				else{
					printf("\nInvalid Char Error!\n");
					head=NULL;
				}
			}
		}
	}
	
	return head; 
}

struct polyUnit* parseTree(struct treeNode* treeHead,struct polyUnit** poly,struct polyUnit** numPoly){
	if(treeHead==NULL)return NULL;
	switch(treeHead->value){
		case ADD:{
			if(parseTree(treeHead->l,poly,numPoly)==NULL||parseTree(treeHead->r,poly,numPoly)==NULL){
				printf("\nNo Poly To Add Error!\n");return NULL;
			}
			else{
				return simPoly(add(parseTree(treeHead->l,poly,numPoly),parseTree(treeHead->r,poly,numPoly)));
			}
			break;
		}
		case MUL:{
			if(parseTree(treeHead->l,poly,numPoly)==NULL||parseTree(treeHead->r,poly,numPoly)==NULL){
				printf("\nNo Poly To Mul Error!\n");return NULL;
			}
			else{
				return simPoly(mul(parseTree(treeHead->l,poly,numPoly),parseTree(treeHead->r,poly,numPoly)));
			}
			break;
		}
		default:{
			if(treeHead->l!=NULL||treeHead->r!=NULL){
				printf("\nNot Empty After Poly Error!\n");return NULL;
			}
			if(treeHead->value<NUM)return poly[treeHead->value-2];
			else return numPoly[treeHead->value-NUM];
			break;
		}
	}
}
