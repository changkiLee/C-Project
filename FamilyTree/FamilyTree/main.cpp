#include <iostream>
#include "Data.h"
#include "Person.h"
#include <Windows.h>
#include <stdio.h>
#include <time.h>

#define X 1
#define Y 1
using namespace std;

typedef enum 
{
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKY_BLUE,
	DARK_RED,
	DARK_VOILET,
	DARK_YELLOW,
	WHITE,
	GRAY,
	BLUE,
	GREEN,
	SKY_BLUE,
	RED,
	VOILET,
	YELLOW
}COLOR_TYPE;

void SetColor(COLOR_TYPE color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void gotoxy(int x, int y)
{
	COORD Pos = {x - 1, y - 1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void clearSquare()
{
	gotoxy(8,13);puts("▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩");
	gotoxy(8,14);puts("▩                                                              ▩");
	gotoxy(8,15);puts("▩                                                              ▩");
	gotoxy(8,16);puts("▩                                                              ▩");
	gotoxy(8,17);puts("▩                                                              ▩");
	gotoxy(8,18);puts("▩                                                              ▩");
	gotoxy(8,19);puts("▩                                                              ▩");
	gotoxy(8,20);puts("▩                                                              ▩");
	gotoxy(8,21);puts("▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩");
}
int main()
{
	// 파일 입출력
	FILE *fp;
	int res;
	fp = fopen("family.txt", "r+");
	if(fp == NULL){
		fprintf(stderr, "파일 생성에 실패하였습니다.\n");
		return 1;
	}
	//start page
	gotoxy(X+2,Y+1); SetColor(GREEN);   printf( "▩▩▩▩▩　　　▩　　　▩　　　▩　▩▩▩　▩　　　　▩　　　▩"); Sleep(100);
	gotoxy(X+2,Y+2); SetColor(GREEN);	printf( "▩　　　　　　▩　▩　　▩▩　▩▩　　▩　　▩　　　　▩　　　▩"); Sleep(100);
	gotoxy(X+2,Y+3); SetColor(GREEN);	printf( "▩▩▩▩▩　▩▩▩▩▩　▩　▩　▩　　▩　　▩　　　　　▩　▩"); Sleep(100);
	gotoxy(X+2,Y+4); SetColor(GREEN);	printf( "▩　　　　　▩　　　▩　▩　▩　▩　　▩　　▩　　　　　　▩"); Sleep(100);
	gotoxy(X+2,Y+5); SetColor(GREEN);	printf( "▩　　　　　▩　　　▩　▩　▩　▩　▩▩▩　▩▩▩▩　　　▩"); Sleep(100);
	Sleep(1000);
	gotoxy(X+2,Y+8); SetColor(YELLOW); printf( "　　　▩▩▩▩▩　　▩▩▩▩▩　　▩▩▩▩▩　　▩▩▩▩▩"); Sleep(100);
	gotoxy(X+2,Y+9); SetColor(YELLOW); printf( "　　　　　▩　　　　▩　　　▩　　▩　　　　　　▩"); Sleep(100);
	gotoxy(X+2,Y+10); SetColor(YELLOW); printf( "▩▩　　　▩　　　　▩▩▩▩▩　　▩▩▩▩▩　　▩▩▩▩▩　　▩▩"); Sleep(100);
	gotoxy(X+2,Y+11); SetColor(YELLOW); printf( "　　　　　▩　　　　▩　　▩　　　▩　　　　　　▩"); Sleep(100);
	gotoxy(X+2,Y+12); SetColor(YELLOW); printf( "　　　　　▩　　　　▩　　　▩　　▩▩▩▩▩　　▩▩▩▩▩"); Sleep(100);
	Sleep(20);
	//gotoxy(15,8);
	gotoxy(X+2,Y+15); SetColor(GRAY); printf("= Digital Family Tree System ="); Sleep(90);
	gotoxy(X+2,Y+16); SetColor(GRAY); printf("= Designed by LEE CHANG KI   ="); Sleep(90);
	gotoxy(X+2,Y+17); SetColor(GRAY); printf("= Designed by KWON SU JIN    ="); Sleep(90);
	gotoxy(X+2,Y+18); SetColor(GRAY); printf("= Designed by LEE HAK HOON   ="); Sleep(90);
	gotoxy(X+2,Y+19); SetColor(GRAY); printf("= Designed by HAN     JUNE   ="); Sleep(90);
	gotoxy(X+2,Y+20); SetColor(GRAY); printf("= AT ALGORITHM CLASS IN 2012 ="); Sleep(90);
	gotoxy(13,21);
	gotoxy(X+50,Y+20); SetColor(RED); printf("ⓒ LKLH's Corp.");

	SetColor(GRAY); printf("\n Do you want to go to next page?");
	getchar();
	fflush(stdin);
	system("cls");
	//end start page
	// 기존 데이터 읽어오기
	char firstName[30];		// 이름
	char lastName[30];		// 성
	char gender[30];		// 성별
	int level;				// 항렬
	char job[30];			// 직업
	char birthOfDate[30];	// 탄생일
	char deathOfDate[30];	// 사망일

	char name[30];			// 누구와의
	char relation[30];		// 관계인가

	Data family;
	while(1){
		// 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴한다. 만일 어떠한 데이터도 형식 지정자와 일치하는 것이 없다면 0 이 될 수 도 있다.
		// 만일 어떠한 데이터도 성공적으로 읽어들이기 전에 입력이 실패한다면 EOF 가 리턴된다. 
		res = fscanf(fp, "%s %s %s %d %s %s %s %s %s", firstName, lastName, gender, &level, job, birthOfDate, deathOfDate, name, relation);
		if(res == EOF) break;
		// 저장되어있는 데이터가 있으면 노드를 만들고,
		Person *temp = new Person(firstName, lastName, gender, level, job, birthOfDate, deathOfDate, NULL, NULL, NULL, NULL, name, relation);
		
		// Data에 저장한다.
		family.insertNode(temp);
	}

	fclose(fp);
	while(1){
		int menu1;

		// 1단계 메뉴 출력
		cout << endl << "<< 전자 족보 >>" << endl;
		cout << "1 : 자료 입력" << endl;
		cout << "2 : 자료 수정" << endl;
		cout << "3 : 자료 탐색" << endl;
		cout << "4 : 자료 출력" << endl;
		cout << "5 : 촌수 계산" << endl;
		cout << "6 : 종     료" << endl;
		cout << "메뉴를 선택하세요.";
		cin >> menu1;
	
		switch (menu1)
		{
		case 1 :
			{				
				cout << endl;
				// 새로운 인물 입력
				Person temp_person;
				Person *new_person;
				new_person = temp_person.make_Person();
				
				// 족보에 추가
				family.insertNode(new_person);

				break;
			}
		case 2 :
			{
				cout << endl;
				// 수정할 이름
				string changeName;
				cout << "수정할 사람의 이름을 입력해 주세요." << endl;
				cin >> changeName;
							
				// 탐색에 성공한 경우
				if(family.SearchNode(changeName, family.getHead())->get_name() != ""){
					// 새로운 인물 입력
					Person temp_person;
					Person *new_person;
					new_person = temp_person.make_Person();
				
					// 데이터 수정
					family.changeNode(changeName, new_person);
				}
				else{
					cout << "존재하지 않습니다." << endl;
				}
			
				break;
			}
		case 3 :
			{
				cout << endl;
				// 탐색할 이름
				string searchName;
				cout << "탐색할 이름을 입력해 주세요." << endl;
				cin >> searchName;

				// 데이터 위치 탐색
				Person* search_p;
				search_p = family.SearchNode(searchName, family.getHead());
				
				// 탐색에 실패한 경우 출력
				if(search_p->get_name() == ""){
					cout << "존재하지 않습니다." << endl;
				}
				else{
					//탐색
					family.initSame();
					family.SearchNode2(searchName, family.getHead());
					Person **s_temp = family.getSame();

					for(int i = 0; s_temp[i] != NULL; i++)
						s_temp[i]->print_Person();
				}

				break;
			}
		case 4 :
			{
				cout << endl;
				family.printFamilyTree(family.getHead());
				break;
			}
		case 5 :
			{
				cout << endl;
				int relation;

				//* 촌수계산 구현
				// 탐색할 이름
				string searchName1, searchName2, birth1, birth2;
				cout << "탐색할 첫번째 이름을 입력해 주세요." << endl;
				cin >> searchName1;
				cout << "해당 인물의 생일을 입력해주세요. (ex->1989.02.19)" <<endl;
				cin >> birth1;
				cout << "탐색할 두번째 이름을 입력해 주세요." << endl;
				cin >> searchName2;
				cout << "해당 인물의 생일을 입력해주세요. (ex->1989.02.19)" <<endl;
				cin >> birth2;

				// 데이터 위치 탐색
				relation = family.CalculateRelation(searchName1, searchName2, birth1, birth2, family.getHead());
				if(relation == -1){
					cout<<"탐색오류!"<<endl;
					break;
				}
				else{
					// 탐색에 성공한 경우 출력
					cout<<"해당 인물간의 촌수는 "<<relation<<"촌 입니다."<<endl;
				}

				break;
			}
		case 6 :
			{
				cout << endl;

				// 모든 자료 저장
				FILE *op;
				op = fopen("family.txt", "w");
				if(op == NULL){
					cout << "출력 파일 개방 실패" << endl;
				}

				family.saveDatas(op, family.getHead());

				fclose(op);
				
				return 0;
			}
		default:
			cout << "잘못된 입력입니다. " << endl;
			exit(1);
		}
	}
}