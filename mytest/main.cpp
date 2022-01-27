//#define _LARGEFILE_SOURCE
//#define _LARGEFILE64_SOURCE
//#define _FILE_OFFSET_BITS 64


#include "../opt/public.h"
#include <fastdb/fastdb.h>
#include <stdio.h>
#include <thread>
#include <vector>


USE_FASTDB_NAMESPACE

#if 1
 
USE_FASTDB_NAMESPACE
 
class Record
{
public:
    Record(){}
    ~Record(){}
public:
    int4 id;           // id 作为主键唯一标识
    int4 value;    
    int4 value1;
    int4 value2;        // value 作为保存值
    int4 value3;   
    int4 value4; 
    int4 value5;   
    int4 value6;   
    int4 value7; 
    int4 value8;  
    int4 value9;   
    int4 value10; 
    int4 value11;  
    int4 value12;   
    int4 value13; 
    int4 value14;  
    int4 value15;   
    int4 value16; 
    int4 value17;   
    int4 value18;   
    int4 value19; 
    int4 value20;  
 
    TYPE_DESCRIPTOR( ( KEY( id, INDEXED  ),
                       FIELD( value ),                     
                      FIELD( value1) ,
                      FIELD( value2) ,        
                      FIELD( value3) ,  
                      FIELD( value4) ,
                      FIELD( value5) ,  
                      FIELD( value6) ,  
                      FIELD( value7) ,
                      FIELD( value8) , 
                      FIELD( value9) ,  
                      FIELD( value10), 
                      FIELD( value11),  
                      FIELD( value12),   
                      FIELD( value13), 
                      FIELD( value14),  
                      FIELD( value15),   
                      FIELD( value16), 
                      FIELD( value17),   
                      FIELD( value18),   
                      FIELD( value19), 
                      FIELD( value20) ) );
};
 
// 创建 Record 数据表
REGISTER(Record);
 

 
//! 插入数据
void insertRecord(int size, int id)
{
//    printf("####### insertRecord #######\n");
    for ( int4 i = 0; i < size; i++ )
    { 
        Record rec;
        rec.id = i;
        rec.value =  i;

        rec.value1 = i;
        rec.value2 = i;
        rec.value3 = i;
        rec.value4 = i;
        rec.value5 = i;
        rec.value6 = i;
        rec.value7 = i;
        rec.value8 = i;
        rec.value9 = i;
        rec.value10 = i;
        rec.value11 = i;
        rec.value12 = i;
        rec.value13 = i;
        rec.value14 = i;
        rec.value15 = i;
        rec.value16 = i;
        rec.value17 = i;
        rec.value18 = i;
        rec.value19 = i;
        rec.value20 = i;
      
           
        // 插入数据
        // printf(" inserted database  threadid:%d\n ",id);
        insert( rec );
                           
    }
}
 
//! 更新数据
void updateRecord()
{
    printf("####### updateRecord #######\n");
   
    dbCursor<Record> cursorWrite(dbCursorForUpdate);  // 写游标对象 dbCursorForUpdate
    int n = cursorWrite.select();                     // 查询
         
    // 存在记录, 进行加1更新
    if(0 < n)
    {
        do
        {
            cursorWrite->value = cursorWrite->value + 1;
            cursorWrite.update();
 
        }while(cursorWrite.next());  // next() 游标向后滚
   }
    
}
 
//! 删除某条记录
void removeRecord(const int id)
{
    printf("####### removeRecord #######\n");
   
    dbQuery q;                                        // 查询语句
    dbCursor<Record> cursorWrite(dbCursorForUpdate);  // 写游标对象 dbCursorForUpdate
   
    q = "id =", id;
    int n = cursorWrite.select(q);   // 查询
         
    // 存在记录
    if(0 < n)
    {
        do
        {
            cursorWrite.removeAllSelected();
         
        }while(cursorWrite.next());  // next() 游标向后滚
    }
}
 
//! 删除所有数据
void removeAllRecord()
{
  //  printf("####### removeAllRecord #######\n");
   
    dbCursor<Record> cursorWrite(dbCursorForUpdate);  // 写游标对象 dbCursorForUpdate
    cursorWrite.removeAll();                          // 这里进行清除, 不清除会进行累加
}
 
