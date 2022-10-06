/**
**======================================
** Elementos de Programa��o
** Professor: Marcos Zuccolotto
** Aluno: Guilherme da Cunha Pioner
** Turma: 4311
** N�: 6
**======================================
*/

/**
**=========================================================================================
** Trabalho Pr�tico - Pegando Onda
** Objetivo: Manipular arquivos de �udio no formato WAV.
**
**=========================================================================================
*/

/*
**=========================================================================================
** Considera��es do c�digo:
**
** Todas as op��es do programa que o usu�rio deve selecionar e envolvem letras,
** tem configura��o "case-insensitive". Ou seja, n�o faz diferen�a se � mai�scula ou
** min�scula.
**
** D�vidas referentes ao trabalho foram tiradas e discutidas com os alunos Pedro Stanislau,
** Jo�o Vitor de Souza e o aluno da 4323 de 2019, Pedro Lawisch.
**
**=========================================================================================
*/



/* Prot�tipos */
int menu(void);					//! Menu do programa
void openFile(void);			//! Fun��o para abrir um arquivo .WAV
void exibeInfo(void);			//! Fun��o para exibir as informa��es de um arquivo .WAV
void modificaVolume(void);		//! Fun��o para modificar o volume de um arquivo .WAV
void recortaArqv(void);			//! Fun��o para recortar um trecho de um arquivo .WAV
void reproduzArqv(void);		//! Fun��o para reproduzir o conte�do de um arquivo .WAV

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>		//! Biblioteca que define conjuntos de tipos inteiros com larguras especificadas

/* Definindo um tipo de estrutura que cont�m todos os dados de um arquivo .WAV*/
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

/* Vari�veis globais */
fileWAV fWAV, auxWAV;		//! Vari�veis do tipo-estrutra fileWAV
FILE *fp, *fp2;				//! Ponteiros para arquivos

int flag_mdata 	= 0;		//! Flag que indica onde est�o os metadados
int size_mdata 	= 0;		//! Cont�m o tamanho em bytes dos metadados
char fname[54];				//! String com o nome do arquivo inserido pelo usu�rio
/* Vari�veis globais */

void main(void)
{
	/* Configura��es prim�rias */
	setlocale(LC_ALL,"portuguese");		//! Habilita caracteres da l�ngua portuguesa
	srand(time(NULL));					//! Habilita a gera��o de n�meros rand�micos

	int cntrl 	= 0;	//! Vari�vel de controle para as op��es do menu

	cntrl = menu();		//! Chama o menu do programa

	while(1)
	{
		switch(cntrl)
		{
			case 1:			//! ABRIR UM ARQUIVO .WAV
            	openFile();
				break;

			case 2:			//! EXIBIR AS INFORMA��ES DO ARQUIVO .WAV
				if(fp == NULL)
				{
					puts(" *� necess�rio abrir um arquivo .WAV antes de utilizar esta opc�o! \n");
				}
				else
					exibeInfo();
				break;

			case 3:			//! MODIFICAR O VOLUME DO ARQUIVO .WAV
				if(fp == NULL)
				{
					puts(" *� necess�rio abrir um arquivo .WAV antes de utilizar esta opc�o! \n");
				}
				else
					modificaVolume();
				break;

			case 4:			//! FAZER UM RECORTE DE UMA PARTE DO ARQUIVO .WAV
				if(fp == NULL)
				{
					puts(" *� necess�rio abrir um arquivo .WAV antes de utilizar esta opc�o! \n");
				}
				else
					recortaArqv();
				break;

			case 5:			//! REPRODUZIR O CONTE�DO DE DO ARQUIVO .WAV
				if(fp == NULL)
				{
					puts(" *� necess�rio abrir um arquivo .WAV antes de utilizar esta opc�o! \n");
				}
				else
					reproduzArqv();
				break;


			case 6:
				exit(0);	//! Indica que o programa encerrou sem erros
				break;
		}

		/* Voltando ao menu de op��es */
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
		puts("*!>  TRABALHO DE PROGRAMA��O - Pegando Onda  <!* ");
		puts("\tGuilherme da Cunha Pioner - n� 6");
		puts("\t      TURMA 4311 - 2020 \n");
		puts("Este programa serve para manipular arquivos de �udio do tipo WAV. ");

		puts("\n=+= Op��es Dispon�veis =+=");
		puts("1. Abrir um novo arquivo do tipo WAV ");
		puts("2. Exibir as informa��es do arquivo .WAV ");
		puts("3. Modificar o volume do arquivo .WAV ");
		puts("4. Recortar um trecho do arquivo .WAV");
		puts("5. Reproduzir o conte�do do arquivo .WAV");
		puts("6. Encerrar o programa");
		printf("Insira a op��o desejada: ");
		scanf("%d",&cntrl);
		fflush(stdin);		//! Limpando o buffer de entrada
	} while( cntrl < 1 || cntrl > 6 );	//! Caso o valor inserido n�o esteja entre 1 e 6,
	//! reapresenta a tela do menu e deixa o usu�rio inserir novamente o valor, at�
	//! receber um que seja v�lido.
	printf("\n\n");			//! Imprime duas novas linhas

	return cntrl;
}

