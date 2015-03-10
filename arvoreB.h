/*
* Trabalho baseado no seguinte código fonte.
* O trabalho original sofreu modificações para atender
* os requisitos do trabalho 2.

* Algoritmos e Estruturas de Dados III
* Professor: André Pimenta
* Alunos: Rogério Dias, Pedro Victor, Pedro Augusto
* Turma: 10A
* Sistema de Referências Bibliográficas utilizando Árvore-B
* IDE utilizada: NetBeans versão 7.2.2
* Sistema Operacional: Ubuntu 12.04
*/

/**
* @file btree.h
* @brief Arquivo com o cabeçalho de implementação da árvore B.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#define Order 2
#define MAXKEYS 2*Order
#define NIL -1
#define TRUE 1
#define FALSE 0
#define PROMOTION -2
#define NO_PROMOTION -3
#define ERROR -4
#define NOT_FOUND -5
#define FOUND -6
#define LEAF -7
#define NOT_LEAF -8
#define UNDERFLOW -9
#define NOT_UNDERFLOW -10
#define CONCATENATE -11
#define TAM_CHAVE 5
#define CHAVE_VAZIA "#####\0" /* Deve conter tantos ' # ' quanto o tamanho da chave */

    extern FILE *Btree;
    extern FILE *Data;

	extern FILE *BtreeID;
	extern FILE *BtreeMAT;


    /*Initial Header with the reference for tree.*/
    typedef struct Index {
        int TreeRRN;
        int CurrentRRN;
        //int DataRRN;
    } Index;

    extern Index*ind;
    
    extern Index*indID;
    extern Index*indMAT;
    
    
    extern Index*indID;
	extern FILE *BtreeID;
	extern Index*indMAT;
	extern FILE *BtreeMAT;
	

	
	typedef struct DataIndex {
		int TotalRRN;
		int AtivosRRN;
		
	} DataIndex;
	
	extern DataIndex *indDados;

	typedef struct Aluno {
		
		char Nome[50];
		char Identidade[TAM_CHAVE + 1];
		char CPF[15];
		char Matricula[TAM_CHAVE + 1];
		double RSG;
	}Aluno;
	
    typedef struct Register {
        /* data */
        char Chave[TAM_CHAVE + 1]; 
        int FileRRN; /*Reference for Structure on File.*/
    } Register;

    /*Structure of page*/
    typedef struct Page {
        /* data */
        int RRN;
        Register RegisterArray[MAXKEYS];
        int RRNsArray[MAXKEYS + 1];
        short NumberOfKeys;
    } Page;

    /*Structure of page*/
    typedef struct AuxPage {
        /* data */
        int RRN;
        short NumberOfKeys;
        Register RegisterArray[MAXKEYS + 1];
        int RRNsArray[MAXKEYS + 2];
    } AuxPage;
    
    	/* Pagina auxiliar que sera gravada no arquivo binario. */
	typedef struct BinaryPage {
			
		Register RegisterArray[MAXKEYS];
		long RRNsArray[MAXKEYS + 1];
		short NumberOfKeys;
	} BinaryPage;


    void ParseString(char *string);
    void Initialize(Page*page);
    void WritePage(Page* page, int RRN);
    Page* getPage(int RRN);
    void WriteIndex();
    int Driver();
    int BinarySearch(Page* page, char *Key, int*found);
    int SearchKey(int RRN, char *Key, int *FOUND_RRN, int *FOUND_POS);
    void InsertKey(Page* page, char *Key, int R_SON, int DataRRN);
    void Split(char *key, int r_child, Page *p_oldpage, char *promo_key, int *promo_r_child, Page *p_newpage, int *DataRRNPromoted);
    Page *createRoot(char *Key, int right, int left);
    int Insert(int CurrentRRN, char *Key, int DataRRN, int *PROMO_R_CHILD, char *PROMO_KEY, int *DataRRNPromoted);
    int DriverRoot(char *Key, int DataRRN);
    void Organize(Page *page, int Position);
    void DeletePage(int *ProblemRRN);
    int DriverRemove(char *Key);
    int isPageLeaf(Page *page);
    int Concatenate(Page *CurrentPage, int *ProblemRRN);
    int Redistribution(Page *CurrentPage, int *ProblemRRN);
    int Remove(int CurrentRRN, char *Key, int *Troca, char *ChaveTroca, Page *Dad, int *Trocando, int *ProblemRRN);
    void printtree(int t);
    void ImprimeAluno(Aluno aluno);
    void Imprime(int RRN, int x);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
