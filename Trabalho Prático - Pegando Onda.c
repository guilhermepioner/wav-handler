/**
**======================================
** Elementos de Programação
** Professor: Marcos Zuccolotto
** Aluno: Guilherme da Cunha Pioner
** Turma: 4311
** N°: 6
**======================================
*/

/**
**=========================================================================================
** Trabalho Prático - Pegando Onda
** Objetivo: Manipular arquivos de áudio no formato WAV.
**
**=========================================================================================
*/

/*
**=========================================================================================
** Considerações do código:
**
** Todas as opções do programa que o usuário deve selecionar e envolvem letras,
** tem configuração "case-insensitive". Ou seja, não faz diferença se é maiúscula ou
** minúscula.
**
** Dúvidas referentes ao trabalho foram tiradas e discutidas com os alunos Pedro Stanislau,
** João Vitor de Souza e o aluno da 4323 de 2019, Pedro Lawisch.
**
**=========================================================================================
*/



/* Protótipos */
int menu(void);					//! Menu do programa
void openFile(void);			//! Função para abrir um arquivo .WAV
void exibeInfo(void);			//! Função para exibir as informações de um arquivo .WAV
void modificaVolume(void);		//! Função para modificar o volume de um arquivo .WAV
void recortaArqv(void);			//! Função para recortar um trecho de um arquivo .WAV
void reproduzArqv(void);		//! Função para reproduzir o conteúdo de um arquivo .WAV

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>		//! Biblioteca que define conjuntos de tipos inteiros com larguras especificadas

/* Definindo um tipo de estrutura que contém todos os dados de um arquivo .WAV*/
typedef struct
{
	//! "RIFF" chunk
	char ChunkID[4];
	uint32_t ChunkSize;
	char Format[4];

	//! "fmt" sub-chunk
	char Subchunk1ID[4];
	uint32_t Subchunk1Size;
	uint16_t AudioFormat;
	uint16_t  NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t  BlockAlign;
	uint16_t  BitsPerSample;

	//! "data" sub-chunk
	char Subchunk2ID[4];
	uint32_t  Subchunk2Size;

} fileWAV;

/* Variáveis globais */
fileWAV fWAV, auxWAV;		//! Variáveis do tipo-estrutra fileWAV
FILE *fp, *fp2;				//! Ponteiros para arquivos

int flag_mdata 	= 0;		//! Flag que indica onde estão os metadados
int size_mdata 	= 0;		//! Contém o tamanho em bytes dos metadados
char fname[54];				//! String com o nome do arquivo inserido pelo usuário
/* Variáveis globais */

void main(void)
{
	/* Configurações primárias */
	setlocale(LC_ALL,"portuguese");		//! Habilita caracteres da língua portuguesa
	srand(time(NULL));					//! Habilita a geração de números randômicos

	int cntrl 	= 0;	//! Variável de controle para as opções do menu

	cntrl = menu();		//! Chama o menu do programa

	while(1)
	{
		switch(cntrl)
		{
			case 1:			//! ABRIR UM ARQUIVO .WAV
            	openFile();
				break;

			case 2:			//! EXIBIR AS INFORMAÇÕES DO ARQUIVO .WAV
				if(fp == NULL)
				{
					puts(" *É necessário abrir um arquivo .WAV antes de utilizar esta opcão! \n");
				}
				else
					exibeInfo();
				break;

			case 3:			//! MODIFICAR O VOLUME DO ARQUIVO .WAV
				if(fp == NULL)
				{
					puts(" *É necessário abrir um arquivo .WAV antes de utilizar esta opcão! \n");
				}
				else
					modificaVolume();
				break;

			case 4:			//! FAZER UM RECORTE DE UMA PARTE DO ARQUIVO .WAV
				if(fp == NULL)
				{
					puts(" *É necessário abrir um arquivo .WAV antes de utilizar esta opcão! \n");
				}
				else
					recortaArqv();
				break;

			case 5:			//! REPRODUZIR O CONTEÚDO DE DO ARQUIVO .WAV
				if(fp == NULL)
				{
					puts(" *É necessário abrir um arquivo .WAV antes de utilizar esta opcão! \n");
				}
				else
					reproduzArqv();
				break;


			case 6:
				exit(0);	//! Indica que o programa encerrou sem erros
				break;
		}

		/* Voltando ao menu de opções */
		printf("\nAperte qualquer tecla para voltar ao menu! \n");
		system("pause");
		cntrl = menu();
	}
}


