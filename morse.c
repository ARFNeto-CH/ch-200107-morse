#define		__COM_SOM__ 1		
//#undef		__COM_SOM__		// comente essa linha para ouvir o som em Morse


#define		_dot_		  (100)	// duracao em ms
#define		_freq_		  500	// frequencia do som 500mhz

#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "windows.h"

struct morse
{
	char* valor[127];
};
typedef struct morse		Codigo_Morse;

void	apaga_tabela(Codigo_Morse*);
void	fase1(char*);
void	fase2(char*, Codigo_Morse*);
void    inicia_tabela(Codigo_Morse*);
void	morse(char*, Codigo_Morse*);
void	morse_frase(char*, Codigo_Morse*);
void	morse_play(char*, Codigo_Morse*);
void	mostra_tabela(Codigo_Morse*);

int main()
{
	Codigo_Morse		codigo;
	char*				arquivo_morse = "morse.txt";
	int modo = 0; // separa uns testes incrementais

	if (modo == 2)
	{	// teste incial so cria e apaga a tabela
		inicia_tabela(&codigo);
		mostra_tabela(&codigo);
		apaga_tabela(&codigo);
		return 2;
	};	// if

	if (modo == 3)
	{	// cria a tabela e varre o arquivo de dados
		inicia_tabela(&codigo);
		mostra_tabela(&codigo);
		fase1(arquivo_morse);
		apaga_tabela(&codigo);
		return 3;
	};	// if

	if (modo == 4)
	{	// cria a tabela, varre o arquivo e carrega a estrutura com
		// os dados e mostra a estrutura na tela
		inicia_tabela(&codigo);
		mostra_tabela(&codigo);
		fase2(arquivo_morse, &codigo);
		mostra_tabela(&codigo);
		apaga_tabela(&codigo);
		return 4;
	};	// if

	// modo normal de execucao

	inicia_tabela(&codigo); // aloca estrutura
	fase2(arquivo_morse, &codigo); // carrega a tabela a partir do arquivo

	char* teste = "Clube do Hardware";
	printf("\n\nUsando [%s] como parametro\n\n", teste);
	morse_frase(teste, &codigo);

	printf("\n\nUsando arquivo do autor do topico @Luis Vilo como parametro\n\n");
	morse("original1.txt", &codigo); // codifica o arquivo
	printf("\n\nFim :)\n\n");

	apaga_tabela(&codigo); // apaga estrutura
	return 0;
}	// main()


// 
// apaga as strings alocadas para a tabela
// do codigo Morse
//
void	apaga_tabela(Codigo_Morse* c)
{
	for (int i = 0; i < 127; i = i + 1)
		free(c->valor[i]);
};	// apaga_tabela()


/*
	fase1

	le o arquivo e mostra a tabela abaixo, com o valor
	em decimal do caracter, o caracter, o equivalente 
	no codigo Morse e o comprimento em sinais

		085 U   [..-]   (3)
		086 V   [...-]  (4)
		087 W   [.--]   (3)
		088 X   [-..-]  (4)
		089 Y   [-.--]  (4)
		090 Z   [--..]  (4)

*/
void	fase1(char* arquivo)
{
	const char* mascara = "%c%[^*]%c ";
	char	valor, codigo[80], asterisco; // o obvio
	int		n; // o total de valores lidos por scanf()
	FILE* tabela;
	tabela = fopen(arquivo, "r");
	if (tabela == NULL)	exit(-1);
	while (1)
	{	n = fscanf(tabela, mascara, &valor, codigo, &asterisco);
		if (n != 3) break;
		printf("\t%03d %c\t[%s]\t(%d)\n", valor, valor, codigo, strlen(codigo));
	}
	fclose(tabela);
	return;
};	// fase1()


