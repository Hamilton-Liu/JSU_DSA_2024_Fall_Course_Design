#pragma once
template <class DataType>
class TreeNode {
public:
	DataType data;									//数据域
	DataType birth;									//生年
	DataType death;									//卒年
	TreeNode<DataType> *Child;						//左孩子指针域
	TreeNode<DataType> *Brother; 					//右孩子指针域
	TreeNode() { Child = Brother = NULL; }			//无参构造函数，构造空节点
	//有参构造函数，构造数据元素节点
	TreeNode(DataType &e, TreeNode<DataType> *child = NULL, TreeNode<DataType> *brother = NULL)
	{
		data = e;  Child = child;  Brother = brother;
	}
};