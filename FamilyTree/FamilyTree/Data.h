#ifndef _DATA_H
#define _DATA_H
#include "Person.h"
#include <string>

using namespace std;

class Data{
public:
	Data();
	~Data();

	void insertNode(Person *per);	// ����
	void changeNode(string _name, Person* per);						// ����
	Person* SearchNode(string _name, Person *hd);					// Ž��
	void SearchNode2(string _name, Person *hd);
	Person* SearchNodebyBirth(string _name, string _birth, Person *hd);
	void printFamilyTree(Person *hd);								// ���
	Person* getHead();                                 
	Person** getSame();                                             //�������� Ž��
	void initSame();                                                //�������� Ž���� �ʿ��� �ʱ�ȭ �޼ҵ�
	int CalculateRelation(string _name1, string _name2, string _birth1, string _birth2, Person *hd);              //�̼����
	void saveDatas(FILE *op, Person *hd);										//������ ����.

private:
	Person *head, *z;

};

#endif // _DATA_H