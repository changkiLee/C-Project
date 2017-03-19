#include "Data.h"
#include <iostream>
#define SAME_SIZE 10

using namespace std;

Person *same[SAME_SIZE];//중복검사허용을 위해 클래스 배열선언
int count = 0;

Data::Data()
{
	head = new Person("","","",-1,"","","",NULL,NULL,NULL,NULL, "", "");
	z = new Person("","","",-1,"","","",NULL,NULL,NULL,NULL, "", "");
}

Data::~Data()
{

}

void Data::insertNode(Person *per)
{
	Person *h = head;
	Person *search_p, *parent_p;
	Person *temp = new Person();
	temp = per;
	string _name = temp->get_relationName();
	string _relation = temp->get_relation();

	if(h->get_name() == "")
	{
		head = temp;	
	}
	else{
		search_p = SearchNode(_name, head);		// 탐색
		if(search_p == z){
			cout << "존재하지 않습니다." << endl;
		}
		// 배우자
		if(_relation == "배우자"){
			if(search_p->get_partner() == NULL){				
				search_p->partner = temp;//찾은사람의 파트너에 뉴노드를 연결
				temp->partner = search_p;//뉴노드의 파트너에 찾은사람을 연결
			}
			else{
				cout << "배우자가 이미 있습니다." << endl;
			}
		}

		// 자식(막내로 저장)
		else if(_relation == "자식"){
			parent_p = search_p;

			if(search_p->get_child() == NULL){
				search_p->child = temp;
				temp->parent = parent_p;//부모와 연결
			}
			else{
				search_p = search_p->get_child();
				while(search_p->get_sibling() != NULL)
				{
					search_p = search_p->get_sibling();
				}
				search_p->sibling = temp;
				temp->parent = parent_p;//부모연결
			}			
		}

		// 형제(막내로 저장)
		else if(_relation == "형제"){
			while(search_p->get_sibling() != NULL)
			{
				search_p = search_p->get_sibling();
			}
			search_p->sibling = temp;
			temp->parent = search_p->parent;//부모와연결
		}

		else{
			cout << "알수없는 관계입니다." << endl;
		}
	}
}

void Data::changeNode(string _name, Person *per)
{
	Person* search_p;
	Person *temp = new Person();
	temp = per;

	search_p = SearchNode(_name, head);

	// 탐색에 성공한 경우 데이터 수정
 	if(search_p != z){
 		search_p->changeData(temp);
 	}
}

Person* Data::SearchNode(string _name, Person *hd)
{
	Person *h = hd;
	Person *res = z;

	while(h->get_name() != ""){
		// 본인 탐색
		if(h->get_name() == _name){
			res = h;
			break;
		}
		// 파트너 탐색
		if(h->get_partner() != NULL){
			if(h->get_partner()->get_name() == _name){
				res = h->get_partner();
				break;
			}
		}
		// 자식 탐색
		if(h->get_child() != NULL){
			if(h->get_child()->get_name() == _name){
				res = h->get_child();
				break;
			}
			res = SearchNode(_name, h->get_child());
		}
		// 형제 탐색
		h = h->get_sibling();
		if(h == NULL)
			break;
	}

	return res;
}
void Data::SearchNode2(string _name, Person *hd)//중복허용 탐색
{
	Person *h = hd;
	Person *res = z;
	while(h->get_name() != ""){
		// 본인 탐색
		if(h->get_name() == _name)
			same[count++] = h;

		// 파트너 탐색
		if(h->get_partner() != NULL){
			if(h->get_partner()->get_name() == _name){
				same[count++] = h->get_partner();
			}
		}
		// 자식 탐색
		if(h->get_child() != NULL)
			SearchNode2(_name, h->get_child());
		
		// 형제 탐색
		h = h->get_sibling();
		if(h == NULL)
			break;
	}
}

Person* Data::SearchNodebyBirth(string _name, string _birth, Person *hd)
{
	Person *h = hd;
	Person *res = z;

	while(h->get_name() != ""){
		// 본인 탐색
		if(h->get_name() == _name && h->get_birthOfDate() == _birth){
			res = h;
			break;
		}
		// 파트너 탐색
		if(h->get_partner() != NULL){
			if(h->get_partner()->get_name() == _name && h->get_partner()->get_birthOfDate() == _birth){
				res = h->get_partner();
				break;
			}
		}
		// 자식 탐색
		if(h->get_child() != NULL){
			if(h->get_child()->get_name() == _name && h->get_child()->get_birthOfDate() == _birth){
				res = h->get_child();
				break;
			}
			res = SearchNodebyBirth(_name, _birth, h->get_child());
		}
		// 형제 탐색
		h = h->get_sibling();
		if(h == NULL)
			break;
	}

	return res;
}