void openFile(void)
{
	int i 	= 0;			//! Vari�vel de controle dos la�os
	int cnt = 0;			//! Vari�vel que atua como contador
	char adc_wav[54];		//! String que cont�m o nome do arquivo acrescido de ".wav"
	char verify_data[5];	//! String para achar "data" no arquivo
	verify_data[4] = '\0';	//! Indica o final da string 'verify_amostra'

		/* Recebendo o arquivo */
	puts("Digite o nome do arquivo de texto desejado: ");
	gets(fname);

	strcpy(adc_wav,fname);		//! Copia o texto da string 'fname' para a string 'adc_wav'
	strcat(adc_wav,".wav");		//! Adiciona o texto '.wav' ao final da string 'adc_wav'

	fp=fopen(adc_wav,"rb");		//! Abre o arquivo inserido pelo usu�rio em modo de leitura bin�ria
		/* Recebendo o arquivo */

		/* Testa se houve erro ao abrir o arquivo */
	if (fp == NULL)		//! Checa se o arquivo foi aberto corretamente
	{
		puts("\n *O Arquivo inserido n�o existe! \n");
	}
		/* Testa se houve erro ao abrir o arquivo */

	else	//! Se n�o houve erro, j� vamos computar as informa��es do arquivo .wav
	{
		fread(&fWAV,sizeof(fWAV)-8,1,fp);		//! Copia as informa��es do arquivo para a estrutura de 'fWAV'
		//! S� pega as inforam��es at� BitsPerSample (ou seja, n�o preenche Subchunk2ID e Subchunk2Size)

		/*
		** Os metadados de um arquivo .WAV podem estar tanto depois do bloco "data" quanto entre
		** os blocos "fmt" e "data".
		** Por este motivo, vamos realizar uma verifica��o em fun��o de compreendermos onde eles est�o
		** localizados.
		*/

		do
	    {
			fseek(fp, cnt+36, SEEK_SET);	//! Coloca o indicador de posi��o do fluxo para o final de "BitsPerSample".
			/* Como consideramos que os metadados s� v�o aparecer depois de "fmt", podemos "pular" a
		    ** verifica��o da exist�ncia de "data" em outros blocos.
		    ** Para isso, somamos 36 (Quantidades de bytes dos blocos "RIFF" chunk e "fmt" subchunk juntos)
			** � vari�vel que conta quantas verifica��es foram realizadas (Isso faz com que seja feito uma verifica��o
			** a cada caracter. Essa verfica��o consiste em pegar o caracter atual e os pr�ximos 3, comparando
			** se eles s�o iguais � palavra "data".
			*/

			for(i=0; i<4; i++)		//! Fazendo a verifica��o com a letra atual e as pr�ximas tr�s
			{
				verify_data[i] = fgetc(fp);
			}
			verify_data[4]='\0';	//! Por via das d�vidas, indicamos novamente o final da string

			cnt++;		//! Conta a quantidade de verifica��es realizadas
		} while(strcmp(verify_data,"data")); 	//! Repete o la�o enquanto n�o encontrarmos a string 'data'
		cnt-= 1;		//! Temos de subtrair 1 do contador, uma vez que o la�o vai passar pela verifica��o correta tamb�m

    	if( cnt > 0 ) 	//! Se o contador for maior que zero, sabemos que os metadados est�o entre "fmt" e "data"
			flag_mdata = 1;
    	if( cnt == 0 ) 	//! Se o contador for igual a zero, sabemos que os metadados est�o depois de "data"
			flag_mdata = 0;

    	fseek(fp,-4,SEEK_CUR);				//! Deslocamos -4 pois o indicador de posi��o do fluxo est�
    										//! logo ap�s "data".
    	fread(fWAV.Subchunk2ID,1,4,fp); 	//! Aloca o nome do bloco "data" no Subchunk2ID da estrutura de fWAV.
        fread(&fWAV.Subchunk2Size,1,4, fp); //! Armazena a quantidade de bytes de "data".

        size_mdata = fWAV.ChunkSize - 36 - fWAV.Subchunk2Size;		//! Tamanho em bytes dos metadados

        printf("\n *O arquivo %s foi aberto com sucesso! \n\n",adc_wav);

        fclose(fp);		//! Fecha o arquivo de leitura bin�ria
	}
}

