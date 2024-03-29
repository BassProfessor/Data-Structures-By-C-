#include<queue>
#include<stack>
#include<iostream>
#include<cmath>
using namespace std;
#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

template<class T>
class Stack : public stack<T> {
public:
	T pop() {
		T tmp = stack<T>::top();
		stack<T>::pop();
		return tmp;
	}
};

template<class T>
class Queue : public queue<T> {
public:
	T dequeue() {
		T tmp = queue<T>::front();
		queue<T>::pop();
		return tmp;
	}
	void enqueue(const T& el) {
		queue<T>::push(el);
	}
};
template<class T> class BST;

template<class T>
class BSTNode {
public:
	BSTNode() {
		left = right = 0;
	}
	BSTNode(const T& e, BSTNode<T>* l = 0, BSTNode<T>* r = 0) {
		el = e; left = l; right = r;
	}
	T el;
	BSTNode<T>* left, * right;
};

template<class T>
class BST {
public:
	BST() {
		root = 0;
	}
	~BST() {
		clear();
	}
	void clear() {
		clear(root);
		root = 0;
	}
	bool isEmpty() const {
		return root == 0;
	}
	void preorder() {
		preorder(root);
	}
	void inorder() {
		inorder(root);
	}
	void postorder() {
		postorder(root);
	}
	void insert(const T&);
	void recursiveInsert(const T& el) {
		recursiveInsert(root, el);
	}
	T* search(const T& el) const {
		return search(root, el);
	}
	T* recursiveSearch(const T& el) const {
		return recursiveSearch(root, el);
	}
	void deleteByCopying(BSTNode<T>*&);
	void findAndDeleteByCopying(const T&);
	void deleteByMerging(BSTNode<T>*&);
	void findAndDeleteByMerging(const T&);
	void iterativePreorder();
	void iterativeInorder();
	void iterativePostorder();
	void breadthFirst();
	void MorrisPreorder();
	void MorrisInorder();
	void MorrisPostorder();
	void balance(T*, int, int);
	void createBackbone();
	void rotateLeft(BSTNode<T>*&);
	void createPerfectTree();
	int counting();
protected:
	BSTNode<T>* root;
	void clear(BSTNode<T>*);
	void recursiveInsert(BSTNode<T>*&, const T&);
	T* search(BSTNode<T>*, const T&) const;
	T* recursiveSearch(BSTNode<T>*, const T&) const;
	void preorder(BSTNode<T>*);
	void inorder(BSTNode<T>*);
	void postorder(BSTNode<T>*);
	virtual void visit(BSTNode<T>* p) {
		cout << p->el << ' ';
	}
};

template<class T>
void BST<T>::clear(BSTNode<T>* p) {
	if (p != 0) {
		clear(p->left);
		clear(p->right);
		delete p;
	}
}

template<class T>
void BST<T>::insert(const T& el) {
	BSTNode<T>* p = root, * prev = 0;
	while (p != 0) { 
		prev = p;
		if (el < p->el)
			p = p->left;
		else p = p->right;
	}
	if (root == 0)    
		root = new BSTNode<T>(el);
	else if (el < prev->el)
		prev->left = new BSTNode<T>(el);
	else prev->right = new BSTNode<T>(el);
}

template<class T>
void BST<T>::recursiveInsert(BSTNode<T>*& p, const T& el) {
	if (p == 0)
		p = new BSTNode<T>(el);
	else if (el < p->el)
		recursiveInsert(p->left, el);
	else recursiveInsert(p->right, el);
}

template<class T>
T* BST<T>::search(BSTNode<T>* p, const T& el) const {
	while (p != 0)
		if (el == p->el)
			return &p->el;
		else if (el < p->el)
			p = p->left;
		else p = p->right;
	return 0;
}

template<class T>
T* BST<T>::recursiveSearch(BSTNode<T>* p, const T& el) const {
	if (p != 0)
		if (el == p->el)
			return &p->el;
		else if (el < p->el)
			return recursiveSearch(p->left, el);
		else return recursiveSearch(p->right, el);
	else return 0;
}

template<class T>
void BST<T>::inorder(BSTNode<T>* p) {
	if (p != 0) {
		inorder(p->left);
		visit(p);
		inorder(p->right);
	}
}

template<class T>
void BST<T>::preorder(BSTNode<T>* p) {
	if (p != 0) {
		visit(p);
		preorder(p->left);
		preorder(p->right);
	}
}