int menu(void)
{
	int cntrl;
	do
	{
		system("cls");
		puts("*!>  TRABALHO DE PROGRAMAÇÃO - Pegando Onda  <!* ");
		puts("\tGuilherme da Cunha Pioner - n° 6");
		puts("\t      TURMA 4311 - 2020 \n");
		puts("Este programa serve para manipular arquivos de áudio do tipo WAV. ");

		puts("\n=+= Opções Disponíveis =+=");
		puts("1. Abrir um novo arquivo do tipo WAV ");
		puts("2. Exibir as informações do arquivo .WAV ");
		puts("3. Modificar o volume do arquivo .WAV ");
		puts("4. Recortar um trecho do arquivo .WAV");
		puts("5. Reproduzir o conteúdo do arquivo .WAV");
		puts("6. Encerrar o programa");
		printf("Insira a opção desejada: ");
		scanf("%d",&cntrl);
		fflush(stdin);		//! Limpando o buffer de entrada
	} while( cntrl < 1 || cntrl > 6 );	//! Caso o valor inserido não esteja entre 1 e 6,
	//! reapresenta a tela do menu e deixa o usuário inserir novamente o valor, até
	//! receber um que seja válido.
	printf("\n\n");			//! Imprime duas novas linhas

	return cntrl;
}

void openFile(void)
{
	int i 	= 0;			//! Variável de controle dos laços
	int cnt = 0;			//! Variável que atua como contador
	char adc_wav[54];		//! String que contém o nome do arquivo acrescido de ".wav"
	char verify_data[5];	//! String para achar "data" no arquivo
	verify_data[4] = '\0';	//! Indica o final da string 'verify_amostra'

		/* Recebendo o arquivo */
	puts("Digite o nome do arquivo de texto desejado: ");
	gets(fname);

	strcpy(adc_wav,fname);		//! Copia o texto da string 'fname' para a string 'adc_wav'
	strcat(adc_wav,".wav");		//! Adiciona o texto '.wav' ao final da string 'adc_wav'

	fp=fopen(adc_wav,"rb");		//! Abre o arquivo inserido pelo usuário em modo de leitura binária
		/* Recebendo o arquivo */

		/* Testa se houve erro ao abrir o arquivo */
	if (fp == NULL)		//! Checa se o arquivo foi aberto corretamente
	{
		puts("\n *O Arquivo inserido não existe! \n");
	}
		/* Testa se houve erro ao abrir o arquivo */

	else	//! Se não houve erro, já vamos computar as informações do arquivo .wav
	{
		fread(&fWAV,sizeof(fWAV)-8,1,fp);		//! Copia as informações do arquivo para a estrutura de 'fWAV'
		//! Só pega as inforamções até BitsPerSample (ou seja, não preenche Subchunk2ID e Subchunk2Size)

		/*
		** Os metadados de um arquivo .WAV podem estar tanto depois do bloco "data" quanto entre
		** os blocos "fmt" e "data".
		** Por este motivo, vamos realizar uma verificação em função de compreendermos onde eles estão
		** localizados.
		*/

		do
	    {
			fseek(fp, cnt+36, SEEK_SET);	//! Coloca o indicador de posição do fluxo para o final de "BitsPerSample".
			/* Como consideramos que os metadados só vão aparecer depois de "fmt", podemos "pular" a
		    ** verificação da existência de "data" em outros blocos.
		    ** Para isso, somamos 36 (Quantidades de bytes dos blocos "RIFF" chunk e "fmt" subchunk juntos)
			** à variável que conta quantas verificações foram realizadas (Isso faz com que seja feito uma verificação
			** a cada caracter. Essa verficação consiste em pegar o caracter atual e os próximos 3, comparando
			** se eles são iguais à palavra "data".
			*/

			for(i=0; i<4; i++)		//! Fazendo a verificação com a letra atual e as próximas três
			{
				verify_data[i] = fgetc(fp);
			}
			verify_data[4]='\0';	//! Por via das dúvidas, indicamos novamente o final da string

			cnt++;		//! Conta a quantidade de verificações realizadas
		} while(strcmp(verify_data,"data")); 	//! Repete o laço enquanto não encontrarmos a string 'data'
		cnt-= 1;		//! Temos de subtrair 1 do contador, uma vez que o laço vai passar pela verificação correta também

    	if( cnt > 0 ) 	//! Se o contador for maior que zero, sabemos que os metadados estão entre "fmt" e "data"
			flag_mdata = 1;
    	if( cnt == 0 ) 	//! Se o contador for igual a zero, sabemos que os metadados estão depois de "data"
			flag_mdata = 0;

    	fseek(fp,-4,SEEK_CUR);				//! Deslocamos -4 pois o indicador de posição do fluxo está
    										//! logo após "data".
    	fread(fWAV.Subchunk2ID,1,4,fp); 	//! Aloca o nome do bloco "data" no Subchunk2ID da estrutura de fWAV.
        fread(&fWAV.Subchunk2Size,1,4, fp); //! Armazena a quantidade de bytes de "data".

        size_mdata = fWAV.ChunkSize - 36 - fWAV.Subchunk2Size;		//! Tamanho em bytes dos metadados

        printf("\n *O arquivo %s foi aberto com sucesso! \n\n",adc_wav);

        fclose(fp);		//! Fecha o arquivo de leitura binária
	}
}

