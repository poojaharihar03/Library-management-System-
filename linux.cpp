#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>

using namespace std;
/*------------------------- class student ----------------------*/
class student
{
    string buffer, name, susn, sec, year;
    string didlist[100];
    int count, addlist[100];

public:
    void createindex();
    string extractdid(string);
    void sortindex();
    void insert();
    void display_student();
    void search(string);
    int searchindex(string);
    void remove(string);
    /* index list */
    void index_create();
    string did_extract(string);
    void index_sort();
    void return_book();
} s;
/*------------------------- class book ------------------------*/
class book
{
public:
    string bname, bauthor, pname;
    string blist[100];
    string pidlist[100];
    int count, addlist[100];
    void createindex();
    void remove(string);
    void insert();
    int searchindex(string);
    void searchindex2(string);
    void search(string);
    void sortindex();
    void display_b_record();
    void print();
    string extractpid(string);

    /* index list */
    void index_create();
    string pid_extract(string);
    void index_sort();
} b;
/*------------------------- class librarian ------------------------*/
class librarian
{
public:
    string lid, lage, laddr, lname;
    string nidlist[100];
    int count, addlist[100];
    void createindex();
    void insert();
    void display_n_record();
    void print();
    void remove(string);
    int searchindex(string);
    void search(string);
    void sortindex();
    string extractnid(string);
    void addstudent();
    /* index list */
    void index_create();
    string nid_extract(string);
    void index_sort();
    void pending_books();
} l;

/* -------- index student --------*/
void student::index_create()
{
    fstream file;
    int pos;

    string susn;
    count = -1;
    file.open("student.txt", ios::in);
    while (!file.eof())
    {
        pos = file.tellg();
        buffer.erase();
        getline(file, buffer);
        if (buffer[0] != '*')
        {
            if (buffer.empty())
                break;
            susn = did_extract(buffer);
            didlist[++count] = susn;
            addlist[count] = pos;
        }
    }
    file.close();
    index_sort();
    buffer.erase();
}
string student::did_extract(string buffer)
{
    string susn;
    int i = 0;
    susn.erase();
    while (buffer[i] != '|')
        susn += buffer[i++];
    return susn;
}
void student::index_sort()
{
    int i, j, tempadd;
    string tempid;
    for (i = 0; i <= count; i++)
    {
        for (j = i + 1; j <= count; j++)
        {
            if (didlist[i] > didlist[j])

            {
                tempid = didlist[i];
                didlist[i] = didlist[j];
                didlist[j] = tempid;
                tempadd = addlist[i];

                addlist[i] = addlist[j];
                addlist[j] = tempadd;
            }
        }
    }

    cout << "\n-------INDEXLIST--------\n";
    cout << "\n ID\tINDEX";
    for (i = 0; i <= count; i++)
        cout << "\n"
             << didlist[i] << "\t" << addlist[i] << endl;
}

void student::return_book()
{
    // Ask for SUSN, name, and book name
    string susn, name, bookName;
    cout << "Enter SUSN: ";
    cin >> susn;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Book Name: ";
    getline(cin, bookName);

    // Open the "bookissue.txt" file
    ifstream inFile("bookissue.txt");
    if (!inFile)
    {
        cout << "Failed to open the file." << endl;
        return;
    }

    // Create a temporary vector to store the remaining records
    vector<string> records;

    string line;
    while (getline(inFile, line))
    {
        // Split the line into tokens using the delimiter '|'
        stringstream ss(line);
        string token;
        string tokens[4];
        int i = 0;

        while (getline(ss, token, '|'))
        {
            tokens[i++] = token;
        }

        // Check if the record matches the input
        if (tokens[0] == susn && tokens[1] == name && tokens[2] == bookName)
        {
            // Skip the record as it will be deleted
            continue;
        }

        // Add the record to the temporary vector
        records.push_back(line);
    }

    inFile.close();

    // Open the file in write mode to overwrite the existing data
    ofstream outFile("bookissue.txt");
    if (!outFile)
    {
        cout << "Failed to open the file." << endl;
        return;
    }

    // Write the remaining records to the file
    for (const string &record : records)
    {
        outFile << record << endl;
    }

    outFile.close();

    cout << "Book returned successfully." << endl;
}

