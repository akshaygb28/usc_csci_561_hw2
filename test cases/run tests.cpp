#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <dirent.h>
#include <sstream>

using namespace std;

#define INPUT_PATH  "C:\\Users\\AKSHAY\\Desktop\\USC\\Sem 1 - CSCI 561(AI)\\Homeworks\\hw2\\test cases\\test cases - 5000"
#define EXE_NAME    "homework.exe"
#define EXE_PATH    "C:\\Users\\AKSHAY\\Desktop\\USC\\Sem 1 - CSCI 561(AI)\\Homeworks\\hw2"

bool compare_output(string s1, string s2);

int main(int argc, char const *argv[])
{
    string input_path = INPUT_PATH;
    string exe_path = EXE_PATH;
    string exe_name = EXE_NAME;
    int count_passed = 0, count_failed = 0;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(input_path.c_str())) != NULL)
    {
        FILE *fp;
        fp = fopen("run results.txt", "w");
        if(!fp)
        {
            printf("File create error \n");
            closedir(dir);
            return 0;
        }
        while((ent = readdir(dir)) != NULL)
        {
            if(ent->d_name[0] == 'i' || ent->d_name[0] == 'I')
            {
                stringstream ss;
                /* Copy */
                ss<<"copy \""<<input_path<<"\\"<<ent->d_name<<"\" \"input.txt\" > NULL";
                system(ss.str().c_str());
                /* Execute */
                ss.clear();
                ss.str("");
                ss<<"\""<<exe_path<<"\\"<<exe_name<<"\" > NULL";
                system(ss.str().c_str());
                /* Compare output */
                ss.clear();
                ss.str("");
                string op1 = "output.txt";
                ss<<input_path<<"\\output"<<string(ent->d_name).substr(5);
                string op2 = ss.str();
                if(compare_output(op1, op2))
                {
                    count_passed++;
                    printf("%-15s : PASSED \n", ent->d_name);
                    fprintf(fp, "%-15s : PASSED \n", ent->d_name);
                }
                else
                {
                    count_failed++;
                    printf("%-15s : FAILED \n", ent->d_name);
                    fprintf(fp, "%-15s : FAILED \n", ent->d_name);
                }
            }
        }
        closedir(dir);
        fprintf(fp, "\n");
        fprintf(fp, "%-10s : %d \n", "PASSED", count_passed);
        fprintf(fp, "%-10s : %d \n", "FAILED", count_failed);
        fprintf(fp, "%-10s : %d \n", "TOTAL", count_passed + count_failed);
        printf("\n\n");
        printf("----- TEST RESULTS ----- \n\n");
        printf("%-10s : %d \n", "PASSED", count_passed);
        printf("%-10s : %d \n", "FAILED", count_failed);
        printf("%-10s : %d \n", "TOTAL", count_passed + count_failed);
        /* Cleanup */
        system("del input.txt output.txt NULL");
    }
    else
    {
        printf("Could not open directory \n");
    }
    return 0;
}

bool compare_output(string s1, string s2)
{
    FILE *fp1, *fp2;
    fp1 = fopen(s1.c_str(), "r");
    fp2 = fopen(s2.c_str(), "r");
    if(!fp1 || !fp2)
    {
        printf("File open failed!! \n");
        return false;
    }
    char c1, c2;
    do
    {
        while((c1 = (char)fgetc(fp1)) == '\n');
        while((c2 = (char)fgetc(fp2)) == '\n');
    }while(c1 != EOF && c2 != EOF && c1 == c2);
    fclose(fp1);
    fclose(fp2);
    return (c1 == c2);
}