void exibeInfo(void)
{
    int ch = 0;				//! Variável para percorrer as strings

	system ("cls");			//! Limpa a tela do programa

    printf("!*>  HEADER do %s.wav  <*! \n",fname);
    puts("==================================");

    /* RIFF chunk */
    printf("ChunkID...........: ");
    while( ch < 4)
    {
    	printf("%c",fWAV.ChunkID[ch]);
    	ch++;
	}
	ch = 0;

	printf("\nChunkSize.........: %d bytes \n",fWAV.ChunkSize);

	printf("Format:...........: ");
	while( ch < 4)
    {
    	printf("%c",fWAV.Format[ch]);
    	ch++;
	}
	ch = 0;
	/* RIFF chunk */

	puts("");

	/* "fmt" sub-chunk */
	printf("\nSubChunk1ID.......: ");
	while( ch < 4)
    {
    	printf("%c",fWAV.Subchunk1ID[ch]);
    	ch++;
	}
	ch = 0;

	printf("\nSubChunk1Size.....: %d bytes \n",fWAV.Subchunk1Size);

	printf("AudioFormat.......: %d \n",fWAV.AudioFormat);

	printf("NumChannels.......: %d canal(s) \n",fWAV.NumChannels);

	printf("SampleRate........: %d amostras por segundo \n",fWAV.SampleRate);

	printf("ByteRate..........: %d bytes por segundo \n",fWAV.ByteRate);

	printf("BlockAlign........: %d bytes por amostra \n",fWAV.BlockAlign);

	printf("BitsPerSample.....: %d bits por amostra \n",fWAV.BitsPerSample);
	/* "fmt" sub-chunk */

	puts("");

	/* "data" sub-chunk */
	printf("SubChunk1ID.......: ");
	while( ch < 4)
    {
    	printf("%c",fWAV.Subchunk2ID[ch]);
    	ch++;
	}
	ch = 0;

	printf("\nSubChunk2Size.....: %d bytes \n",fWAV.Subchunk2Size);
	/* "data" sub-chunk */

	puts("==================================");
	printf("!*>  HEADER do %s.wav  <*! \n",fname);
}

