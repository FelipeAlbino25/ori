#include <iostream>
#include <fstream>
#include <limits> 
#include <cstring>
using namespace std;

struct Student {
    char name[30];
    char ra[7];
    double grade;
};

Student getStudentInput() {
    Student s;
    // Limpa a struct para evitar que lixo de memória vá para o arquivo
    memset(&s, 0, sizeof(Student));

    cout << "Nome do Estudante: ";
    cin.getline(s.name, 30);

    cout << "RA (6 digitos): ";
    cin >> s.ra;

    cout << "Nota: ";
    while (!(cin >> s.grade)) {
        cout << "Erro! Digite um numero para a nota: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // para não quebrar o próximo getline ou a próxima volta do menu
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return s;
}

void getAllStudents() {
    fstream file("students.dat", std::ios::in | std::ios::out | std::ios::binary);
    if(!file){
        ofstream create("students.dat",ios::binary);
        create.close();
        file.open("students.dat",ios::in | ios::out | ios::binary);
    }

    file.seekg(0, ios::beg);
    Student tmp;

    int count = 0;

    while(file.read(reinterpret_cast<char*>(&tmp),sizeof(Student))){
        cout << "\nRA: " << tmp.ra
        <<"\nNome: "<< tmp.name << "\nNota: "<< tmp.grade<<endl;
        count++;
    }
    if(count== 0){
        cout << "\nNenhuma entrada foi adicionada ainda.\n";
    }
    file.close();
}

void createStudent() {
    Student newStudent = getStudentInput();

    fstream file("students.dat", ios::in | ios::out | ios::binary);
    
    if (!file) {
        ofstream create("students.dat", ios::binary | ios::trunc);
        create.close();
        file.open("students.dat", ios::in | ios::out | ios::binary);
    }

    file.seekp(0, ios::end);
    file.write(reinterpret_cast<char*>(&newStudent), sizeof(Student));

    file.close();
    cout << "\nEstudante '" << newStudent.name << "' cadastrado com sucesso!" << endl;
}
void getStudentByRa(){
    Student tmp;

    char ra[7];
    cin >> ra;

    fstream file("students.dat",ios::in | ios::out | ios::binary);

    file.seekg(0,ios::beg);
    while(file.read(reinterpret_cast<char*>(&tmp),sizeof(Student))){
        if(strcmp(ra, tmp.ra)==0){
             cout << "\nRA: " << tmp.ra
        <<"\nNome: "<< tmp.name << "\nNota: "<< tmp.grade<<endl;
        break;
        }
    }
}
void updateStudent(){
    fstream file("students.dat", std::ios::in | std::ios::out | std::ios::binary);
   
    file.seekg(0, ios::beg);
    Student tmp;
    char ra[7];
    cin >> ra;
    int count =0;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');


    while(file.read(reinterpret_cast<char*>(&tmp),sizeof(Student))){
        if(strcmp(tmp.ra, ra)==0){
            Student newStudent = getStudentInput();
            file.seekp(count * sizeof(Student));
            file.write(reinterpret_cast<char*>(&newStudent),sizeof(Student));
            break;
        }
        count++;
    }
  
    file.close();
}

int main(){


    int userOption = -1;
    while(userOption != 5){
        cout << "\nEscolha uma opção:\n1.GET all students\n2.CREATE a student\n3.GET student by RA\n4.UPDATE a student\n5.QUIT\nOPCAO: ";
        cin >> userOption;
        cin.ignore(1000, '\n');
        switch(userOption){
            case 1:
                getAllStudents();
                break;
            case 2:
                createStudent();
                break;
            case 3: 
                getStudentByRa();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                return 0;
            default:
                cout << "Selecione uma opcao valida.";
                break;

        }
    }


    return 0;
}