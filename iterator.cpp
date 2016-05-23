#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <iterator>
#include <cmath>
#include <stack>

using namespace std;
class Container;
class Base;
class Sort;
class Iterator;
class OperatorIterator;
class UnaryIterator;

class Container{
    protected:
        Sort * sort_function;

    public:
        //constructor
        Container() { this->sort_function = NULL; }
        //Container(Sort * function(Sort * sort_function)); //select algorithm
        Container(Sort* function) { this->sort_function = function; }

    virtual void set_sort_function(Sort * sf){
        this->sort_function = sf;
    }

    //pure virtual
    virtual void add_element(Base * element) = 0;
    virtual void print() = 0;
    virtual void sort() = 0;

    //sort functions
    virtual void swap(int i, int j) = 0;
    virtual Base * at(int i) = 0;
    virtual int size() = 0;
};

class Sort{
    public:
        Sort() {};

        virtual void sort(Container * container) = 0;
};


class Base{
    public: 
    //constructors
    Base() { };

    Base* leftOperand;
    Base* rightOperand;

    //Pure virtual functions
    virtual double evaluate() = 0;
    virtual Iterator * make_itr() = 0;
};

class BubbleSort: public Sort{
    public:
         BubbleSort(){};
       virtual void sort(Container * container){
            for (int i = 0; i < container->size(); i++){
                for (int j = 1; j < container->size(); j++){
                    if ((container->at(j-1))->evaluate() > (container->at(j))->evaluate()){
                       container->swap(j-1,j);
                     }
                  }  
                }
             }

  /* int find_min(Container * container, int start){
       double temp1 = container->at(start)->evaluate(); 
       int pos = start;

       for (int i = start; i < container->size(); ++i){
            if(temp1 > container->at(i)->evaluate()){
                pos = i;
                temp1 = container->at(i)->evaluate();
            }
       } 
       return pos;
    }*/


};

class SelectionSort: public Sort{
    public:
        SelectionSort(){};
        virtual void sort(Container * container){
            int i,j,min = 0;
            for (i = 0; i < container->size()-1; ++i){
                min = i;
                for (j = i+1; j < container->size(); ++j){
                    if ( (container->at(j))->evaluate() < (container->at(min))->evaluate() ){
                        min = j;
                    }
                    if (min !=i){
                        container->swap(i, min);  
                    }
                }
            }

        }

};

class ListContainer: public Container{
    private:
        list<Base *> lis;
    protected: 
        //Sort* sort_function;

    public:
        //constructor
        ListContainer() : Container() { };
        //ListContainer() : sort_function(NULL){};
        //ListContainer(Sort* function(Sort* sort_function)); // slect algorithm

        //pure virtual
        virtual void add_element(Base * element){ lis.push_back(element); }

        virtual void print(){
           list<Base *>::iterator i;
            for (i = lis.begin(); i != lis.end(); ++i)
                cout << (*i)->evaluate() << " ";
            cout << endl;
        }

        virtual void sort(){
            if(this->sort_function == NULL) { cout << "ERROR!" << endl;}
            sort_function->sort(this);
            return;
        }

        virtual void swap(int i, int j){
            list<Base *>::iterator itr1 = lis.begin();
            advance (itr1, i);//set = i
            list<Base *>::iterator itr2 = lis.begin();
            advance (itr2, j);//set = j
            std::swap(*itr1, *itr2);
        }

        virtual int size () { return lis.size(); }

        virtual Base * at(int i){
            list<Base *>::iterator it = lis.begin();
            for (int j = 0; j <i; ++j){
                ++it;
            }
            return *(it);
        }

};

class VectorContainer: public Container{
   private:
        vector<Base *> vec;
   protected:
        Sort* sort_function;

    public:
        //constructor
       VectorContainer() : sort_function(NULL){};
       VectorContainer(Sort* function(Sort* sort_function)); //select algorithm
    //pure virtual
    virtual void add_element(Base * element){ vec.push_back(element); }
    virtual void print(){
        for (int i = 0; i < vec.size(); ++i){
            cout << vec.at(i)->evaluate() << " ";
        }
        cout << endl;
    }
    virtual void sort(){
        sort_function->sort(this);
        return;
    }

    //sort functions
    virtual void swap(int i, int j){
        Base * temp = vec.at(i);
        vec.at(i) = vec.at(j);
        vec.at(j) = temp;
    }
    virtual Base * at(int i){ return vec.at(i); }
    virtual int size(){ return vec.size(); }
  

};

class Iterator{
    protected:
        Base * self_ptr;
        Base * current_ptr;

    public:
        Iterator (Base* ptr) {this->self_ptr = ptr;}

        //set up itr to start at beginning
        virtual void first() = 0;
        //move to next element
        virtual void next() = 0;
        //true if finished with all elements
        virtual bool is_done() = 0;
        //Return element iterator currently points too
        virtual Base* current() = 0;
};

class OperatorIterator: public Iterator{
    public:

        OperatorIterator(Base * ptr): Iterator(ptr){;}
        void first(){
            current_ptr = self_ptr->leftOperand;
        }

        void next(){
            if (current_ptr == self_ptr->leftOperand){
                current_ptr = self_ptr->rightOperand;
            }
            else 
                current_ptr = NULL;
        }

        bool is_done(){
            if(current_ptr == NULL){
                return true ;
            }
            else
                return false;
        }

        Base* current(){
            cout << "current operator" << endl;
            return current_ptr;
        }

};