void modificaVolume(void)
{
	int16_t amostra;		//! Variável que representa uma amostra do áudio ( 2 bytes)
	float coef;				//! Coeficiente para aumentar/diminuir o volume do áudio
	int i;					//! Variável de controle dos laços
	char adc_wav[54];		//! String que contém o nome do arquivo acrescido de ".wav"
	char arqv_mod[58]; 		//! String que contém o nome do arquivo acrescido de "_mod.wav"

	puts("O programa permite efetuar uma variação no volume do áudio com ");
	puts("um coeficiente entre 0 e 100. \n");

	puts("A variação pode tanto aumentar quanto diminuir a amplitude do ");
	puts("sinal. Um coeficiente entre 0 e 1 indica que o volume deve diminuir, ");
	puts("enquanto um coeficiente maior que 1 indica que o volume deve aumentar.");

	printf("\n\n -+> Digite o valor do coeficiente: ");
	scanf("%f",&coef);
	fflush(stdin);		//! Limpando o buffer de entrada
	while( coef < 0 || coef > 100  )		//! Caso o valor inserido não esteja entre 0 e 100,
	{	//! o programa fica mostrando mensagem de erro, até receber um valor válido
		printf("\nValor inválido! Tente novamente : ");
		scanf("%f",&coef);
		fflush(stdin);	//! Limpando o buffer de entrada
	}

		/* Abrindo o arquivo original e o arquivo a ser modificado */
	strcpy(adc_wav,fname);			//! Copia o texto da string 'fname' para a string 'adc_wav'
	strcpy(arqv_mod,fname);			//! Copia o texto da string 'fname' para a string 'arqv_mod'
	strcat(adc_wav,".wav");			//! Adiciona o texto '.wav' ao final da string 'adc_wav'
	strcat(arqv_mod,"_mod.wav");	//! Adiciona o texto '_mod.wav' ao final da string 'arqv_mod'

	fp = fopen(adc_wav,"rb");		//! Abre o arquivo inserido pelo usuário em modo de leitura binária
	fp2 = fopen(arqv_mod,"wb");		//! Abre o arquivo a ser modificado em modo de escrita binária
		/* Abrindo o arquivo original e o arquivo a ser modificado */

	fwrite(&fWAV, sizeof(fileWAV),1, fp2);	//! Transcreve todos os dados do arquivo original para o arquivo a ser modificado.
	//! Aqui fazemos uma suposição que os metadados estão no final do arquivo.

	if(flag_mdata == 0)		//! Se a flag é zero, significa que os metadados estão no final do arquivo
	{
		fseek(fp,44,SEEK_SET);		//! "Pula" os 44 primeiros bytes, colocando o indicador de
									//! posição do fluxo logo em "data".
	}
	else if(flag_mdata > 1)	//! Se a flag é maior que 1, significa que os metadados estão entre "fmt" e "data"
	{
		fseek(fp,44+size_mdata,SEEK_SET);	//! "Pula" os 44 primeiros bytes, acrescido do tamanho dos
											//! metadados. Isso também faz com que o indicador de posição
											//! do fluxo fique em "data".
	}
	else	//! Se a flag não entra em nenhum dos outros casos, algo deu errado e, portanto, o programa encerra.
	{
		puts("\n  **!! Problema com os metadados do arquivo  !!** ");
		puts("Encerrando o programa...");
		exit(1);		//! Finaliza o programa indicando um erro
	}

	for(i=0; i<fWAV.Subchunk2Size; i+=2)	//! Percore todos os bytes de "data" (de 2 em 2).
	{
			/* Obtendo a amostra de 2 bytes do áudio */
		amostra = fgetc(fp);
		amostra |= ( fgetc(fp) << 8 );
			/* Obtendo a amostra de 2 bytes do áudio */

		if( ( amostra * coef ) >= pow(2,15) )		//! Checamos se a amostra multiplicada pelo coeficiente
		{	//! irá resultador em um valor maior que o limite definido pelos 2 bytes.
			//! Nesse caso, 2^16 é signed, por tanto, seu valor máximo é divido por 2, resultando em um valor
			//! decimal máximo de 2^15.
			amostra = pow(2,15) - 1;	//! Amostra fica definida como o valor positivo máximo
		}
		else if( ( amostra * coef ) <= pow(-2,15) )
		{
			amostra = pow(-2,15);		//! Amostra fica definida como o valor negativo máximo
		}
		else	//! Se não ultrapassar nenhum dos dois limites...
		{
			amostra *= coef;	//! Amostra é multiplicada normalmente pelo coeficiente
		}

		fwrite(&amostra,2,1,fp2);		//! Escreve os dois bytes já alterados no arquivo que está sendo modificado
	}

	if(flag_mdata)		//! Caso nossa suposição de que os metadados estão no final do arquivo estiver errada,
	{					//! devemos ajustar o indicador de posição do fluxo para logo após o final de BitsPerSample
		fseek(fp,36,SEEK_SET);
	}

	for(i=0; i<size_mdata; i++)			//! Escrevendo os metadados no arquivo alterado
	{
		fprintf(fp2,"%c", fgetc(fp) );
	}

	printf("\n\n *O volume do arquivo %s foi alterado com sucesso! \n",adc_wav);
	printf("    -O resultado está gravado no arquivo %s ! \n\n",arqv_mod);

	fclose(fp);			//! Fecha o arquivo de leitura binária
	fclose(fp2);		//! Fecha o arquivo de escrita binária
}

