
/*
Satoru Yamamoto
OSU email: yamamsat@oregonstate.edu

*********************************************/
/*
    Comments from Satoru: Because we onlu have 22 buffer, we cannot have temp space to sort buffer information.
    Therefore, I sort buffer when we stpre information from original file. To do implement sorting feature without extra temp memory in phase one,
    I did not follow skelton code of Professor.

    To test this code, we used osu engineering sever that is running on Linux. I went to office hours checked that I was not able to access hadoop-master.eecs.oregonstate.edu server with Professor.

*/

#include <bits/stdc++.h>
using namespace std;

//defines how many buffers are available in the Main Memory
#define buffer_size 22

struct EmpRecord {
    int eid;
    string ename;
    int age;
    double salary;
};
EmpRecord buffers[buffer_size]; // this structure contains 22 buffers; available as your main memory.

// Grab a single block from the Emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpRecords and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpRecord Grab_Emp_Record(ifstream &empin) {
    string line, word;
    EmpRecord  emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);

        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

//Printing whatever is stored in the buffers of Main Memory
//Can come in handy to see if you've sorted the records in your main memory properly.
void Print_Buffers(int cur_size) {
    for (int i = 0; i < cur_size; i++)
    {
        cout << i << " " << buffers[i].eid << " " << buffers[i].ename << " " << buffers[i].age << " " << buffers[i].salary << endl;
    }
}


//Sorting the buffers in main_memory based on 'eid' and storing the sorted records into a temporary file
//You can change return type and arguments as you want.

// I actually sorted before this function, and I talked about it with TA
// it stores each 22 sorted records into temp files
void Sort_in_Main_Memory(int temp_filenum, int input_size){
    string out_file = to_string(temp_filenum);
    // for write
    ofstream out;
    out.open(out_file);

    for(int i = 0; i  < input_size; ++i){

        string converted_salary;
        converted_salary = to_string(buffers[i].salary);
        // delete following zeros after floating point
        if(converted_salary.find_last_not_of('0') == converted_salary.find('.')){
            converted_salary.erase(converted_salary.find('.'),converted_salary.size()-1);
        }else{
            converted_salary.erase(converted_salary.find_last_not_of('0')+1,converted_salary.size()-1);
        }

        out << buffers[i].eid <<',' << buffers[i].ename <<',' << buffers[i].age <<',' << converted_salary;
        if(i!=input_size-1){
            out<<'\n';
        }
    }
    out.close();

    return;
}

//You can use this function to merge your M-1 runs using the buffers in main memory and storing them in sorted file 'EmpSorted.csv'(Final Output File)
//You can change return type and arguments as you want.
// This function will receive the total number of temp files and next file number to merge. Calculate the number of blocks to use, and merge records by taking
// min of records from each temp files top record.
bool Merge_Runs_in_Main_Memory(int &temp_filenum, int &next_merge){
    int last_file = temp_filenum - 1;
    int num_block;
    bool is_last = false;
    string out_file;
    // if there are more than 21 temp files
    if(next_merge+buffer_size-2 <= last_file){
        // M-1: one memory for merge
        num_block = buffer_size-1;
        out_file = to_string(temp_filenum);
    }else{
        // This is final marge
        num_block = last_file-next_merge+1;
        is_last = true;
        out_file = "EmpSorted.csv";
    }
    ofstream out;
    out.open(out_file);

    ifstream input[num_block];

    for(int i = 0; i < num_block;++i){
        input[i].open(to_string(i+next_merge));
    }
    // take first elements from temp files: There is at least one record in each temp file
    bool has_record[num_block];
    for(int i = 0; i < num_block;++i){
        buffers[i] = Grab_Emp_Record(input[i]);
        has_record[i] = true;
    }

    // find min value of top of block and store it into out file
    bool isEmpty = false;
    int min_index;
    int count;
    while(!isEmpty){
        min_index = -1;
        count = 0;
        // pick lowest eid record
        for(int i = 0; i < num_block;++i){

            if(has_record[i] == true){
                count++;
                if(min_index == -1){
                   min_index = i;
                }else{
                    if(buffers[i].eid < buffers[min_index].eid){
                        min_index = i;
                    }
                }
            }


        }
        string converted_salary;
        converted_salary = to_string(buffers[min_index].salary);

        // delete following zeros after floating point
        if(converted_salary.find_last_not_of('0') == converted_salary.find('.')){
            converted_salary.erase(converted_salary.find('.'),converted_salary.size()-1);
        }else{
            converted_salary.erase(converted_salary.find_last_not_of('0')+1,converted_salary.size()-1);
        }

        out << buffers[min_index].eid <<',' << buffers[min_index].ename <<',' << buffers[min_index].age <<',' << converted_salary;
        buffers[min_index] = Grab_Emp_Record(input[min_index]);
        if(buffers[min_index].eid == -1){
            has_record[min_index] = false;
            --count;
        }
        if(count == 0){
            isEmpty = true;
        }else{
            out << '\n';
        }

    }

    //Print_Buffers(num_block);

    //close used files
    out.close();
    for(int i = 0; i < num_block;++i){
        input[i].close();
    }

    for(int i = 0; i < num_block;++i){
        input[i].open(to_string(i+next_merge));
    }

    // update counts
    //merge num_blocks of block
    next_merge += num_block;
    //create new temp file
    temp_filenum++;
    //cout << num_block << " "<< next_merge << " "<<  temp_filenum << endl;
    return is_last;
}

