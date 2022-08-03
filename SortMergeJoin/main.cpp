
/*
Satoru Yamamoto
OSU email: yamamsat@oregonstate.edu

********************************/

// ask: how many buffers do we need for join process? I use 10 buffers for each relation, and 2 buffers for results.

#include <bits/stdc++.h>
#include "record_class.h"

using namespace std;

//defines how many blocks are available in the Main Memory
#define buffer_size 22

Records buffers[buffer_size]; //use this class object of size 22 as your main memory



void Store_in_File(int temp_filenum, int input_size, int file_type){
    string out_file;
    if(file_type == 1){
        out_file = "Dept" + to_string(temp_filenum);
    }else if(file_type == 2){
        out_file = "Emp" + to_string(temp_filenum);
    }


    // for write
    ofstream out;
    out.open(out_file);
    if(file_type == 1){
        for(int i = 0; i  < input_size; ++i){

            string converted_salary;
            converted_salary = to_string(buffers[i].dept_record.budget);
            // delete following zeros after floating point
            if(converted_salary.find_last_not_of('0') == converted_salary.find('.')){
                converted_salary.erase(converted_salary.find('.'),converted_salary.size()-1);
            }else{
                converted_salary.erase(converted_salary.find_last_not_of('0')+1,converted_salary.size()-1);
            }

            out << buffers[i].dept_record.did <<',' << buffers[i].dept_record.dname <<',' << converted_salary <<',' << buffers[i].dept_record.managerid ;
            if(i!=input_size-1){
                out<<'\n';
            }
        }

    }else if(file_type == 2){
        for(int i = 0; i  < input_size; ++i){

            string converted_salary;
            converted_salary = to_string(buffers[i].emp_record.salary);
            // delete following zeros after floating point
            if(converted_salary.find_last_not_of('0') == converted_salary.find('.')){
                converted_salary.erase(converted_salary.find('.'),converted_salary.size()-1);
            }else{
                converted_salary.erase(converted_salary.find_last_not_of('0')+1,converted_salary.size()-1);
            }

            out << buffers[i].emp_record.eid <<',' << buffers[i].emp_record.ename <<',' << buffers[i].emp_record.age <<',' << converted_salary;
            if(i!=input_size-1){
                out<<'\n';
            }
        }
    }

    out.close();

    return;
}

//Sorting the buffers in main_memory and storing the sorted records into a temporary file (runs)
int Sort_Buffer(fstream &input_file, int file_type){
    //Remember: You can use only [AT MOST] 22 blocks for sorting the records / tuples and create the runs

    bool flag = true;
    int cur_size = 0;
    int cur_filenum = 1;

    while (flag) {
        // FOR BLOCK IN RELATION EMP
        Records single_EmpRecord;
        // grabs a block
        if(file_type == 1){
            single_EmpRecord = Grab_Dept_Record(input_file);
        }else if(file_type == 2){
            single_EmpRecord = Grab_Emp_Record(input_file);
        }

        // checks if filestream is empty

        if (single_EmpRecord.no_values == -1) {
            flag = false;
            Store_in_File(cur_filenum, cur_size,file_type);
            cur_filenum++;
            //Print_Buffers(cur_size); // The main_memory is not filled up but there are some leftover data that needs to be sorted.
        }
        // We store information into buffer
        if(cur_size + 1 <= buffer_size){
            //Memory is not full store current record into buffers.
            //sort it here

            //if buffer has no input
            if(file_type == 1 ){
                if(cur_size == 0){
                buffers[cur_size] = single_EmpRecord;
                }else{
                    for(int i = cur_size; i > 0; --i){
                        if(single_EmpRecord.dept_record.managerid >= buffers[i-1].dept_record.managerid){
                            buffers[i] = single_EmpRecord;
                            break;
                        }else if(single_EmpRecord.dept_record.managerid < buffers[i-1].dept_record.managerid){
                            buffers[i] = buffers[i-1];
                            if(i-1 == 0){
                                buffers[i-1] = single_EmpRecord;
                            }
                        }
                    }
                }
            }else if(file_type == 2){
                if(cur_size == 0){
                buffers[cur_size] = single_EmpRecord;
                }else{
                    for(int i = cur_size; i > 0; --i){
                        if(single_EmpRecord.emp_record.eid >= buffers[i-1].emp_record.eid){
                            buffers[i] = single_EmpRecord;
                            break;
                        }else if(single_EmpRecord.emp_record.eid < buffers[i-1].emp_record.eid){
                            buffers[i] = buffers[i-1];
                            if(i-1 == 0){
                                buffers[i-1] = single_EmpRecord;
                            }
                        }
                    }
                }
            }


            //buffers[cur_size] = single_EmpRecord ;
            cur_size += 1;
        }
        else{
            //memory is full now. Sort the blocks in Main Memory and Store it in a temporary file (runs)
            //cout << "Main Memory is full. Time to sort and store sorted blocks in a temporary file" << endl;
            Store_in_File(cur_filenum, cur_size,file_type);

            //Print_Buffers(buffer_size);

            //After sorting start again. Clearing memory and putting the current one into main memory.
            cur_size = 0;
            buffers[cur_size] = single_EmpRecord;
            cur_size += 1;
            cur_filenum++;
        }

    }

    return cur_filenum-1;
}

