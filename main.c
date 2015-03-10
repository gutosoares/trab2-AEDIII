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
* @file main.c
* @brief Arquivo principal com função main e interface implementadas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreB.h"

/**
* @brief Função principal main e Interface.
*/
int main(int argc, char** argv) {

	if (argc > 1) {
		
		char *filename;
		filename = argv[1];
		
		Driver(filename);
		printf("Sistema de gerenciamento de alunos usando árvore B.\n");

		int j = 0;
		int i = 0;
		char INS[4] = "INS\0";
		char REM[4] = "REM\0";
		char BUS[4] = "BUS\0";
		char ATU[4] = "ATU\0";
		char EXI[4] = "EXI\0";
		char MAT[4] = "MAT\0";
		char ID[3] = "ID\0";
		char opcao[4] = "NAO\0";
		
		//while (strcmp(opcao, EXI) != 0) {
		while(1) {
			scanf("%s", opcao);
			
/* INSERIR */			
			if (strcmp(INS, opcao) == 0 ) {
				//printf("INSERÇÃO\n");
				
				Aluno novoAluno;
				
				scanf("%s %s %s %s %lf", novoAluno.Nome, novoAluno.Identidade, novoAluno.CPF, novoAluno.Matricula, &novoAluno.RSG);

				int erroChave;
				Btree = BtreeMAT;
				ind = indMAT;
				erroChave = DriverRoot(novoAluno.Matricula, indDados->TotalRRN);
				
				if (erroChave != ERROR) {
									
					Btree = BtreeID;
					ind = indID;
					erroChave = DriverRoot(novoAluno.Identidade, indDados->TotalRRN);
					
					if (erroChave != ERROR) {
						
							indDados->TotalRRN++;
							indDados->AtivosRRN++;
							rewind(Data);
							fwrite(&(*indDados), sizeof(struct DataIndex), 1, Data);
							/* Write indexdata */
							fseek(Data, 0, SEEK_END);
							fwrite(&novoAluno, sizeof (struct Aluno), 1, Data);
							printf("SUCESSO\n");
					
					}else {
						Btree = BtreeMAT;
						ind = indMAT;
						DriverRemove(novoAluno.Matricula);
						printf("ERRO\n");	
					}
				} else {
					printf("ERRO\n");
				}				
/*  REMOVER  */
			} else 
				if (strcmp(REM, opcao) == 0) {
					//printf("REMOVER REFERENCIA\n");
					char op[4], Key[TAM_CHAVE + 1];
					scanf("%s %s", op, Key);
					
				
					if (strcmp(ID, op) == 0) { /* Por Identidade */
						
						Btree = BtreeID;
						ind = indID;
						
						/* Busca a chave para remover no arquivo de dados */
						int FOUND_RRN = -1;
						int FOUND_POS = -1;
						if (SearchKey(ind->TreeRRN, Key, &FOUND_RRN, &FOUND_POS) == TRUE) {
							
							Page *Seekpage = getPage(FOUND_RRN);

							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET);
							Aluno aluno;
							fread(&aluno, (sizeof (struct Aluno)), 1, Data);
							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET); /* Volta pra posicao pra marcar como apagado */
							fprintf(Data, "#");
							indDados->AtivosRRN--;
							rewind(Data);
							fwrite(&(*indDados), sizeof (struct DataIndex), 1, Data);/* Grava o index dos dados*/

							DriverRemove(aluno.Identidade); /* Remove */
							
							Btree = BtreeMAT; /* Remove nas duas arvores */
							ind = indMAT;
							DriverRemove(aluno.Matricula);
							printf("SUCESSO\n");
							
						} else {
							printf("ERRO\n");
						}	
						
					}else if(strcmp(MAT, op) == 0){ /* Por Matricula */
						
						Btree = BtreeMAT;
						ind = indMAT;
						
						/* Busca a chave para remover no arquivo de dados */
						int FOUND_RRN = -1;
						int FOUND_POS = -1;
						if (SearchKey(ind->TreeRRN, Key, &FOUND_RRN, &FOUND_POS) == TRUE) {
							
							Page *Seekpage = getPage(FOUND_RRN);

							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET);
							Aluno aluno;
							fread(&aluno, (sizeof (struct Aluno)), 1, Data);
							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET);
							fprintf(Data, "#");
							rewind(Data);
							indDados->AtivosRRN--;
							fwrite(&(*indDados), sizeof (struct DataIndex), 1, Data);/* Grava o index dos dados*/
							
							DriverRemove(aluno.Identidade);/* Remove */
							
							Btree = BtreeID; /* Remove nas duas arvores */
							ind = indID;
							DriverRemove(aluno.Matricula);
							printf("SUCESSO\n");
							
						} else {
							printf("ERRO\n");
						}	
					}
