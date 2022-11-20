#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		void insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
		void remove_fix(AVLNode<Key,Value>* n, int8_t diff);
		void rotateRight(AVLNode<Key,Value>* n);
		void rotateLeft(AVLNode<Key,Value>* n);


};
//Right Rotation
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* n){
	AVLNode<Key,Value>* p = n->getParent();
	AVLNode<Key,Value>* x = n->getLeft();
	AVLNode<Key,Value>* b = n->getLeft()->getRight();

	if(p != NULL){
		if(p->getLeft() == n){
			p->setLeft(x);
		}
		else{
			p->setRight(x);
		}
	}
	else{
		BinarySearchTree<Key,Value>::root_ = x;
	}

	x->setParent(p);
	x->setRight(n);
	n->setParent(x);
	n->setLeft(b);

	if(b != NULL){
		b->setParent(n);
	}
}

//Left Rotation
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* n){
	AVLNode<Key,Value>* p = n->getParent();
	AVLNode<Key,Value>* y = n->getRight();
	AVLNode<Key,Value>* b = n->getRight()->getLeft();

	if(p != NULL){
		if(p->getLeft() == n){
			p->setLeft(y);
		}
		else{
			p->setRight(y);
		}
	}
	else{
		BinarySearchTree<Key,Value>::root_ = y;
	}

	y->setParent(p);
	y->setLeft(n);
	n->setParent(y);
	n->setRight(b);

	if(b != NULL){
		b->setParent(n);
	}
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
	if(BinarySearchTree<Key,Value>::root_ == NULL){
		BinarySearchTree<Key,Value>::root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
		return;
	}

	AVLNode<Key, Value>* newNode = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::internalFind(new_item.first));
	if(newNode != NULL){
			newNode->setValue(new_item.second);
			return;
	}

	newNode = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::root_);

	while((new_item.first < newNode->getKey() && newNode->getLeft() != NULL) || 
				(new_item.first > newNode->getKey() && newNode->getRight() != NULL)){
		if(new_item.first < newNode->getKey()){
			newNode = newNode->getLeft();
		}
		else{
			newNode = newNode->getRight();
		}
	}
	if(new_item.first < newNode->getKey()){
		newNode->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, newNode));
		if(newNode->getBalance() == 0){
			newNode->updateBalance(-1);
			insert_fix(newNode, newNode->getLeft());
		}
		else{
			newNode->setBalance(0);
		}
	}
	else{
		newNode->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, newNode));
		if(newNode->getBalance() == 0){
			newNode->updateBalance(1);
			insert_fix(newNode, newNode->getRight());
		}
		else{
			newNode->setBalance(0);
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){
	if(p == NULL){
		return;
	}
	else if(p->getParent() == NULL){
		return;
	}
	else{
		AVLNode<Key,Value>* g = p->getParent();
		//left child
		if(p == g->getLeft()){
			g->updateBalance(-1);
			if(g->getBalance() == 0){
				return;
			}
			else if(g->getBalance() == -1){
				insert_fix(g, p);
			}
			else if(g->getBalance() == -2){
				//zigzig
				if(n == p->getLeft()){
					rotateRight(g);
					p->setBalance(0);
					g->setBalance(0);
				}
				//zigzag
				else{
					rotateLeft(p);
					rotateRight(g);
					if(n->getBalance() == -1){
						p->setBalance(0);
						g->setBalance(1);
						n->setBalance(0);
					}
					else if(n->getBalance() == 0){
						p->setBalance(0);
						g->setBalance(0);
						n->setBalance(0);
					}
					else if(n->getBalance() == 1){
						p->setBalance(-1);
						g->setBalance(0);
						n->setBalance(0);
					}
				}
			}
		}
		//right child
		else{
			g->updateBalance(1);
			if(g->getBalance() == 0){
				return;
			}
			else if(g->getBalance() == 1){
				insert_fix(g, p);
			}
			else if(g->getBalance() == 2){
				//zigzig
				if(n == p->getRight()){
					rotateLeft(g);
					p->setBalance(0);
					g->setBalance(0);
				}
				//zigzag
				else{
					rotateRight(p);
					rotateLeft(g);
					if(n->getBalance() == 1){
						p->setBalance(0);
						g->setBalance(-1);
						n->setBalance(0);
					}
					else if(n->getBalance() == 0){
						p->setBalance(0);
						g->setBalance(0);
						n->setBalance(0);
					}
					else if(n->getBalance() == -1){
						p->setBalance(1);
						g->setBalance(0);
						n->setBalance(0);
					}
				}
			}
		}
	}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
	AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::internalFind(key));
	if (temp == NULL){
			return;
	}

	int8_t diff;