template<class T>
void BST<T>::postorder(BSTNode<T>* p) {
	if (p != 0) {
		postorder(p->left);
		postorder(p->right);
		visit(p);
	}
}

template<class T>
void BST<T>::deleteByCopying(BSTNode<T>*& node) {
	BSTNode<T>* previous, * tmp = node;
	if (node->right == 0)                  
		node = node->left;
	else if (node->left == 0)              
		node = node->right;
	else {
		tmp = node->left;
		previous = node;                  
		while (tmp->right != 0) {         
			previous = tmp;
			tmp = tmp->right;
		}
		node->el = tmp->el;              
		if (previous == node)
			previous->left = tmp->left;
		else previous->right = tmp->left;
	}
	delete tmp;                           
}

template<class T>
void BST<T>::findAndDeleteByCopying(const T& el) {
	BSTNode<T>* p = root, * prev = 0;
	while (p != 0 && !(p->el == el)) {
		prev = p;
		if (el < p->el)
			p = p->left;
		else p = p->right;
	}
	if (p != 0 && p->el == el)
		if (p == root)
			deleteByCopying(root);
		else if (prev->left == p)
			deleteByCopying(prev->left);
		else deleteByCopying(prev->right);
	else if (root != 0)
		cout << "el " << el << " is not in the tree\n";
	else cout << "the tree is empty\n";
}

template<class T>
void BST<T>::deleteByMerging(BSTNode<T>*& node) {
	BSTNode<T>* tmp = node;
	if (node != 0) {
		if (!node->right)           
			node = node->left;     
		else if (node->left == 0)  
			node = node->right;    
		else {                     
			tmp = node->left;     
			while (tmp->right != 0)
				tmp = tmp->right;
			tmp->right =           
				node->right;        
									
			tmp = node;            
			node = node->left;     
		}
		delete tmp;                
	}
}

template<class T>
void BST<T>::findAndDeleteByMerging(const T& el) {
	BSTNode<T>* node = root, * prev = 0;
	while (node != 0) {
		if (node->el == el)
			break;
		prev = node;
		if (el < node->el)
			node = node->left;
		else node = node->right;
	}
	if (node != 0 && node->el == el)
		if (node == root)
			deleteByMerging(root);
		else if (prev->left == node)
			deleteByMerging(prev->left);
		else deleteByMerging(prev->right);
	else if (root != 0)
		cout << "el " << el << " is not in the tree\n";
	else cout << "the tree is empty\n";
}

template<class T>
void BST<T>::iterativePreorder() {
	Stack<BSTNode<T>*> travStack;
	BSTNode<T>* p = root;
	if (p != 0) {
		travStack.push(p);
		while (!travStack.empty()) {
			p = travStack.pop();
			visit(p);
			if (p->right != 0)
				travStack.push(p->right);
			if (p->left != 0)             
				travStack.push(p->left); 
		}
	}
}

template<class T>
void BST<T>::iterativeInorder() {
	Stack<BSTNode<T>*> travStack;
	BSTNode<T>* p = root;
	while (p != 0) {
		while (p != 0) {                 
			if (p->right)                
				travStack.push(p->right); 
			travStack.push(p);
			p = p->left;
		}
		p = travStack.pop();            
		while (!travStack.empty() && p->right == 0) { 
			visit(p);                                
			p = travStack.pop();
		}
		visit(p);                        
		if (!travStack.empty())          
			p = travStack.pop();
		else p = 0;
	}
}

template<class T>
void BST<T>::iterativePostorder() {
	Stack<BSTNode<T>*> travStack;
	BSTNode<T>* p = root, * q = root;
	while (p != 0) {
		for (; p->left != 0; p = p->left)
			travStack.push(p);
		while (p->right == 0 || p->right == q) {
			visit(p);
			q = p;
			if (travStack.empty())
				return;
			p = travStack.pop();
		}
		travStack.push(p);
		p = p->right;
	}
}

template<class T>
void BST<T>::breadthFirst() {
	Queue<BSTNode<T>*> queue;
	BSTNode<T>* p = root;
	if (p != 0) {
		queue.enqueue(p);
		while (!queue.empty()) {
			p = queue.dequeue();
			visit(p);
			if (p->left != 0)
				queue.enqueue(p->left);
			if (p->right != 0)
				queue.enqueue(p->right);
		}
	}
}