void exibeInfo(void)
{
    int ch = 0;				//! Vari�vel para percorrer as strings

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
	int16_t amostra;		//! Vari�vel que representa uma amostra do �udio ( 2 bytes)
	float coef;				//! Coeficiente para aumentar/diminuir o volume do �udio
	int i;					//! Vari�vel de controle dos la�os
	char adc_wav[54];		//! String que cont�m o nome do arquivo acrescido de ".wav"
	char arqv_mod[58]; 		//! String que cont�m o nome do arquivo acrescido de "_mod.wav"

	puts("O programa permite efetuar uma varia��o no volume do �udio com ");
	puts("um coeficiente entre 0 e 100. \n");

	puts("A varia��o pode tanto aumentar quanto diminuir a amplitude do ");
	puts("sinal. Um coeficiente entre 0 e 1 indica que o volume deve diminuir, ");
	puts("enquanto um coeficiente maior que 1 indica que o volume deve aumentar.");

	printf("\n\n -+> Digite o valor do coeficiente: ");
	scanf("%f",&coef);
	fflush(stdin);		//! Limpando o buffer de entrada
	while( coef < 0 || coef > 100  )		//! Caso o valor inserido n�o esteja entre 0 e 100,
	{	//! o programa fica mostrando mensagem de erro, at� receber um valor v�lido
		printf("\nValor inv�lido! Tente novamente : ");
		scanf("%f",&coef);
		fflush(stdin);	//! Limpando o buffer de entrada
	}

		/* Abrindo o arquivo original e o arquivo a ser modificado */
	strcpy(adc_wav,fname);			//! Copia o texto da string 'fname' para a string 'adc_wav'
	strcpy(arqv_mod,fname);			//! Copia o texto da string 'fname' para a string 'arqv_mod'
	strcat(adc_wav,".wav");			//! Adiciona o texto '.wav' ao final da string 'adc_wav'
	strcat(arqv_mod,"_mod.wav");	//! Adiciona o texto '_mod.wav' ao final da string 'arqv_mod'

	fp = fopen(adc_wav,"rb");		//! Abre o arquivo inserido pelo usu�rio em modo de leitura bin�ria
	fp2 = fopen(arqv_mod,"wb");		//! Abre o arquivo a ser modificado em modo de escrita bin�ria
		/* Abrindo o arquivo original e o arquivo a ser modificado */

	fwrite(&fWAV, sizeof(fileWAV),1, fp2);	//! Transcreve todos os dados do arquivo original para o arquivo a ser modificado.
	//! Aqui fazemos uma suposi��o que os metadados est�o no final do arquivo.

	if(flag_mdata == 0)		//! Se a flag � zero, significa que os metadados est�o no final do arquivo
	{
		fseek(fp,44,SEEK_SET);		//! "Pula" os 44 primeiros bytes, colocando o indicador de
									//! posi��o do fluxo logo em "data".
	}
	else if(flag_mdata > 1)	//! Se a flag � maior que 1, significa que os metadados est�o entre "fmt" e "data"
	{
		fseek(fp,44+size_mdata,SEEK_SET);	//! "Pula" os 44 primeiros bytes, acrescido do tamanho dos
											//! metadados. Isso tamb�m faz com que o indicador de posi��o
											//! do fluxo fique em "data".
	}
	else	//! Se a flag n�o entra em nenhum dos outros casos, algo deu errado e, portanto, o programa encerra.
	{
		puts("\n  **!! Problema com os metadados do arquivo  !!** ");
		puts("Encerrando o programa...");
		exit(1);		//! Finaliza o programa indicando um erro
	}

	for(i=0; i<fWAV.Subchunk2Size; i+=2)	//! Percore todos os bytes de "data" (de 2 em 2).
	{
			/* Obtendo a amostra de 2 bytes do �udio */
		amostra = fgetc(fp);
		amostra |= ( fgetc(fp) << 8 );
			/* Obtendo a amostra de 2 bytes do �udio */

		if( ( amostra * coef ) >= pow(2,15) )		//! Checamos se a amostra multiplicada pelo coeficiente
		{	//! ir� resultador em um valor maior que o limite definido pelos 2 bytes.
			//! Nesse caso, 2^16 � signed, por tanto, seu valor m�ximo � divido por 2, resultando em um valor
			//! decimal m�ximo de 2^15.
			amostra = pow(2,15) - 1;	//! Amostra fica definida como o valor positivo m�ximo
		}
		else if( ( amostra * coef ) <= pow(-2,15) )
		{
			amostra = pow(-2,15);		//! Amostra fica definida como o valor negativo m�ximo
		}
		else	//! Se n�o ultrapassar nenhum dos dois limites...
		{
			amostra *= coef;	//! Amostra � multiplicada normalmente pelo coeficiente
		}

		fwrite(&amostra,2,1,fp2);		//! Escreve os dois bytes j� alterados no arquivo que est� sendo modificado
	}

	if(flag_mdata)		//! Caso nossa suposi��o de que os metadados est�o no final do arquivo estiver errada,
	{					//! devemos ajustar o indicador de posi��o do fluxo para logo ap�s o final de BitsPerSample
		fseek(fp,36,SEEK_SET);
	}

	for(i=0; i<size_mdata; i++)			//! Escrevendo os metadados no arquivo alterado
	{
		fprintf(fp2,"%c", fgetc(fp) );
	}

	printf("\n\n *O volume do arquivo %s foi alterado com sucesso! \n",adc_wav);
	printf("    -O resultado est� gravado no arquivo %s ! \n\n",arqv_mod);

	fclose(fp);			//! Fecha o arquivo de leitura bin�ria
	fclose(fp2);		//! Fecha o arquivo de escrita bin�ria
}

