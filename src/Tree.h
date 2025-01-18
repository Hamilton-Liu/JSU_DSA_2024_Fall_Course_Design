#pragma once
#include "TreeNode.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>

template <class DataType>
class Tree {
protected:
	bool isBuildingTree = false;
	TreeNode<DataType> *root; ;   												//二叉树根节点
	void Destroy(TreeNode<DataType> * &r);										//删除以r为根的树
	int Height(TreeNode<DataType> *r);  										//求以r为根的树高度						
	TreeNode<DataType> *Parent(TreeNode<DataType> *r, TreeNode<DataType> *p, TreeNode<DataType>* bro);	//求以r为根的树中节点p的双亲
	TreeNode<DataType> *LocateElem(TreeNode<DataType> *r, DataType &e);			//在以r为根的树中寻找数据元素等于e的节点
	void PrintTree(TreeNode<DataType>* r, string pre = "", bool isLast = true); //孩子兄弟结构打印
    bool PrintLayer(TreeNode<DataType>* r, int layer);                          //按层次打印
public:
	Tree():root(NULL) {}													//构造函数
	virtual ~Tree() { Destroy(root); }										//析构函数
	TreeNode<DataType>* GetRoot() { return root; }							//返回根指针
	bool IsEmpty() { return root == NULL ? true : false; }					//判断二叉树是否为空
	bool DeleteChild(TreeNode<DataType>* p)							    	//删除节点p以及子树
	{	
        if (!p) return false; // 如果节点为空，直接返回失败

        TreeNode<DataType>* parent = Parent(p); // 找到父节点
        if (!parent) return false; // 如果找不到父节点，返回失败

        // 如果目标节点是父节点的首孩子
        if (parent->Child == p) {
            parent->Child = p->Brother; // 父节点的子指针指向目标节点的下一个兄弟
        } 
        else {
            // 如果目标节点不是首孩子，找到目标节点的前一个兄弟
            TreeNode<DataType>* pre = parent->Child;
            while (pre && pre->Brother != p) {
                pre = pre->Brother;
            }
            if (pre) {
                pre->Brother = p->Brother; // 前一个兄弟跳过目标节点，指向目标节点的下一个兄弟
            }
        }

        // 删除目标节点及其子树
        p->Brother = NULL;
        Destroy(p->Child);
        delete p;
        return true;
    }
	int Height() ;															//求树的高度
	TreeNode<DataType>* Parent(TreeNode<DataType> *p) 						//在树中求节点p的双亲
	{	return (root == NULL || p == root) ? NULL : Parent(root, p, root->Child);	}
    DataType Parent(DataType &e)                                            //在树中求节点p的双亲的数据
    {   if (Parent(LocateElem(e))) { TreeNode<DataType> *p = Parent(LocateElem(e)); return p->data;}
        else return "NULL";   }
	TreeNode<DataType>* LocateElem(DataType &e);							//在树中寻找数据元素等于e的节点
	bool BuildTreeFrom(const string& filename);								//根据族谱原始数据创建树
    bool SaveTreeTo(const string& filename);
	int InsertNode(DataType& data, DataType& birth, DataType& death, DataType& parentData);
	bool Search(DataType &e, DataType& birth, DataType& death, DataType& parentName);
	void PrintTree();
    bool LayerShow(int layer);
    bool PrintChild(DataType &e);
    bool PrintBro(DataType &e);
    bool Edit(string name, string newname, string birth, string death, string father);
    void SaveTreeNode(ofstream& file, TreeNode<DataType>* p);
};


template <class DataType>
void Tree<DataType>::Destroy(TreeNode<DataType>* &r) {  
    if (r) {                                // 如果当前节点不为空
        Destroy(r->Child);                  // 递归删除首孩子子树
        Destroy(r->Brother);                // 递归删除兄弟节点
        delete r;                           // 删除当前节点
        r = NULL;                           // 置空指针，防止悬挂指针
    }
}


template <class DataType>
int Tree<DataType>::Height(TreeNode<DataType> *r) {         //求以r为根的二叉树高度
    
    if (r == NULL) return 0;		//空树高度为0
    int Height = 0;  				//初始化高度为0
    TreeNode<DataType>* p = r->Child;  //把孩子节点的地址存到p里
    while (p!= NULL) {				//如果孩子节点存在
        int pHeight = this->Height(p);   //递归计算孩子高度
        p = p->Brother;            //将指针移动到兄弟指针，以遍历每一个节点
		if (pHeight > Height) {
			Height = pHeight; 	//如果孩子的高度大于Height，更新Height为pHeight
		}     
    }
    return Height + 1;
}
template <class DataType>
int Tree<DataType>::Height() {         //接口
    return Height(root);
}