/*--------------index librarian ----------------*/

void librarian::index_create()
{
    fstream file;
    int pos;
    string buffer, lid;
    count = -1;
    file.open("librarian.txt", ios::in);
    while (!file.eof())
    {
        pos = file.tellg();
        buffer.erase();
        getline(file, buffer);
        if (buffer[0] != '*')
        {
            if (buffer.empty())
                break;
            lid = nid_extract(buffer);
            nidlist[++count] = lid;
            addlist[count] = pos;
        }
    }
    file.close();
    index_sort();
    buffer.erase();
}
string librarian::nid_extract(string buffer)
{
    string lid;
    int i = 0;
    lid.erase();
    while (buffer[i] != '|')
        lid += buffer[i++];
    return lid;
}
void librarian::index_sort()
{
    int i, j, tempadd;
    string tempid;
    for (i = 0; i <= count; i++)
    {
        for (j = i + 1; j <= count; j++)
        {
            if (nidlist[i] > nidlist[j])

            {
                tempid = nidlist[i];
                nidlist[i] = nidlist[j];
                nidlist[j] = tempid;
                tempadd = addlist[i];

                addlist[i] = addlist[j];
                addlist[j] = tempadd;
            }
        }
    }

    cout << "\n-------INDEXLIST--------\n";
    cout << "\n ID\tINDEX";

    for (i = 0; i <= count; i++)
        cout << "\n"
             << nidlist[i] << "\t" << addlist[i] << endl
             << endl;
}
void librarian::pending_books()
{
    // Get the current date
    time_t now = time(0);
    struct tm *currentDate = localtime(&now);

    // Open the "bookissue.txt" file
    ifstream file("bookissue.txt");
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            // Split the line into tokens using the delimiter '|'
            stringstream ss(line);
            string token;
            string tokens[4];
            int i = 0;

            while (getline(ss, token, '|'))
            {
                tokens[i++] = token;
            }

            // Extract the date from the tokens
            string dateStr = tokens[3];
            int day, month, year;
            sscanf(dateStr.c_str(), "%d-%d-%d", &day, &month, &year);

            // Check if the issue date is 3 months older than the current date
            // Note: This comparison assumes that the format of the date in the file is always "dd-mm-yyyy"
            struct tm issueDate = {0};
            issueDate.tm_year = year - 1900;
            issueDate.tm_mon = month - 1;
            issueDate.tm_mday = day;

            time_t issueTime = mktime(&issueDate);
            double diffDays = difftime(now, issueTime) / (60 * 60 * 24);

            if (diffDays >= 90)
            {
                // Print the book details as pending
                cout << "Pending Book:\n";
                cout << "Book ID: " << tokens[0] << "\n";
                cout << "Borrower Name: " << tokens[1] << "\n";
                cout << "Book Title: " << tokens[2] << "\n";
                cout << "Issue Date: " << dateStr << "\n";
                cout << endl;
            }
        }
        file.close();
    }
    else
    {
        cout << "Failed to open the file." << endl;
    }
}

/*-----------book index -----------------*/
void book::index_create()
{
    fstream file;
    int pos;
    string buffer;
    count = -1;
    file.open("book.txt", ios::in);
    while (!file.eof())
    {
        pos = file.tellg();
        buffer.erase();
        getline(file, buffer);
        if (buffer[0] != '*')
        {
            if (buffer.empty())
                break;
            bname = pid_extract(buffer);
            pidlist[++count] = bname;
            addlist[count] = pos;
        }
    }
    file.close();
    index_sort();
    buffer.erase();
}

string book::pid_extract(string buffer)
{
    string bname;
    int i = 0;
    bname.erase();
    while (buffer[i] != '|')
        bname += buffer[i++];
    return bname;
}

void book::index_sort()
{
    int i, j, tempadd;
    string tempusn;
    for (i = 0; i <= count; i++)
    {
        for (j = i + 1; j <= count; j++)
        {
            if (pidlist[i] > pidlist[j])
            {
                tempusn = pidlist[i];
                pidlist[i] = pidlist[j];
                pidlist[j] = tempusn;
                tempadd = addlist[i];
                addlist[i] = addlist[j];
                addlist[j] = tempadd;
            }
        }
    }
    cout << "\n------INDEXLIST-------\n";
    cout << "\nBOOK ID\tINDEX";
    for (i = 0; i <= count; i++)
        cout << "\n"
             << pidlist[i] << "\t" << addlist[i] << endl;
}

