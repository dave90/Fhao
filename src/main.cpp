//============================================================================
// Name        : HashProbe.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <functional>
#include <bitset>
#include <time.h>
#include <unordered_set>

#include "FhaoTable.h"

using namespace std;

unsigned size,charset,check;


void printBit(unsigned p){
	bitset<32> x(p);
	cout<<x<<endl;
}


string generateString(){
	string s="";
	unsigned range=122-97;
	for(unsigned i=0;i<charset;i++){
		unsigned r=(rand()%range)+97;
		s+=r;
	}
	return s;
}

struct HE{
	bool operator()(const string& s1,const string& s2){
		return s1==s2;
	}
	size_t operator()(const string& s2){
		hash<string> h;
		return h(s2);
	}

};

int main(int argc,char *argv[]) {
	srand(time(0));
	if(argc<5){
		cerr<<"ERROR ARGUMENT"<<endl;
		exit(0);
	}
	unsigned reserve;
	size=atoi(argv[1]);
	charset=atoi(argv[2]);
	reserve=atoi(argv[3]);
	check=atoi(argv[4]);

	unsigned N=size;
	FhaoTable<string,HE,HE> fa;
	unordered_set<string> h;
	if(reserve){
		fa.reserve(N);
		h.reserve(N);
	}
	vector<string> strings;
	for(unsigned i=0;i<N;i++){
		string s=generateString();
		strings.push_back(s);
	}

//	cout<<"FINISH GENERATION"<<endl;

	clock_t start=clock();
	for(unsigned i=0;i<N;i++){
		h.insert(strings[i]);
	}
	clock_t finish=clock();
	auto timeH=((float)(finish-start)/CLOCKS_PER_SEC);
	cout<<"HASH "<<timeH<<endl;

	start=clock();
	for(unsigned i=0;i<N;i++){
		fa.insert(strings[i]);
	}
	finish=clock();
	auto timeF=((float)(finish-start)/CLOCKS_PER_SEC);
	cout<<"Probe "<<timeF<<endl;

	int g=(timeH-timeF)/timeH*100;
	cout<<"GAIN "<<g<<"%"<<endl;
	if(check){

//		cout<<"SIZE "<<h.size()<<" "<<fa.size()<<endl;


		if(fa.size()!=h.size())cout<<"ERROR SIZE"<<endl;
		for(unsigned i=0;i<fa.size();i++)
			if(!h.count( fa[i]))
				cout<<"ERROR ELEMENT "<<fa[i]<<" not exist in hash"<<endl;
	}

	return 0;
}