/*
	fase 2

	Le o arquivo e coloca os valores na estrutura *cod. Os primeiros 10
	valores sao os codigos para os digitos de 0 a 9 e os ultimos 26
	os codigos para as letras de A a Z
*/
void	fase2(char* arquivo, Codigo_Morse* cod)
{
	const char* mascara = "%c%[^*]%c ";
	char*       pausa = "7"; // vai marcar as pausas na tabela
	char		Valor; // o caracter lido a cada linha
	char		Algo; // para 'comer' o asterisco no final das linhas
	char		Codigo[80]; // para o valor em Morse
	int			n; // o total de valores lidos por scanf()
	FILE*		tabela;

	tabela = fopen(arquivo, "r");
	if (tabela == NULL)
	{
		perror("Erro ao abrir tabela:");
		return;
	}	// if
	while (1)
	{
		n = fscanf(tabela, mascara, &Valor, Codigo, &Algo);
		if (n != 3) break;
		cod->valor[Valor] = malloc(sizeof(char) * strlen(Codigo) + 1);
		strcpy(cod->valor[Valor], Codigo);
	};	// while()

	// patch para inserir as pausas para espaco e TAB e '\n'
	free(cod->valor[9]); // por via das duvidas
	cod->valor[9] = malloc(sizeof(char) * strlen(pausa) + 1);
	strcpy(cod->valor[9], pausa); // TAB
	free(cod->valor[10]); // por via das duvidas
	cod->valor[10] = malloc(sizeof(char) * strlen(pausa) + 1);
	strcpy(cod->valor[10], pausa); // newline
	free(cod->valor[32]); // por via das duvidas
	cod->valor[32] = malloc(sizeof(char) * strlen(pausa) + 1);
	strcpy(cod->valor[32], pausa); // espaco

	// patch para inserir as letras minusculas que nao estao no 
	// arquivo do post no CH
	for (int i = 'a'; i <= 'z'; i = i + 1)
	{
		cod->valor[i] = malloc(sizeof(char) * strlen(cod->valor[i - 32]) + 1);
		strcpy(cod->valor[i], cod->valor[i - 32]);
	};	// end for

	fclose(tabela);
	return;
};	// fase2()


void    inicia_tabela(Codigo_Morse* c)
{
	for (int i = 0; i < 127; i = i + 1) c->valor[i] = NULL;
	return;
};	// inicia_tabela()


void	morse(char* texto, Codigo_Morse* cod)
{
	char	Valor; // o caracter lido a cada linha
	char*	pV = &Valor;
	FILE*   txt;

	txt = fopen(texto, "r");
	if (txt == NULL)
	{
		perror("Erro ao abrir txt:");
		return;
	}	// if
	while (! feof(txt) )
	{
		Valor = fgetc(txt);
		if(Valor>0) morse_play(pV, cod );
	};	// while
	fclose(txt);
	return;

};	// morse()


void	morse_frase(char* frase, Codigo_Morse* cod)
{
	char  l = (char)strlen(frase);
	for (int i = 0; i < l; i++)	morse_play(&frase[i], cod);
	return;
};	// morse_play()


void	morse_play(char* Valor, Codigo_Morse* cod)
{
	char* m_string = cod->valor[*Valor];
	char  l = (char)strlen(m_string);

	if (strcmp(m_string, "7") == 0)
	{	// pausa entre palavras

#ifdef __COM_SOM__
		Sleep(_dot_ * 7); // pausa

#endif
		if (*Valor == 10)
			printf("\n");
		else
			printf("    ");
		return;
	};	// if

	for (int i = 0; i < l; i++)
	{
		printf("%c", m_string[i]);

#ifdef __COM_SOM__
		if (m_string[i] == '.')
			Beep(_freq_, _dot_);
		else
			Beep(_freq_, (_dot_ * 3));
#endif

	};

	printf(" %c ", *Valor);

#ifdef __COM_SOM__
	Sleep(_dot_ * 3); // pausa por 3 dots
#endif
	return;
};	// morse_play()


void	mostra_tabela(Codigo_Morse* cod)
{
	for (int i = 0; i < 127; i++)
	{
		if (i == 32) continue;
		if (i == 10) continue;
		if (i == 9) continue;

		if (cod->valor[i] != NULL)
			printf("\t%03d %c\t[%s]\t(%d)\n", i, i, cod->valor[i], strlen(cod->valor[i]));
	};	// for
	return;
};	// mostra_tabela()


// fim do texto