int main() {
    // open file streams to read and write
    ifstream input_file;
    input_file.open("Emp.csv", ios::in);
    cout << fixed;

    // flags check when relations are done being read
    bool flag = true;
    int cur_size = 0;
    int cur_filenum = 1;
    /*First Pass: The following loop will read each block put it into main_memory
        and sort them then will put them into a temporary file for 2nd pass */
    while (flag) {
        // FOR BLOCK IN RELATION EMP

        // grabs a block
        EmpRecord  single_EmpRecord  = Grab_Emp_Record(input_file);
        // checks if filestream is empty
        if (single_EmpRecord.eid == -1) {
            flag = false;
            Sort_in_Main_Memory(cur_filenum, cur_size);
            cur_filenum++;
            //Print_Buffers(cur_size); // The main_memory is not filled up but there are some leftover data that needs to be sorted.
        }
        // We store information into buffer
        if(cur_size + 1 <= buffer_size){
            //Memory is not full store current record into buffers.
            //sort it here

            //if buffer has no input
            if(cur_size == 0){
                buffers[cur_size] = single_EmpRecord;
            }else{
                for(int i = cur_size; i > 0; --i){
                    if(single_EmpRecord.eid >= buffers[i-1].eid){
                        buffers[i] = single_EmpRecord;
                        break;
                    }else if(single_EmpRecord.eid < buffers[i-1].eid){
                        buffers[i] = buffers[i-1];
                        if(i-1 == 0){
                            buffers[i-1] = single_EmpRecord;
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
            Sort_in_Main_Memory(cur_filenum,cur_size);

            //Print_Buffers(buffer_size);

            //After sorting start again. Clearing memory and putting the current one into main memory.
            cur_size = 0;
            buffers[cur_size] = single_EmpRecord;
            cur_size += 1;
            cur_filenum++;
        }

    }
    input_file.close();

    // cur_filenum holds the number of created file. # of file = cur_filenum-1
    // name of file is [1 to cur_filenum-1]

    /* Implement 2nd Pass: Read the temporary sorted files and utilize main_memory to store sorted runs into the EmpSorted.csv*/

    //Uncomment when you are ready to store the sorted relation
    //fstream sorted_file;
    //sorted_file.open("EmpSorted.csv", ios::out | ios::app);

    //Pseudocode
    bool flag_sorting_done = false;
    int test_num = 23;
    int next_merge = 1;
    while(!flag_sorting_done){
        flag_sorting_done = Merge_Runs_in_Main_Memory(cur_filenum, next_merge);
        //flag_sorting_done = Merge_Runs_in_Main_Memory(test_num, next_merge);

    }

    //You can delete the temporary sorted files (runs) after you're done if you want. It's okay if you don't.

    for(int i = 1; i < next_merge; ++i){
        char file_name[3+sizeof(char)];
        snprintf(file_name,sizeof(file_name),"%d",i);
        if(std::remove(file_name) != 0){
            cout << file_name << " is not deleted." << endl;
        }else{
            cout << file_name << " is deleted." << endl;
        }

    }

    return 0;
}
