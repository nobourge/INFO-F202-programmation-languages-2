#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

//--- Q1

class Rectangle {
	double width, height;
public:
	Rectangle(double width, double height):width{width},height{height}{}
	double getWidth() const {return width;}
	double getHeight() const {return height;}
	double area() const {return width*height;}
	operator string() const {return "["+to_string(width)+","+to_string(height)+"]";}
	operator pair<double,double>() const{return pair<double,double>(width,height);}
	Rectangle(pair<double,double> p):width{p.first},height{p.second}{};
};

void demo1(){
	Rectangle r{2,10};
	cout<<r.area()<<endl;	
	pair<double,double> p{5,10};
	Rectangle r2{p};
	string s{r};
	cout<<s<<endl;
	p=r;
	cout<<p.first<<endl;
}

//--- Q2 

template<typename T,typename F>
vector <T>transform(const vector<T> &A,F f){
	vector <T> B;
	for (auto a:A) B.push_back(f(a));
	return B; 
}

class Square{
public:
	int operator()(int x){return x*x;}
};

template<class T>
void print(T A){
	cout<<"[ ";
	for (auto x:A) cout<<x<<" ";
	cout<<"]"<<endl;
}


void demo2(){
	vector <int>V1{1,2,3,4};
	auto V2=transform(V1,Square());
	print(V2);

	vector <string>V3{"1","2","3","4"};
	auto V4=transform(V3,[](const string &x){return "0x"+x;} );
	print(V4);


}

//--- Q3

class Range{
	int start,stop,step;
	class Iterator{
		int index,step;
	public:
		Iterator(int index,int step):index{index},step{step}{}
		int operator *(){return index;}
		Iterator &operator++(){index+=step;return *this;}
		bool operator ==(const Iterator&I)const{return index==I.index;}
	};
public:
	Range(int stop):start{0},stop{stop},step{1}{}
	Range(int start,int stop,int step=1):start{start},stop{stop},step{step}{}
	Iterator begin(){return Iterator{start,step};}
	Iterator end(){return Iterator{stop,step};}
};

void demo3(){
	for (auto x:Range(5,10)) cout<<x<<" "; cout<<endl;
	for (auto x:Range(10)) cout<<x<<" "; cout<<endl;
	for (auto x:Range(100,1000,100)) cout<<x<<" "; cout<<endl;

	int sum=0;
	for (auto x:Range(1000000)) sum+=x;
	cout<<sum<<endl;
}

//--- Q4 

class Expression{
public:
	virtual double evaluate(double x) const = 0;
	virtual Expression *clone() const =0;
	virtual ~Expression()=default;
};

class Number : public Expression {
	double num;
public:
	Number(double num):num{num}{};
	Number *clone() const override {return new Number(num);};
	double evaluate(double) const override { return num;}
};

class X : public Expression {
public:
	double evaluate(double x) const override { return x;}
	X *clone() const override {return new X();};
};


class Sum: public Expression {
	Expression *l,*r;
public:
	Sum(Expression *l,Expression *r):l{l},r{r}{};
	Sum(const Sum &other):l{other.l->clone()},r{other.r->clone()}{};
	Sum(Sum &&other):l{other.l},r{other.r}{};
	~Sum() override {delete l;delete r;}
	Sum &operator =(const Sum &other){delete l;delete r;l=other.l->clone();r=other.r->clone();return *this;}	
	Sum &operator =(Sum &&other){delete l;delete r;l=other.l;r=other.r;return *this;}	
	double evaluate(double x) const override {return l->evaluate(x)+r->evaluate(x);}
	Sum *clone() const override {return new Sum(l,r);}
};

class Product: public Expression {
	Expression *l,*r;
public:
	Product(Expression *l,Expression *r):l{l},r{r}{};
	Product(const Product &other):l{other.l->clone()},r{other.r->clone()}{};
	Product(Product &&other):l{other.l},r{other.r}{};
	~Product(){delete l;delete r;}
	Product &operator =(const Product &other){delete l;delete r;l=other.l->clone();r=other.r->clone();return *this;}	
	Product &operator =(Product &&other){delete l;delete r;l=other.l;r=other.r;return *this;}	
	double evaluate(double x) const override {return l->evaluate(x)*r->evaluate(x);}
	Product *clone() const override {return new Product(l,r);}
};

void demo4(){
	auto E=new Sum(new X(),new Product(new Number(5),new X()));
	for (auto x:Range(10))
		cout<<x<<" "<<E->evaluate(x)<<endl;
}
//-- Q5

class Count {
	static int count;
	int myCount=0;
public:
	Count() {myCount=count++;}
	int get() const {return myCount;}
};
int Count::count=0;


class Point : public Count {
	int x,y;
public:
	Point(int x,int y):x{x},y{y}{};
	string description() const {return to_string(x)+" "+to_string(y);}
};

class Color : public Count {
	int r,g,b;
public:
	Color(int r,int g,int b):r{r},g{g},b{b}{}
	string description() const {return to_string(r)+" "+to_string(g)+" "+to_string(b);}
};

class ColoredPoint : public Point, public Color {
public:
	ColoredPoint(int x,int y,int r,int g,int b):Point{x,y},Color{r,g,b}{}
	string description() const {return Point::description()+" "+Color::description();}
};

void demo5(){
	ColoredPoint cp{1,2,3,4,5};
	//cout<<cp.get(); 
	cout<<cp.Point::get()<<endl;
	cout<<cp.Color::get()<<endl; 
	//cout<<cp.Count::get()<<endl; 
	cout<<cp.description()<<endl;
	Point *p=&cp;
	cout<<p->description()<<endl;
}

//-----------------

int main(void){
	cout<<"Demo 1"<<endl;
	demo1();
	cout<<"Demo 2"<<endl;
	demo2();
	cout<<"Demo 3"<<endl;
	demo3();
	cout<<"Demo 4"<<endl;
	demo4();
	cout<<"Demo 5"<<endl;
	demo5();
}