template <class DataType>
TreeNode<DataType>* Tree<DataType>::Parent(TreeNode<DataType>* r, TreeNode<DataType>* p, TreeNode<DataType>* bro) {
    if (!r || !bro) return NULL;             // 空节点直接返回 NULL
    if (bro == p) return r;                  // 当前兄弟是目标节点，返回父节点

    // 在兄弟链表中查找父节点
    TreeNode<DataType>* b = Parent(r, p, bro->Brother);
    if (b) return b;

    // 递归到兄弟的子树中查找
    return Parent(bro, p, bro->Child ? bro->Child : NULL);
}

template <class DataType>
TreeNode<DataType> * Tree<DataType>::LocateElem(TreeNode<DataType> *r, DataType &e) {	
	TreeNode<DataType> * p = NULL;
	if(r)
	{
		if(r->data == e)	p = r;
		else
		{
			p = LocateElem(r->Child, e);
			if(!p)	p = LocateElem(r->Brother, e);
		}
	}
	return p;
}
template <class DataType>
TreeNode<DataType>* Tree<DataType>::LocateElem(DataType &e)					//在二叉树中寻找值为e的节点
{
	return LocateElem(root, e);
}


template <class DataType>
bool Tree<DataType>::BuildTreeFrom(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "无法打开文件: " << filename;
        return false;
    }//打开文件

	isBuildingTree = true;
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        DataType data, birth, death, parentName;
        iss >> data >> birth >> death;
        if (!(iss >> parentName)) {
            parentName = ""; // 根节点没有父亲
        }

        InsertNode(data, birth, death, parentName);
    }
	isBuildingTree = false;

    infile.close();
	return true;
}


template <class DataType>
int Tree<DataType>::InsertNode(DataType& data, DataType& birth, DataType& death, DataType& parentData) {

	//创建新节点
    TreeNode<DataType>* newNode = new TreeNode<DataType>();
    newNode->data = data;
    newNode->birth = birth;
    newNode->death = death;

	// 没有父节点
    if (parentData.empty() ||parentData =="0") {
        if (root == NULL) {
            root = newNode;
			return 1;
        } else {
            delete newNode;
            return 2;
        }
    } 

	// 查找父节点
	else { 
		if (LocateElem(data)) {
			return 3; 
		}
        TreeNode<DataType>* parent = LocateElem(parentData);
        if (!parent) {
            delete newNode;
            return 4;
        }

        // 插入为父节点的第一个孩子
        if (parent->Child == NULL) {
            parent->Child = newNode;
        } 
		// 已经有孩子，按照数据次序插入其兄弟链表
		else {
            TreeNode<DataType>* bro = parent->Child;
            TreeNode<DataType>* tmp = NULL;
            int tag = stoi(newNode->birth);
            while (bro && stoi(bro->birth) < tag) {
                tmp = bro; // 保存上一个节点
                bro = bro->Brother;
            }

            if (tmp == NULL) {
                // 插入到首孩子之前
                newNode->Brother = parent->Child;
                parent->Child = newNode;
            } else {
                // 插入到兄弟中间
                tmp->Brother = newNode;
                newNode->Brother = bro;
            }
        }

		/*
        if(!isBuildingTree){
			ofstream outfile("GenghisKhan.txt", ios::app); // 以追加模式打开文件
			if (!outfile) {
				return 0;
			}
			outfile << data << " " << birth << " " << death;
			if (!parentData.empty()) {
				outfile << " " << parentData;
			}
			outfile << endl;
			outfile.close();
		}
        */

		return 5;
    }
}


template <class DataType>
void Tree<DataType>::PrintTree(TreeNode<DataType>* r, string pre, bool isLast) {
    if (r == NULL) {
        return;
    }

    // 输出当前节点
    cout << pre;
    if (isLast) {
        cout << "└── ";
        pre += "    ";
    } else {
        cout << "├── ";
        pre += "│   ";
    }
    cout << r->data << endl;

    // 递归处理孩子节点
    TreeNode<DataType>* child = r->Child;
    while (child) {
        // 判断是否是最后一个孩子节点
        PrintTree(child, pre, child->Brother == NULL);
        child = child->Brother;
    }
}
template <class DataType>
void Tree<DataType>::PrintTree() {
	SetConsoleOutputCP(CP_UTF8);
    PrintTree(root, "", true);
}


