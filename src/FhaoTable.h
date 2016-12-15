/*
 * FhaoTable.h
 *
 *  Created on: Nov 1, 2016
 *      Author: david
 */

#ifndef FHAOTABLE_H_
#define FHAOTABLE_H_

#include <vector>
#include <algorithm>

using namespace std;

template <typename T,typename Hash,typename Equal>
class FhaoTable{
	using keyvalue=uint32_t;

public:

	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;


	FhaoTable():table_key(new keyvalue[INIT_SIZE]),table_size(INIT_SIZE),table_size_value(0){initTable();};
	FhaoTable(unsigned size):table_key(new keyvalue[size]),table_size(size),table_size_value(0){initTable();};
	FhaoTable(const FhaoTable& ft):table_key(new keyvalue[ft.table_size]),table_size(ft.table_size),table_size_value(ft.table_size_value){

		auto it1=table_key,it2=ft.table_key;
		for(;it1!=table_key+table_size;it1++,it2++)
			*it1=*it2;
		copy(ft.table_value.begin(),ft.table_value.begin(),table_value.begin());
	}

	~FhaoTable(){
		delete[] table_key;
	}

	bool insert(T t){
		if(((float)table_size_value/table_size)>MAX_LOADFACTOR)
			expandTable(findNewSize());

		size_t i=h(t)%table_size;
		keyvalue key=table_key[i];
		if(isEmptyKey(key)){
			insertElement(i,t);
			return true;
		}
		unsigned pos=getPosition(key);
		if(e(t,table_value[pos]))
			return false;
		if(!isCollision(key))
			table_key[i]=generateKey(pos,false,true);

		auto res=for_loop(i,t);
		if(!res.first)return false;
		insertElement(res.second,t);
		return true;
	}

	void clear(){
		delete []table_key;
		table_value.clear();
		table_key=new keyvalue[INIT_SIZE];
		table_size = INIT_SIZE;
		table_size_value=0;
	}

	bool count(T& t){
		if(find(t)==table_value.end())
			return false;
		return true;
	}

	iterator find(T& t){
		size_t i=h(t)%table_size;
		keyvalue key=table_key[i];
		unsigned pos=getPosition(key);
		if(isEmptyKey(key))
			return table_value.end();
		if(e(t,table_value[pos]))
			return (table_value.begin()+pos);
		if(!isCollision(key))
			return table_value.end();
		auto res=for_loop(i,t);
		if(res.first)
			return table_value.end();
		return table_value.begin()+getPosition(table_key[res.second]);
	}

	iterator begin(){
		return table_value.begin();
	}


	iterator end(){
		return table_value.end();
	}

	inline unsigned size(){
		return table_value.size();
	}

	inline T& operator[](unsigned i){
		return table_value[i];
	}

	void reserve(unsigned s){
		if(table_size*MAX_LOADFACTOR>=s)return ;
		expandTable(s*(1/(float)MAX_LOADFACTOR));
	}

private:


	inline keyvalue generateKey(unsigned pos,bool empty,bool collision){
		keyvalue e=(empty)?TEMPTY:0;
		keyvalue c=(collision)?TCOLLISION:0;
		return e|c|pos;
	}

	inline pair<bool,unsigned> for_loop(unsigned i,T& t){
		keyvalue key;
		for(unsigned j=1;j<table_size;j++){
			key=table_key[(i+j)%table_size];
			if(isEmptyKey(key)){
				return {true,(i+j)%table_size};
			}
			if(e(t,table_value[getPosition(key)]))
				return {false,(i+j)%table_size};
		}
		return {false,0};
	}

	inline pair<bool,keyvalue*> for_loop_it(keyvalue* i,T& t){
		keyvalue* j=i+1;
		for(;j!=table_key+table_size;j++){
			if(isEmptyKey(*j)){
				return {true,j};
			}
			if(e(t,table_value[getPosition(*j)]))
				return {false,j};
		}
		for(j=table_key;j!=i;j++){
			if(isEmptyKey(*j)){
				return {true,j};
			}
			if(e(t,table_value[getPosition(*j)]))
				return {false,j};
		}
		return {false,nullptr};
	}

	inline bool isEmptyKey(keyvalue k){
		return k>>31;
	}

	void initTable(){
		for(unsigned i=0;i<table_size;i++)
			table_key[i]=generateKey(0,true,false);

	}

	inline void insertElement(unsigned table_pos,T& t){
		table_key[table_pos]=generateKey(table_size_value,false,false);
		table_value.push_back(t);
		table_size_value++;
	}

	inline void insertElementIt(keyvalue& table_pos,T& t){
		table_pos=generateKey(table_size_value,false,false);
		table_value.push_back(t);
		table_size_value++;
	}

	inline bool isCollision(keyvalue key){
		return (key<<1)>>31;
	}

	inline unsigned getPosition(keyvalue key){
		return (key<<2)>>2;
	}


	inline unsigned findNewSize(){
		return table_size_value*(1/MIN_LOADFACTOR);
	}

	void expandTable(unsigned newsize){
		table_size=newsize;
		delete[] table_key;
		table_key = new keyvalue[table_size];
		initTable();
		for(unsigned j=0;j<table_size_value;j++){
			auto& t=table_value[j];
			size_t i=h(t)%table_size;
			keyvalue key=table_key[i];
			unsigned pos=getPosition(key);
			if(isEmptyKey(key)){
				table_key[i]=generateKey(j,false,false);
			}else{
				if(!isCollision(key))
					table_key[i]=generateKey(pos,false,true);
				do{
					i=(i+1)%table_size;
				}while(!isEmptyKey(table_key[i]));
				table_key[i]=generateKey(j,false,false);

			}
		}
	}


	static constexpr unsigned INIT_SIZE=100;
	static constexpr unsigned TEMPTY=1<<31;
	static constexpr unsigned TCOLLISION=1<<30;
	static constexpr unsigned TPOS=1073741823;
	static constexpr float MAX_LOADFACTOR=0.7;
	static constexpr float MIN_LOADFACTOR=0.15;

	Hash h;
	Equal e;
	keyvalue* table_key;
	vector<T> table_value;
	unsigned table_size;
	unsigned table_size_value;

};


#endif /* FHAOTABLE_H_ */
