typedef struct {
    char **lines;
    int size;
} Merged_file;

typedef struct {
    Merged_file** merged;
    int size;
} Main_table;

Main_table create_table(int);
void merge_files(int , char **);
int create_block(Main_table *);
int lines_number(Merged_file *);
void remove_block(Main_table *, int);
void remove_row(Merged_file *, int);
void print_merged(Main_table *);

