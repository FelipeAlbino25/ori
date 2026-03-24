#include <iostream>
#include <fstream>
#include <limits> 
#include <cstring>
using namespace std;

struct Student {
    int ra;        
    char name[30];
    double grade;
};

Student getStudentInput() {
    Student s;
    memset(&s, 0, sizeof(Student));

    cout << "Nome do Estudante: ";
    cin.getline(s.name, 30);

    cout << "Nota: ";
    while (!(cin >> s.grade)) {
        cout << "Erro! Digite um numero para a nota: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return s;
}

void getAllStudents() {
    fstream file("students.dat", ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "\nNenhuma entrada foi adicionada ainda.\n";
        return;
    }

    file.seekg(0, ios::beg);
    Student tmp;
    int count = 0;

    while (file.read(reinterpret_cast<char*>(&tmp), sizeof(Student))) {
        cout << "\nRA: " << tmp.ra << " | Nome: " << tmp.name << " | Nota: " << tmp.grade;
        count++;
    }
    cout << endl;
    
    if (count == 0) cout << "\nO banco de dados esta vazio.\n";
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
    int pos = file.tellp(); // Pega a posição atual (tamanho do arquivo em bytes)
    
    newStudent.ra = pos / sizeof(Student); 

    file.write(reinterpret_cast<char*>(&newStudent), sizeof(Student));
    file.close();
    
    cout << "\nEstudante '" << newStudent.name << "' cadastrado com sucesso! O RA gerado foi: " << newStudent.ra << endl;
}

void getStudentByRa() {
    int raProcurado;
    cout << "\nDigite o RA do estudante que deseja buscar: ";
    cin >> raProcurado;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream file("students.dat", ios::in | ios::binary);
    if (!file) return;

    file.seekg(0, ios::end);
    int fileSize = file.tellg();

    if (raProcurado < 0 || (raProcurado * sizeof(Student)) >= fileSize) {
        cout << "Estudante com RA " << raProcurado << " nao encontrado (fora dos limites)." << endl;
        return;
    }

    file.seekg(raProcurado * sizeof(Student), ios::beg);
    
    Student tmp;
    file.read(reinterpret_cast<char*>(&tmp), sizeof(Student));
    
    cout << "\n--- ESTUDANTE ENCONTRADO ---"
         << "\nRA: " << tmp.ra 
         << "\nNome: " << tmp.name 
         << "\nNota: " << tmp.grade << "\n----------------------------\n";
         
    file.close();
}

void updateStudent() {
    int raProcurado;
    cout << "\nDigite o RA do estudante que deseja atualizar: ";
    cin >> raProcurado;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream file("students.dat", ios::in | ios::out | ios::binary);
    if (!file) return;

    file.seekg(0, ios::end);
    int fileSize = file.tellg();

    if (raProcurado < 0 || (raProcurado * sizeof(Student)) >= fileSize) {
        cout << "Estudante com RA " << raProcurado << " nao encontrado." << endl;
        return;
    }

    cout << "Estudante encontrado! Digite os novos dados:\n";
    Student updated = getStudentInput();
    
    updated.ra = raProcurado; 

    // ACESSO DIRETO PARA ESCRITA:
    file.seekp(raProcurado * sizeof(Student), ios::beg);
    file.write(reinterpret_cast<char*>(&updated), sizeof(Student));
    
    cout << "Estudante atualizado com sucesso!" << endl;
    file.close();
}

int main() {
    int userOption = -1;
    while (userOption != 5) {
        cout << "\nEscolha uma opcao:\n1.GET all students\n2.CREATE a student\n3.GET student by RA\n4.UPDATE a student\n5.QUIT\nOPCAO: ";
        if (!(cin >> userOption)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (userOption) {
            case 1: getAllStudents(); break;
            case 2: createStudent(); break;
            case 3: getStudentByRa(); break;
            case 4: updateStudent(); break;
            case 5: cout << "Encerrando...\n"; return 0;
            default: cout << "Selecione uma opcao valida.\n"; break;
        }
    }
    return 0;
}