#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <fstream>
using namespace std;
// if windows: record_size+=4;
// if linux: recordPsize+=3;
// used website: https://www.mathcs.emory.edu/~cheung/Courses/554/Syllabus/3-index/linear-hashing.html
// especially for increment n

// message1: by using page_index* (page size), it does not work because of special symbol which takes 2 bits
// I sent an email if we need to add special symbol space to used space, but I could not receive any response.
// Therefore, I used tell function to get start address for each block.
// Because of this, we no longer need to count nextFreePage, but I count during execution.

// message2: I was using \0 for my special symbol between each block. but it caused an error. In windows, \0 is 2 bites, and it is 1 bit in Linux.
// because of weird behavior in Linux, I am going to change the method to find next wrinting potision in each block.
// I know Professor suggested to use size to find next writing position, and it works on windows. If I need to show the code that use size to wrinte, I am able to show. it is in my repository.
// Updated message: size function on Linux

// From message2, I decided to use tellp() function to find next writing position.

// Updated information, the error came from different behavior of file library. Now I do not need to use tellp() function to find next writing position.

// Start position, size, overflow information
// empty if size = 0
// page_index != hash value.

const int FIXED_OVERFLOW_AD_SIZE = 32;

struct Block{
    long size = 0;
    // start position = page_index* (page size)
    // because we are using \n for special symbol between each block, we want to store
    // specific address (see the message 1)
    streampos page_start;
    // pointer to overflow block
    struct Block *of_block = nullptr;
};

// id,name,bio,manager_id| id,name,bio,manager_id|#overflow_address(null)

class Record {
    public:
        int id, manager_id;
        std::string bio, name;
        long record_size = 0;
        // receive vector of string and store them
        Record(vector<std::string> fields) {
            /*
            cout << "stoi:" << fields[0] <<"|"<<endl;
            cout << "name: " << fields[1] <<endl;
            cout << "bio: " << fields[2] <<endl;
            cout << "m_id:" << fields[3] <<"|"<<endl;
            */

            id = stoi(fields[0]);
            name = fields[1];
            bio = fields[2];
            manager_id = stoi(fields[3]);

            record_size+= to_string(id).size();
            record_size+= name.size();
            record_size+= bio.size();
            record_size+= to_string(manager_id).size();
            /*
            for(int i = 0; i < 4; ++i){
                record_size += fields[i].size();
            }*/
            // for 3 comma + special symbol
            record_size+=4;

        }

        void print() {
            cout << "ID: " << id << "\n";
            cout << "NAME: " << name << "\n";
            cout << "BIO: " << bio << "\n";
            cout << "MANAGER_ID: " << manager_id << "\n";
        }

        // for debug
        void print_bits(int id_hash, int i){
            bitset<32> bit(id_hash);
            bitset<32> ibit((id_hash & ((1 << i) - 1)));

            cout << "id_hash: " << id_hash << "\nbits: " << bit << "\nith:  " << ibit << endl;
            cout << "flip: " << ibit.flip(i-1)<< endl;
        }
};


class LinearHashIndex {

private:
    const int PAGE_SIZE = 4096;
    //vector<int>pageDirectry;
    vector<Block>pageDirectry;    // Where pageDirectory[h(id)] gives page index of block
                                  // can scan to pages using index*PAGE_SIZE as offset (using seek function)
    int numBlocks; // n
    int i;
    int numRecords; // Records in index
    int nextFreePage; // Next page to write to (number of pages)
    //int block_size; // (ask) number of page in block
    string fName;   // output file name


    int total_block_size(){
        int total = 0;

        for(int j = 0; j < numBlocks; ++j){
            total+=pageDirectry[j].size;
            Block *block = &pageDirectry[j];
            while(block->of_block != nullptr){
                block = block->of_block;
                total+=block->size;
            }
        }
        return total;
    }