void recortaArqv(void)
{
	int cut_start 	= 0;	//! Vari�vel que armazena o segundo inicial do recorte do arquivo original
	int	cut_end	  	= 0;	//! Vari�vel que armazena o segundo final do recorte do arquivo original
	int end_of_wav 	= 0;	//! Vari�vel que armazena o tempo do �udio em bytes
	int cut_size 	= 0;	//! Vari�vel que armazena o tamanho do recorte em bytes
	int i 			= 0;	//! Vari�vel de controle dos la�os
	char adc_wav[54];		//! String que cont�m o nome do arquivo acrescido de ".wav"
	char arqv_mod[58];		//! String que cont�m o nome do arquivo acrescido de "_mod.wav"

		/* Abrindo o arquivo original e o arquivo a ser modificado */
	strcpy(adc_wav,fname);			//! Copia o texto da string 'fname' para a string 'adc_wav'
	strcpy(arqv_mod,fname);			//! Copia o texto da string 'fname' para a string 'arqv_mod'
	strcat(adc_wav,".wav");			//! Adiciona o texto '.wav' ao final da string 'adc_wav'
	strcat(arqv_mod,"_mod.wav");	//! Adiciona o texto '_mod.wav' ao final da string 'arqv_mod'

	fp = fopen(adc_wav,"rb");		//! Abre o arquivo original em modo de leitura bin�ria
	fp2 = fopen(arqv_mod,"wb");		//! Abre o arquivo a ser modificado em modo de escrita bin�ria
		/* Abrindo o arquivo original e o arquivo a ser modificado */

	end_of_wav = (int)(fWAV.Subchunk2Size/fWAV.ByteRate);	//! Divide a quantidade de bytes de "data" pela
															//! quantidade de bytes em um segundo.

		/* Recebendo o segundo inicial do recorte do arquivo */
	printf("\n\n -+> Digite o segundo inicial do corte: ");
	scanf("%d",&cut_start);
	fflush(stdin);			//! Limpando o buffer de entrada
	while( cut_start < 0 || cut_start > end_of_wav )	//! Caso o valor inserido n�o esteja entre as condi��es,
	{	//! o programa fica mostrando mensagem de erro, at� receber um valor v�lido
		printf("\nValor inv�lido! Tente novamente : ");
		scanf("%d",&cut_start);
		fflush(stdin);		//! Limpando o buffer de entrada
	}
		/* Recebendo o segundo inicial do recorte do arquivo */

		/* Recebendo o segundo final do recorte do arquivo */
	printf("\n\n -+> Digite o segundo final do corte: ");
	scanf("%d",&cut_end);
	fflush(stdin);			//! Limpando o buffer de entrada
	while( cut_end <= cut_start || cut_end > end_of_wav )	//! Caso o valor inserido n�o esteja entre as condi��es,
	{	//! o programa fica mostrando mensagem de erro, at� receber um valor v�lido
		printf("\nValor inv�lido! Tente novamente : ");
		scanf("%d",&cut_end);
		fflush(stdin);		//! Limpando o buffer de entrada
	}
		/* Recebendo o segundo final do recorte do arquivo */

	cut_size = (cut_end - cut_start) * fWAV.ByteRate;	//! Calcula o tamanho do corte em bytes

	memcpy(&auxWAV, &fWAV, sizeof(fileWAV));		//! Copia todos os valores da vari�vel 'fWAV' para 'auxWAV'.
	auxWAV.Subchunk2Size = cut_size;				//! Alterando o valor de Subchunk2Size para 'size_cut'.
	auxWAV.ChunkSize = 36 + cut_size + size_mdata;	//! Alterando o tamanho total em bytes do arquivo.
	fwrite(&auxWAV, sizeof(fileWAV),1, fp2);		//! Transcreve todos os dados da vari�vel de aux�lio
													//! para o arquivo a ser modificado.

	if(flag_mdata == 0)		//! Se a flag � zero, significa que os metadados est�o no final do arquivo
	{
		fseek(fp,44,SEEK_SET);		//! "Pula" os 44 primeiros bytes, colocando o indicador de
									//! posi��o do fluxo logo em "data".
	}
	else if(flag_mdata > 1)	//! Se a flag � maior que 1, significa que os metadados est�o entre "fmt" e "data"
	{
		fseek(fp,44+size_mdata,SEEK_SET);	//! "Pula" os 44 primeiros bytes, acrescido do tamanho dos
											//! metadados. Isso tamb�m faz com que o indicador de posi��o
											//! do fluxo fique em "data".
	}
	else	//! Se a flag n�o entra em nenhum dos outros casos, algo deu errado e, portanto, o programa encerra.
	{
		puts("\n  **!! Problema com os metadados do arquivo  !!** ");
		puts("Encerrando o programa...");
		exit(1);		//! Finaliza o programa indicando um erro
	}

	/* Neste momento, nosso indicador de posi��o do fluxo est� no come�o de "data",
	 * portanto, basta deslocar a quantidade de bytes referente ao in�cio do corte,
	 * que foi referenciado pelo usu�rio. */
	fseek(fp,cut_start*auxWAV.ByteRate,SEEK_CUR);		//! Coloca o indicador de posi��o de fluxo no segundo inicial do corte

	for(i=0; i<cut_size; i++)		//! Copiando todos os bytes do arquivo original at� o limite definido pelo usu�rio
	{
		fprintf(fp2,"%c", fgetc(fp) );
	}

	if(flag_mdata) 	//! Se os metadados estiverem no meio ...
        fseek(fp,36,SEEK_SET); 		//! Posiciona o indicador de posi��o do fluxo entre o BitsPerSample e o Subchunk2ID.
    else
        fseek(fp, (fWAV.ChunkSize + 8 - size_mdata), SEEK_SET); //! Posiciona o indicador de posi��o do fluxo exatamente no inicio dos metadados.

    for(i=0; i<size_mdata; i++)		//! Escrevendo os metadados no arquivo alterado
    {
        fprintf(fp2,"%c", fgetc(fp) );
    }

	printf("\n\n *O recorte do arquivo %s foi realizado com sucesso! \n",adc_wav);
	printf("    -O resultado est� gravado no arquivo %s ! \n\n",arqv_mod);

	fclose(fp);			//! Fecha o arquivo de leitura bin�ria
	fclose(fp2);		//! Fecha o arquivo de escrita bin�ria
}