//! 查询数据
void selectRecord()
{
   printf("####### selectRecord #######\n");
   
   diff_count diff;
   diff.start();
    dbCursor<Record> cursorRead;  // 只读游标对象
    int n = cursorRead.select();  // 查询
    diff.add_snap();
    int a,b;
    diff.show_diff(a,b,true);
    // 存在记录
    if(0 < n)
    {
        int cnt = 1;
        do
        {
            printf("%d---%d---%d --- size= %d\n", n, cursorRead->id, cursorRead->value,n);
            cnt-- ;
        }while(cursorRead.next() && cnt > 0);  // next() 游标向后滚
    }
}
    #define ROW 10
    #define COL 2

void test_insert(int test_count, int test_par[][COL], int test_result[][COL], int threadid)
{
    unsigned long initsize = 3 *1024* 1024* 1024UL;
    unsigned long extqn = 4194304UL;
    unsigned long initindexsize = 524288UL;
   dbDatabase db(dbDatabase::dbAllAccess, initsize,extqn,initindexsize,1, 6 );
//   printf(" will open database  threadid:%d\n ",threadid);
//   sleep(1);
    if (db.open(_T("testpar"),nullptr,3))
    {
//    printf(" opened database  threadid:%d\n ",threadid);
//    sleep(1);
        do
        {
        // 插入数据

        for(int i = 0 ; i < ROW; i++)
        {
            diff_count diff;
            for(int j = 0; j < test_par[i][1]; j++)
            {
                insertRecord(test_par[i][0], threadid);

                // 提交
                diff.start();
                db.precommit();
                //db.commit();
                diff.stop();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
               //  printf(" -- ");
            }
            db.commit();
            diff.show_diff(test_result[i][0],test_result[i][1]);
            // 删除所有数据
           // removeAllRecord();
           // db.commit();  
           // sleep(5);
           
        }

       
        // 查询数据
        selectRecord();

        continue;   

        while (1)
        {
            sleep(100);
        }    

        
        // 更新数据加1
        updateRecord();                
        // 提交
        db.commit();     
        // 查询数据
        selectRecord();      
        // 删除数据
        removeRecord(1);
        // 提交
        db.commit();     
        // 查询数据
        selectRecord();     
        // 删除所有数据
        removeAllRecord();
        // 提交
        db.commit();   
        // 查询数据
        selectRecord();
        }
        while (false);
    }
    else
    {
        printf(" open database failed ! ");
    }
std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if(db.isOpen())
    {
        // 删除所有数据
       // removeAllRecord();
        // 关闭数据库
        db.close();
    }   
}
 
int main()
{
    // 打开数据库 testpar
    int th_count =1;

    const int test_count = 10 *10000;
    int test_par[ROW][COL];  // 批量测试参数
    int test_result[th_count][ROW][2];  // 批量测试参数
    for(int i = 0 ; i < ROW; i++)
    {
        test_par[i][0] = 2*i+1;
        test_par[i][1] = test_count/(test_par[i][0]);
    }

    std::vector<std::thread> test_th;
   
    for(int i= 0; i< th_count; i++)
    {
        std::thread  th(test_insert,test_count,test_par,test_result[i], i);
        test_th.emplace_back(std::move(th));
    }
    //test_insert(test_count, test_par, test_result);

     for(auto &t : test_th)
     {
         if(t.joinable())
            t.join();
     }
     
    for(int j= 0; j< th_count; j++)
    {
        printf("\n %s\t\t %s\t %s\t %s\t\t %s\t %s\t\t\n" , "总条数", "每次发送条数" ,"发送线程数" ,"延时ms" , "仅发送延迟" , "IPS");
        for(int i = 0 ; i < ROW; i++)
        {
           printf("%16d %8d %8d %16d %16d \t %16f \n" ,  test_par[i][0]*test_par[i][1] ,  test_par[i][0], 1 , test_result[j][i][0], test_result[j][i][1],   (test_count*1000 *1.0 )/ (test_result[j][i][0] *1.0)   );
        }
    }

    
   return 0;
}

#endif


#if 0
//#define _LARGEFILE_SOURCE
//#define _LARGEFILE64_SOURCE
//#define _FILE_OFFSET_BITS 64