   void insertion(Record record, string o_file){


        int id_mod = record.id % (int)pow(2,16);      // mod 2^%16
        int hash_id = (id_mod & ((1 << i) - 1));    // ith hash value in int
        bitset<32> hash_id_bit(hash_id);             // hash value in binary

        //cout << "id: " << record.id << "\nid_hash: " << id_mod << "\nbits: " << hash_id << "\nith:  " << hash_id_bit << endl;
        //record.print_bits(id_mod,i);

        // case when we need to flip significat bit
        // id starts from 0.
        if(hash_id > numBlocks-1){
            //cout << "************************ flip ***************************" << endl;
            hash_id_bit.flip(i-1);
            hash_id = hash_id_bit.to_ulong();
            //cout << "flip: " << hash_id << endl;
        }

        // Add record to the index in the correct block, creating overflow block if necessary

        // when the hash_id page is empty
        if(pageDirectry[hash_id].size == 0){
            //cout << hash_id << "---------------------------------------------" << endl;
            //create_Block(pageDirectry[hash_id], hash_id);

            ofstream output;
            output.open(o_file, std::ios::in | std::ios::ate);
            //output.seekp(pageDirectry[hash_id].page_index*PAGE_SIZE, ios_base::beg);
            output.seekp(0, ios_base::end);
            // store start pos of block
            pageDirectry[hash_id].page_start = output.tellp();

            //cout << hash_id <<endl;
            //cout << pageDirectry[hash_id].page_start << endl;

            // fill nulls
            // may need PAGE_SIZE-2 for special symbol
            for(int j = 0; j < PAGE_SIZE; ++j){
                output << '\0';
            }
            // endl
            output << '\n';
            output.close();
        }

        // has to be pointer
        Block *block = &pageDirectry[hash_id];

        // if the block has overflow
        while(block->of_block != nullptr){
            block = block->of_block;
        }
        //cout << record.record_size << endl;
        //cout << hash_id <<endl;
        //debug overflow function
        //if(block->size != 0){
        // if block has no overflow and size > page_size
        //if(false){
        if(block->size + record.record_size > PAGE_SIZE - FIXED_OVERFLOW_AD_SIZE ){
            //cout << "*******************************"<< record.id <<" overflow *********************************" <<endl;
            Block *newBlock = new Block;
            ofstream output;
            output.open(o_file, std::ios::in | std::ios::ate);


            //output.seekp(pageDirectry[hash_id].page_index*PAGE_SIZE, ios_base::beg);
            output.seekp(0, ios_base::end);
            // store start pos of block
            newBlock->page_start = output.tellp();
            // fill nulls
            // may need PAGE_SIZE-2 for special symbol
            for(int j = 0; j < PAGE_SIZE; ++j){
                output << '\0';
            }
            // endl
            output << '\n';


            // store overflow block
            block->of_block = newBlock;
            // add address
            output.seekp(block->page_start+block->size, ios_base::beg);
            output << '#' << newBlock->page_start;

            block = newBlock;

            output.close();

        }

        ofstream output;
        output.open(o_file, std::ios::in | std::ios::ate);
        output.seekp(block->page_start+block->size, ios_base::beg);
        output << record.id <<',' << record.name <<','<< record.bio <<',' << record.manager_id << '|';
        //cout << record.id <<',' << record.name <<','<< record.bio <<',' << record.manager_id << '|';
        //update size
        block->size += record.record_size;
        numRecords++;
        output.close();

    }


    // Insert new record into index
    // part 1
    void insertRecord(Record record) {

        // No records written to index yet
        if (numRecords == 0) {
            // Initialize index with first blocks (start with 2)
            numBlocks = 2;
            i = 1;
            // make blocks
            pageDirectry.resize(numBlocks);
        }

        insertion(record, fName);

        // Take neccessary steps if capacity is reached

        //cout << "total size: " <<  total_block_size() << endl;
        int total = total_block_size();
        double occupancy = (double)total/(PAGE_SIZE*numBlocks);

        //cout <<"occupancy: " << occupancy << endl;
        // debug incrememt
        //if(true){
        //if(false){
        //remove("tempIndex");
        // 70%
        if(occupancy > 0.70){
            //increment n and update i = log(n)
            //remove("tempIndex");
            std::ofstream ofs;
            ofs.open("tempIndex", std::ofstream::out | std::ofstream::trunc);
            ofs.close();

            numBlocks++;
            i = ceil(log2(numBlocks));
            //cout << "incrememted: " << numBlocks << endl;

            //pageDirectry.resize(numBlocks);

            // index starts 0
            //int page_index = numBlocks-1;
            //int start_index = page_index^(1 << (i - 1));

            // need to pick up the record hash_id = n-1

            //vector<Block> temp_pageDirectry(pageDirectry);
            pageDirectry.clear();
            // needed to unlink addresses
            /*
            for(int j = 0; j < numBlocks-1; ++j){
                pageDirectry[j].size = 0;
                pageDirectry[j].page_start = NULL;

                Block *temp_block = pageDirectry[j].of_block;
                while(temp_block != nullptr){
                    temp_block->size = 0;
                    temp_block->page_start = NULL;

                    temp_block = temp_block->of_block;
                }


            }*/

            numRecords = 0;
            pageDirectry.resize(numBlocks);

            // read all record in file
            ifstream stored_file;
            bool isEmpty = false;
            stored_file.open(fName);
            //stored_file.seekg(pageDirectry[].page_start,ios_base::beg);

            while(!isEmpty){
                string line, word;
                // grab entire line (re-used from previos assignment)
                if (getline(stored_file, line)) {
                    stringstream s(line);
                    while(getline(s, word,'|')){

                        vector<string> emp;
                        string word2;
                        // not null and not overflow address
                        if(word[0]!='\0' and word[0]!='#'){
                            //cout << word << "end"<< endl;

                            stringstream s2(word);
                            // id
                            getline(s2, word2,',');
                            emp.push_back(word2);
                            // name
                            getline(s2, word2, ',');
                            emp.push_back(word2);
                            // bio
                            getline(s2, word2, ',');
                            emp.push_back(word2);
                            // maneger id
                            getline(s2, word2, ',');
                            emp.push_back(word2);

                            Record rec(emp);
                            //rec.print();
                            //cout << "-------insert temp--------" <<endl;
                            insertion(rec,"tempIndex");
                        }
                    }
                } else {
                    isEmpty = true;
                }
            }
            stored_file.close();
            // after increment n
            remove("EmployeeIndex");
            rename("tempIndex","EmployeeIndex");

        }

    }



public:
    LinearHashIndex(string indexFileName) {
        numBlocks = 0;
        nextFreePage = 0;
        i = 0;
        numRecords = 0;
        //block_size = 0;
        fName = indexFileName;

        // initialize page index for each hash value
        /*
        for(int i = 0; i < MAX_NUM_PAGES; ++i){
            pageDirectry.push_back(-1);
        }
        */
    }

