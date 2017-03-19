#include "Person.h"
#include <iostream>

using namespace std;

Person::Person()
{
	firstName = "";	// 이름
	lastName = "";	// 성
	gender = "";		// 성별
	level = -1;			// 항렬
	job = "";		// 직업
	birthOfDate = "";// 탄생일
	deathOfDate = "";// 사망일
	partner = NULL;	// 배우자
	child = NULL;	// 자식
	sibling = NULL;	// 형제
	parent = NULL;  //부
	name = "";
	relation = "";
}

Person::Person(string _name, string _relation)
{
	firstName = "";	// 이름
	lastName = "";	// 성
	gender = "";		// 성별
	level = -1;			// 항렬
	job = "";		// 직업
	birthOfDate = "";// 탄생일
	deathOfDate = "";// 사망일
	partner = NULL;	// 배우자
	child = NULL;	// 자식
	sibling = NULL;	// 형제
	parent = NULL;  //부
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
	cout << "성 : ";
	cin >> lastName;
	cout << "이름 : ";
	cin >> firstName;
	cout << "성별 : ";
	cin >> gender;
	cout << "항렬 : ";
	cin >> level;
	cout << "직업 : ";
	cin >> job;
	cout << "탄생일 : ";
	cin >> birthOfDate;
	cout << "사망일 : ";
	cin >> deathOfDate;
	partner = NULL;
	child = NULL;
	sibling = NULL;
	parent = NULL;
	
	cout << "저장될 위치와 관계를 입력해 주세요." << endl;
	cout << "이름 : ";
	cin >> name;
	cout << "관계(배우자, 자식, 형제) : ";
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
string* Person::ref_relation()	// relation을 참조하는데 사용.
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
	cout << "<<<<<<<<<<<< 인물 정보 >>>>>>>>>>>>>>" << endl;
	cout << "성 : " << lastName << endl;
	cout << "이름 : " << firstName << endl;
	cout << "성별 : " << gender << endl;
	cout << "항렬 : " << level << endl;
	cout << "직업 : " << job << endl;
	cout << "탄생일 : " << birthOfDate << endl;
	cout << "사망일 : " << deathOfDate << endl;
	cout << "=====================================" << endl;
}