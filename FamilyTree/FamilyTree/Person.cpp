#include "Person.h"
#include <iostream>

using namespace std;

Person::Person()
{
	firstName = "";	// �̸�
	lastName = "";	// ��
	gender = "";		// ����
	level = -1;			// �׷�
	job = "";		// ����
	birthOfDate = "";// ź����
	deathOfDate = "";// �����
	partner = NULL;	// �����
	child = NULL;	// �ڽ�
	sibling = NULL;	// ����
	parent = NULL;  //��
	name = "";
	relation = "";
}

Person::Person(string _name, string _relation)
{
	firstName = "";	// �̸�
	lastName = "";	// ��
	gender = "";		// ����
	level = -1;			// �׷�
	job = "";		// ����
	birthOfDate = "";// ź����
	deathOfDate = "";// �����
	partner = NULL;	// �����
	child = NULL;	// �ڽ�
	sibling = NULL;	// ����
	parent = NULL;  //��
	name = _name;
	relation = _relation;
}

Person::Person(string _firstName, string _lastName, string _gender, int _level,	string _job, string _birthOfDate, string _deathOfDate, Person *_partner, Person *_child, Person *_sibling, Person *_parent, string _name, string _relation)
{
	firstName = _firstName;
	lastName = _lastName;
	gender = _gender;
	level = _level;
	job = _job;
	birthOfDate = _birthOfDate;
	deathOfDate = _deathOfDate;
	partner = _partner;
	child = _child;
	sibling = _sibling;
	parent = _parent;
	name = _name;
	relation = _relation;
}

Person::~Person()
{

}

Person* Person::make_Person()
{
	cout << "�� : ";
	cin >> lastName;
	cout << "�̸� : ";
	cin >> firstName;
	cout << "���� : ";
	cin >> gender;
	cout << "�׷� : ";
	cin >> level;
	cout << "���� : ";
	cin >> job;
	cout << "ź���� : ";
	cin >> birthOfDate;
	cout << "����� : ";
	cin >> deathOfDate;
	partner = NULL;
	child = NULL;
	sibling = NULL;
	parent = NULL;
	
	cout << "����� ��ġ�� ���踦 �Է��� �ּ���." << endl;
	cout << "�̸� : ";
	cin >> name;
	cout << "����(�����, �ڽ�, ����) : ";
	cin >> relation;

	Person *t = new Person(firstName, lastName, gender, level, job, birthOfDate, deathOfDate, partner, child, sibling, parent, name, relation);
	return t;
}

void Person::changeData(Person *d)
{
	lastName = d->lastName;
	firstName = d->firstName;
	gender = d->gender;
	level = d->level;
	job = d->job;
	birthOfDate = d->birthOfDate;
	deathOfDate = d->deathOfDate;
}

Person* Person::get_partner()
{
	return partner;
}

Person* Person::get_child()
{
	return child;
}

Person* Person::get_sibling()
{
	return sibling;
}

Person* Person::get_parent()
{
	return parent;
}

string Person::get_name()
{
	return lastName + firstName;
}

string Person::get_firstName()
{
	return firstName;
}

string Person::get_lastName()
{
	return lastName;
}

string Person::get_gender()
{
	return gender;
}

string Person::get_job()
{
	return job;
}

string Person::get_birthOfDate()
{
	return birthOfDate;
}

string Person::get_deathOfDate()
{
	return deathOfDate;
}

string Person::get_relationName()
{
	return name;
}

string Person::get_relation()
{
	return relation;
}
/*
string* Person::ref_relation()	// relation�� �����ϴµ� ���.
{
	return &relation;
}
*/

int Person::get_level()
{
	return level;
}


void Person::print_Person()
{
	cout << "<<<<<<<<<<<< �ι� ���� >>>>>>>>>>>>>>" << endl;
	cout << "�� : " << lastName << endl;
	cout << "�̸� : " << firstName << endl;
	cout << "���� : " << gender << endl;
	cout << "�׷� : " << level << endl;
	cout << "���� : " << job << endl;
	cout << "ź���� : " << birthOfDate << endl;
	cout << "����� : " << deathOfDate << endl;
	cout << "=====================================" << endl;
}