/* ---------------student starts from here----------------- */

void student::createindex()
{
    fstream file;
    int pos;
    string susn;
    count = -1;
    file.open("student.txt", ios::in);
    while (!file.eof())
    {
        pos = file.tellg();
        buffer.erase();
        getline(file, buffer);
        if (buffer[0] != '*')
        {
            if (buffer.empty())
                break;
            susn = extractdid(buffer);
            didlist[++count] = susn;
            addlist[count] = pos;
        }
    }
    file.close();
    sortindex();
    buffer.erase();
}

void student::insert()
{
    string susn, name, year, sec;
    int pos, h = 0;
    fstream fp;

    cout << "\nUsn\n";
    cin >> susn;
    cout << "\nName\n";
    cin >> name;
    cout << "\nyear\n";
    cin >> year;
    cout << "\nsection\n";
    cin >> sec;
    buffer.erase();
    buffer = susn + "|" + name + "|" + year + "|" + sec + "$\n";
    h = h + 1;
    fp.open("student.txt", ios::out | ios::app);
    pos = fp.tellp();
    fp << buffer;
    fp.close();
    didlist[++count] = susn;
    addlist[count] = pos;

    sortindex();
    cout << "\nstudent added Successfully....\n";
}

string student::extractdid(string buffer)
{
    string susn;
    int i = 0;
    susn.erase();
    while (buffer[i] != '|')
        susn += buffer[i++];
    return susn;
}

void student::sortindex()
{
    int i, j, tempadd;
    string tempid;
    for (i = 0; i <= count; i++)
    {
        for (j = i + 1; j <= count; j++)
        {
            if (didlist[i] > didlist[j])

            {
                tempid = didlist[i];
                didlist[i] = didlist[j];
                didlist[j] = tempid;
                tempadd = addlist[i];

                addlist[i] = addlist[j];
                addlist[j] = tempadd;
            }
        }
    }
    /*
    cout<<"\n-------INDEXLIST--------\n";
    cout<<"\n ID\tINDEX";
    for(i=0;i<=count;i++)
    cout<<"\n"<<adminlist[i]<<"\t"<<addlist[i]<<endl;
   */
}

void student::search(string key)
{
    int pos = 0, t;
    fstream file;
    buffer.erase();
    pos = searchindex(key);
    if (pos == -1)
        cout << "Record not found\n";
    else if (pos >= 0)
    {
        file.open("student.txt");
        t = addlist[pos];
        file.seekp(t, ios::beg);
        getline(file, buffer);
        cout << "Record found\n";
        cout << "\nRecord:\n"
             << buffer;
        file.close();
    }
}

int student::searchindex(string key)
{
    int low = 0, high = count, mid = 0, flag = 0;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (didlist[mid] == key)
        {
            flag = 1;
            break;
        }
        else if (didlist[mid] < key)
            low = mid + 1;
        else if (didlist[mid] > key)
            high = mid - 1;
    }
    if (flag)
        return mid;
    else
        return -1;
}

void student::remove(string key)
{
    fstream fp;
    char delch = '*';
    int pos = 0, i, address;
    string buffer, susn, name, year, section;
    fstream file;
    pos = searchindex(key);
    if (pos >= 0)
    {
        file.open("student.txt");
        address = addlist[pos];
        file.seekp(address, ios::beg);
        file.put(delch);
        file.close();
        cout << "Record has been deleted\n";
        for (i = pos; i < count; i++)
        {
            didlist[i] = didlist[i + 1];
            addlist[i] = addlist[i + 1];
        }
        count--;
    }
    else
        cout << "Record not found\n";
}
void student::display_student()
{
    string line;
    cout << "The student details are\n";
    fstream fp;
    fp.open("student.txt");
    while (!fp.eof())
    {
        getline(fp, line);
        cout << line;
        cout << "\n";
    }
}
/*system("cat student.txt");*/