template <class DataType>
bool Tree<DataType>::Search(DataType &e, DataType& birth, DataType& death, DataType& parentName){
	TreeNode<DataType>* p = LocateElem(e);
    if (p == NULL) {
        birth = death = parentName = "无";
        return false;
    }

    birth = p->birth;
    death = p->death;

    TreeNode<DataType>* parent = Parent(p);
    if (parent == NULL) {
        parentName = "无";
    } else {
        parentName = parent->data;
    }
	return true;
}


template <class DataType>
bool Tree<DataType>::LayerShow(int layer) {
    if (layer < 1) {
        return false;
    }

    bool found = PrintLayer(root, layer); // 从根节点开始递归
    if (!found) {
        return false;
    } else {
        cout << endl;
        return true;
    }
}
template <class DataType>
bool Tree<DataType>::PrintLayer(TreeNode<DataType>* r, int layer) {
    bool flag = false;
    if(!flag)
        if (!r)
            return false; // 当前节点为空，返回

    if (layer == 1) { // 如果当前是目标层，输出节点
        cout << r->data << "|";
        flag = true;
    }

    // 继续递归孩子节点和兄弟节点，层数减一
    bool found = flag;
    if (r->Brother) {
        bool brotherFound = PrintLayer(r->Brother, layer); // 在兄弟分支中查找
        found = found || brotherFound; // 更新 found 的值
    }
    if (r->Child) {
        bool childFound = PrintLayer(r->Child, layer - 1); // 在孩子分支中查找
        found = found || childFound; // 更新 found 的值
    }
    return found;
}


template <class DataType>
bool Tree<DataType>::PrintChild(DataType &e){
    TreeNode<DataType>* p = LocateElem(e);
    TreeNode<DataType>* bro = p->Child;
    if(p->Child){
        while(bro){
            cout << bro->data << "|";
            bro = bro->Brother;
        }
        return true;
    }
    else return false;
}
template <class DataType>
bool Tree<DataType>::PrintBro(DataType &e){
    TreeNode<DataType>* p = LocateElem(e);
    TreeNode<DataType>* par = Parent(p);
    TreeNode<DataType>* bro = par->Child;
    if(bro){
        while(bro){
            if(bro->data != e)
                cout << bro->data << "|";
            bro = bro->Brother;
        }
        return true;
    }
    else return false;
}


template <class DataType>
bool Tree<DataType>::Edit(string name, string newname, string birth, string death, string father){
    TreeNode<DataType> * p = LocateElem(name);

    if (!p) return false;

    TreeNode<DataType>* parent = Parent(p); // 找到父节点
    if (!parent) return false; // 如果找不到父节点，返回失败

    TreeNode<DataType> * tmp = p->Child;    

    // 如果目标节点是父节点的首孩子
    if (parent->Child == p) {
        parent->Child = p->Brother; // 父节点的子指针指向目标节点的下一个兄弟
    } 
    else {
        // 如果目标节点不是首孩子，找到目标节点的前一个兄弟
        TreeNode<DataType>* pre = parent->Child;
        while (pre && pre->Brother != p) {
            pre = pre->Brother;
        }
        if (pre) {
            pre->Brother = p->Brother; // 前一个兄弟跳过目标节点，指向目标节点的下一个兄弟
        }
    }
    
    p->Brother = p->Child = NULL;

    delete p;
    InsertNode(newname, birth, death, father);

    TreeNode<DataType> * newNode = LocateElem(newname);
    newNode->Child = tmp;
    
    return true;
}


template <class DataType>
void Tree<DataType>::SaveTreeNode(ofstream& file, TreeNode<DataType>* p) {
    if (p) {
        
        file << p->data << " " << p->birth << " " << p->death << " " << Parent(p->data) << "\n";

        // 递归保存子树和兄弟树
        SaveTreeNode(file, p->Child);
        SaveTreeNode(file, p->Brother);
    }
}

template <class DataType>
bool Tree<DataType>::SaveTreeTo(const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        return false; // 打开文件失败
    }

    // 从根节点开始保存
    SaveTreeNode(file, this->root);

    file.close();
    return true;
}