/* BUSCA */					
			} else 
				if (strcmp(BUS, opcao) == 0) {
					//printf("BUSCAR REFERENCIA\n");
					
					char op[4], Key[TAM_CHAVE + 1];
					scanf("%s %s", op, Key);
					
					if (strcmp(ID, op) == 0) { /* Por Identidade */
						
						Btree = BtreeID;
						ind = indID;

						int FOUND_RRN = -1;
						int FOUND_POS = -1;
						if (SearchKey(ind->TreeRRN, Key, &FOUND_RRN, &FOUND_POS) == TRUE) {

							Page *Seekpage = getPage(FOUND_RRN);

							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET);
							Aluno aluno;
							fread(&aluno, sizeof (struct Aluno), 1, Data);
							ImprimeAluno(aluno);
						} else {
							printf("ERRO\n");
						}	
					} else if(strcmp(MAT, op) == 0){ /* Por Matricula */
						
						Btree = BtreeMAT;
						ind = indMAT;

						int FOUND_RRN = -1;
						int FOUND_POS = -1;
						if (SearchKey(ind->TreeRRN, Key, &FOUND_RRN, &FOUND_POS) == TRUE) {

							Page *Seekpage = getPage(FOUND_RRN);

							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * sizeof (struct Aluno), SEEK_SET);
							Aluno aluno;
							fread(&aluno, sizeof (struct Aluno), 1, Data);
							ImprimeAluno(aluno);
							
						} else {
							printf("ERRO\n");
						}
				}
/* ATUALIZA */				
			} else 
				if (strcmp(ATU, opcao) == 0) {
					//printf("ATUALIZAR REFERENCIA\n");
					char op[4];
					char Key[TAM_CHAVE + 1];
					Aluno novoAluno;
					scanf("%s %s %s %s %s %s %lf", op, Key, novoAluno.Nome, novoAluno.Identidade, novoAluno.CPF, novoAluno.Matricula, &novoAluno.RSG);			
					
					if (strcmp(ID, op) == 0) {
						
						Btree = BtreeID;
						ind = indID;

						int FOUND_RRN = -1;
						int FOUND_POS = -1;
						if (SearchKey(ind->TreeRRN, Key, &FOUND_RRN, &FOUND_POS) == TRUE) {

							Page *Seekpage = getPage(FOUND_RRN);

							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET);
							Aluno aluno;
							fread(&aluno, sizeof (struct Aluno), 1, Data);
							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET);
							fwrite(&novoAluno, sizeof (struct Aluno), 1, Data);

							if (strcmp(novoAluno.Identidade, aluno.Identidade) != 0) {		
								DriverRemove(aluno.Identidade);
								DriverRoot(novoAluno.Identidade, Seekpage->RegisterArray[FOUND_POS].FileRRN);
							}
							if (strcmp(novoAluno.Matricula, aluno.Matricula) != 0) {			
								Btree = BtreeMAT;
								ind = indMAT;
								DriverRemove(aluno.Matricula);
								DriverRoot(novoAluno.Matricula, Seekpage->RegisterArray[FOUND_POS].FileRRN);
							}	
						} else {
							printf("ERRO\n");
						}	
					} else if(strcmp(MAT, op) == 0){ /* Por Matricula */
										
						Btree = BtreeMAT;
						ind = indMAT;

						int FOUND_RRN = -1;
						int FOUND_POS = -1;
						if (SearchKey(ind->TreeRRN, Key, &FOUND_RRN, &FOUND_POS) == TRUE) {

							Page *Seekpage = getPage(FOUND_RRN);

							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET);
							Aluno aluno;
							fread(&aluno, sizeof (struct Aluno), 1, Data);
							ImprimeAluno(aluno);
							ImprimeAluno(novoAluno);
							fseek(Data, sizeof(struct DataIndex) + Seekpage->RegisterArray[FOUND_POS].FileRRN * (sizeof (struct Aluno)), SEEK_SET);
							fwrite(&novoAluno, sizeof (struct Aluno), 1, Data);
							
							
							if (strcmp(novoAluno.Matricula, aluno.Matricula) != 0) {		
								DriverRemove(aluno.Matricula);
								DriverRoot(novoAluno.Matricula, Seekpage->RegisterArray[FOUND_POS].FileRRN);
							}
							
							if (strcmp(novoAluno.Identidade, aluno.Identidade) != 0) {			
								Btree = BtreeID;
								ind = indID;
								DriverRemove(aluno.Identidade);
								DriverRoot(novoAluno.Identidade, Seekpage->RegisterArray[FOUND_POS].FileRRN);
							}	
							
						} else {
							printf("ERRO\n");
						}
				}	
				
			} else
				if (strcmp(EXI, opcao) == 0) {
					char op[4];
					scanf("%s", op);
					
					if (strcmp(ID, op) == 0) {

						Btree = BtreeID;
						ind = indID;	
						Imprime(ind->TreeRRN, 0);
						break;
					} else 
						if (strcmp(MAT, op) == 0){
						
						Btree = BtreeMAT;
						ind = indMAT;	
						Imprime(ind->TreeRRN, 0);
						break;
					}
					
			} else {
				printf("Comando invalido\n");
			}
			//getchar();
		}
		
		/* Escreve o indice da arvore e fecha o arquivo */
		Btree = BtreeID;
		ind = indID;
		WriteIndex();
		free(ind);
		fclose(Btree);

		Btree = BtreeMAT;
		ind = indMAT;
		WriteIndex();
		free(ind);
		fclose(Btree);
		
		fclose(Data);
		return (EXIT_SUCCESS);
	}else {
		printf("\n Favor passar o nome do arquivo de dados como parametro\n Ex= SysAlunos 'nome do arquivo de dados'\n");
		return (EXIT_FAILURE);	
		
	}
}


