#include <iostream>
#include <windows.h>
#include <string>
using namespace std;
#include "Genealogy.h"
Genealogy genealogy;

int menu();
void Insert();
void Show();
void Search();
void Delete();
void Edit();

int main(void)
{	
	SetConsoleOutputCP(CP_UTF8);
	//初始数据录入
	puts("Thanks for using Genealogy Ver 1.0 .\nYou can use this system to creat a genealogy and check or edit it as you want to.");
	puts("---------------------------------------------------------------------------------");
	puts("Please any key for initialization.\n");
	system("pause");
	if(genealogy.Create("E:/Library/JSU_DSA_2024_Fall_Course_Design/src/GenghisKhan.txt")) {
		system("cls");
		//进入菜单
		menu();
	}

	else { system("cls"); puts("error! Please check and reboot the program."); }

	return 0;
}


int menu() {
	int op = 0;
	system("cls");
	puts("---------+MENU+---------");
	puts("Enter\tTo");
	puts("0\tShow the genealogy");
	puts("1\tSearching mode");
	puts("2\tInsert a member");
	puts("3\tEdit a member");
	puts("4\tRemove a member");
	puts("5\tQuit the program");
	puts("------------------------");
	scanf("%d", &op);
	switch (op) {
	case 0:
		system("cls");
		Show();
		system("cls");
		menu();
		break;
	case 1:
		system("cls");
		Search();
		system("cls");
		menu();
		break;
	case 2:
		system("cls");
		Insert();
		system("cls");
		menu();
		break;
	case 3:
		system("cls");
		Edit();
		system("cls");
		menu();
		break;
	case 4:
		system("cls");
		Delete();
		system("cls");
		menu();
		break;
	case 5:
		system("cls");
		puts("Thanks for your using.\nDo you want to save the changes?(Y/N)");
		char op;
		cin >> op;
		if(op == 'N' || op == 'n') exit(0);
		else if (op == 'Y' || op == 'y'){
			if(genealogy.Save("E:/Library/JSU_DSA_2024_Fall_Course_Design/src/newtree.txt"))
				puts("Saved successfully.");
			else
				puts("Saved unsuccessfully.");
		}
		else
			puts("Changes haven't been saved.");
		system("pause");
		exit(0);
		break;
	default:
		system("cls");
		puts("Please enter a correct number.\nPress any key to continue.");
		system("pause");
		system("cls");
		menu();
	}
	return 0;
}

void Insert() {
	string name, fatherName, birthDate, deathDate;
	puts("Please enter the information of the person you want to insert.");
	puts("Name:");
	cin >> name;
	puts("Birth date(year):");
	cin >> birthDate;
	puts("Death date(year):");
	cin >> deathDate;
	puts("Father's name:\n* '0' means 'no one'");
	cin >> fatherName;
	int stat = genealogy.Insert(name, birthDate, deathDate, fatherName); //){
	switch (stat) {
	case 0:
		puts("无法打开文件进行写入！按任意键回到主菜单");
		system("pause");
		system("cls");
		menu();
		break;
	case 1:
		puts("已作为家族始祖列入族谱。按任意键回到主菜单");
		system("pause");
		system("cls");
		menu();
		break;
	case 2:
		puts("已存在始祖，请输入父亲以列入族谱。按任意键回到主菜单");
		system("pause");
		system("cls");
		menu();
		break;
	case 3:
		puts("此人已存在。按任意键回到主菜单");
		system("pause");
		system("cls");
		menu();
		break;
	case 4:
		puts("输入的父亲有误，不存在此人。按任意键回到主菜单");
		system("pause");
		system("cls");
		menu();
		break;
	case 5:
		puts("列入族谱成功！按任意键回到主菜单");
		system("pause");
		system("cls");
		menu();
		break;
	default:
		system("cls");
		menu();
		return;
	}
}

void Show(){
	genealogy.Show();
	system("pause");
}

void nameSearch(string target){
	genealogy.Search(target);

	system("pause");
	int op;
	puts("---------+Secondary Search+---------");
	puts("Enter\tTo");
	puts("0\tSearch for his father");
	puts("1\tSearch for his children");
	puts("2\tSearch for his brother");
	puts("Other \tGo back to menu");
	puts("------------------------------------");
	scanf("%d", &op);
	switch (op) {
	case 0:
		system("cls");
		if(genealogy.GetFather(target) == "NULL"){
			puts("此人已为始祖！");
			system("pause");
			system("cls");
			menu();
			break;
		}
		else {
			nameSearch(genealogy.GetFather(target));
			system("cls");
			menu();
			break;
		}
	case 1:
		system("cls");
		if(genealogy.childShow(target)){
			cout << endl <<"Please enter the name to search(enter '0' to quit):";
			string tgt;
			cin >> tgt;
			if (tgt == "0") return;
			nameSearch(tgt);
			break;
		} else { puts("查找失败！请检查输入。"); system("pause"); break; }
	case 2:
		system("cls");
		if(genealogy.broShow(target)){
			cout << endl << "Please enter the name to search(enter '0' to quit):";
			string tgt;
			cin >> tgt;
			if (tgt == "0") return;
			nameSearch(tgt);
			break;
		} else { puts("查找失败！请检查输入。"); system("pause"); break; }
	default:
		system("cls");
		return;
	}
}

void geneSearch(int gene){
	if(genealogy.genShow(gene)){
		cout << "Please enter the name to search(enter '0' to quit):";
		string tgt;
		cin >> tgt;
		if (tgt == "0") return;
		nameSearch(tgt);
		}
	else { puts("查找失败！请检查输入。"); system("pause"); return; }
}

void Search() {
	int op = 0;
	string tgt;
	system("cls");
	puts("---------+Search+---------");
	puts("Enter\tTo");
	puts("0\tGet back to menu");
	puts("1\tSearch by name");
	puts("2\tSearch by generation");
	puts("--------------------------");
	scanf("%d", &op);
	switch (op) {
	case 0:
		system("cls");
		menu();
		break;
	case 1:
		system("cls");
		cout << "Please enter the name(enter '0' to quit):";
		cin >> tgt;
		if (tgt == "0") break;
		nameSearch(tgt);
		system("cls");
		menu();
		break;
	case 2:
		system("cls");
		cout << "Please enter the generation(from 1 to " << genealogy.Generation() << "):";
		int gen;
		cin >> gen;
		geneSearch(gen);
		system("cls");
		menu();
		break;
	default:
		system("cls");
		puts("Please enter a correct number.\nPress any key to continue.");
		system("pause");
		system("cls");
		Search();
	}
}

void Delete(){
	string name;
	cout << "Please enter the name to delete(enter '0' to quit):";
	cin >> name;
	if(name == "0") return;
	else {
		if(genealogy.Delete(name)){
			puts("删除成功!");
			system("pause");
			return;
		}
		else{
			puts("删除失败！");
			system("pause");
			return;
		}
	}
}

void Edit(){
	string name;
	cout << "Please enter the name to edit(enter '0' to quit):";
	cin >> name;
	if(name == "0") return;
	else{
		if (!genealogy.Search(name)) {
			puts("修改失败！此人不存在");
			system("pause"); 
			return;
		}
		string newname,birth,death,father;
		puts("Please enter the new information.");
		puts("New name:");
		cin >> newname;
		puts("New birth:");
		cin >> birth;
		puts("New death:");
		cin >> death;
		puts("New father:");
		cin >> father;
		if(genealogy.Edit(name,newname,birth,death,father)){
			system("cls");
			puts("修改成功！");
			return;
		}
		else{
			system("cls");
			puts("修改失败！请检查父亲是否存在！");
			return;
		}
	}
}