//Prints out the attributes from empRecord and deptRecord when a join condition is met
//and puts it in file Join.csv
void PrintJoin(Records &dept, Records &emp, fstream &joinout) {
        string converted_budget;
        converted_budget = to_string(dept.dept_record.budget);
        // delete following zeros after floating point
        if(converted_budget.find_last_not_of('0') == converted_budget.find('.')){
            converted_budget.erase(converted_budget.find('.'),converted_budget.size()-1);
        }else{
            converted_budget.erase(converted_budget.find_last_not_of('0')+1,converted_budget.size()-1);
        }

        string converted_salary;
        converted_salary = to_string(emp.emp_record.salary);
        // delete following zeros after floating point
        if(converted_salary.find_last_not_of('0') == converted_salary.find('.')){
            converted_salary.erase(converted_salary.find('.'),converted_salary.size()-1);
        }else{
            converted_salary.erase(converted_salary.find_last_not_of('0')+1,converted_salary.size()-1);
        }

    cout << dept.dept_record.did << ',' << dept.dept_record.dname << ',' << converted_budget << ',' << dept.dept_record.managerid << ',';
    cout << emp.emp_record.eid << ',' << emp.emp_record.ename << ',' << emp.emp_record.age << ',' << converted_salary << endl;

    joinout << dept.dept_record.did << ',' << dept.dept_record.dname << ',' << converted_budget << ',' << dept.dept_record.managerid << ',';
    joinout << emp.emp_record.eid << ',' << emp.emp_record.ename << ',' << emp.emp_record.age << ',' << converted_salary << endl;

    return;
}

//Use main memory to Merge and Join tuples
//which are already sorted in 'runs' of the relations Dept and Emp