#include <fastdb/fastdb.h>
#include <fastdb/database.h>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <string>

using namespace std;
USE_FASTDB_NAMESPACE

dbDatabase db;

#define random(a,b) (rand()%(b-a))+a+1  //生成随机数
#define TeacherNum 5   //老师个数
#define StudentNum 20  //学生个数

class Student;
class Teacher
{
public:
	const char* teacher_name;
	dbArray<dbReference<Student>> std_code;
	TYPE_DESCRIPTOR((KEY(teacher_name,INDEXED),OWNER(std_code,tech_code)));
};

class Student
{
public:
	const char* std_name;
	dbArray<int4> scores;  //Chinese English Math
	dbReference<Teacher> tech_code;
	TYPE_DESCRIPTOR((KEY(std_name,INDEXED),
					 FIELD(scores),
					 RELATION(tech_code,std_code)));

};

REGISTER(Teacher);  //注册Teacher表
REGISTER(Student);  //注册Student表

//生成老师的编号
void getRandomCharTeach(char * randnum)
{
	int num = random(0,10);  
	snprintf(randnum,sizeof(randnum),"%d",num);  //int类型转换为char *
}

//生成学生的编号
void getRandomCharStu(char * randnum)
{
	int num = random(20,50);
	snprintf(randnum,sizeof(randnum),"%d",num);  //int类型转换为char *
}

//生成0-100之间的随机数
int4 getRandomIntSocre()
{
	int num = random(0,100);
	return num;
}

//查询老师编号
void selectTeachName()
{
	cout<<"***** selectTeachName *****"<<endl;
	dbCursor<Teacher> cursorTeach;  //只读游标
	int n = cursorTeach.select();  //查询
	cout<<"Teacher的数量为："<<n<<endl;

	if(0<n)
	{
		do
		{
			cout<<"teacher_name = "<<cursorTeach->teacher_name<<" ."<<endl;
		}while(cursorTeach.next());  //游标向后滚
	}
}

//查询学生信息
void selectStuInfo()
{
	cout<<"***** selectStuInfo *****"<<endl;
	dbCursor<Student> cursorStu;  //只读游标
	int n = cursorStu.select();  //查询
	cout<<"Student的数量为："<<n<<endl;

	if(0<n)
	{
		do
		{
			cout<<"std_name = "<<cursorStu->std_name<<" , "<<
			"Chinese = "<<cursorStu->scores[0]<<" , "<<
			"English = "<<cursorStu->scores[1]<<" , "<<
			"Math = "<<cursorStu->scores[2]<<" ."<<endl;
		}while(cursorStu.next());  //游标向后滚
	}
}

//删除所有数据
void removeAllRecord()
{
	cout<<"***** removeAllRecord *****"<<endl;
	dbCursor<Teacher> cursorTeach(dbCursorForUpdate);  //写游标对象
	cursorTeach.removeAll();
	dbCursor<Student> cursorStu(dbCursorForUpdate);  //写游标对象
	cursorStu.removeAll();
}


int main()
{
	Teacher techer;
	Student student;

	if(db.open(_T("test")))
	{
		cout<<"Start inserting data..."<<endl;
		for(int i = 0; i < TeacherNum; i++)
		{
			char tech_num[2];
			getRandomCharTeach(tech_num);
			techer.teacher_name = tech_num;
			dbReference<Teacher> techer_class = insert(techer);
			cout<<"Insert "<<techer.teacher_name<<" to Teacher."<<endl;
			db.commit();

			//插入多个学生信息对应当前老师
			for(int j = 0; j < StudentNum; j++)
			{
				char std_num[2];
				getRandomCharStu(std_num);
				student.std_name = std_num;
				int Chinese = getRandomIntSocre();  //语文成绩
				int English = getRandomIntSocre();  //英语成绩
				int Math = getRandomIntSocre();  //数学成绩
				
				student.scores.resize(3);  //数组大小为3
				student.scores.putat(0,Chinese);  //把各科成绩放到数组中
				student.scores.putat(1,English);
				student.scores.putat(2,Math);
				student.tech_code = techer_class;  //把当前学生的信息与对应老师相关联
				insert(student);  
				cout<<"Insert std_name : "<<student.std_name<<" , "<<
				"Chinese : "<<Chinese<<" , "<<
				"English : "<<English<<" , "<<
				"Math : "<<Math<<" ."<<endl;
			}
			db.commit();
		}
		//查询、删除的实例可以在open表之后直接调就可以了，这里就不写了
	}
	db.commit();
	return 0;
}
#endif