/*void student::print()
{
       ifstream fp;string line;string a[100],fp[100];int count;
       fp.open("student.txt");
       split(fp,'|');
       for(i=0;i<=count;i++)
          {
              a[i]=fp[i];
          }
        for(i=0;i<=count;i++)
          {
            cout<<a[i];
          }
       ifstream fp;string buffer;string a[100][100];
       fp.open("student.txt"); int i,h,j;
       while(fp.good())
            {
           for(i=0;i<h;i++)
               {
                 for(j=0;j<4;j++)
               {
                   getline(fp,buffer,'|');
                   a[i][j]=buffer;i++;j++;
               }

               }
               }

           for(i=0;i<h;i++)
               {
                 for(j=0;j<4;j++)
               {

                    cout<<a[i][j];i++;j++;
                    cout<<"\n";
               }

            }
           /* for(i=0;i<=4;i++)

            {
               getline(fp,buffer,'|');
               a[i]=line;
            }
            }
            for( i=0;i<4;i++)
            {
               cout<<a[i];cout<<"\n";
            }
       cout<<"name"<<a[1];
       cout<<"section"<<a[4];


}
*/

/*  ------------- librarian starts from here -----------*/

void librarian::createindex()
{
    fstream file;
    int pos;
    string lid, buffer;
    count = -1;
    file.open("librarian.txt", ios::in);
    while (!file.eof())
    {
        pos = file.tellg();
        buffer.erase();
        getline(file, buffer);
        if (buffer[0] != '*')
        {
            if (buffer.empty())
                break;
            lid = extractnid(buffer);
            nidlist[++count] = lid;
            addlist[count] = pos;
        }
    }
    file.close();
    sortindex();
    buffer.erase();
}

void librarian::insert()
{
    string buffer, lname, laddr, lid, lage;
    int pos;
    fstream fp;

    cout << "\nID\n";
    cin >> lid;
    cout << "\nEnter Name\n";
    cin >> lname;
    cout << "\nAge\n";
    cin >> lage;
    cout << "\nAddress\n";
    cin >> laddr;
    buffer.erase();
    buffer = lid + "|" + lname + "|" + lage + "|" + laddr + "$\n";
    fp.open("librarian.txt", ios::out | ios::app);
    pos = fp.tellp();
    fp << buffer;
    fp.close();
    nidlist[++count] = lid;
    addlist[count] = pos;

    sortindex();
    cout << "\nlibrarian added Successfully....\n";
}

void librarian::addstudent()
{
    string buffer, sname, susn, bname, date_of_issue;
    int pos;
    fstream fp;

    cout << "\nStudent usn :\n";
    cin >> sname;
    cout << "\nstudent name :\n";
    cin >> susn;
    cout << "\nBook name :\n";
    cin >> bname;
    cout << "\n date of issue :\n";
    cin >> date_of_issue;
    buffer.erase();
    buffer = sname + "|" + susn + "|" + bname + "|" + date_of_issue + "$\n";
    fp.open("bookissue.txt", ios::out | ios::app);
    pos = fp.tellp();
    fp << buffer;
    fp.close();

    cout << "\nBook issued Successfully....\n";
}

string librarian::extractnid(string buffer)
{
    string lid;
    int i = 0;
    lid.erase();
    while (buffer[i] != '|')
        lid += buffer[i++];
    return lid;
}
void librarian::sortindex()
{
    int i, j, tempadd;
    string tempid;
    for (i = 0; i <= count; i++)
    {
        for (j = i + 1; j <= count; j++)
        {
            if (nidlist[i] > nidlist[j])

            {
                tempid = nidlist[i];
                nidlist[i] = nidlist[j];
                nidlist[j] = tempid;
                tempadd = addlist[i];

                addlist[i] = addlist[j];
                addlist[j] = tempadd;
            }
        }
    }
    /*
    cout<<"\n-------INDEXLIST--------\n";
    cout<<"\n ID\tINDEX";
    for(i=0;i<=count;i++)
    cout<<"\n"<<adminlist[i]<<"\t"<<addlist[i]<<endl;
*/
}

