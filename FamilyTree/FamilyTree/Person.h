#ifndef _PERSON_H
#define _PERSON_H
#include <string>
using namespace std;

class Person{
public:
	Person();				// 생성자
	Person(string _name, string _relation);
	Person(string _firstName, string _lastName, string _gender, int _level,	string _job, string _birthOfDate, string _deathOfDate, Person *_partner, Person *_child, Person *_sibling, Person *_parent, string _name, string _relation);
	~Person();				// 소멸자

	Person* make_Person();	// 인물 생성
	void print_Person();	// 인물 출력
	
	void changeData(Person *d);	// 인물 수정

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
	Person *partner;	// 배우자
	Person *child;		// 자식
	Person *sibling;	// 형제
	Person *parent;     //부
private:
	// data
	string firstName;	// 이름
	string lastName;	// 성
	string gender;		// 성별
	int level;			// 항렬
	string job;			// 직업
	string birthOfDate;	// 탄생일
	string deathOfDate;	// 사망일
	string relation;	// 관계
	string name;
};

#endif // _PERSON_H