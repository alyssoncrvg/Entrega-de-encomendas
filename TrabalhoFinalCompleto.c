#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
	float x;
	float y;
} Ponto;

float calcularDistancia(Ponto *PtrPlano, float **PtrDistancias,int qtdCidade){
    int i,j;
    float distancia;
    //percorrer as linhas da matriz distancia
    for(i=0;i<qtdCidade;i++){
        //percorrer as colunas da matriz distância
        for(j=0;j<qtdCidade;j++){
            //se linha e coluna forem iguais, quer dizer que calculará a distância para ela mesma
            if(i==j){
                distancia = 0;
            }
            else{
                //verificar que o valor da coluna da matriz distancia equivale a linha da cidade que estamos calculando
                //a distância, ou seja, o valor de x2 e y2 e o valor da linha da matriz distancia equivale a cidade que estamos verificando a 
                //distância, ou seja, a x1 e y1 
                distancia = sqrt(pow(PtrPlano[j].x - PtrPlano[i].x,2) + pow(PtrPlano[j].y-PtrPlano[i].y,2));
            }
            PtrDistancias[i][j] = distancia;
        }
    }
}

int calcularRota(int qtdCidade,float **PtrDistancias,int cidadeAtual, int *cidadesVisitadas, float *distancias, FILE *rota){

	static int pos=0;
	static int quebrarlinha = 1;
    //utilizando uma maneira de calcular a menor distância sem um número pré-definido para evitar futuros problemas
	int i,prxCidade;
    float menorDistancia;

	//pegar o primeiro valor diferente da cidade de onde está 
	// e que a cidade ainda não tenha sido visitada para usar como a menor distância inicial
	for(i=0;i<qtdCidade;i++){
		if(cidadeAtual != i && cidadesVisitadas[i]!=1) {
			menorDistancia = PtrDistancias[cidadeAtual][i]; 
			prxCidade = i;
			break;
		}
	}
	
	//escolhendo a cidade com a menor distância ainda não visitada
	for(i;i<qtdCidade;i++){
        //condicional caso a distancia pega anteriormente seja a menor do vetor
	    if(cidadeAtual != i && PtrDistancias[cidadeAtual][i]<menorDistancia && cidadesVisitadas[i]!=1){
            menorDistancia = PtrDistancias[cidadeAtual][i];
            prxCidade = i;
	    }
	}

	//adcionando ao vetor a distância gasta de uma cidade a outra
	distancias[pos] = menorDistancia;
	pos++;

	//organizando o vetor das cidades já visitadas
	cidadesVisitadas[prxCidade] = 1;

	//mostrando na tela a ordem das cidades a serem percorridas
	printf("%d --> ", prxCidade+1);
	if(quebrarlinha%10==0){
		fprintf(rota,"%d -> \n", prxCidade+1);
	} else fprintf(rota,"%d -> ", prxCidade+1);
	quebrarlinha++;

	return prxCidade;  
};

float calcularDistanciaTotal(float *distancias,int qtdCidade){
	if(qtdCidade<0) return 0;

	return distancias[qtdCidade] + calcularDistanciaTotal(distancias, qtdCidade-1);
}

// função atoi não estava atendendo o requisitado então foi criada uma função para que o problema fosse resolvido
int myAtoi(char s[]){
    int i=0,pos=0,constante=1;
    int *nums = NULL;

	// verificar quando o array está com um caracter que é reconhecido como número na tabela ASCII
    for(i;i<strlen(s);i++){
        if(s[i]>47 && s[i]<58){
            break;
        }
    }

    for(i;i<strlen(s);i++){
		// verificar se na posição anterior existe o sinal de negativo
        if(s[i-1]=='-'){
            constante=-1;
        }
		// quando chega em um espaço quer dizer que aquele número acabou
        if(s[i]==' ') break;

		// guardar os números separados dentro de um vetor
        if(s[i]>47 && s[i]<58){
            nums = realloc(nums, sizeof(int));
            nums[pos] = 0;
            int num = s[i] - '0';
            nums[pos] = num;
            pos++;
        }
    }

    int tam = pos-1;
    int valor=0;
    int auxTam = tam+1;

	// realizar a soma desses números com as casas decimais que as representam
    for(i=0;i<auxTam;i++){
        valor += pow(10,tam)*nums[i];
        tam -= 1;
    }
    
    return valor*constante;
}

