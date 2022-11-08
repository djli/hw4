#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int heightCheck(Node * root){
	//this didnt work
	/*
	if(root->left != nullptr || root->right != nullptr){
		return true;
	}
	else{
		return false;
	}
	*/

	//returns longest path
	if (root == nullptr) return 0;

	int lHeight = heightCheck(root->left);
	int rHeight = heightCheck(root->right);
	if(lHeight >= rHeight){
		return lHeight+1;
	}
	else{
		return rHeight+1;
	}
	
}


bool equalPaths(Node * root)
{
	//based case
  if(root == nullptr){
		return true;
	}
	int lHeight = heightCheck(root->left);
	int	rHeight = heightCheck(root->right);

	//if paths are equal or there is only one path
	if(lHeight == rHeight || lHeight == 0 || rHeight == 0){
		return equalPaths(root->left) && equalPaths(root->right);
	}
	else{
		return false;
	}
}

