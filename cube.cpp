#include "cube.h"

default_random_engine generator;
normal_distribution<long double> distribution(0.0,1.0);

myvectorcube::myvectorcube(char* name,int* pts,int dim,bool t){
		type=t;
		d=dim;
		nameid= new char[strlen(name)+1];
		strcpy(nameid,name);
		points= new int[dim];
		for(int i=0;i<dim;i++){
				points[i]=pts[i];
		}

}


myvectorcube::myvectorcube(int n,long double* pts,int dim,bool t){
		type=t;
		d=dim;
		id=n;
		flag=-1;
		pointsd= new long double[dim];
		for(int i=0;i<dim;i++){
				pointsd[i]=pts[i];

		}

}

myvectorcube::~myvectorcube(){
	delete []nameid;
	if(type==0){
		delete []points;
	}
	else{
		delete []pointsd;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


listcubenode::listcubenode(){
	next=NULL;
	ptr=NULL;
	g=NULL;
}

listcubenode::~listcubenode(){
	if(g!=NULL)
	delete []g;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
listcube::listcube(){
	head=NULL;
	size=0;
}

listcube::~listcube(){
	listcubenode* current = head;
	while( current != NULL) {
    	listcubenode* next = current->next;
    	current->ptr=NULL;
    	delete current;
    	current = next;
	}
	head = NULL;
}

void listcube::insert(myvectorcube* nodept,int* g){
	listcubenode* temp = new listcubenode();
	temp->ptr=nodept;
	temp->g=g;
	if(head==NULL){
		head=temp;
	}
	else{
		temp->next=head;
		head=temp;
	}
	size++;
		
}

void listcube::insertend(myvectorcube* nodept){
	listcubenode* temp = new listcubenode();
	temp->ptr=nodept;
	if(head==NULL){
		head=temp;
	}
	else{
			listcubenode* current = head;
			while(current->next != NULL){
				current=current->next;
			}
			current -> next = temp;

	}
	size++;
		
}

bool listcube::is_empty(){
	if (size<=0){
		return true;
	}
	return false;
}

void listcube::display(){
	listcubenode* temp = head;
	while(temp!=NULL){
		cout<<temp->ptr->nameid<<" ";
		for(int i=0;i<temp->ptr->d;i++){
			cout<<temp->ptr->points[i]<<" ";
		}
		cout<<endl;
		temp=temp->next;
	}

}

int listcube::length(){
	return size;
}

myvectorcube* listcube::search(myvectorcube* pt,bool metric,int K,int* g){
	long double min=100000;
	int flag=0;
	myvectorcube *vec=NULL;
	listcubenode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			if((euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d)<min)&&(flag==0)){
				min=euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
		else{
			if((cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d)<min)&&(flag==0)){
				min=cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
	}
	//cout<<min<<endl;
	if(vec!=NULL)vec->dis=min;
	return vec;

}
myvectorcube* listcube::exhaustedsearch(myvectorcube* pt,bool metric,int K){
	long double min=100000;
	myvectorcube *vec=NULL;
	listcubenode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			if(euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d)<min){
				min=euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
		else{
			if(cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d)<min){
				min=cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
	}
	//cout<<min<<endl;
	if(vec!=NULL)vec->dis=min;
	return vec;

}

void listcube::Rangesearch(myvectorcube* pt,bool metric,int R,ofstream& output,int K,int* g){
	myvectorcube *vec=NULL;
	int flag=0;
	listcubenode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			if((euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d)<R)&&(flag==0)){
				vec=temp->ptr;
				output<<vec->nameid<<endl;
			}
			temp=temp->next;
		}
		else{
			if((cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d)<R)&&(flag==0)){
				vec=temp->ptr;
				output<<vec->nameid<<endl;
			}
			temp=temp->next;
		}
	}

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int* hashtable:: hashfunction(myvectorcube* p,bool metric){
	if(metric==0){
		int* g=new int[K+1];
		int* binary=new int[K];
		for(int i=0;i<K;i++){
			g[i]=floor((inner_product(p,v[i])+t[i])/w);
			binary[i]=g[i]%2;
		}
		long sum =0;
		for(int i=K-1;i>=0;i--){
			if(binary[i]==1){
				sum+=pow(2.0,K-i-1);
			}
		}

		delete []binary;
		//cout<<sum<<endl;
		g[K]=sum;
		return g;
	}
	else if(metric==1){
		int* g=new int[K+1];
		for(int i=0;i<K;i++){
			//cout<<inner_product(p,v[i])<<endl;
			if(inner_product(p,v[i])<0){
				g[i]=0;
			}
			else{
				g[i]=1;
			}
		}

		long sum =0;
		for(int i=K-1;i>=0;i--){
			if(g[i]==1){
				sum+=pow(2.0,K-i-1);
			}
		}

		//delete []g;
		//cout<<sum<<endl;
		g[K]=sum;
		return g;
		//return sum;

	}
}



