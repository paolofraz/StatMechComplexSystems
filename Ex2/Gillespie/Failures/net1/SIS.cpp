#include <iostream>
#include <fstream>
#include <random>
#include <math.h>
#include <stdio.h>
#include <ctime>


using namespace std;

//double random_generator()
//{
//	std::random_device rd;
//	std::mt19937 gen((rd()));
//	std::uniform_real_distribution<> dis(0,1);

//	double r;
//	r=dis(gen);

//	cout<<"random number	"<<r<<endl;

//return r;
//}


int counting(int array[1000],int element)
{	
	int count=0;
	for (int i=0;i<1000;++i)
	{
		if (array[i]==element)
		{
			count=count+1;
		}
	}
return count;
}



int count_SI_edges(int state[1000],int node1[2573], int node2[2573])
{	
	int count=0;
	for (int i=0;i<2573;++i)
	{
		if (state[node1[i]]!=state[node2[i]])
		{
			count=count+1;
		}
	}
return count;
}


void infecting(int state[1000],int node1[2573],int node2[2573],int l)
{
	random_device rd;
	mt19937 gen((rd()));
	uniform_int_distribution<> dis(1,l);

	double r;
	r=dis(gen);

	//srand(time(NULL));
	//int j=(rand()%l)+1;
	//int count=0;	
	
	int i=0;
	int count=0;

	do 				//200=lunghezza node1
	{
		if (node1[i]!=node2[i])
		{
			count=count+1;
		}
	i=i+1;
	}while(count!=r);
					
	state[node1[i-1]]=1;
	state[node2[i-1]]=1;

}

void recovering(int state[1000],int inf)
{
	random_device rd;
	mt19937 gen((rd()));
	uniform_int_distribution<> dis(1,inf);

	double r;
	r=dis(gen);

	//double r;
	//r=random_generator();
	

	//cout<<"r"<<r<<endl;
	int count=0;
	
	int i=0;
	do  //metti la lunghezza di state
	{
		if(state[i]==1)
		{
			count=count+1;
		}
	i=i+1;
	} while(count!=r);
	
	state[i-1]=0;
}

int main()
{	

random_device rd;
mt19937 gen((rd()));
uniform_real_distribution<> dis(0,1);

cout<<"E' partito il programma"<<endl;
double mu=0.5;									//definire i parametri
										//definire i parametri




int node1[2573];						//trovo i miei file 
int node2[2573];

ifstream iFile("net1.txt"); 
if (!iFile) // Always test file open
{
cout << "Error opening input file" << endl; return -1;
}
int value1;
int value2;
int i=0;
while (iFile>>value1>>value2)
{
	node1[i]=value1;
	node2[i]=value2;
	i=i+1;
}


cout<<"E' arrivato quì"<< endl;


ofstream diagramma("diagramma.txt"); //apro un file di scrittura if(!libretto){ //verifica corretta apertura del file
if(!diagramma){ ///verifica corretta apertura del file
cout << "Errore in apertura del file di output." << endl; 
return -1;
}

const clock_t begin_time = clock();
for(double lambda=0.06;lambda<0.0700001;lambda=lambda+0.001)
{

		
//vector <double> inf_sim;					//inf medio per simulazione

for (int sim=0; sim<1000; ++sim)	//numero di simulazioni da mediare sul parametro
{											
										//inizializza stato
	int state[1000];					// 1=infetto 0=sano
	for(int i=0;i<1000;++i)
	{
		if(i<50)						//inserire numero di infetti inziiali
		state[i]=1;
		else
		{
		state[i]=0;						
		}
	}

	double t=0;
	vector <int> inf;

	//ofstream libretto("inf.txt"); //apro un file di scrittura if(!libretto){ //verifica corretta apertura del file
	//if(!libretto){ //verifica corretta apertura del file
	//cout << "Errore in apertura del file di output." << endl; 
	//return -1;
	//}	

	int n=10000;//numero iterazioni
	i=0;	
	vector <double> t_inf;				//traccia degli infetti nel tempo della simulazione
	while(i<n)		//via alla simulazione
	{	
		//cout<<i<<endl;
		int l=count_SI_edges(state,node1,node2);	//calcola tutti i parametri che mi servono 
		int inf=counting(state,1);					//infetti propensity rate ecc
		//int sus=counting(state,0);
		double a1=l*lambda;
		double a2=inf*mu;
		double a0=a1+a2;
		//cout<<"I:	"<<inf<<"\t"<<endl;
		//cout<<"S:	"<<sus<<endl;

		//libretto<<t<<"\t"<<inf<<endl;
	
		if(inf!=0)									//controlla che il numero di infetti 
		{											//non sia zero
			
				if(n>8000)							//da quando vuoi cominciare
					{								//a tenere traccia degli inf
					t_inf.push_back(inf);
					}
				
			//double r1;
			//r1=dis(gen);
			
			//double r;
			//r=random_generator();
			//double r1;
			//r1=random_generator();
		
			//t=-log(r1/a0)/a0;					//sceglie il tempo (inutile per il diagramma)

			double r2;							//sceglie che reazione fare
			r2=dis(gen);
			//cout<<"p   "<<r2<< endl;
			//double r2;
			//r2=random_generator();
			if (r2<a1/a0)						//si infetta
			{
				infecting(state,node1,node2,l);
				
				//cout<<"infecting"<<endl;
			}
			else
			{
				recovering(state,inf);
				//cout<<"recovering"<<endl;
			}

			i=i+1;		
		}
		else
		{
			t_inf.clear();
			t_inf.push_back(inf);			
			break;									//esce dalla simulazione
		}

	}


	double media_sim=0;
	for(int k=0;k<t_inf.size();++k)
		{
		media_sim=media_sim+t_inf[k];			//calcola la media della della
												//della singola simulaizone
		}
		media_sim=media_sim/t_inf.size();
		//cout<<media_sim<< endl;
		//inf_sim.push_back(media_sim);
	
	diagramma<< lambda<<"\t"<<media_sim<<endl;
		
}

//double media_100=0;								//media delle 100 simulazioni
//	for(int k=0;k<inf_sim.size();++k)
//		{
//		media_100=media_100+inf_sim[k];															
//		}
//		media_100=media_100/inf_sim.size();
//		diagramma<< lambda<<"\t"<<media_100<<endl;
//		//cout<<media_100<<endl;
		//inf_sim.push_back(media_100);
//		

cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC<<endl;
}
return 0;
}