int main(){
	// variáveis que serão utilizadas durante o código
    int  *PtrCidades = NULL, cidadeAtual, cidadePartida, concluido = 0;
    float **PtrDistancias = NULL;
    int i,j,qtdCidade,id;
	char NomeArquivo[100];
	char NomeArqRota[100] = "Rota - ";

	// verificar de qual arquivo será lido os pontos
	printf("Nome do arquivo a ser lido: ");
	scanf("%s", NomeArquivo);
	fflush(stdin);
	// abrir arquivos de pontos
	FILE *arq = NULL;
	arq = fopen(NomeArquivo, "r");

	if(arq==NULL) {
		printf("Arquivo de pontos não encontrado\n");
		return -2;
	};

	// encontrar qual a cidade que o arquivo representa
	int contador = 0, verificador = 1;
	char NomeCidade[100];
	char cidade[100];
	// variáveis descartáveis para armazenar na linha que é padronizada e evitar ficar comparando através de caracteres
	char nomeDescart1[100],nomeDescart2[100], nomeDescart3[100], nomeDescart4[100], nomeDescart5[100];
	while(fgets(NomeCidade, 100, arq)!=NULL){
		contador++;
		// verificar na linha 2 do arquivo
		if(contador==1){
			fscanf(arq,"%s %s %s %s %s %s", &nomeDescart1,&nomeDescart2,&nomeDescart3,&nomeDescart4,&nomeDescart5,&cidade);
			break;
		}
	}
	
	// Salvar arquivo de rota junto com o nome da cidade para diferenciar dos demais arquivos lidos e poder salvar todos eles separadamente
	int pos = 7;
	for(i=0;i<strlen(cidade);i++){
		NomeArqRota[pos] = cidade[i];
		pos++;
	}

	// adcionando a extensão txt no nome do arquivo
	char txt[4] = ".txt";
	for(i=0;i<4;i++){
		NomeArqRota[pos] = txt[i];
		pos++;
	}	

	// arquivo aonde será armazenada a rota
	FILE *rota = NULL;
	rota = fopen(NomeArqRota, "w");

	if(rota==NULL) {
		printf("Arquivo de rota não encontrado\n");
		return -2;
	};

	printf("Cidade: %s\n", cidade);
	fprintf(rota, "Cidade %s\n", cidade);
	fclose(arq);

	// enocntrar qual a dimensão do arquivo
	contador=0; 
	verificador = 1;
	char dimensao[100];
	arq = fopen(NomeArquivo,"r");
	while (fgets(dimensao, 100,arq)!=NULL){
		contador++;
		for(i=0;i<strlen(dimensao);i++){
			// arquivo segue padrão que não se repete esta sequência para qualquer outro dado presente nele
			// então aquela string que se inicia com D e logo após vem o I é a linha de dimensão
			if(dimensao[0]=='D' && dimensao[1]=='I'){
				qtdCidade=myAtoi(dimensao);
				verificador = 0;
				break;
			}
		}
		// parar leitura assim que encontrado tamanho do arquivo
		if(verificador==0) break;
	}
	fclose(arq);
	printf("Quantidade de cidades: %d\n", qtdCidade);
	
	// deixando propício a quantidade de cidades que irá receber
    PtrCidades = malloc(sizeof(int) * qtdCidade);

    if(PtrCidades == NULL) return -1;
    //iniciando os vetores com 0 para evitar possíveis lixos de memória
    for(i=0;i<qtdCidade;i++){
        PtrCidades[i] = 0;
    }

    PtrDistancias = (float**)malloc(sizeof(int*)*qtdCidade);
	for(i=0;i<qtdCidade;i++){
		PtrDistancias[i] = (float*)malloc(sizeof(int)*qtdCidade);
	}

    if(PtrDistancias == NULL) {
		printf("Distancias não adcionadas\n");
		return -1;
		};

    for(i=0;i<qtdCidade;i++){
        for(j=0;j<qtdCidade;j++){
            PtrDistancias[i][j] = 0;
        }
    }
	//Finalizando de preencher as matrizes e vetores com o valor 0

    //armazenando os pontos do plano
	Ponto *PtrLocPlano;
	PtrLocPlano = malloc(sizeof(Ponto));
	
	// verficar os pontos 
    int contador2=0;
	i = 0;
	arq = fopen(NomeArquivo, "r");

	if(arq==NULL) {
		printf("Arquivo não encontrado\n");
		return -2;
	};
	// Utilizando uma lógica para que se considere a leitura duas linhas após a linha de dimensão ser lida
	// como os arquivos seguem um certo padrão essas linhas de pontos sempre estarão duas linhas a mais que a de
	// dimensão.
	while(!feof(arq)){
		// ler até aonde se inicia os pontos
		contador2++;
		fgets(dimensao, 100, arq);
		
		if(contador2>contador+1){
			// quando encontrado a linha de inicio ler e armazenar os pontos
			PtrLocPlano = realloc(PtrLocPlano, (i+1)*sizeof(Ponto));
			fscanf(arq,"%d %f %f",&id, &PtrLocPlano[i].x, &PtrLocPlano[i].y);
			fflush(stdin);
			i++;
		}
	}

	fclose(arq);

	// vetor para ser armazenado as distancias percorridas e somada ao final
	float *distancias;
	distancias = malloc(sizeof(float) * qtdCidade);

    //transformando os pontos do plano em distâncias entre um ponto e outro
    calcularDistancia(PtrLocPlano,PtrDistancias,qtdCidade);

	// Colocando como cidade de partida a cidade 1, supondo que seja dela que comece a jornada de entregas
	cidadeAtual = 0;
	cidadePartida = 0;
	
	//adcionando no vetor de cidades visitadas a cidade de partida
	PtrCidades[cidadeAtual] = 1;
	
	printf("%d -> ", cidadeAtual+1);
	fprintf(rota,"%d -> ", cidadeAtual+1);

    while(concluido !=1){
		//verificando se já se foi passado por todas as cidades
	  	for(i=0;i<qtdCidade;i++){
	      if(PtrCidades[i]==0) break;
	    }
	    //se já tiver sido passada por todas as cidades voltará para cidade de partida
	    if(i==qtdCidade) {
			concluido =1;
			break;
		}
	    
	    //calcular a rota pela função
	    cidadeAtual = calcularRota(qtdCidade,PtrDistancias,cidadeAtual,PtrCidades, distancias, rota);
	  }

    //Retornando a cidade de partida
	printf("%d\n", cidadePartida + 1);
	fprintf(rota, "%d\n", cidadePartida+1);
	
	// adcionando a distancia da ultima cidade para a cidade de partida
	distancias[qtdCidade] = PtrDistancias[cidadeAtual][cidadePartida];

	// chamar a soma da distancia a partir da função para calcular a distancia total gasta
	float distanciafunc;
	distanciafunc = calcularDistanciaTotal(distancias, qtdCidade);

	// printando saida da distancia e confirmando ao usuário que o arquivo foi criado
	printf("Distancia Total Percorrida: %.2fkm\n", distanciafunc);
	printf("Arquivo com a rota a ser percorrida foi criado com sucesso!!!");
	fprintf(rota,"Distancia Total Percorrida: %.2fkm", distanciafunc);

    return 0;
}