template<class T>
void BST<T>::MorrisInorder() {
	BSTNode<T>* p = root, * tmp;
	while (p != 0)
		if (p->left == 0) {
			visit(p);
			p = p->right;
		}
		else {
			tmp = p->left;
			while (tmp->right != 0 && tmp->right != p)
				tmp = tmp->right;
			if (tmp->right == 0) {   
				tmp->right = p;     
				p = p->left;        
			}
			else {                   
				visit(p);          
				tmp->right = 0;     
				p = p->right;       
			}                       
		}
}

template<class T>
void BST<T>::MorrisPreorder() {
	BSTNode<T>* p = root, * tmp;
	while (p != 0) {
		if (p->left == 0) {
			visit(p);
			p = p->right;
		}
		else {
			tmp = p->left;
			while (tmp->right != 0 &&tmp->right != p)  
				tmp = tmp->right;   
			if (tmp->right == 0) {   
				visit(p);      
				tmp->right = p;     
				p = p->left;      
			}
			else {                   
				tmp->right = 0;     
				p = p->right;       
			}                        
		}
	}
}

template<class T>
void BST<T>::MorrisPostorder() {
	BSTNode<T>* p = new BSTNode<T>(), * tmp, * q, * r, * s;
	p->left = root;
	while (p != 0)
		if (p->left == 0)
			p = p->right;
		else {
			tmp = p->left;
			while (tmp->right != 0 &&
				tmp->right != p)  
				tmp = tmp->right;   
			if (tmp->right == 0) {   
				tmp->right = p;     
				p = p->left;        
			}
			else {             
				for (q = p->left, r = q->right, s = r->right;
					r != p; q = r, r = s, s = s->right)
					r->right = q;
				for (s = q->right; q != p->left;
					q->right = r, r = q, q = s, s = s->right)
					visit(q);
				visit(p->left);     
				tmp->right = 0;     
				p = p->right;       
			}                        
		}
}

template<class T>
void BST<T>::balance(T *data, int first, int last) {
	if (first <= last) {
		int middle = (first + last) / 2;
		insert(data[middle]);
		balance(data, first, middle - 1);
		balance(data, middle + 1, last);
	}
}
template<class T>
void BST<T>::createBackbone() {
	BSTNode<T>* tmp = root, * father, * trans;
	trans = father = tmp;
	while(tmp != 0) {
		if (tmp->left != 0) {
			while (trans != tmp && trans->right != tmp)
				trans = trans->right;
			father = tmp;
			tmp = tmp->left;
			father->left = tmp->right;
			tmp->right = father;
			if (trans->right == father)
				trans->right = tmp;
			else
				root = trans = tmp;
		}
		else
			tmp = tmp->right;
	}
}
template<class T>
void BST<T>::rotateLeft(BSTNode<T>*& data) {
	if (!data)
		return;
	BSTNode<T>* par = data->right;
	if (!par)
		return;
	data->right = par->left;
	par->left = data;
	data = par;
}
template<class T>
void BST<T>::createPerfectTree() {
	int n = counting();
	int m = (1 << ((int)(log2(n + 1)) ))- 1;
	int i;
	BSTNode<T>* Gr = 0, * tmp;
	if (n < 3)
		return; 
	for (i = 0, Gr = root; i < n - m; i++) {
		if (i == 0) {
			Gr = Gr->right;
			rotateLeft(root);
		}
		else if (Gr && Gr->right) {
			tmp = Gr->right->right;
			rotateLeft(Gr->right);
			Gr = tmp;
		}
	}
	while (m > 1) {
		m = m / 2;
		for (i = 0, Gr = root; i < m; i++) {
			if (i == 0) {
				Gr = Gr->right;
				rotateLeft(root);
			}
			else if (Gr && Gr->right) {
				tmp = Gr->right->right;
				rotateLeft(Gr->right);
				Gr = tmp;
			}
		}
	}
}
template<class T>
int BST<T>::counting() {
	Queue<BSTNode<T>*> queue;
	BSTNode<T>* p = root;
	int all = 0;
	if (p != 0) {
		queue.enqueue(p);
		while (!queue.empty()) {
			p = queue.dequeue();
			all++;
			if (p->left != 0)
				queue.enqueue(p->left);
			if (p->right != 0)
				queue.enqueue(p->right);
		}
	}
	return all;
}

#endif