#if 0
#include "fastdb/fastdb.h"

//extern 
dbDatabase db; // create database object

class MyTable { 
public:
    char const* someField;
  public:
    TYPE_DESCRIPTOR((FIELD(someField)));
};

//
// Implementation
//
REGISTER(MyTable);

int main() 
{ 
    if (db.open("mydatabase")) { 
        dbCursor<MyTable> cursor;
        dbQuery q;

    int bufSize = 100;
	char value[bufSize];

	q = "someField=",value;
	gets(value);
	if (cursor.select(q) > 0) { 
	    do { 
	        printf("%s\n", cursor->someField);
	    } while (cursor.next());
        }
	db.close();
	return EXIT_SUCCESS;
    } else { 
        return EXIT_FAILURE;
    }
}

#endif 

#if 0
#include "fastdb/fastdb.h"
#include <iostream>

#ifndef NO_NAMESPACES
using namespace std;
#endif

USE_FASTDB_NAMESPACE

class Guess {
  public:
    dbReference<Guess> yes;
    dbReference<Guess> no;
    string             question;

    TYPE_DESCRIPTOR((FIELD(yes), FIELD(question), FIELD(no)));
};

REGISTER(Guess);

void input(string const& prompt, string& answer)
{
    do {
        cout << prompt;
#if 0
// doesn't work with Visual C++ 7.0
        getline(cin, answer);
#else
        char buf[256];
        buf[0] = '\0';
        fgets(buf, (int)sizeof(buf), stdin);
        int len = (int)strlen(buf);
        if (len > 0) { 
            buf[len-1] = '\0';
        }
        answer = buf;
#endif
    } while (answer.length() == 0);
}

bool askQuestion(string const& question) {
    string answer;
    while (true) { 
        answer = "";
        input(question, answer);
        if (answer == "y") { 
            return true;
        }
        if (answer == "n") { 
            return false;
        }
        cout << "Please answer y/n\n";
    }
}


dbReference<Guess> whoIsIt(dbReference<Guess> const& parent) {
    string animal, difference;
    input("What is it ? ", animal);
    input("What is a difference from other ? ", difference);
    Guess node;
    node.question = animal;
    dbReference<Guess> child = insert(node);
    node.question = difference;
    node.yes = child;
    node.no = parent;
    return insert(node);
}


dbReference<Guess> dialog(dbCursor<Guess>& cur) {
    string question;
    dbCursor<Guess> c(dbCursorForUpdate);
    question = "May be ";
    question += cur->question;
    question += " (y/n) ? ";
    if (askQuestion(question)) {
        if (cur->yes == null) {
            cout << "It was very simple question for me...\n";
        } else {
            c.at(cur->yes);
            dbReference<Guess> clarify = dialog(c);
            if (clarify != null) {
                cur->yes = clarify;
                cur.update();
            }
        }
    } else {
        if (cur->no == null) {
            if (cur->yes == null) {
                return whoIsIt(cur.currentId());
            } else {
                cur->no = whoIsIt(null);
                cur.update();
            }
        } else {
            c.at(cur->no);
            dbReference<Guess> clarify = dialog(c);
            if (clarify != null) {
                cur->no = clarify;
                cur.update();
            }
        }
    }
    return null;
}




int main()
{

    //dbDatabase(dbDatabase::dbConcurrentReadMode)

    dbDatabase db;
    if (db.open(_T("guess.dbs"))) {
        dbCursor<Guess> cur(dbCursorForUpdate);
        while (askQuestion("Think of an animal. Ready (y/n) ? ")) {
            if (cur.select() != 0) {
                cur.next(); // first question is in record number 2
                dialog(cur);
            } else {
                whoIsIt(null);
            }
            db.commit();
        }
        db.close();
        cout << "End of the game\n";
        return EXIT_SUCCESS;
    } else {
        cerr << "Failed to open database\n";
        return EXIT_FAILURE;
    }
}
#endif