//2 KIDS
	if(temp->getLeft() != NULL && temp->getRight() != NULL){
		AVLNode<Key, Value>* tempPred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::predecessor(temp));
		nodeSwap(temp, tempPred);
		AVLNode<Key,Value>* rmParent = static_cast<AVLNode<Key, Value>*>(temp->getParent());
		AVLNode<Key,Value>* rmLeft = static_cast<AVLNode<Key, Value>*>(temp->getLeft());
		AVLNode<Key,Value>* rmRight = static_cast<AVLNode<Key, Value>*>(temp->getRight());
		if(rmParent != NULL){
			if(temp == rmParent->getLeft()){
				diff = 1;
				//rmParent->updateBalance(diff);
			}
			else{
				diff = -1;
				//rmParent->updateBalance(diff);
			}
		}

		//root check
		if(tempPred->getParent() == NULL){
			BinarySearchTree<Key,Value>::root_ = tempPred;
		}

		//leaf check for node to be removed
		if(rmLeft == NULL && rmRight == NULL){
			if(rmParent->getKey() < tempPred->getKey()){
				rmParent->setRight(NULL);
			}
			else{
				rmParent->setLeft(NULL);
			}
		}
		//check 1 kid
		else if(rmLeft != NULL && rmRight == NULL){
			rmLeft->setParent(rmParent);
			if(rmParent->getKey() < tempPred->getKey()){
				rmParent->setRight(rmLeft);
			}
			else{
				rmParent->setLeft(rmLeft);
			}
		}
		else if(rmRight != NULL && rmLeft == NULL){
			rmRight->setParent(rmParent);
			if(rmParent->getKey() < tempPred->getKey()){
				rmParent->setRight(rmRight);
			}
			else{
				rmParent->setLeft(rmRight);
			}
		}
		delete temp;
		remove_fix(rmParent, diff);
	}
	//1 kid
	else if(temp->getLeft() != NULL && temp->getRight() == NULL){
		AVLNode<Key,Value>* rmLeft = temp->getLeft();
		AVLNode<Key,Value>* rmParent = temp->getParent();
		if(rmParent != NULL){
			if(temp == rmParent->getLeft()){
				diff = 1;
				//rmParent->updateBalance(diff);
			}
			else{
				diff = -1;
				//rmParent->updateBalance(diff);
			}
		}
		//root check
		if(temp == BinarySearchTree<Key,Value>::root_){
			BinarySearchTree<Key,Value>::root_ = rmLeft;
		}
		else if(rmParent->getKey() < temp->getKey()){
			rmParent->setRight(rmLeft);
		}
		else{
			rmParent->setLeft(rmLeft);
		}
		rmLeft->setParent(rmParent);
		delete temp;
		remove_fix(rmParent, diff);
	}
	else if(temp->getRight() != NULL && temp->getLeft() == NULL){
		AVLNode<Key,Value>* rmRight = temp->getRight();
		AVLNode<Key,Value>* rmParent = temp->getParent();

		if(rmParent != NULL){
			if(temp == rmParent->getLeft()){
				diff = 1;
				//rmParent->updateBalance(diff);
			}
			else{
				diff = -1;
				//rmParent->updateBalance(diff);
			}
		}

		//root check
		if(temp == BinarySearchTree<Key,Value>::root_){
			BinarySearchTree<Key,Value>::root_ = rmRight;
		}
		else if(rmParent->getKey() < temp->getKey()){
			rmParent->setRight(rmRight);
		}
		else{
			rmParent->setLeft(rmRight);
		}
		rmRight->setParent(rmParent);
		delete temp;
		remove_fix(rmParent, diff);
	}
	//no kids
	else if(temp->getRight() == NULL && temp->getLeft() == NULL && temp->getParent() != NULL){
		AVLNode<Key,Value>* rmParent = temp->getParent();

		if(rmParent != NULL){
			if(temp == rmParent->getLeft()){
				diff = 1;
				//rmParent->updateBalance(diff);
			}
			else{
				diff = -1;
				//rmParent->updateBalance(diff);
			}
		}

		if(rmParent->getKey() < temp->getKey()){
			rmParent->setRight(NULL);
		}
		else{
			rmParent->setLeft(NULL);
		}
		delete temp;
		remove_fix(rmParent, diff);
	}
	//root no kids
	else if(temp == BinarySearchTree<Key,Value>::root_){
		BinarySearchTree<Key,Value>::root_ = NULL;
		delete temp;
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key,Value>* n, int8_t diff){
	if(n == NULL){
		return;
	}

	int8_t ndiff;
	AVLNode<Key,Value>* p = n->getParent();
	if(p != NULL){
		if(n == p->getLeft()){
			ndiff = 1;
		}
		else{
			ndiff = -1;
		}
	}

	if(diff == -1){
		if(n->getBalance() + diff == -2){
			AVLNode<Key,Value>* c = n->getLeft();
			if(c->getBalance() == -1){
				rotateRight(n);
				n->setBalance(0);
				c->setBalance(0);
				remove_fix(p, ndiff);
			}
			else if(c->getBalance() == 0){
				rotateRight(n);
				n->setBalance(-1);
				c->setBalance(+1);
			}
			else if(c->getBalance() == 1){
				AVLNode<Key,Value>* g = c->getRight();
				rotateLeft(c);
				rotateRight(n);
				if(g->getBalance() == 1){
					n->setBalance(0);
					c->setBalance(-1);
					g->setBalance(0);
				}
				else if(g->getBalance() == 0){
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}
				else if(g->getBalance() == -1){
					n->setBalance(1);
					c->setBalance(0);
					g->setBalance(0);
				}
				remove_fix(p, ndiff);
			}
		}
		else if(n->getBalance() + diff == -1){
			n->setBalance(-1);
		}
		else if(n->getBalance() + diff == 0){
			n->setBalance(0);
			remove_fix(p, ndiff);
		}
	}
	else if(diff == 1){
		if(n->getBalance() + diff == 2){
			AVLNode<Key,Value>* c = n->getRight();
			if(c->getBalance() == 1){
				rotateLeft(n);
				n->setBalance(0);
				c->setBalance(0);
				remove_fix(p, ndiff);
			}
			else if(c->getBalance() == 0){
				rotateLeft(n);
				n->setBalance(1);
				c->setBalance(-1);
			}
			else if(c->getBalance() == -1){
				AVLNode<Key,Value>* g = c->getLeft();
				rotateRight(c);
				rotateLeft(n);
				if(g->getBalance() == -1){
					n->setBalance(0);
					c->setBalance(1);
					g->setBalance(0);
				}
				else if(g->getBalance() == 0){
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}
				else if(g->getBalance() == 1){
					n->setBalance(-1);
					c->setBalance(0);
					g->setBalance(0);
				}
				remove_fix(p, ndiff);
			}
		}
		else if(n->getBalance() + diff == 1){
			n->setBalance(1);
		}
		else if(n->getBalance() + diff == 0){
			n->setBalance(0);
			remove_fix(p, ndiff);
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
