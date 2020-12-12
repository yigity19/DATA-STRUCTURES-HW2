/* @Author
Student Name: <YUNUS EMRE YİĞİT>
Student ID : <150190107>
Date: <09.12.2020> */

#include <iostream>
#include<fstream>
using namespace std;

struct resistor{
    char group;
    float value;
    int quantity;
    resistor *next;
};

struct supp_resistor{
    float value;
    int quantity;
    supp_resistor* next;
};

struct circuit{
    resistor* head;
    void create();
    void add_resistor(char group, double value);
    void remove_resistor(char, double);
    void delete_resistor(char);
    void circuit_info(supp_resistor* c);
    void clear();
};

struct supp_circuit{
    supp_resistor* head;
    void create();
    supp_resistor* add(circuit c1);
    void clear();
};



int main(int argc, char* argv[]){                                                                                 //MAIN FUNCTION
    ifstream file(argv[1]);
    if(!(file.is_open())){
        cout<<"Program couldn't read the file";
        return 0;
    }
    circuit my_circuit;                                                     //CIRCUIT FORMATION
    my_circuit.create();
    supp_circuit supp;                                                      //supp_circuit formation
    supp.create();
    char letter; float num;
    while(file>>letter>>num){
        //cout<<letter<<" "<< num<<endl;
        if(num>0)
            my_circuit.add_resistor(letter,num);
        else if (num<0)
            my_circuit.remove_resistor(letter,num);
        else if(letter == 'A' && num == 0){
                supp.create();
                supp_resistor* c = supp.add(my_circuit);
                my_circuit.circuit_info(c);
                supp.clear();
            }
    }
    my_circuit.clear();
    file.close();
    return 0;
}

void circuit::create(){
    head = NULL;
}


void circuit::add_resistor(char group, double value){
    resistor* ptr = head;

    bool checker = false;
    while(head && ptr->next != NULL){
        if (ptr->group == group){
            checker = true;
            if (ptr-> value > 0)
                ptr->quantity++;
            break;
        }
        ptr = ptr->next;
    }

    if (checker == false){
        resistor* toadd = new resistor;
        resistor* tail = head;
        toadd->group = group;
        toadd->value = value;
        toadd->quantity = 1;
        toadd->next = NULL;

        
        if(!head){                                                               //MAKE TOADD HEAD IF HEAD IS NULL
            head = toadd;
            tail = toadd;
            
        }
    else{                                                         //FIND TAIL
        tail = head;
        while(tail->next != NULL)
            tail = tail->next;   
        
        if (toadd->group >= tail->group){
           // cout<<"BİZİMKİ  "<<tail->group<<" value "<<tail->value<<endl;
            tail->next = toadd;
            tail = toadd;
           // cout<<"tail kısmında"<<endl;
        }
            
        else if(toadd->group <= head->group){                                         //ADD TO HEAD
            toadd->next = head;
            head = toadd;
            

        }
        else if (toadd->group > head-> group && tail->group > toadd->group){
            resistor* ptr = head;
            while (ptr->next && ptr->next->group < toadd->group)
                ptr = ptr->next;
            
            toadd->next = ptr->next;
            ptr->next = toadd;
           
            }
    }

    }
}

void circuit::remove_resistor(char group, double value){
    resistor* ptr = head;
    while(ptr && ptr->group != group)
        ptr = ptr->next;
    if (ptr == NULL){
        cout<<"NO_RESISTOR"<<endl;
        return;
    }
    ptr->quantity--;
    if(ptr->quantity == 0){
        delete_resistor(group);
    }
}


void circuit::delete_resistor(char group){
    resistor* ptr = head;
    resistor* prev = head;
    while(ptr->group != group){
        ptr = ptr->next;
    }
    if(ptr == head)
        head = head->next;
    else{
        while(prev->next != ptr)
            prev = prev->next;
        prev->next = ptr->next;
        ptr->next = ptr;
    }
    delete ptr;
}

void circuit::circuit_info(supp_resistor* c){
    resistor* ptr = head;
    supp_resistor* supp_ptr= c; 
    double sum = 0;
    while (ptr!=NULL){
        sum+=ptr->value/ptr->quantity;
        ptr = ptr->next;
    }
    while(supp_ptr != NULL){
        cout<<supp_ptr->value<<":"<<supp_ptr->quantity<<endl;
        supp_ptr = supp_ptr->next;
    }
    cout<<"Total resistance="<<sum<<" ohm"<<endl;
}

void circuit::clear(){
    resistor* curr = head;
    resistor* trail = curr;
    while(curr != NULL){
        trail = curr->next;
        curr->next = curr;
        delete curr;
        curr = trail;
    }
}

void supp_circuit::create(){
    head = NULL;
}

supp_resistor* supp_circuit::add(circuit c1){
    resistor* node = c1.head;
    while(node){
        supp_resistor* toadd = new supp_resistor;
        toadd-> value = node->value;
        toadd->quantity = node->quantity;
        toadd->next = NULL;
        if (head ==NULL){                           //add to head
            head = toadd;
        }
        else{                                       //if head is not empty

                if(toadd->value < head->value){         //add to beginning
                    toadd->next = head;
                    head = toadd;
                }

                else{
                    bool checker = false;
                    supp_resistor* ptr= head;          //increase quantity
                    while(ptr){                                       
                        if(ptr->value == toadd->value){
                            ptr->quantity++;
                            checker = true;
                        }    
                        ptr = ptr->next;
                        }
                    if (checker == false){                      //add beyond head
                        ptr = head;
                        while(ptr && ptr->next && ptr->next->value < toadd->value)
                            ptr = ptr->next;
                        toadd->next = ptr->next;
                        ptr->next = toadd;
                        }
                    }
        }
        node = node->next;
    }
    return head;
}

void supp_circuit::clear(){
    supp_resistor* curr = head;
    supp_resistor* trail = curr;
    while(curr != NULL){
        trail = curr->next;
        curr->next = curr;
        delete curr;
        curr = trail;
    }
}