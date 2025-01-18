#pragma once
#include "Tree.h"
#include <fstream>
#include <string>
using namespace std;

class Genealogy {
protected:
    Tree<string> genealogy;
public:
    bool Create(const string& filename); //创建族谱
    int Insert(string name, string birth, string death, string parentName); //插入子嗣
    void Show();    //显示族谱
    bool Search(string name);
    string GetFather(string name) { return genealogy.Parent(name); }
    int Generation(){ return genealogy.Height(); };
    bool genShow(int gene);
    bool childShow(string name);
    bool broShow(string name);
    bool Delete(string name);
    bool Edit(string name, string newname, string birth, string death, string father);
    bool Save(const string& filename){ return(genealogy.SaveTreeTo(filename)); }
};

bool Genealogy::Create(const string& filename){
    if(genealogy.BuildTreeFrom(filename)) return true;
    else return false;
}

int Genealogy::Insert(string name, string birth, string death, string parentName){
    return(genealogy.InsertNode(name, birth, death, parentName));
}

void Genealogy::Show(){
    genealogy.PrintTree();
}

bool Genealogy::Search(string name){
    string birth, death, fatherName;
    if (genealogy.Search(name, birth, death, fatherName)){
        cout << "Name: " << name << " | Birth date(A.D.): " << birth << " | Death date(A.D.): " << death << " | Father: " << fatherName << endl;
        return true;
    }
    else {
        cout << "未找到此人！" << "(" << name << ")";
        return false;
    }
}

bool Genealogy::genShow(int gene){
    return (genealogy.LayerShow(gene));
}

bool Genealogy::childShow(string name){
    return (genealogy.PrintChild(name));
}

bool Genealogy::broShow(string name){
    return (genealogy.PrintBro(name));
}

bool Genealogy::Delete(string name){
    return(genealogy.DeleteChild(genealogy.LocateElem(name)));
}

bool Genealogy::Edit(string name, string newname, string birth, string death, string father){
    return(genealogy.Edit(name, newname, birth, death, father));
}