void librarian::search(string key)
{
    int pos = 0, t;
    string buffer;
    fstream file;
    buffer.erase();
    pos = searchindex(key);
    if (pos == -1)
        cout << "Record not found\n";
    else if (pos >= 0)
    {
        file.open("librarian.txt");
        t = addlist[pos];
        file.seekp(t, ios::beg);
        getline(file, buffer);
        cout << "Record found\n";
        cout << "\nRecord:" << buffer;
        file.close();
    }
}

int librarian::searchindex(string key)
{
    int low = 0, high = count, mid = 0, flag = 0;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (nidlist[mid] == key)
        {
            flag = 1;
            break;
        }
        else if (nidlist[mid] < key)
            low = mid + 1;
        else if (nidlist[mid] > key)
            high = mid - 1;
    }
    if (flag)
        return mid;
    else
        return -1;
}

void librarian::remove(string key)
{
    fstream fp;
    char delch = '*';
    int pos = 0, i, address;
    string buffer, lid, lname, lage, laddr;
    fstream file;
    pos = searchindex(key);
    if (pos >= 0)
    {
        file.open("librarian.txt");
        address = addlist[pos];
        file.seekp(address, ios::beg);
        file.put(delch);
        file.close();
        cout << "Record has been deleted\n";
        for (i = pos; i < count; i++)
        {
            nidlist[i] = nidlist[i + 1];
            addlist[i] = addlist[i + 1];
        }
        count--;
    }
    else
        cout << "Record not found\n";
}
void librarian::display_n_record()
{

    cout << "The librarian details are\n";

    fstream fp;
    string line;
    fp.open("librarian.txt");
    while (!fp.eof())
    {
        getline(fp, line);
        cout << line;
        cout << "\n";
    }
}

void librarian::print()
{

    cout << "librarian Id:" << lid << " librarian Name:" << lname << " Age: " << lage << " Address :" << laddr << endl;
}

/*--------------------------------book -- starts from here ----------------------*/

void book::createindex()
{
    fstream file;
    int pos;
    string buffer, bname;
    count = -1;
    file.open("book.txt", ios::in);
    while (!file.eof())
    {
        pos = file.tellg();
        buffer.erase();
        getline(file, buffer);
        if (buffer[0] != '*')
        {
            if (buffer.empty())
                break;
            bname = extractpid(buffer);
            pidlist[++count] = bname;
            addlist[count] = pos;
        }
    }
    file.close();
    sortindex();
    buffer.erase();
}

string book::extractpid(string buffer)
{
    string bname;
    int i = 0;
    bname.erase();
    while (buffer[i] != '|')

        bname += buffer[i++];
    return bname;
}

void book::insert()
{
    string bname, bauthor, pname, buffer;
    int pos, j;
    fstream file;

    cout << "Enter  book name \n";
    cin >> bname;
    cout << "Enter  Author name \n";
    cin >> bauthor;
    cout << "Enter  Publisher name \n";
    cin >> pname;

    buffer += bname + "|" + bauthor + "|" + pname + "$" + "\n";
    file.open("book.txt", ios::out | ios::app);
    pos = file.tellp();
    file << buffer;
    file.close();

    pidlist[++count] = bname;
    addlist[count] = pos;
    sortindex();
    cout << "\nBook Registered Successfully....\n";
}

void book::sortindex()
{
    int i, j, tempadd;
    string tempusn;
    for (i = 0; i <= count; i++)
    {
        for (j = i + 1; j <= count; j++)
        {
            if (pidlist[i] > pidlist[j])
            {
                tempusn = pidlist[i];
                pidlist[i] = pidlist[j];
                pidlist[j] = tempusn;
                tempadd = addlist[i];
                addlist[i] = addlist[j];
                addlist[j] = tempadd;
            }
        }
    }
    /*     cout<<"\n------INDEXLIST-------\n";
         cout<<"\nPATIENT ID\tINDEX";
         for(i=0;i<=count;i++)
         cout<<"\n"<<pidlist[i]<<"\t"<<addlist[i]<<endl;
    */
}
void book::search(string key)
{
    int pos = 0, t;
    string buffer;
    fstream file;
    buffer.erase();
    searchindex2(key);
}

