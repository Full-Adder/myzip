// class A{
// private:
// 	const int mm;
// public:
// 	A(){
// 		puts("hello");
// 	}
// 	int s;
// 	/*
// 		A(int m):mm(m){//常量，引用，父类构造函数，类对象
// 			//mm = m;const常量必须列表初始化
// 		}

// 		//
// 	*/

// }

// //为对象默认提供 构造、析构、等号运算符重载、拷贝构造函数
// //拷贝构造函数：用一个对象初始化另一个对象。
// // A a; A b(a);
// //b = a 等号运算符重载 对于一个已经创建好的对象，用另一个对象复制
// //A b = a; 还是拷贝构造函数，因为b对象还没有创建
// //在对象中涉及到动态内存分配new，我们需要重写拷贝、等号

// class B: A{

// }


// int main(){
// 	int a;
// 	int b;
// 	int *p;//地址分配在栈里，地址连续

// 	//A a(); 不是创建A的对象，会被理解为函数声名；
// 	B *b = new B;
// 	A *a;
// 	a = b//向上转型
// 	delet a;//会调用a的析构函数，所以我们会将父类的析构函数写成虚函数。 

// 	A a(1);
// 	a = 10;
// 	//是可行的，因为A有一个单参数的构造函数，可以充当类型转换的作用，将10—>A(10),继而调用=重载。

// 	A a(1);
// 	int m = a;
// 	//不可以，可以使用int 运算夫重载 operator int(){ return mm;}

// 	//链表继承出栈
// 	//类实现数组
// 	//函数模板
// }

#include<iostream>
using namespace std;
template<typename T>
T Max(T t1, T t2){
	if(t1>t2) return t1;
	return t2;
}

#define MAXSIZE 100
template<typename T>
class myStack
{
private:
	T data[MAXSIZE];
	int len;
public:
	myStack(){
		len = 0;
	}
	void push(T x){
		if(len<MAXSIZE-1) data[len++] = x;
		else cout<<"Stack is full"<<endl;
	}
	void pop(){
		if(!this->isEmpty()) len--;
		else cout<<"Stack is empty"<<endl;
	}
	T top(){
		if(!this->isEmpty()) return data[len-1];
		else {
			cout<<"Stack is empty"<<endl;
			return 1;
		}
	}
	bool isEmpty(){
		return len==0;
	}
	~myStack(){}
};


template<typename T>
void disp(T t){
	cout<<t<<endl;
}

int main(){
	// int a = 1,b = 2;cout<<Max(a,b);
	// myStack<int> ms;
	// for(int i = 0;i<20;i++){
	// 	ms.push(i*i);
	// }
	// while(!ms.isEmpty()){
	// 	cout<<ms.top()<<" ";
	// 	ms.pop();
	// }
	// cout<<endl;
	// for(int i = 0;i<20;i++){
	// 	ms.push(i*7);
	// }
	// while(!ms.isEmpty()){
	// 	cout<<ms.top()<<" ";
	// 	ms.pop();
	// }
	// return 0;
	int a = 1;
	double b = 0.2222;
	char c = 'p';
	char d[9] = "okok";
	char *e = d;
	disp(a);
	disp(b);
	disp(c);
	disp(d);	
	disp(e);
}