void recortaArqv(void)
{
	int cut_start 	= 0;	//! Variável que armazena o segundo inicial do recorte do arquivo original
	int	cut_end	  	= 0;	//! Variável que armazena o segundo final do recorte do arquivo original
	int end_of_wav 	= 0;	//! Variável que armazena o tempo do áudio em bytes
	int cut_size 	= 0;	//! Variável que armazena o tamanho do recorte em bytes
	int i 			= 0;	//! Variável de controle dos laços
	char adc_wav[54];		//! String que contém o nome do arquivo acrescido de ".wav"
	char arqv_mod[58];		//! String que contém o nome do arquivo acrescido de "_mod.wav"

		/* Abrindo o arquivo original e o arquivo a ser modificado */
	strcpy(adc_wav,fname);			//! Copia o texto da string 'fname' para a string 'adc_wav'
	strcpy(arqv_mod,fname);			//! Copia o texto da string 'fname' para a string 'arqv_mod'
	strcat(adc_wav,".wav");			//! Adiciona o texto '.wav' ao final da string 'adc_wav'
	strcat(arqv_mod,"_mod.wav");	//! Adiciona o texto '_mod.wav' ao final da string 'arqv_mod'

	fp = fopen(adc_wav,"rb");		//! Abre o arquivo original em modo de leitura binária
	fp2 = fopen(arqv_mod,"wb");		//! Abre o arquivo a ser modificado em modo de escrita binária
		/* Abrindo o arquivo original e o arquivo a ser modificado */

	end_of_wav = (int)(fWAV.Subchunk2Size/fWAV.ByteRate);	//! Divide a quantidade de bytes de "data" pela
															//! quantidade de bytes em um segundo.

		/* Recebendo o segundo inicial do recorte do arquivo */
	printf("\n\n -+> Digite o segundo inicial do corte: ");
	scanf("%d",&cut_start);
	fflush(stdin);			//! Limpando o buffer de entrada
	while( cut_start < 0 || cut_start > end_of_wav )	//! Caso o valor inserido não esteja entre as condições,
	{	//! o programa fica mostrando mensagem de erro, até receber um valor válido
		printf("\nValor inválido! Tente novamente : ");
		scanf("%d",&cut_start);
		fflush(stdin);		//! Limpando o buffer de entrada
	}
		/* Recebendo o segundo inicial do recorte do arquivo */

		/* Recebendo o segundo final do recorte do arquivo */
	printf("\n\n -+> Digite o segundo final do corte: ");
	scanf("%d",&cut_end);
	fflush(stdin);			//! Limpando o buffer de entrada
	while( cut_end <= cut_start || cut_end > end_of_wav )	//! Caso o valor inserido não esteja entre as condições,
	{	//! o programa fica mostrando mensagem de erro, até receber um valor válido
		printf("\nValor inválido! Tente novamente : ");
		scanf("%d",&cut_end);
		fflush(stdin);		//! Limpando o buffer de entrada
	}
		/* Recebendo o segundo final do recorte do arquivo */

	cut_size = (cut_end - cut_start) * fWAV.ByteRate;	//! Calcula o tamanho do corte em bytes

	memcpy(&auxWAV, &fWAV, sizeof(fileWAV));		//! Copia todos os valores da variável 'fWAV' para 'auxWAV'.
	auxWAV.Subchunk2Size = cut_size;				//! Alterando o valor de Subchunk2Size para 'size_cut'.
	auxWAV.ChunkSize = 36 + cut_size + size_mdata;	//! Alterando o tamanho total em bytes do arquivo.
	fwrite(&auxWAV, sizeof(fileWAV),1, fp2);		//! Transcreve todos os dados da variável de auxílio
													//! para o arquivo a ser modificado.

	if(flag_mdata == 0)		//! Se a flag é zero, significa que os metadados estão no final do arquivo
	{
		fseek(fp,44,SEEK_SET);		//! "Pula" os 44 primeiros bytes, colocando o indicador de
									//! posição do fluxo logo em "data".
	}
	else if(flag_mdata > 1)	//! Se a flag é maior que 1, significa que os metadados estão entre "fmt" e "data"
	{
		fseek(fp,44+size_mdata,SEEK_SET);	//! "Pula" os 44 primeiros bytes, acrescido do tamanho dos
											//! metadados. Isso também faz com que o indicador de posição
											//! do fluxo fique em "data".
	}
	else	//! Se a flag não entra em nenhum dos outros casos, algo deu errado e, portanto, o programa encerra.
	{
		puts("\n  **!! Problema com os metadados do arquivo  !!** ");
		puts("Encerrando o programa...");
		exit(1);		//! Finaliza o programa indicando um erro
	}

	/* Neste momento, nosso indicador de posição do fluxo está no começo de "data",
	 * portanto, basta deslocar a quantidade de bytes referente ao início do corte,
	 * que foi referenciado pelo usuário. */
	fseek(fp,cut_start*auxWAV.ByteRate,SEEK_CUR);		//! Coloca o indicador de posição de fluxo no segundo inicial do corte

	for(i=0; i<cut_size; i++)		//! Copiando todos os bytes do arquivo original até o limite definido pelo usuário
	{
		fprintf(fp2,"%c", fgetc(fp) );
	}

	if(flag_mdata) 	//! Se os metadados estiverem no meio ...
        fseek(fp,36,SEEK_SET); 		//! Posiciona o indicador de posição do fluxo entre o BitsPerSample e o Subchunk2ID.
    else
        fseek(fp, (fWAV.ChunkSize + 8 - size_mdata), SEEK_SET); //! Posiciona o indicador de posição do fluxo exatamente no inicio dos metadados.

    for(i=0; i<size_mdata; i++)		//! Escrevendo os metadados no arquivo alterado
    {
        fprintf(fp2,"%c", fgetc(fp) );
    }

	printf("\n\n *O recorte do arquivo %s foi realizado com sucesso! \n",adc_wav);
	printf("    -O resultado está gravado no arquivo %s ! \n\n",arqv_mod);

	fclose(fp);			//! Fecha o arquivo de leitura binária
	fclose(fp2);		//! Fecha o arquivo de escrita binária
}

