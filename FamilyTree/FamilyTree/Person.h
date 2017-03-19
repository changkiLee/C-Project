#ifndef _PERSON_H
#define _PERSON_H
#include <string>
using namespace std;

class Person{
public:
	Person();				// ������
	Person(string _name, string _relation);
	Person(string _firstName, string _lastName, string _gender, int _level,	string _job, string _birthOfDate, string _deathOfDate, Person *_partner, Person *_child, Person *_sibling, Person *_parent, string _name, string _relation);
	~Person();				// �Ҹ���

	Person* make_Person();	// �ι� ����
	void print_Person();	// �ι� ���
	
	void changeData(Person *d);	// �ι� ����

	Person* get_partner();
	Person* get_child();
	Person* get_sibling();
	Person* get_parent();
	string get_name();
	string get_firstName();
	string get_lastName();
	string get_gender();
	string get_job();
	string get_birthOfDate();
	string get_deathOfDate();
	string get_relation();
	string get_relationName();
	int get_level();

	// link
	Person *partner;	// �����
	Person *child;		// �ڽ�
	Person *sibling;	// ����
	Person *parent;     //��
private:
	// data
	string firstName;	// �̸�
	string lastName;	// ��
	string gender;		// ����
	int level;			// �׷�
	string job;			// ����
	string birthOfDate;	// ź����
	string deathOfDate;	// �����
	string relation;	// ����
	string name;
};

#endif // _PERSON_H