void reproduzArqv(void)
{
	char adc_wav[54];			//! String que cont�m o nome do arquivo acrescido de ".wav"
	char opcao = 0;				//! Vari�vel para obter as op��es selecionadas pelo usu�rio

	strcpy(adc_wav,fname);		//! Copia o texto da string 'fname' para a string 'adc_wav'
	strcat(adc_wav,".wav");		//! Adiciona o texto '.wav' ao final da string 'adc_wav'

	PlaySound(adc_wav, NULL, SND_ASYNC);		//! Fun��o para reproduzir o �udio

	system("cls");
	puts("\t\t\t!*>  PLAYER  <*! ");
	puts("==================================================================== \n");
	puts("O �udio do arquivo est� sendo reproduzido! \n");

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

	puts("Op��es dispon�veis durante o menu do player: ");
	puts(" -+> 'E' <+- Encerra a reprodu��o");
	puts(" -+> 'R' <+- Reinicia a reprodu��o");
	puts(" -+> 'M' <+- Retorna ao menu");
	puts("\n   *Retornar ao menu n�o encerra a reprodu��o do conte�do do arquivo");

	puts("");
	puts("====================================================================");
	puts("\t\t\t!*>  PLAYER  <*! ");

	do
	{
		fflush(stdin);			//! Limpando o buffer de entrada
		opcao = getch();		//! Recebe um caracter do usu�rio

		if(opcao == 'R' || opcao == 'r' )
			PlaySound(adc_wav, NULL, SND_ASYNC);		//! Fun��o para reproduzir o �udio

		if( opcao == 'E' || opcao == 'e' )
			PlaySound(NULL,NULL,SND_ASYNC);				//! Fun��o para reproduzir o �udio


	} while (opcao != 'M' && opcao != 'm' );
}