void Merge_Join_Runs( int num_deptFile, int num_empFile, fstream &joinout){

    if(num_deptFile == 0 or num_empFile == 0 or (num_deptFile+num_empFile) > buffer_size-2){
        cout << "don't have enough relation table to join" << endl;
        cout << "or" << endl;
        cout << "too many touples to join" << endl;
    }else{

        int num_block = num_deptFile+num_empFile;
        fstream input[num_block];

        // open temp files for dept
        for(int i = 0; i < num_deptFile;++i){
            string file_name = "Dept"+to_string(i+1);
            //cout << "open: " << file_name << endl;
            input[i].open(file_name,ios::in);
        }

        // open temp files for emp
        for(int i = num_deptFile; i < num_block;++i){
            string file_name = "Emp"+to_string(i+1-num_deptFile);
            //cout << "open: " << file_name << endl;
            input[i].open(file_name,ios::in);
        }

        bool has_record[num_block];

        // talke first touple from temp files for dept
        for(int i = 0; i < num_deptFile;++i){
            buffers[i] = Grab_Dept_Record(input[i]);
            has_record[i] = true;
        }

        // talke first touple from temp files for emp
        for(int i = num_deptFile; i < num_block;++i){
            buffers[i] = Grab_Emp_Record(input[i]);
            has_record[i] = true;
        }


        // ---------------------- find min values for dept and emp -------------------------

        bool isEmpty = false;
        int min_dept_index, min_emp_index;
        int count_dept, count_emp;

        while(!isEmpty){
            min_dept_index = -1;
            count_dept = 0;
            // take min dept
            for(int i = 0; i < num_deptFile;++i){

                if(has_record[i] == true){
                    count_dept++;
                    if(min_dept_index == -1){
                        min_dept_index = i;
                    }else{
                        if(buffers[i].dept_record.managerid < buffers[min_dept_index].dept_record.managerid){
                            min_dept_index = i;
                        }
                    }
                }
            }

            min_emp_index = -1;
            count_emp = 0;
            // take min emp
            for(int i = num_deptFile; i < num_block;++i){

                if(has_record[i] == true){
                    count_emp++;
                    if(min_emp_index == -1){
                        min_emp_index = i;
                    }else{
                        if(buffers[i].emp_record.eid < buffers[min_emp_index].emp_record.eid){
                            min_emp_index = i;
                        }
                    }
                }
            }

            // -------------------
            // compare ->
            // A > B: next B
            // A = B: join and next A
            // A < B: next A
            // --------------------

            if(buffers[min_dept_index].dept_record.managerid > buffers[min_emp_index].emp_record.eid){
                buffers[min_emp_index] = Grab_Emp_Record(input[min_emp_index]);

                if(buffers[min_emp_index].no_values == -1){
                    has_record[min_emp_index] = false;
                    --count_emp;
                }
            }else if(buffers[min_dept_index].dept_record.managerid == buffers[min_emp_index].emp_record.eid){

                PrintJoin(buffers[min_dept_index], buffers[min_emp_index], joinout);
                buffers[min_dept_index] = Grab_Dept_Record(input[min_dept_index]);

                if(buffers[min_dept_index].no_values == -1){
                    has_record[min_dept_index] = false;
                    --count_dept;
                }

            }else if(buffers[min_dept_index].dept_record.managerid < buffers[min_emp_index].emp_record.eid){
                buffers[min_dept_index] = Grab_Dept_Record(input[min_dept_index]);

                if(buffers[min_dept_index].no_values == -1){
                    has_record[min_dept_index] = false;
                    --count_dept;
                }
            }

            if(count_dept == 0 or count_emp == 0){
                isEmpty = true;
            }


        }



    }

    //and store the Joined new tuples using PrintJoin()
    return;
}

int main() {

    //Open file streams to read and write
    //Opening out two relations Emp.csv and Dept.csv which we want to join
    fstream deptin;
    fstream empin;
    deptin.open("Dept.csv", ios::in);
    empin.open("Emp.csv", ios::in);

    int num_empFile, num_deptFile;

    //Creating the Join.csv file where we will store our joined results
    fstream joinout;
    joinout.open("Join.csv", ios::out | ios::trunc);

    //1. Create runs for Dept and Emp which are sorted using Sort_Buffer()
    num_deptFile = Sort_Buffer(deptin,1);
    deptin.close();
    num_empFile = Sort_Buffer(empin,2);
    empin.close();
    //cout << num_deptFile << " " << num_empFile << endl;

    // ----------------
    // temp files of Dept and Emp.
    // #: num_deptFile, and num_empFile
    // take smallest manager id from temp files of Dept and smallest eid from temp files of Emp.
    // --------------------

    //2. Use Merge_Join_Runs() to Join the runs of Dept and Emp relations
    Merge_Join_Runs(num_deptFile,num_empFile,joinout);

    //Please delete the temporary files (runs) after you've joined both Emp.csv and Dept.csv

    for(int i = 0; i < num_deptFile; ++i){
        char file_name[16];
        snprintf(file_name,sizeof(file_name),"Dept%d",i+1);
        if(std::remove(file_name) != 0){
            cout << file_name << " is not deleted." << endl;
        }else{
            cout << file_name << " is deleted." << endl;
        }
    }

    for(int i = 0; i < num_empFile; ++i){
        char file_name[16];
        snprintf(file_name,sizeof(file_name),"Emp%d",i+1);
        if(std::remove(file_name) != 0){
            cout << file_name << " is not deleted." << endl;
        }else{
            cout << file_name << " is deleted." << endl;
        }
    }


    return 0;
}