void reproduzArqv(void)
{
	char adc_wav[54];			//! String que contém o nome do arquivo acrescido de ".wav"
	char opcao = 0;				//! Variável para obter as opções selecionadas pelo usuário

	strcpy(adc_wav,fname);		//! Copia o texto da string 'fname' para a string 'adc_wav'
	strcat(adc_wav,".wav");		//! Adiciona o texto '.wav' ao final da string 'adc_wav'

	PlaySound(adc_wav, NULL, SND_ASYNC);		//! Função para reproduzir o áudio

	system("cls");
	puts("\t\t\t!*>  PLAYER  <*! ");
	puts("==================================================================== \n");
	puts("O áudio do arquivo está sendo reproduzido! \n");

	puts("\t     ;;");
	puts("\t     ;';.");
	puts("\t     ;  ;;                 ;;;;;;;;;;;;;;;;;;; ");
	puts("\t     ;   ;;                ;;;;;;;;;;;;;;;;;;; ");
	puts("\t     ;    ;;               ;                 ; ");
	puts("\t     ;    ;;               ;                 ; ");
	puts("\t     ;   ;'                ;                 ; ");
	puts("\t     ;  '                  ;                 ; ");
	puts("\t,;;;,;                     ;                 ; ");
	puts("\t;;;;;;                     ;                 ; ");
	puts("\t`;;;;'                     ;                 ; ");
	puts("\t                      ,;;;;;            ,;;;;; ");
	puts("\t                      ;;;;;;            ;;;;;; ");
	puts("\t                      `;;;;'            `;;;;' ");

	puts("\n");

	puts("Opções disponíveis durante o menu do player: ");
	puts(" -+> 'E' <+- Encerra a reprodução");
	puts(" -+> 'R' <+- Reinicia a reprodução");
	puts(" -+> 'M' <+- Retorna ao menu");
	puts("\n   *Retornar ao menu não encerra a reprodução do conteúdo do arquivo");

	puts("");
	puts("====================================================================");
	puts("\t\t\t!*>  PLAYER  <*! ");

	do
	{
		fflush(stdin);			//! Limpando o buffer de entrada
		opcao = getch();		//! Recebe um caracter do usuário

		if(opcao == 'R' || opcao == 'r' )
			PlaySound(adc_wav, NULL, SND_ASYNC);		//! Função para reproduzir o áudio

		if( opcao == 'E' || opcao == 'e' )
			PlaySound(NULL,NULL,SND_ASYNC);				//! Função para reproduzir o áudio


	} while (opcao != 'M' && opcao != 'm' );
}
