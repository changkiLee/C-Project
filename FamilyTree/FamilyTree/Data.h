#ifndef _DATA_H
#define _DATA_H
#include "Person.h"
#include <string>

using namespace std;

class Data{
public:
	Data();
	~Data();

	void insertNode(Person *per);	// 삽입
	void changeNode(string _name, Person* per);						// 삭제
	Person* SearchNode(string _name, Person *hd);					// 탐색
	void SearchNode2(string _name, Person *hd);
	Person* SearchNodebyBirth(string _name, string _birth, Person *hd);
	void printFamilyTree(Person *hd);								// 출력
	Person* getHead();                                 
	Person** getSame();                                             //동명이인 탐색
	void initSame();                                                //동명이인 탐색에 필요한 초기화 메소드
	int CalculateRelation(string _name1, string _name2, string _birth1, string _birth2, Person *hd);              //촌수계산
	void saveDatas(FILE *op, Person *hd);										//데이터 저장.

private:
	Person *head, *z;

};

#endif // _DATA_H