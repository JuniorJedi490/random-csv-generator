#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile int error_status = 0;

struct col {
	char type;
	char* header;
};

struct node;
struct node {
	struct node* next;
	struct col* payload;
};


void invalid_column_message(int index, char* arg) {
	printf("Column %d: invalid column type: %s\n", index, arg);
	puts("Valid types are:");
	puts("    f - float");
	puts("    i - integer");
	puts("    b - boolean");
	puts("    s - string");
	puts("    p - percentage");
	error_status = 1;
}

void help_message(FILE* outfile) {
	fputs("Usage: random-csv-generator [filename] [col1type] [col1header] [col2type] [...] [#rows] [seed]", outfile);
	fputs("  filename - the file extension will be .csv if it isn't already", outfile);
	fputs("  column header - a string", outfile);
	fputs("  column type - one of the following characters", outfile);
	fputs("    f - float", outfile);
	fputs("    i - integer", outfile);
	fputs("    b - boolean", outfile);
	fputs("    s - string", outfile);
	fputs("    p - percentage", outfile);
}


int main(int argc, char** argv) {
	if (argc <= 1)
	{
		help_message(stdout);
		FILE* outfile = fopen("error.log", "w");
		help_message(outfile);
		fclose(outfile);
		exit(1);
	}
	else
	{
		char* filename = argv[1];
		char* filename_extension = filename + strlen(filename) - 4;
		if(strcmp(filename_extension, ".csv") != 0)
		{
			char* filename_extended = (char*) malloc((strlen(filename) + 5) * sizeof(char));
			strcpy(filename_extended, filename);
			strcpy(filename_extended + strlen(filename), ".csv");
			filename = filename_extended;
		}
		
		struct node* col_list = (struct node*) malloc(sizeof(struct node));
		int row_count = 0;
		int row_count_index = 0;
		int seed = 0;
		struct node* curr_col = col_list;
		col_list->payload = 0;
		col_list->next = 0;
		
		for (int i = 2; i < argc; ++i)
		{
			if (row_count_index == 0)
			{
				if (i % 2 == 0)
				{
					row_count = atoi(argv[i]);
					if (row_count)
					{
						row_count_index = i;
						if (!col_list->payload)
						{
							puts("No columns exist; minimum amount is 1");
							help_message(stdout);
							error_status = 1;
						}
					}
					else
					{
						// Column description
						if (argv[i][0] != 'f' && argv[i][0] != 'i' && argv[i][0] != 'b' && argv[i][0] != 's' && argv[i][0] != 'p' && argv[i][0] != 'F' && argv[i][0] != 'I' && argv[i][0] != 'B' && argv[i][0] != 'S' && argv[i][0] != 'P') invalid_column_message(i, argv[i]);
						
						if (!col_list->payload)
						{
							col_list->payload = (struct col*) malloc(sizeof(struct col));
							col_list->payload->type = argv[i][0];
						}
						else
						{
							curr_col->next = (struct node*) malloc(sizeof(struct node));
							curr_col = curr_col->next;
							curr_col->next = 0;
							curr_col->payload = (struct col*) malloc(sizeof(struct col));
							curr_col->payload->type = argv[i][0];
						}
					}
				}
				else
				{
					// Column header
					curr_col->payload->header = argv[i];
				}
			}
			else 
			{
				if (i == (row_count_index + 1))
				{
					// Seed
					seed = atoi(argv[i]);
				}
				else
				{
					puts("Too many arguments after number of rows.");
					help_message(stdout);
					error_status = 1;
				}
			}
		}
		if (error_status) exit(error_status);
		
		FILE* outfile = fopen(filename, "w");
		if (!outfile) { printf("Couldn't open file %s\n", filename); exit(1); }
		
		srand(seed);
		
		for (int i = 0; i <= row_count; ++i)
		{
			curr_col = col_list;
			
			int counter = 0;
			
			do
			{
				if (i == 0)
				{
					fprintf(outfile, "%s", curr_col->payload->header);
				}
				else if (curr_col->payload->type == 'f' || curr_col->payload->type == 'F')
				{
					fprintf(outfile, "%f", (double) rand() / RAND_MAX + rand());
				}
				else if (curr_col->payload->type == 'p' || curr_col->payload->type == 'P')
				{
					fprintf(outfile, "%f", (double) rand() / RAND_MAX * 100);
				}
				else if (curr_col->payload->type == 'i' || curr_col->payload->type == 'I')
				{
					fprintf(outfile, "%d", rand());
				}
				else if (curr_col->payload->type == 'b' || curr_col->payload->type == 'B')
				{
					fprintf(outfile, "%s", (rand() & 1) ? "TRUE" : "FALSE");
				}
				else if (curr_col->payload->type == 's' || curr_col->payload->type == 'S')
				{
					int thelen = 5 + rand() % 5;
					char* thestr = (char*) malloc((thelen + 1) * sizeof(char));
					thestr[thelen] = 0;
					for (int i = 0; i < thelen; ++i)
					{
						char thechar = rand() % 52;
						thestr[i] = (thechar >= 26) ? thechar + 'A' - 26 : thechar + 'a';
					}
					fprintf(outfile, "%s", thestr);
					free(thestr);
				}
				else
				{
					fprintf(stderr, "Invalid column type: %c", curr_col->payload->type);
					fclose(outfile);
					exit(2);
				}
				if (curr_col->next) {fprintf(outfile, ",");} else {fprintf(outfile, "\n");}
				
				curr_col = curr_col->next;
				
				if (++counter == 100) exit(3);
				
			} while (curr_col);
		}
		fclose(outfile);
		
		printf("Wrote %d lines to file %s\n", row_count + 1, filename);
		
		if (filename != argv[1]) free(filename);
	}
	return 0;
}
