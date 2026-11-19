//Programação Imperativa - linguagem C.
//PjBL-2: criptografar e descriptografar.
//Desenvolvedoras: Andréa Kiew Kozicki; Beatriz Tozzi Ferraz; Rafaela Uto.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Criptografa a mensagem do usuário:
void criptografar(){

    FILE *msg_cripto;
    FILE *chave_cripto;
    char frasesec[1000];
    int i, msg_tamanho, tam_chave;
    int *chave;
    int contador = 0;
    int comando;

    //Usuário insere a mensagem:
    printf("\n=*=*=*=*=*=*=* CRIPTOGRAFAR *=*=*=*=*=*=*=");
    getchar();
    printf("\nDigite o texto a ser criptografado: ");
    gets(frasesec);

    msg_cripto = fopen("arquivo-cripto.txt","w");
    if(msg_cripto == NULL){

	printf("Erro ao criar arquivo da mensagem criptografada\n");
	return;
    }

    chave_cripto = fopen("arquivo-chave.txt","w");
    if(chave_cripto == NULL){

	printf("Erro ao criar arquivo da chave\n");
	return;
    }

    srand(time(NULL));

    //Geração aleatótia do tamanho da chave:
    tam_chave = (rand() % (7 - 3 + 1)) + 3;
    fprintf(chave_cripto,"%d\n", tam_chave);

    chave = (int *) malloc (tam_chave*sizeof(int));

    //Geração aleatória da chave:
    for(i = 0; i < tam_chave; i++){
            chave[i] = rand() % 127;
            fprintf(chave_cripto,"%d\n", chave[i]);
    }

    fclose(chave_cripto);

    //Tamanho da mensagem inserida pelo usuário:
    msg_tamanho = strlen(frasesec);

    //Criptografando a mensagem
    for(i=0; i<msg_tamanho;i++){
            if(contador == tam_chave){
                contador = 0;
            }
            frasesec[i] = frasesec[i] + chave[contador];
            contador += 1;
            fprintf(msg_cripto,"%c", frasesec[i]);
    }

    fclose(msg_cripto);

    recomecar();     //Exibe opções do que o usuário deseja realizar
}


//Descriptografa amensagem do usuário
void descriptografar(){
    FILE *msg_cripto;
    FILE *chave_cripto;
    char frasesec[30] ;
    int i, msg_tamanho, tam_chave;
    int *chave;
    int contador = 0;

    //Leitura arquivos:
        msg_cripto = fopen("arquivo-cripto.txt","r");
        chave_cripto = fopen("arquivo-chave.txt","r");

    //Leitura do tamanho da chave:
        fscanf(chave_cripto,"%d\n",&tam_chave);
        chave = (int *) malloc (tam_chave*sizeof(int));

    //Leitura do vetor da chave:
        for(i=0;i<tam_chave;i++){
                fscanf(chave_cripto,"%d\n",&chave[i]);
        }

    //Leitura da mensagem criptografada:
        fscanf(msg_cripto, "%s",&frasesec);

    //Variavel pega o tamanho da mensagem:
        msg_tamanho = strlen(frasesec);

    //Descriptografar:
    for(i=0;i<msg_tamanho;i++){
            if(contador == tam_chave){
                contador = 0;
            }
            frasesec[i] = frasesec[i] - chave[contador];
            contador += 1;
    }

        fclose(msg_cripto);
        printf("\n=*=*=*=*=*=*=* DESCRIPTOGRAFAR *=*=*=*=*=*=*=");
        printf("\nDE-Codificado: %s\n", frasesec);

        recomecar();     //Exibe opções do que o usuário deseja realizar
}

//Exibe o menu para o usuário:
void menu(comando){
        printf("=*=*=*=*=*=*=* MENU *=*=*=*=*=*=*=");
        printf("\nDigite 1 para criptografar uma mensagem ou 2 para descriptografar a ultima mensagem: ");
        scanf("%d", &comando);

    //Enquanto o usuário digitar um valor válido, a mensagem não poderá ser criptografada ou descriptografada:
        while(comando!=1 && comando!=2){
                printf("Valor invalido! Digite 1 para criptografar a mensagem ou 2 para descriptografar a ultima mensagem: ");
                scanf("%d", &comando);
        }

        if (comando == 1){                    //Criptografa mensagem digitada pelo usuário
                criptografar();
        }
        else if(comando == 2){                //Descriptografa a última mensagem digitada
                descriptografar();
        }
        else {                                //Exibe alerta para usuário caso ocorra algum erro
                printf("Erro, tente novamente!");
        }
}

//Recomeça o programa caso o usuário queira:
void recomecar(comando){
        printf("\n=*=*=*=*=*=*=* OPCOES *=*=*=*=*=*=*=");
        printf("\nDigite:\n<1>Para criptografar outra mensagem.\n<2>Para descriptografar a ultima mensagem.\n<3>Para finalizar.");
        printf("\nQual opcao deseja executar? ");
        scanf("%d",&comando);
        switch(comando){
            case 1:                                             //Criptografa mensagem digitada pelo usuário
                criptografar();
                break;
            case 2:                                             //Descriptografa a última mensagem digitada
                descriptografar();
                break;
            case 3:
                printf("\n=*=*=*=*=*=*=* PROGRAMA FINALIZADO COM SUCESSO!*=*=*=*=*=*=*=");     //Finaliza o programa
                break;
        }
}

//Executa as funções:
int main(){

    menu();

    return 0;
}
