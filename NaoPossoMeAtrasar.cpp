#include <iostream>
#include <vector>
#include <queue>
#include <climits> // para utilizar INT_MAX e representar a distância infinita

using namespace std;

class vertice{
	public:
        vertice();
        virtual ~vertice() {}
        int getD() const {return this->d;}
        void setD(int d) {this->d = d;}
        int getPi() const {return this->pi;}
        void setPi(int p) {this->pi = p;}
        void setInd(int i) {this->indice = i;}
        int getInd() const {return this->indice;}
        string getNome() const {return this->nome;}
        void setNome(string n) {this->nome = n;}
        bool operator<(const vertice &other) const;
        bool operator>(const vertice &other) const;
	protected:
        int pi; // predecessor
        int d; // distancia
        int indice; // numero do vertice
        string nome;
};

vertice::vertice(){
	this->pi = -1;
	this->d = 0;
	this->indice = 0;
	this->nome = "";
}

bool vertice::operator<(const vertice &other) const {
    return (this->d < other.getD());
}

bool vertice::operator>(const vertice &other) const {
    return (this->d > other.getD());
}

class grafo{
	public:
        grafo(int A, vector<vector<int>> &adj, vector<vector<int>> &pesos, vector<vertice> &v, int dFinal);
        virtual ~grafo() {}
        void initialize_single_source(int s);
        void relax(int o, int v);
        void dijkstra(int s);
        void calculaTempo(int k);
	private:
        priority_queue<vertice, vector<vertice>, greater<vertice>> Q; // fila de prioridades
        vector<vector<int>> Adj; // matriz de adjacência
        vector<vector<int>> w;  // w é a matriz de pesos
        vector<vertice> u; // vetor de vertices que compoe o grafo
        int destinoFinal; //indice do vertice correspondente ao alto
};

grafo::grafo(int A, vector<vector<int>> &adj, vector<vector<int>> &pesos, vector<vertice> &v, int dFinal){
	u = v;
	this->Adj = adj;
	this->w = pesos;
	this->destinoFinal = dFinal;
	this->Q = {};
}

void grafo::initialize_single_source(int s){
	for (unsigned long int i = 0; i<u.size();i++){
    	this->u[i].setD(INT_MAX); // representa a distância infinita
    	this->u[i].setPi(-1);
    	this->u[i].setInd(i+1);
	}
	this->u[s-1].setD(0);
}
        	// recebe o índice dos vértices para o relaxamento
void grafo::relax(int o, int v){
    vertice aux;
	if ((u[o-1].getD() != INT_MAX) && u[v-1].getD() > (u[o-1].getD() + w[o-1][v-1])){
    	u[v-1].setD(u[o-1].getD() + w[o-1][v-1]);
    	u[v-1].setPi(o);
    	aux = u[v-1]; // pego o vertice com as suas informações
        this->Q.push(aux); // vou enfileirar todas as vezes q houver relaxamento
	}
}

void grafo::dijkstra(int s){
    vertice aux;
	initialize_single_source(s);
	aux = u[s-1];
    this->Q.push(aux); // coloco a origem na fila de prioridades
    while(!Q.empty()){
        aux = Q.top(); // pego o mínimo
        Q.pop(); // retiro da fila
        for (unsigned long int i = 0; i<u.size(); i++){ // percorro todos os vertices
            if (Adj[aux.getInd()-1][i] == 1){ // percorro os vizinhos do vertices
                relax(aux.getInd(), i+1); // chamo o relaxamento para a aresta em questão
            }
        }
    }
}

void grafo::calculaTempo(int k){
    int tempo;
    if (k <= 30){ // se Anne gastar ate 30 min do trabalho ao ponto ela consegue pegar o onibus das 17:30
        tempo = 30;
    } else { // caso sontrario so o onibus das 17:50
        tempo = 50;
    }
    int j, aux = 0;
    int i = destinoFinal; // pego o índice referente ao destino
    while (u[i].getPi() != -1){ // calculo a distância total do caminho encontrado, por meio dos predecessores do destino até a origem
        j = u[i].getPi();
        aux += w[j-1][i];
        i = j-1;
    }
    tempo += aux;
    int h = 17;
    int m = 0;
    if (tempo < 60){
        cout << "17:" << tempo << endl;
    } else { // para converter minutos para horas e minutos
        h += tempo / 60;
        m = tempo % 60; // pega o resto da divisão: os minutos que sobram das horas completas
        cout << h << ":";
        if (m == 0){
            cout << "00" << endl;
        } else {
            if (m < 10){
                cout << "0" << m << endl;
            } else {
                cout << m << endl;
            }
        }
    }
    if (tempo > 60){ // caso Anee gaste mais que 60 minutos ela chegará à faculdade depois das 18, ou seja, se atrasou
        cout << "Ira se atrasar" << endl;
    } else {
        if (tempo <= 60){
            cout << "Nao ira se atrasar" << endl;
        }
    }
}

int main()
{
    int T, X, N, V, origem, destino, dFinal, oInicial;
    string O, D;
    vector<vertice> vert;
    vertice aux;
	while ((cin >> X >> N >> V) && (!(X==0 && N==0 && V==0))){ // lendo os testes
        dFinal = -1;
    	vector<vector<int>> ad(V, vector<int> (V, 0)); // matriz de adjacência
    	vector<vector<int>> p(V, vector<int> (V, 0)); // matriz de pesos
    	for (int l = 0; l < V; l++){ // preencho a matriz de adjacência auxiliar com zeros
        	for (int c = 0; c < V; c++){
            	ad[l][c] = 0;
        	}
    	}
   	   	vert = {};
    	for(int i = 0; i < N; i++){ // lendo as arestas (rotas)
        	cin >> O >> D >> T; // aresta de O para D e T é a distância
        	origem = -1; destino = -1;
        	for (unsigned long int i=0; i<vert.size(); i++){ // procuro se os vertices de origem e destino informados já estao no vetor de vertices
                if (vert[i].getNome() == O){ // se já está
                    origem = i; // pego o índice
                }
                if (vert[i].getNome() == D){ // se estiver pego indice
                    destino = i;
                }
        	}
        	if (origem == -1){ // caso o vertice nao tenha sido encontrado no for anterior basta adicionar o vertice
                aux.setNome(O);
                vert.push_back(aux);
                origem = vert.size()-1;
            }
        	if (destino == -1){ // basta adicionar o vertice
                aux.setNome(D);
                vert.push_back(aux);
                destino = vert.size()-1;
        	}
        	if (O == "alto"){ // pegando o indice correspondente ao destino e da origem
                dFinal = origem;
        	}
        	if (D == "alto"){
                dFinal = destino;
        	}
        	if (O == "varzea"){
                oInicial = origem;
        	}
        	if (D == "varzea"){
                oInicial = destino;
        	}
        	ad[origem][destino] = 1; // grafo direcional, posso ir apenas de a para b e não o contrário
        	p[origem][destino] = T; // pego o tempo da aresta que liga os dois locais
    	}
    	grafo g(V, ad, p, vert, dFinal); // entrego o nº de vértices, matriz de adjacência e pesos, vetor de vertices e indice do destino
    	g.dijkstra(oInicial+1); // começo a busca por varzea
    	g.calculaTempo(X); //  calculo o tempo gasto por Anne e mostro
	}
	return 0;
}
