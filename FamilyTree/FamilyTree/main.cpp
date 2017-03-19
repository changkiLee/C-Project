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
	gotoxy(8,13);puts("�̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢�");
	gotoxy(8,14);puts("��                                                              ��");
	gotoxy(8,15);puts("��                                                              ��");
	gotoxy(8,16);puts("��                                                              ��");
	gotoxy(8,17);puts("��                                                              ��");
	gotoxy(8,18);puts("��                                                              ��");
	gotoxy(8,19);puts("��                                                              ��");
	gotoxy(8,20);puts("��                                                              ��");
	gotoxy(8,21);puts("�̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢�");
}
int main()
{
	// ���� �����
	FILE *fp;
	int res;
	fp = fopen("family.txt", "r+");
	if(fp == NULL){
		fprintf(stderr, "���� ������ �����Ͽ����ϴ�.\n");
		return 1;
	}
	//start page
	gotoxy(X+2,Y+1); SetColor(GREEN);   printf( "�̢̢̢̢̡������̡������̡������̡��̢̢̡��̡��������̡�������"); Sleep(100);
	gotoxy(X+2,Y+2); SetColor(GREEN);	printf( "�̡������������̡��̡����̢̡��̢̡����̡����̡��������̡�������"); Sleep(100);
	gotoxy(X+2,Y+3); SetColor(GREEN);	printf( "�̢̢̢̢̡��̢̢̢̢̡��̡��̡��̡����̡����̡����������̡���"); Sleep(100);
	gotoxy(X+2,Y+4); SetColor(GREEN);	printf( "�̡����������̡������̡��̡��̡��̡����̡����̡�������������"); Sleep(100);
	gotoxy(X+2,Y+5); SetColor(GREEN);	printf( "�̡����������̡������̡��̡��̡��̡��̢̢̡��̢̢̢̡�������"); Sleep(100);
	Sleep(1000);
	gotoxy(X+2,Y+8); SetColor(YELLOW); printf( "�������̢̢̢̢̡����̢̢̢̢̡����̢̢̢̢̡����̢̢̢̢�"); Sleep(100);
	gotoxy(X+2,Y+9); SetColor(YELLOW); printf( "�����������̡��������̡������̡����̡�������������"); Sleep(100);
	gotoxy(X+2,Y+10); SetColor(YELLOW); printf( "�̢̡������̡��������̢̢̢̢̡����̢̢̢̢̡����̢̢̢̢̡����̢�"); Sleep(100);
	gotoxy(X+2,Y+11); SetColor(YELLOW); printf( "�����������̡��������̡����̡������̡�������������"); Sleep(100);
	gotoxy(X+2,Y+12); SetColor(YELLOW); printf( "�����������̡��������̡������̡����̢̢̢̢̡����̢̢̢̢�"); Sleep(100);
	Sleep(20);
	//gotoxy(15,8);
	gotoxy(X+2,Y+15); SetColor(GRAY); printf("= Digital Family Tree System ="); Sleep(90);
	gotoxy(X+2,Y+16); SetColor(GRAY); printf("= Designed by LEE CHANG KI   ="); Sleep(90);
	gotoxy(X+2,Y+17); SetColor(GRAY); printf("= Designed by KWON SU JIN    ="); Sleep(90);
	gotoxy(X+2,Y+18); SetColor(GRAY); printf("= Designed by LEE HAK HOON   ="); Sleep(90);
	gotoxy(X+2,Y+19); SetColor(GRAY); printf("= Designed by HAN     JUNE   ="); Sleep(90);
	gotoxy(X+2,Y+20); SetColor(GRAY); printf("= AT ALGORITHM CLASS IN 2012 ="); Sleep(90);
	gotoxy(13,21);
	gotoxy(X+50,Y+20); SetColor(RED); printf("�� LKLH's Corp.");

	SetColor(GRAY); printf("\n Do you want to go to next page?");
	getchar();
	fflush(stdin);
	system("cls");
	//end start page
	// ���� ������ �о����
	char firstName[30];		// �̸�
	char lastName[30];		// ��
	char gender[30];		// ����
	int level;				// �׷�
	char job[30];			// ����
	char birthOfDate[30];	// ź����
	char deathOfDate[30];	// �����

	char name[30];			// ��������
	char relation[30];		// �����ΰ�

	Data family;
	while(1){
		// ������ ���, fscanf �Լ��� �о���� �������� ���� �����Ѵ�. ���� ��� �����͵� ���� �����ڿ� ��ġ�ϴ� ���� ���ٸ� 0 �� �� �� �� �ִ�.
		// ���� ��� �����͵� ���������� �о���̱� ���� �Է��� �����Ѵٸ� EOF �� ���ϵȴ�. 
		res = fscanf(fp, "%s %s %s %d %s %s %s %s %s", firstName, lastName, gender, &level, job, birthOfDate, deathOfDate, name, relation);
		if(res == EOF) break;
		// ����Ǿ��ִ� �����Ͱ� ������ ��带 �����,
		Person *temp = new Person(firstName, lastName, gender, level, job, birthOfDate, deathOfDate, NULL, NULL, NULL, NULL, name, relation);
		
		// Data�� �����Ѵ�.
		family.insertNode(temp);
	}

	fclose(fp);
	while(1){
		int menu1;

		// 1�ܰ� �޴� ���
		cout << endl << "<< ���� ���� >>" << endl;
		cout << "1 : �ڷ� �Է�" << endl;
		cout << "2 : �ڷ� ����" << endl;
		cout << "3 : �ڷ� Ž��" << endl;
		cout << "4 : �ڷ� ���" << endl;
		cout << "5 : �̼� ���" << endl;
		cout << "6 : ��     ��" << endl;
		cout << "�޴��� �����ϼ���.";
		cin >> menu1;
	
		switch (menu1)
		{
		case 1 :
			{				
				cout << endl;
				// ���ο� �ι� �Է�
				Person temp_person;
				Person *new_person;
				new_person = temp_person.make_Person();
				
				// ������ �߰�
				family.insertNode(new_person);

				break;
			}
		case 2 :
			{
				cout << endl;
				// ������ �̸�
				string changeName;
				cout << "������ ����� �̸��� �Է��� �ּ���." << endl;
				cin >> changeName;
							
				// Ž���� ������ ���
				if(family.SearchNode(changeName, family.getHead())->get_name() != ""){
					// ���ο� �ι� �Է�
					Person temp_person;
					Person *new_person;
					new_person = temp_person.make_Person();
				
					// ������ ����
					family.changeNode(changeName, new_person);
				}
				else{
					cout << "�������� �ʽ��ϴ�." << endl;
				}
			
				break;
			}
		case 3 :
			{
				cout << endl;
				// Ž���� �̸�
				string searchName;
				cout << "Ž���� �̸��� �Է��� �ּ���." << endl;
				cin >> searchName;

				// ������ ��ġ Ž��
				Person* search_p;
				search_p = family.SearchNode(searchName, family.getHead());
				
				// Ž���� ������ ��� ���
				if(search_p->get_name() == ""){
					cout << "�������� �ʽ��ϴ�." << endl;
				}
				else{
					//Ž��
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

				//* �̼���� ����
				// Ž���� �̸�
				string searchName1, searchName2, birth1, birth2;
				cout << "Ž���� ù��° �̸��� �Է��� �ּ���." << endl;
				cin >> searchName1;
				cout << "�ش� �ι��� ������ �Է����ּ���. (ex->1989.02.19)" <<endl;
				cin >> birth1;
				cout << "Ž���� �ι�° �̸��� �Է��� �ּ���." << endl;
				cin >> searchName2;
				cout << "�ش� �ι��� ������ �Է����ּ���. (ex->1989.02.19)" <<endl;
				cin >> birth2;

				// ������ ��ġ Ž��
				relation = family.CalculateRelation(searchName1, searchName2, birth1, birth2, family.getHead());
				if(relation == -1){
					cout<<"Ž������!"<<endl;
					break;
				}
				else{
					// Ž���� ������ ��� ���
					cout<<"�ش� �ι����� �̼��� "<<relation<<"�� �Դϴ�."<<endl;
				}

				break;
			}
		case 6 :
			{
				cout << endl;

				// ��� �ڷ� ����
				FILE *op;
				op = fopen("family.txt", "w");
				if(op == NULL){
					cout << "��� ���� ���� ����" << endl;
				}

				family.saveDatas(op, family.getHead());

				fclose(op);
				
				return 0;
			}
		default:
			cout << "�߸��� �Է��Դϴ�. " << endl;
			exit(1);
		}
	}
}