int book::searchindex(string key)
{
    int low = 0, high = count, mid = 0, flag = 0;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (pidlist[mid] == key)
        {
            flag = 1;
            break;
        }
        else if (pidlist[mid] < key)
            low = mid + 1;
        else if (pidlist[mid] > key)
            high = mid - 1;
    }
    if (flag)
        return mid;
    else
        return -1;
}

void book::searchindex2(string key) // here we want to match the common elements present in the pidlist
{
    int i, count, t, j;
    string buffer;
    fstream file;
    buffer.erase();
    cout << "\nRecords details: \n";
    for (i = 0; i <= count; i++)
    {
        if (pidlist[i] == key)
        {
            file.open("book.txt");
            t = addlist[i];
            file.seekp(t, ios::beg);
            getline(file, buffer);
            cout << "\n"
                 << buffer;
            file.close();
        }
    }
}

void book::remove(string key)
{
    fstream fp;
    char delch = '*';
    int pos = 0, i, address;

    fstream file;
    pos = searchindex(key);
    if (pos >= 0)
    {
        file.open("book.txt");
        address = addlist[pos];
        file.seekp(address, ios::beg);
        file.put(delch);
        file.close();
        cout << "Record has been deleted\n";
        for (i = pos; i < count; i++)
        {
            pidlist[i] = pidlist[i + 1];
            addlist[i] = addlist[i + 1];
        }
        count--;
    }
    else
        cout << "Record not found\n";
}

void book::display_b_record()
{

    cout << "book details";

    fstream fp;
    string line;
    fp.open("book.txt");
    while (!fp.eof())
    {
        getline(fp, line);
        cout << line;
        cout << "\n";
    }
}

void book::print()
{
    cout << "Book name:" << bname << " Author name" << bauthor << "Publisher name" << pname << endl;
}

