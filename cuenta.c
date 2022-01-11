#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define READ  0
#define WRITE 1

int main () {
    int fds[2];
    int fds2[2];
    pid_t pid;

    /* Creacion de pipes */
    pipe(fds);
    pipe(fds2);

    /* Crea un proceso hijo. */
    pid = fork();
    
    if (pid == (pid_t) 0) {
        /* Proceso HIJO. Lee desde el pipe. */
        char buffer[20];
        FILE* stream;
	/* Cierra su copia del extremo de escritura. */
        close(fds[WRITE]);
        close(fds2[READ]);
		
        /* Convierte el descriptor de lectura en un objeto FILE, y lee
	 * desde el mismo. */
        stream = fdopen(fds[READ], "r");
		
        fgets(buffer, sizeof(buffer), stream);
        close(fds[READ]);
	//fputs(buffer, stdout);
	printf("Entro.... %c\n", buffer[0]);

	//Analiza cadena
        int operando1=0;
	int i;
        for (i=0; i<20; i++) {
            if(buffer[i]>='0' && buffer[i]<='9') {
               operando1 = operando1*10 + (buffer[i]-'0');
	    } else if (buffer[i]==' ') { // ignoro espacios
	       continue;
	    } else { // cualquier otra cosa corto analisis    
	       break;	    
            }
	}
	//Primer operando
	printf("Operando1.... %d\n", operando1);

	char operador;
        for (; i<20; i++) { //continuamos con el valor de i
           if(buffer[i]=='/' || buffer[i]=='+' || buffer[i]=='-' || buffer[i]=='*') {
               operador = buffer[i];
	       break;
           } else if (buffer[i]==' ') {
	       continue;	   
           } else {
               break;
	   }

	}
	//Operador
	printf("Operador.... %c\n", operador);

        int operando2=0;
	i++; //para salir del operador
        for (; i<20; i++) {  //continuamos con el valor de i
            if(buffer[i]>='0' && buffer[i]<='9') {
               operando2 = operando2*10 + (buffer[i]-'0');
            } else if (buffer[i]==' ') { // ignoro espacios
               continue;
            } else { // cualquier otra cosa corto analisis
               break;
            }
        }
	//Segundo operando
	printf("Operando2.... %d\n", operando2);

	//Se hace la operacion
	int res=0;
        switch(operador){
           case '+':
                    res = operando1+operando2;
                    break;
           case '-':
                    res = operando1-operando2;
                    break;
           case '/':
                    if(operando2==0){
                        printf("Error. No se puede dividir por 0.\n");
                           return -1;
                    }else{
                        res = operando1/operando2;
                    }
                    break;
           case '*':
                    res = operando1*operando2;
                    break;
        }
	
	printf("\nProceso hijo resuelve la operacion: resultado->%d\n\n", res);


	//Resultado
	char resultado[20];
	sprintf(resultado, "%d", res);

        //ENVIA RESULTADO AL PADRE
	printf("\nProceso hijo envia resultado al padre....\n");
	stream = fdopen(fds2[WRITE], "w");
        fprintf(stream, "%s", resultado);
        fflush(stream);
        close(fds2[WRITE]);

    } else { 
        /* Proceso PADRE. Escribe en el pipe*/
        FILE* stream;
	char buffer[20];
        /* Cierra su extremo de lectura. */
        close(fds[READ]);
        close(fds2[WRITE]);

	/* Proceso PADRE pide operacion por pantalla */
	char operacion[20];
        scanf("%[^\n]",operacion);
	strcat(operacion,"=");
		
        /* Convierte el descriptor de escritura en un objeto FILE, y 
		 * escribe en el mismo. */
        stream = fdopen(fds[WRITE], "w");
	fprintf(stream, "%s", operacion);
	fflush(stream);
        close(fds[WRITE]);

        //LEE RESULTADO DEL HIJO EN EL OTRO PIPE
        stream = fdopen(fds2[READ], "r");
        fgets(buffer, sizeof(buffer), stream);
        close(fds2[READ]);

	printf("\nRecibido del proceso hijo: \n");
        fputs(buffer, stdout);

	printf("\n\n");
    }

    return 0;
}
