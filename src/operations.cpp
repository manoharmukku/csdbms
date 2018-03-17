#include <bits/stdc++.h>
#include "reader.h"
#include "operations.h"
using namespace std;

void show_schema(const char *filename)
{
    printf("\nSHOW_SCHEMA : Opening file.... %s\n",filename);
    read_from_file(filename);

    printf("Relation Schema: \n");
    printf("----------------------------------------\n");
    for(int i = 0 ; i < NO_COLUMNS; i++){
        cout << col[i].col_name << "\t";
        cout << types[DATA_TYPES[i]] << "\t";
        cout << "(" << col[i].size << ")\t";
        printf("\n");
    }
    printf("\nNO of Records : %d\n",NO_RECORDS);
    printf("Primary Key : %s\n",col[PRIMARY_KEY_COL_NO].col_name);
    printf("----------------------------------------\n");
}

void build_hash_table(const char *filename)
{
    FILE *fp = fopen(filename, "r+");
    fseek(fp,DATA_HEAD,SEEK_SET);
    for(int i = 0 ; i < NO_RECORDS ; i++){

    }   
}

void insert_data(const char *filename, int num_ins = 1)
{
    printf("\nINSERTION : Opening file.... %s\n",filename);
    read_from_file(filename);
    FILE *fp = fopen(filename, "r+");
    assert(fp != NULL);
    int int_data;
    int nval = 0;
    char *dest = Malloc(char,255);
    char *blank = (char*)calloc(BLOCK_SIZE - TOTAL_RECORD_SIZE, sizeof(char));
    assert(dest != NULL && blank != NULL);
    // build index if not already built

    // Goto Last 
    fseek(fp,DATA_END,SEEK_SET);
    int counter;
    int succ_ins = 0;
    for(counter = 1 ; counter <= num_ins; counter++){
        for(int i = 0 ; i < NO_COLUMNS ; i++){
            if(DATA_TYPES[i] == 1){          
                scanf("%d",&int_data);
                assert(int_data <= MAX_INTEGER);
                fwrite(&int_data,int(sizeof(int)),1,fp);
            }
            else if(DATA_TYPES[i] == 3){
                scanf("%s",dest);
                assert(strlen(dest) <= 255);
                fwrite(dest,sizeof(char),255,fp);
            }
        }
        // write previous
        // fwrite(&(counter-1),sizeof(int),1,fp);
        // write next
        int prev;
        if(counter == 1)
        {
            //go to the last record and write its next
            long int temp_pos = ftell(fp);
            fseek(fp,BLOCK_START(LAST_REC_NO) + RECORD_SIZE + PTR_SIZE, SEEK_SET);
            int t = NO_RECORDS + 1;
            fwrite(&t,PTR_SIZE,1,fp);
            // back to the position
            fseek(fp,temp_pos,SEEK_SET);
        }
        // write previous pointer
        int temp = counter == 1 ? LAST_REC_NO : NO_RECORDS + counter - 1;
        fwrite(&temp,PTR_SIZE,1,fp);
        // write the next pointer
        temp = counter == num_ins ? 0 : NO_RECORDS + counter + 1;
        fwrite(&temp,PTR_SIZE,1,fp);
        // write extra blank space
        fwrite(blank,BLOCK_SIZE-TOTAL_RECORD_SIZE,1,fp);
        succ_ins++;
        assert(NO_RECORDS < MAX_RECORDS);
    }
    NO_RECORDS += succ_ins;
    LAST_REC_NO += succ_ins;
    fseek(fp,LAST_REC_NO_POS,SEEK_SET);
    fwrite(&LAST_REC_NO,sizeof(int),1,fp);
    fseek(fp,NO_REC_POSITION,SEEK_SET);
    fwrite(&NO_RECORDS,RECORD_NO_SIZE,1,fp);
    DATA_END = D_END;
    fseek(fp,DATA_END_POSITION,SEEK_SET);
    fwrite(&DATA_END,DATA_END_SIZE,1,fp);
    printf("%d row inserted.\n",succ_ins);
    free(dest);
    free(blank);
    fclose(fp);
}

void delete_data(const char *filename, int recNo)
{
    printf("\nDELETE_DATA : Opening file.... %s\n",filename);
    read_from_file(filename);
    if(NO_RECORDS == 0) {
        printf("No Data In this Table\n");
        return ;
    }
    int prev, next;
    FILE *fp = fopen(filename, "r+");
    assert(fp != NULL);
    fseek(fp,BLOCK_START(recNo) + RECORD_SIZE , SEEK_SET);
    fread(&prev,PTR_SIZE,1,fp);
    fread(&next,PTR_SIZE,1,fp);

    // go to 'prev' block and change its next to 'next'
    fseek(fp,BLOCK_START(prev) + RECORD_SIZE + PTR_SIZE , SEEK_SET);
    fwrite(&next,PTR_SIZE,1,fp);
    // go to 'next' blok and change its previous to 'prev'
    fseek(fp,BLOCK_START(next) + RECORD_SIZE , SEEK_SET);
    fwrite(&prev,PTR_SIZE,1,fp);

    if(recNo == LAST_REC_NO)    LAST_REC_NO--;
    NO_RECORDS--;
    fclose(fp);    
}

void show_data(const char *filename)
{
    printf("\nSHOW_DATA : Opening file.... %s\n",filename);
    read_from_file(filename);
    if(NO_RECORDS == 0) {
        printf("No Data In this Table\n");
        return ;
    }
    for(int i = 0 ; i < NO_COLUMNS; i++)
        printf("%-12s",col[i].col_name);
    printf("\n");
    printf("----------------------------------------------------------\n");
    FILE *fp = fopen(filename, "r");
    assert(fp != NULL);
    fseek(fp,DATA_HEAD,SEEK_SET);
    int int_data;
    char *dest = Malloc(char,255);
    int prev, next;
    do{
        for(int j = 0 ; j < NO_COLUMNS; j++){
             if(DATA_TYPES[j] == 1){          
                fread(&int_data,sizeof(int),1,fp);
                printf("%-12d",int_data);
            }
            else if(DATA_TYPES[j] == 3){
                fread(dest,sizeof(char),255,fp);
                printf("%-12s",dest);
           }
        }
        fread(&prev,PTR_SIZE,1,fp);
        //printf("%-12d",prev);
        fread(&next,PTR_SIZE,1,fp);
        //printf("%-12d",next);
        if(next == 0)   break;  // end of data
        fseek(fp,BLOCK_START(next),SEEK_SET);
        printf("\n");
    }while(true);
    printf("\n");
    free(dest);
    fclose(fp);
}