int main()
{
    int ch;
    string key;
    book b;
    student s;
    librarian l;

    b.createindex();
    s.createindex();
    l.createindex();

mainpage:

    while (1)
    {
        int sch, nch, rch;
        string username = "admin", password = "admin", u, p;
        cout << "\n\t******* LIBRARY MANAGEMENT USING PRIMARY INDEX *******\n";
        cout << "\n\t\tEnter \n\t\t1. Admin\n\t\t2. Student\n\t\t3. librarian\n\t\t0. Exit\n";
        cout << "\nEnter your choice\n";
        cin >> ch;
        switch (ch)
        {
        case 1:
            cout << "Enter your username\n";
            cin >> u;
            cout << "Enter your password\n";
            cin >> p;
            if (u == username && p == password)
            {
                int ach;
                cout << "\nWelcome Admin\n";
            admin:
                while (1)
                {

                    cout << "----------------------\n";
                    cout << "|   ADMIN SECTION     |\n";
                    cout << "----------------------\n";
                    cout << "\n1.student\t\t2.librarian\n3.book\t\n\n\t\t\t(0) <<-- BACK\n";
                    cout << "\nEnter your choice\n";
                    cin >> ach;
                    switch (ach)
                    {
                    case 1:
                        while (1)
                        {
                            int sch;
                            cout << "-----------------------** STUDENT **---------------------------\n";
                            cout << "\n1. Add student \t\t\t2. Display student records\n3. Remove student Record\t\t4. View index list\n\n\t\t\t(0) <<-- BACK\n";
                            cout << "--------------------------------------------------------------\n";
                            cout << "\nEnter your choice\n";
                            cin >> sch;
                            switch (sch)
                            {
                            case 1:
                                s.insert();
                                break;
                            case 2:
                                s.display_student();
                                break;
                            case 3:
                            {
                                cout << "Enter student usn :";
                                cin >> key;
                                s.remove(key);
                                cout << "Updated file is :\n";

                                fstream fp;
                                string line;
                                fp.open("student.txt");
                                while (!fp.eof())
                                {
                                    getline(fp, line);
                                    cout << line;
                                    cout << "\n";
                                }
                            }
                            break;
                            case 4:
                                s.index_create();
                                break;
                            case 0:
                                goto admin;
                                break;
                            default:
                                cout << "Invalid choice";
                            }
                        }
                        break;

                    case 2:
                        while (1)
                        {
                            int n_ch;
                            cout << "-----------------------** LIBRARIAN **---------------------------\n";
                            cout << "\n1. Add librarian \t\t\t2. Display librarian records\n3. Remove librarian Record\t\t4. View index list\n\n\t\t\t(0) <<-- BACK\n";
                            cout << "--------------------------------------------------------------\n";
                            cout << "\nEnter your choice\n";
                            cin >> n_ch;
                            switch (n_ch)
                            {
                            case 1:
                                l.insert();

                                break;
                            case 2:
                                l.display_n_record();
                                break;
                            case 3:
                            {
                                cout << "Enter librarian ID :";
                                cin >> key;
                                l.remove(key);
                                cout << "Updated file is :\n";

                                fstream fp;
                                fp.open("librarian.txt");
                                while (!fp.eof())
                                {
                                    string line;
                                    getline(fp, line);
                                    cout << line;
                                    cout << "\n";
                                }
                            }
                            break;
                            case 4:
                                l.index_create();
                                break;
                            case 0:
                                goto admin;
                                break;
                            default:
                                cout << "Invalid choice";
                            }
                        }
                        break;

                    case 3:
                        while (1)
                        {
                            int b_ch;
                            cout << "-----------------------** BOOK **---------------------------\n";
                            cout << "\n1. Add books \t\t\t2.Remove book Record\t\t3. Display book records\t\t4. View index list\n\n\t\t\t(0) <<-- BACK\n";
                            cout << "--------------------------------------------------------------------\n";
                            cout << "\nEnter your choice\n";
                            cin >> b_ch;
                            switch (b_ch)
                            {
                            case 1:
                                system("cls");
                                b.insert();
                                break;

                            case 2:
                            {
                                cout << "Enter book ID :";
                                cin >> key;
                                b.remove(key);
                                cout << "Updated file is :\n";

                                fstream fp;
                                fp.open("book.txt");
                                while (!fp.eof())
                                {
                                    string line;
                                    getline(fp, line);
                                    cout << line;
                                    cout << "\n";
                                }
                            }
                            break;
                            case 3:
                                b.display_b_record();
                                break;
                            case 4:
                                b.index_create();
                                break;
                            case 0:
                                goto admin;
                                break;
                            default:
                                cout << "Invalid choice";
                            }
                        }
                        break;

                    case 0:
                        goto mainpage;
                        break;
                    default:
                        cout << "Invalid choice";
                    }
                }
            }
            else
                exit(0);

        case 2:
            cout << "Welcome\n";
            cout << "----------------------\n";
            cout << "|   STUDENT SECTION     |\n";
            cout << "----------------------\n";
            cout << "1. Search book \n2. Display book details\n\n\t\t\t(0) <<--BACK\nEnter your choice \n";
            cout << "------------------------\n";
            cin >> sch;
            switch (sch)
            {
            case 1:
                system("cls");
                cout << "Enter book name\n";
                cin >> key;
                b.search(key);
                break;
            case 2:
                b.display_b_record();
                break;

                break;
            case 0:
                goto mainpage;
            default:
                cout << "Invalid choice";
            }
            break;
        case 3:
            cout << "Welcome librarian\n";
            cout << "----------------------\n";
            cout << "|   LIBRARIAN SECTION     |\n";
            cout << "----------------------\n";
            cout << "1. Search student Record\n 2.Display student Record\n3. Book  issue \n 4.Display issued book\n 5.Pending Books \n 6.Return Book\nEnter your choice\n\n\t\t\t(0) <<-- BACK\n";
            cout << "------------------------\n";
            cin >> nch;
            switch (nch)
            {
            case 1:
                system("cls");
                cout << "Enter student ID\n";
                cin >> key;
                s.search(key);
                break;

            case 3:
                l.addstudent();
                break;
            case 5:
                l.pending_books();
                break;

            case 2:
                s.display_student();
                break;
            case 4:
                system("cat bookissue.txt");
            case 6:
                s.return_book();
                break;
            case 0:
                goto mainpage;
            default:
                cout << "Invalid choice";
            }
            break;
        case 0:
            exit(0);
        default:
            cout << "Invalid Choice";
        }
    }
}