hashtable::hashtable(int s,int k,int d,bool metric,int p):size(s),K(k),dim(d),probes(p){
	listcube = new listcube*[s];
	for(int i=0;i<s;i++){
		listcube[i]=new listcube();
	}
	if(metric==0){
		v=new myvectorcube*[k];
		t=new long double[k];
		r=new int[k];
		for(int i=0;i<k;i++){
		  	long double* map=new long double[dim];
			for (int i=0; i<dim; i++) {
				long double number = distribution(generator);
			    //cout<<number<<endl;
			   	map[i]=number;
			}
			char buff[4];
			strcpy(buff,"vvv");
			v[i] = new myvectorcube(buff,map,dim,1);
			t[i] = w * (rand() / (RAND_MAX + 1.0));
			r[i] = rand()%800;
			delete []map;
			//r[i] = 800 * (rand() / (RAND_MAX + 1.0));
			//r[i] = -1000 + (rand() / (RAND_MAX + 1.0))*(2000+1);
		}
	}
	else if (metric==1){
		v=new myvectorcube*[k];
		//cout<<"product"<<endl;
		for(int i=0;i<k;i++){
			//long double* map=normal_distribute(dim);
	  		long double* map=new long double[dim];
			for (int i=0; i<dim; i++) {
				long double number = distribution(generator);
			    //cout<<number<<endl;
			   	map[i]=number;
			}
			char buff[4];
			strcpy(buff,"vvv");
			v[i] = new myvectorcube(buff,map,dim,1);
			delete []map;
		}
		t=NULL;
		r=NULL;


	}
}

hashtable::~hashtable(){
	for(int i=0;i<size;i++){
		delete listcube[i];
	}
	delete []listcube;
	for(int i=0;i<K;i++){
		delete v[i];
	}
	delete []v;

	if(t!=NULL)
	delete []t;

	if(r!=NULL)
	delete []r;

}

void hashtable::insert(myvectorcube* nodept,bool metric) { 
		int* g=hashfunction(nodept,metric);
		///g[K] is the index
		listcube[g[K]]->insert(nodept,g);
		//listcube[hashfunction(nodept,metric)]->insert(nodept);
}


void hashtable::hashprint(){
	for(int i=0;i<size;i++){
		listcube[i]->display();
	}
}

myvectorcube* hashtable::search(myvectorcube* pt,bool metric){
		int* g=hashfunction(pt,metric);
		long double min;
		///g[K] is the index
		myvectorcube* temp = listcube[g[K]]->search(pt,metric,K,g);
		if(temp!=NULL){
			min = temp -> dis;
		}
		else{
			min=100000;
		}
		int* result = Nextprobes(g[K]);
		for(int i=0;i<probes;i++){
			myvectorcube* temp2=listcube[result[i]]->search(pt,metric,K,g);
			if(temp2!=NULL){
				if(temp2->dis<min){
					min = temp2 -> dis;
					temp=temp2;
				}
			}
		}
		delete []result;
		delete []g;
		return temp;
}

void hashtable::Rangesearch(myvectorcube* pt,bool metric,int R,int j,myvectorcube** centroids){
		int* g=hashfunction(pt,metric);
		///g[K] is the index
		listcube[g[K]]->Rangesearch(pt,metric,R,output,K,g);
		int* result = Nextprobes(g[K]);
		for(int i=0;i<probes;i++){
			listcube[result[i]]->Rangesearch(pt,metric,R,output,K,g);
		}
		delete []result;
		delete []g;
}

int* hashtable::Nextprobes(int index){
	int* d = new int[size];
	for(int i=0;i<size;i++){
		d[i]=HammingDist(i,index);
		//cout<<i<<" "<<d[i]<<endl;
	}	
	int* e = new int[size];
	int* result = new int[probes];
	for (int i = 0; i < probes; i++)
	{
			int min=100000,min2;
			for(int j=0;j<size;j++){
				if(d[j]!=0){
					if(d[j]<min){
						min=d[j];
						min2=j;
					}
				}
			}
			d[min2]=0;
			result[i]=min2;
	}
	delete []d;
	delete []e;
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


long double euc(long double* X,long double* Y,int dim){
	long double sum=0;
	for(int i=0;i<dim;i++){
		sum+=((X[i]-Y[i])*(X[i]-Y[i]));
	}
	return sqrt(sum);
}

long double cos(long double* X,long double* Y,int dim){
	long double sum1=0,sum2=0,sum3=0;
	for(int i=0;i<dim;i++){
		sum1+=X[i]*Y[i];
		sum2+=X[i]*X[i];
		sum3+=Y[i]*Y[i];
	}
	long double dis = sum1 / (sqrt(sum2)*sqrt(sum3));
	return 1 - dis;
}

long double inner_product(myvectorcube* X,myvectorcube* Y){
	long double sum1=0;
	for(int i=0;i<X->d;i++){
		sum1+=(X->points[i])*(Y->pointsd[i]);
	}
	return sum1;
}

long double* normal_distribute(int times){

  default_random_engine generator;
  normal_distribution<long double> distribution(0.0,1.0);

  long double* p=new long double[times];
  //cout<<times<<endl;
  for (int i=0; i<times; i++) {
    long double number = distribution(generator);
    //cout<<number<<endl;
    p[i]=number;
  }

  return p;
}

int HammingDist(int x, int y)
{
  int dist = 0;
  int val = x^y;
  while(val)
  {
    ++dist; 
    val &= val - 1;
  }
  return dist;
}



