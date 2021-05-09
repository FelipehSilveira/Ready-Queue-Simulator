/* Trabalho Prático 2 - Simulador de Fila de Prontos(Ready Queue)
Disciplina C012 - Sistemas Operacionais
Professor: Renzo Paranaíba Mesquita
Instituto Nacional de Telecomunicações - Inatel

Equipe: 
Felipe Pereira Silveira					Matrícula: 676		Curso: GEA		Período: P7
Flávio Henrique Madureira Bergamini		Matrícula: 1450		Curso: GEC		Período: P6
*/

#include <bits/stdc++.h>

struct processos {
    int id;                   // ID do processo            
    int burst;                // Tempo de duração do processo
    int prioridade;           // Prioridade do Processo
    int execucao;             // Tempo de execução do processo
    int espera;               // Tempo de espera do processo
    struct processos *prox;
};

struct processos *init_processos (int id, int burst, int prioridade);
void fcfs (struct processos *proc, int burst_time[]);
void listprocs (struct processos *proc);
void rr (struct processos *proc, int quantum, int burst_time[]);

 
int main (void) {
    
    struct processos *plist, *ptmp; //Inicializando os processos
    int burst_time[5];
    burst_time[0] = 10;
    burst_time[1] = 1;
    burst_time[2] = 2;
    burst_time[3] = 1;
	burst_time[4] = 5;    
    
    //Declaração dos processos
    plist       = init_processos(1, burst_time[0], 3);
    plist->prox = init_processos(2,  burst_time[1], 1); ptmp = plist->prox;
    ptmp->prox  = init_processos(3,  burst_time[2], 3); ptmp = ptmp->prox;
    ptmp->prox  = init_processos(4,  burst_time[3], 4); ptmp = ptmp->prox;
    ptmp->prox  = init_processos(5,  burst_time[4], 2);
    
   
    listprocs(plist); //Listando os processos
    fcfs(plist,burst_time); //Algoritmo não-preemptivo First Come First Served
    rr(plist,1,burst_time); //Algoritmo Preemptivo Round Robin
    
    while (plist != NULL) { //Limpando a struct com os processos - Fim da execução
        ptmp = plist;
        plist = plist->prox;
        free(ptmp);
    };
    return(0);
};

// Inicialização da lista de processos
struct processos *init_processos (int id, int burst, int prioridade) {
    struct processos *proc;
 	 proc = (struct processos*)malloc(sizeof(struct processos)); 
  if (proc == NULL) {
        printf("Erro Fatal: Falha Alocacao de memoria.\nFinalizar.\n");
        exit(1);
    };
    proc->id = id;
    proc->burst = burst;
    proc->prioridade = prioridade;
    proc->execucao = 0;
    proc->espera = 0;
    proc->prox = NULL;
    return(proc);
};

// Escalonamento FCFS - o primeiro que chega é o primeiro a ser executado
void fcfs (struct processos *proc, int burst_time[]) {
    int tempo = 0, inicio, fim;
    int tempo_medio=0;
    int ordem[50];
    int i = 0;
  struct processos *tmp = proc;
  printf("\tEscalonamento FCFS\n");
    printf("\n");
  while (tmp != NULL) {
    inicio = tempo;
    tempo += tmp->burst;
    fim = tempo;
    printf("Processo: %d\tBurst: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tmp->burst, inicio, fim);
    tempo_medio += inicio;
    ordem[i] = tmp->id;
    tmp = tmp->prox;
    i++;
  };
  
    printf("\nTempo medio de espera: %f\tms\n",tempo_medio/5.0);
    printf("\nGantt Chart: \n\n");
    for(int i = 0; i < 5; i++)
	{
		printf("P%d", ordem[i]);
		for(int j = 0; j < burst_time[ordem[i]-1];j++ )
			printf("-");
	}
    printf("\n\n\n");
};

// Listando Processos 
void listprocs (struct processos *proc) {
  struct processos *tmp = proc;
  printf("\tProcessos\n");
  printf("\n");
  while (tmp != NULL) {
    printf("Processo: %d\tPrioridade: %d\tBurst: %d\n", tmp->id, tmp->prioridade, tmp->burst);
    tmp = tmp->prox;
  };
  printf("\n\n");
 };
 
// Escalonamento Round-Robin 
void rr (struct processos *proc, int quantum, int burst_time[]) {
  int jobsremain, passes;
  int tempo_medio = 0;
  int ordem[50];
   int i = 0;
  struct processos *copia, *tmpsrc, *tmp, *slot;
  printf("\tEscalonamento Round-Robin - Quantum: %d)\n", quantum);
  printf("\n");
  tmpsrc = proc;
  copia = tmp = NULL;
  while (tmpsrc != NULL) {
  	
    if (copia == NULL) {
    copia = init_processos(tmpsrc->id, tmpsrc->burst, tmpsrc->prioridade);
    tmp = copia;
    } else {
    tmp->prox = init_processos(tmpsrc->id, tmpsrc->burst, tmpsrc->prioridade);
    tmp = tmp->prox;
    };
    tmpsrc = tmpsrc->prox;
    
  };
  
  // Programa rotina de análise de prioridade  
  jobsremain = 2;
  slot = NULL;
 
  while (jobsremain) {
  	
    jobsremain = 0;
    // Seleciona o próximo processo efetuando sua alocação 
    if (slot == NULL) {
    slot = copia;
    jobsremain = 2;
    } else {
    passes = 0;
    do {
      if (slot->prox == NULL) {
        passes++;
        slot = copia;
      } else {
        slot = slot->prox;
      };
    } while (passes <= 3 && slot->burst == slot->execucao);
    if (passes <= 3) {
      jobsremain = 2;
    };
    };
    // Executa um ciclo 
    tmp = copia;
    while (tmp != NULL) {
    if (tmp->burst > tmp->execucao) {
 	
      if (tmp == slot) {
        tmp->execucao += quantum;
        ordem[i] = tmp->id;
 		i++;
      } else {
        tmp->espera += quantum;
      };
      
    };
    
    tmp = tmp->prox;
    };
  };
  // Exibe os resultados e elimina as replicações 
  tmp = copia;
  while (tmp != NULL) {
    printf("Processo: %d\tBurst: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tmp->burst, tmp->espera, tmp->execucao + tmp->espera);
    tmpsrc = tmp;
    tempo_medio += tmp->espera;	
    tmp = tmp->prox;
    free(tmpsrc);
  };
  
  printf("\nTempo medio de espera: %f\tms\n",tempo_medio/5.0);
  
   printf("\nGantt Chart: \n\n");
    for(int ii = 0; ii < i; ii++)
	{
		printf("P%d", ordem[ii]);
		for(int j = 0; j < quantum;j++ )
		{
			if(ordem[ii + j] == ordem[ii])
				printf("-");
			
		}
			
	}
	
  printf("\n");
};

