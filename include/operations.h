#ifndef INSERT_H
#define INSERT_H

#include "metadata_struct.h"
#include <variant>
#include <unordered_map>

extern int             NO_RECORDS;
extern int             NO_COLUMNS;
extern char            *TABLE_NAME;
extern int             RECORD_SIZE;
extern int             TOTAL_RECORD_SIZE;
extern int              DATA_HEAD;
extern int              DATA_END;
extern struct Column   *col;
extern std::string      types[6] ;//= {"INTEGER", "DOUBLE", "STRING", "DATE", "TIME"};
extern int              TYPE_SIZE[6];
extern int             PRIMARY_KEY_COL_NO;
extern int             *CUM_POS;
extern int             LAST_REC_NO;
extern int              IS_READ;

extern std::unordered_map<std::variant<int,char*>,int> hash_table;

void show_schema(const char *);
void build_hash_table(const char *);
void insert_data(const char *, int);
void delete_data_from_rec(const char *, int );
void delete_data(const char *, std::map<char*,std::variant<int,char*>, cmp_str >);
void show_data(const char *, std::map<char*,std::variant<int,char*>, cmp_str >);

#endif // INSERT_H