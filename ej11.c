#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ 0	//LEER
#define WRITE 1	//ESCRIBIR
#define MAX 3

int main(){

	int fds1[2], fds2[2];
	
	pid_t pid;
	//int opc = 1;
	char oper[MAX];

	//Operacion aritmetica separada por porciones
	int ent1=0; int ent2=0;
	char op='\0';
	int res=0;

	//Se usa para obtener operando y entero
	char *p = NULL;
	char tmp[MAX];
	int i=0;

	//while(opc==1){

	pipe(fds1);
	pipe(fds2);

	pid = fork();

	if(pid != (pid_t) 0){

		//PROCESO PADRE
		
		//1. El padre solicita al usuario el ingreso de una op aritm.

		printf("\nIngrese una operacion aritmetica en la forma 'ent op ent':\n-> ");
		scanf("%[^\n]",oper);

		close(fds1[READ]);
		close(fds2[WRITE]);

		write(fds1[WRITE],oper,sizeof(oper)+1);
		close(fds1[WRITE]);

		//3. El resultado es devuelto al padre en otro pipe

		read(fds2[READ],oper,sizeof(oper)+1);

		//4. Muestra el resultado por pantalla

        	printf("oper: %s\n",oper);

		close(fds2[READ]);

	} else {

		//PROCESO HIJO

		//2. El hijo recibe la operacion mediante un pipe y lo resuelve

		close(fds1[WRITE]);
		read(fds1[READ],oper,sizeof(oper));
		ent1 = atoi(oper);

		p = oper;
		while(*p != '\0'){
			if(*p == '/' || *p == '+' || *p == '-' || *p == '*'){
				op = *p;
				while(*p != '\0'){
					if(*p >= 48 && *p <= 57){
						tmp[i]=*p;
						i++;
					}
					p++;
				}
				ent2 = atoi(tmp);
				break;
			}
		}

		close(fds1[READ]);


		//Calculo
		switch(op){
		case '+':
			res = ent1+ent2;
			break;
		case '-':
			res = ent1-ent2;
			break;
		case '/':
			if(ent2==0){
				printf("Error. No se puede dividir por 0.\n");
				return -1;
			}else{
				res = ent1/ent2;
			}
			break;
		case '*':
			res = ent1*ent2;
			break;
		default:
			printf("Cualquier cosa hay ahi flaco");
			return -2;
		}
		
		//Corroboro resultados
		printf("ent1: %d / op: %c / ent2: %d\n",ent1,op,ent2);
		printf("resultado: %d\n",res);

		close(fds2[READ]);
		sprintf(oper,"%d",res);
		write(fds2[WRITE],oper,sizeof(oper));

		close(fds2[WRITE]);

		//exit(0);
	}
	
	//printf("Desea ingresar una nueva operacion? (SI: 1 / NO:0):\n-> ");
	//scanf("%d",opc);
	//}

	return 0;
}
