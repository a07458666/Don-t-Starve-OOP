#include<iostream>

int sub(int i,int j){
	return i-j;
}
int add(int i,int j){
	return i+j;
}int main(){
	int i=3,j=1;
	cout<<"Add Result:\n"+ add(i,j)+"\n";
	cout<<"Result:"+sub(i,j)+"\n";	return 0;
}