void Data::printFamilyTree(Person *hd)
{
	Person *h = hd;

	while(h->get_name() != ""){
		// 본인 출력
		cout << "( " << h->get_name() << " )" << endl;
		h->print_Person();

		// 파트너 출력
 		if(h->get_partner() != NULL){
			cout << "( " << h->get_name() << "의 배우자 )" << endl;
			h->get_partner()->print_Person();
		}
		// 자식 탐색
		if(h->get_child() != NULL){
			cout << "( " << h->get_name() << "의 장손 )" << endl;
			printFamilyTree(h->get_child());
		}
		// 형제 탐색	::= 제일 마지막에 하므로 종손이 먼저 출력됨.
		h = h->get_sibling();
		if(h == NULL)
			break;
	}
}

Person* Data::getHead()
{
	return head;
}

Person** Data::getSame(){
	return same;
}

void Data::initSame(){
	int i;
	for(i = 0; i < SAME_SIZE; i++)
		same[i] = NULL;
	count = 0;
}

int Data::CalculateRelation(string _name1, string _name2, string _birth1, string _birth2, Person *hd){
	Person *p1, *p2;
	Person *h = hd;
	int i, level1, level2, level_f = -1;

	p1 = SearchNodebyBirth(_name1, _birth1, h);
	if(p1->get_name() == ""){
		cout << "데이터가 존재하지 않습니다." << endl;
		return -1;
	}
	p2 = SearchNodebyBirth(_name2, _birth2, h);
	if(p2->get_name() == ""){
		cout << "데이터가 존재하지 않습니다." << endl;
		return -1;
	}
	if(p1->get_name() == p2->get_name() && p1->get_birthOfDate() == p2->get_birthOfDate()){
		cout << "동일인입니다." << endl;
		return -1;
	}

	level1 = p1->get_level();
	level2 = p2->get_level();

	if(level1 == level2){
		level_f = 0;
		for(i = 1;; i++){
			p1 = p1->get_parent();
			p2 = p2->get_parent();
			level_f+=2;
			if(p1 == p2){
				break;
			}
		}
	}
	else if(level1 < level2){
		level_f = 0;
		for(i = 0; i < level2 - level1; i++){
			p2 = p2->get_parent();
			level_f++;	
		}
		for(i = 1;; i++){
			if(p1 == p2){
				break;
			}
			else{
				level_f+=2;
			}
			p1 = p1->get_parent();
			p2 = p2->get_parent();			
		}
	}
	else if(level1 > level2){
		level_f = 0;
		for(i = 0; i < level1 - level2; i++){
			p1 = p1->get_parent();
			level_f++;
		}
		for(i = 1;; i++){
			if(p1 == p2){
				break;
			}
			else{
				level_f+=2;
			}
			p1 = p1->get_parent();
			p2 = p2->get_parent();			
		}
	}
	return level_f;
}

void Data::saveDatas(FILE* op, Person *hd)
{
	Person* h = hd;
	
	while(h->get_name() != "") 
	{

		fprintf(op, "%s %s %s %d %s %s %s %s %s\n", &(h->get_firstName())[0], &(h->get_lastName())[0], &(h->get_gender())[0], h->get_level(), 
			&(h->get_job())[0], &(h->get_birthOfDate())[0], &(h->get_deathOfDate())[0], &(h->get_relationName())[0], &(h->get_relation())[0] );

		if(h->get_partner() != NULL) {
			fprintf(op, "%s %s %s %d %s %s %s %s %s\n", &(h->get_partner()->get_firstName())[0], &(h->get_partner()->get_lastName())[0], &(h->get_partner()->get_gender())[0], h->get_partner()->get_level(), 
				&(h->get_partner()->get_job())[0], &(h->get_partner()->get_birthOfDate())[0], &(h->get_partner()->get_deathOfDate())[0], &(h->get_partner()->get_relationName())[0], &(h->get_partner()->get_relation())[0] );
		}
		
		// 자식 탐색
		if(h->get_child() != NULL){			
			saveDatas(op, h->get_child());
		}

		h = h->get_sibling();

		if(h == NULL) 
		{
			break;
		}
	}
}