class UnaryIterator: public Iterator{
    public:
        UnaryIterator(Base * ptr): Iterator(ptr){;}

        void first(){
           current_ptr = self_ptr->leftOperand;
        }

        void next(){
            if (current_ptr = self_ptr->leftOperand){
                current_ptr = NULL;
             }
        }

        bool is_done(){
            if(current_ptr == NULL){
                return true;
            }
            else
                return false;
        }
        Base* current(){
            cout << "current unary" << endl;
            return current_ptr;
        }
};

class NullIterator: public Iterator{
    public:
        NullIterator(Base * ptr): Iterator(ptr){;}
        void first(){};
        void next(){};
        bool is_done(){
            return true;
        }
        Base* current(){
            cout << "current null" << endl;
            return NULL;
        }
};

class PreOrderIterator : public Iterator{
    protected:
        stack<Iterator*> iterators;

    public:
        PreOrderIterator(Base* ptr): Iterator(ptr){}

        void first(){
            while ( iterators.size() > 0 ){
                iterators.pop();
            }

            Iterator * it = self_ptr->make_itr();
            it->first();
            iterators.push(it);
        }
        void next(){
            Iterator * itr = iterators.top()->current()->make_itr();
            itr->first();
            iterators.push(itr);

            while (iterators.top()->is_done()){
                iterators.pop();
                if (iterators.size() == 0)
                    break;

                iterators.top()->next();
            }
        }
        bool is_done(){
            if( iterators.size() == 0)
                return true;
            return false;
        }
        Base* current(){
            return iterators.top()->current();
        }
};



//leaf node
class Op: public Base{
  public:
    double num;
    Op(){num = 0;}
    Op(double val){ num = val;}
    double evaluate(){
        cout << "evaluate op" << endl;
        return num;
    }
    virtual Iterator * make_itr(){ return new NullIterator(this); }

};

//composites 2 children
class Mult: public Base{
    public:
    Base* leftOperand;
    Base* rightOperand;
    
    Mult(){};
    Mult (Base* a, Base* b){leftOperand = a; rightOperand = b;}
    double evaluate(){
        cout << "evaluate mult " << endl;
        return leftOperand->evaluate() * rightOperand->evaluate();
    }
    Iterator * make_itr(){ return new OperatorIterator(this); }
};

class Div: public Base{
    public:
    Base* leftOperand;
    Base* rightOperand;
    
    Div(){};
    Div (Base* a, Base* b){leftOperand = a; rightOperand = b;}
    
    double evaluate(){
        cout << "evaluate div " << endl;
        if(rightOperand->evaluate() == 0){
            cout << "Error: Cannot divide by zero";
            exit(1);
        }
        return leftOperand->evaluate() / rightOperand->evaluate();
    }
    Iterator * make_itr(){ return new OperatorIterator(this); }

};


class Add: public Base{
    public:
    Base* leftOperand;
    Base* rightOperand;
    
    Add(){};
    Add (Base* a, Base* b){leftOperand = a; rightOperand = b;}
    double evaluate(){
        cout << "evaluate add " << endl;

        return leftOperand->evaluate() + rightOperand->evaluate();
    }
    Iterator * make_itr(){ return new OperatorIterator(this); }

};

class Sub: public Base{
    public:
    Base* leftOperand;
    Base* rightOperand;
    
    Sub(){};
    Sub (Base* a, Base* b){leftOperand = a; rightOperand = b;}
    double evaluate(){
        cout << "evaluate sub " << endl;

        return leftOperand->evaluate() - rightOperand->evaluate();
    }
    Iterator * make_itr(){ return new OperatorIterator(this); }


};

class Sqr: public Base{
    public:
    Base* Operand;
    
    Sqr(){};
    Sqr (Base* a){Operand = a;}
    double evaluate(){
        cout << "evaluate sqr " << endl;

        return Operand->evaluate() * Operand->evaluate();
    }
    Iterator * make_itr(){ return new UnaryIterator(this); }
 
};

class Abs: public Base{
    public:
        Base* Operand;
    
        Abs(){};
        Abs (Base* a){Operand = a;}
        double evaluate(){
            return abs(Operand->evaluate());
         }
};
class Floor: public Base{
    public:
        Base* Operand;
    
        Floor(){};
        Floor (Base* a){Operand = a;}
        double evaluate(){
            return floor(Operand->evaluate());
         }
};
class Ceil: public Base{
    public:
        Base* Operand;
    
        Ceil(){};
        Ceil (Base* a){Operand = a;}
        double evaluate(){
            return ceil(Operand->evaluate());
         }
};


int main(){
    Op* op3 = new Op(3);
    Op* op4 = new Op(4);
    Op* op2 = new Op(2);
    Add* add = new Add(op3, op4);
    Sqr* sqr = new Sqr(op2);
    Sub* sub = new Sub(add, sqr);
    Sqr* sqr2 = new Sqr(sub);

    cout << "--- PreOrder Iteration ---" << endl;
    cout << "test1" << endl;
    PreOrderIterator* pre_itr = new PreOrderIterator(add);
    int i = 2;
    pre_itr->first(); 
    cout<<pre_itr->current()->evaluate()<<endl;
    if(!pre_itr->is_done()){cout<<"true"<<endl;}
    /*for (pre_itr->first(); !pre_itr->is_done(); pre_itr->next()){
            cout << "test" << i << endl;
            
           cout<< pre_itr->current()->evaluate();
            cout << endl;
        }
   */
    return 0;
}

