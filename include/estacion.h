class estacion
{
private:
    static const int MAX = 100;
    int n;
    bool adj[MAX][MAX]= {};


public:
    estacion(int cantidad){
        n = cantidad;
    }

    void agregarArista(int a,int b){
        adj[a][b]=true;
        adj[b][a]=true;
    }

    bool estanConectados(int a,int b){
        return adj[a][b];
    }
};