    // Read csv file and add records to the index
    // part 1
    void createFromFile(string csvFName) {
        // open file, read each touple, insertRecord to store it

        ifstream emp_file;
        emp_file.open(csvFName);
        bool isEmpty = false;

        while(!isEmpty){
            vector<string> emp;
            string line, word;

            // grab entire line (re-used from previos assignment)
            if (getline(emp_file, line, '\n')) {
                // turn line into a stream
                stringstream s(line);
                // gets everything in stream up to comma
                getline(s, word,',');
                emp.push_back(word);
                getline(s, word, ',');
                emp.push_back(word);
                getline(s, word, ',');
                emp.push_back(word);
                getline(s, word, ',');
                emp.push_back(word);

                Record record(emp);
                // insert record
                //cout << "-------insert--------" <<endl;
                insertRecord(record);

            } else {
                isEmpty = true;
            }
        }
        //findRecordById(1);
        //cout << "Records:" << numRecords << endl;
        emp_file.close();

    }

    // Given an ID, find the relevant record and print it
    // part 2

    void findRecordById(int id) {

        // got hash index for input id

        int id_mod = id % (int)pow(2,16);      // mod 2^%16
        int hash_id = (id_mod & ((1 << i) - 1));    // ith hash value in int
        bitset<32> hash_id_bit(hash_id);             // hash value in binary


        // id starts from 0.
        if(hash_id > numBlocks-1){
            hash_id_bit.flip(i-1);
            hash_id = hash_id_bit.to_ulong();
            //cout << "flip: " << hash_id << endl;
        }



        ifstream emp_file;
        bool isFind = false;

        emp_file.open("EmployeeIndex");
        //stored_file.seekg(pageDirectry[1].page_start,ios_base::beg);

        Block *block = &pageDirectry[hash_id];

        if(block->size != 0){


            // overflow buffer
            while(block->of_block != nullptr and !isFind){

                emp_file.seekg(block->page_start,ios_base::beg);

                string line, word;

                // take one block
                getline(emp_file, line);
                stringstream s(line);

                // check each record
                while(getline(s, word,'|') and !isFind){
                        vector<string> emp;
                        string word2;
                        // not null and not overflow address
                        if(word[0]!='\0' and word[0]!='#'){
                            //cout << word << "end"<< endl;

                            stringstream s2(word);
                            // id
                            getline(s2, word2,',');
                            emp.push_back(word2);
                            // name
                            getline(s2, word2, ',');
                            emp.push_back(word2);
                            // bio
                            getline(s2, word2, ',');
                            emp.push_back(word2);
                            // maneger id
                            getline(s2, word2, ',');
                            emp.push_back(word2);

                            Record rec(emp);
                            //rec.print();

                            if(rec.id == id){
                                rec.print();
                                isFind = true;
                            }

                        }
                }
                // next overflow buffer
                block = block->of_block;
            }

            if(!isFind){
                emp_file.seekg(block->page_start,ios_base::beg);

                string line, word;

                // take one block
                getline(emp_file, line);
                stringstream s(line);

                // check each record
                while(getline(s, word,'|') and !isFind){
                        vector<string> emp;
                        string word2;
                        // not null and not overflow address
                        if(word[0]!='\0' and word[0]!='#'){
                            //cout << word << "end"<< endl;

                            stringstream s2(word);
                            // id
                            getline(s2, word2,',');
                            emp.push_back(word2);
                            // name
                            getline(s2, word2, ',');
                            emp.push_back(word2);
                            // bio
                            getline(s2, word2, ',');
                            emp.push_back(word2);
                            // maneger id
                            getline(s2, word2, ',');
                            emp.push_back(word2);

                            Record rec(emp);
                            //rec.print();

                            if(rec.id == id){
                                rec.print();
                                isFind = true;
                            }

                        }
                }
            }

            if(!isFind){
                cout << "No record for id: " << id << endl;
            }

        }

    }

};
