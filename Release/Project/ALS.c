#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE  1024
#define ARGV_SIZE  100
#define MAX_PRODUCTION_LINES 3
#define MAX_PRODUCT_NOS 5
#define TOTAL_CHILD_PROCESS 3
#define MAX_EQUIPMENT_NOS 8
#define MAX_ORDER_NOS  600
#define TOTAL_DAYS  60
#define DAILY_PRODUCTION_CAPACITY 1000

char *argv[ARGV_SIZE];

char OrderNumber[MAX_ORDER_NOS][20]={0};
char StartDate[MAX_ORDER_NOS][20]={0};
char EndDate[MAX_ORDER_NOS][20]={0};
char DueDate[MAX_ORDER_NOS][20]={0};
char ProductRequired[MAX_ORDER_NOS][20]={0};
char quantity[MAX_ORDER_NOS][10]={0};
char *product_requirement[MAX_PRODUCT_NOS][6];
char *production_line_equipment[MAX_PRODUCTION_LINES][MAX_EQUIPMENT_NOS+1];
int orderStatus[MAX_ORDER_NOS]={0};

 //0=not allocated, -1 = fail, >0 = no. of the production line.
// char IntStartDate[MAX_ORDER_NOS]={0}; //integer array of StartDate that is converted to integer e.g. "D001" -> "001"
// char IntDueDate[MAX_ORDER_NOS]={0}; //integer array of DueDate that is converted to integer e.g. "D001" -> "001"

// int quantity[MAX_ORDER_NOS]={0};
int intquantity[20]={0};
int intstartdate[20];
int intenddate[20];

int startdateint=0;
int enddateint=0;
int quantityint=0;
int ordernumber=0;

int current_last_record_index =0;

char allorder[MAX_ORDER_NOS][360];
//int i,j;
int allordernumber[MAX_ORDER_NOS][4];

FILE *filein,*fileout;


//function prototypes
// char *output_file_name;

/*ALS*/
int child_id[TOTAL_CHILD_PROCESS];
int fd[2 * TOTAL_CHILD_PROCESS]; //pipe: parent => child
int pfd[2 * TOTAL_CHILD_PROCESS]; //pipe: child => parent

// int fd[2]; //pipe: parent => child
// int pfd[2]; //pipe: child => parent

/*ALS*/

//function prototypes
void load_config();
void input_standard_call();
void addOrder();
void addBatchOrder(char InputFile[20]);
void parse(char *line, char **argv);
int check_argv_numbers(char **argv,int beginning,int ending);
void print_all_commands();
void run_ALS();
void run_FCFS();
void run_SJF();
void run_print_report();
void print_schedule();
void print_report();


//main function
int main(/*int argc, char const *argv[]*/) {
	int i,j;
	char line[MAX_LINE_SIZE];
	char endline[] = "EndProgram";
	char buf[80]={0};
	char buf2[80]={0};
	printf("   ~~WELCOME TO ALS~~\n");
	remove("formattedForALS.log");
	remove("ALSFCFSoutfile.log");
	remove("ALSSJFoutfile.log");

	for (i=0; i<TOTAL_CHILD_PROCESS; i++) {
		pipe(&fd[2*i]); //parent=>child
		pipe(&pfd[2*i]); //child=>parent
		child_id[i] = fork();
		if (child_id[i]<0) {
			printf("Pipe Error!!!\n");
			exit(1);
		}else if (child_id[i]==0) { //This is a child, then execute the statements for child below
			close(fd[2*i+1]);
			close(pfd[2*i]);
			for (j=0; j<i; j++) {
				close(fd[2*j+1]);
				close(pfd[2*j]);
				close(fd[2*j+1]);
				close(pfd[2*j]);
			}
			// printf("Closed pipes of Child: %d\n",i);
			break;
		}else{
			close(fd[2*i]);
			close(pfd[2*i+1]);
			continue; //This is a parent, continue to fork new child.
		}
	}

	if (i!=TOTAL_CHILD_PROCESS) {
		while(1) {
			memset(buf,0,sizeof(buf));
			int n;
			n = read(fd[2*i],buf,80);
			buf[n]=0;
			// printf("Children %d Buf: %s\n",i,buf);
			if (strcmp(buf,"end")==0) {
				// printf("Child bye!\n");
				break;
				//do something here!!!
			}else if(strcmp(buf,"FCFS")==0) {
				char InputFileName[60]={0};
				char OutputFileName[60]={0};
				strcpy(buf2, "K");
				write(pfd[1],buf2,80);

				n = read(fd[0],buf,80);
				buf[n] = 0;
				strcpy(InputFileName, buf);
				strcpy(buf2, "K");
				write(pfd[1],buf2,80);

				n = read(fd[0],buf,80);
				buf[n] = 0;
				strcpy(OutputFileName, buf);
				run_FCFS(InputFileName,OutputFileName);
				strcpy(buf2, "K");
				write(pfd[1],buf2,80);
			}else if(strcmp(buf,"SJF")==0) {
				char InputFileName[60]={0};
				char OutputFileName[60]={0};
				strcpy(buf2, "K");
				write(pfd[3],buf2,80);

				n = read(fd[2],buf,80);
				buf[n] = 0;
				strcpy(InputFileName, buf);
				strcpy(buf2, "K");
				write(pfd[3],buf2,80);

				n = read(fd[2],buf,80);
				buf[n] = 0;
				strcpy(OutputFileName, buf);
				// printf("filename: %s,%s\n",InputFileName,OutputFileName);
				run_SJF(InputFileName,OutputFileName);
				strcpy(buf2, "K");
				write(pfd[3],buf2,80);

			}else if (strcmp(buf,"schedule")==0) {
				char infilename[50]={0};
				char outfilename[50]={0};
				char algorithm[10]={0};
				strcpy(buf2, "K");
				write(pfd[5],buf2,80);

				n = read(fd[4],buf,80);
				buf[n] = 0;
				strcpy(infilename, buf);
				strcpy(buf2, "K");
				write(pfd[5],buf2,80);

				n = read(fd[4],buf,80);
				buf[n] = 0;
				strcpy(outfilename, buf);
				strcpy(buf2, "K");
				write(pfd[5],buf2,80);


				n = read(fd[4],buf,80);
				buf[n] = 0;
				strcpy(algorithm, buf);
				print_schedule(infilename,outfilename,algorithm);
				strcpy(buf2, "K");
				write(pfd[5],buf2,80);

			}else{
				printf("Child %d Received something!\n",i);
				printf("%s\n",buf);
			}

		}
	}else{
		while (1) {
			input_standard_call();
			fgets(line,MAX_LINE_SIZE,stdin);
			parse(line,argv);
			// printf("%s\n",argv[0]);
			// printf("%d\n",strcmp(argv[0],endline));
			// printf("%d\n",strcmp(argv[1],"True!"));

			if (strcmp(argv[0],"endProgram")==0) {
				int i,n;
				for (i=0; i<TOTAL_CHILD_PROCESS; i++) {
					strcpy(buf, "end");
					n = strlen(buf);
					write(fd[2*i+1],buf,n);
					read(pfd[2*i],buf2,80);
				}
				wait(NULL);
				printf("Bye-bye!\n");
				remove("formattedForALS.log");
				remove("ALSFCFSoutfile.log");
				remove("ALSSJFoutfile.log");
				exit(0);

			}else if (strcmp(argv[0],"addBatchOrder")==0 && check_argv_numbers(argv,0,1)==0) {
				addBatchOrder(argv[1]);
				// printf("called addBatchOrder\n");

			}else if (strcmp(argv[0],"addOrder")==0 && check_argv_numbers(argv,0,5)==0) {
				//addOrder
				addOrder();
				// printf("called addOrder\n");

			}else if (strcmp(argv[0],"runALS")==0 && check_argv_numbers(argv,0,5)==0) { //TODO: clear this later!
				run_ALS();

				// printf("called runALS\n");
			}else if (strcmp(argv[0],"printReport")==0 && check_argv_numbers(argv,0,2)==0) {

				print_report("ALSFCFSoutfile.log",argv[2],"FCFS","w");
				print_report("ALSSJFoutfile.log",argv[2],"SJF","a");

				// print_report(argv[2],"FCFS","a");
				// print_report(argv[2],A);
				// printf("called printReport\n");
			}else{
				print_all_commands();

			}
		}
	}

	return 0;
}

void load_config(){
	// filein = fopen("line.conf","r");
	// int record_count=0;
	// if (filein!=NULL){
	// 	while(!feof(filein)||record_count>=MAX_PRODUCTION_LINES) {
	// 		fscanf(filein, "%s %s %s %s %s %s %s %s %s",production_line_equipment[record_count][0],production_line_equipment[record_count][1],production_line_equipment[record_count][2],production_line_equipment[record_count][3],production_line_equipment[record_count][4],production_line_equipment[record_count][5],production_line_equipment[record_count][6],production_line_equipment[record_count][7],production_line_equipment[record_count][8]);
	// 		record_count++;
	//
	// 	}
	// }

	filein = fopen("product_config","r");
	record_count=0;
	if (filein!=NULL){
		while(!feof(filein)||record_count>=MAX_PRODUCT_NOS) {
			fscanf(filein, "%s %s %s %s %s %s",product_requirement[record_count][0],product_requirement[record_count][1],product_requirement[record_count][2],product_requirement[record_count][3],product_requirement[record_count][4],product_requirement[record_count][5]);
			record_count++;

		}
	}


}

void addOrder(){
	strcpy(OrderNumber[current_last_record_index],argv[1]);
	strcpy(StartDate[current_last_record_index],argv[2]);
	strcpy(DueDate[current_last_record_index],argv[3]);
	strcpy(ProductRequired[current_last_record_index],argv[4]);
	strcpy(quantity[current_last_record_index],argv[5]);
	fileout = fopen("formattedForALS.log", "a");
	if (OrderNumber[current_last_record_index][0]=='R') {
		char temp[20];
		strcpy(temp, OrderNumber[current_last_record_index]+1);
		strcpy(OrderNumber[current_last_record_index], temp);
		// printf("Order No.:%s\n", OrderNumber[current_last_record_index]);
	}else{
		printf("Incorrect format of data!\n");
		return;
	}

	if (StartDate[current_last_record_index][0]=='D') {
		char temp[20];
		strcpy(temp, StartDate[current_last_record_index]+1);
		strcpy(StartDate[current_last_record_index], temp);
		// printf("Start Date:%s\n", StartDate[current_last_record_index]);
	}else{
		printf("Incorrect format of data!\n");
		return;
	}

	if (DueDate[current_last_record_index][0]=='D') {
		char temp[20];
		strcpy(temp, DueDate[current_last_record_index]+1);
		strcpy(DueDate[current_last_record_index], temp);
		if(atoi(DueDate[current_last_record_index])<atoi(StartDate[current_last_record_index])) {
			printf("Incorrect Date! Due date is earlier than start date!\n");
			return;
		}
		if(atoi(DueDate[current_last_record_index])>TOTAL_DAYS||atoi(StartDate[current_last_record_index])>TOTAL_DAYS){
			printf("Incorrect Date! Start date or Due date is after the period!\n");
			return;
		}
		// printf("Due Date:%s\n",DueDate[current_last_record_index]);
	}else{
		printf("Incorrect format of data!\n");
		return;
	}
	// printf("No. %d\n",strncmp(ProductRequired[current_last_record_index],"Product_",9));
	if (strncmp(ProductRequired[current_last_record_index],"Product_",8)==0) {
		// printf("Match!\n");
		char temp[20];
		strcpy(temp, ProductRequired[current_last_record_index]+8);
		strcpy(ProductRequired[current_last_record_index], temp);
		// printf("Product:%s\n",ProductRequired[current_last_record_index]);

	}else{
		printf("Incorrect format of data!\n");
		return;
	}
	fprintf(fileout,"%s %s %s %s %s\n",OrderNumber[current_last_record_index],StartDate[current_last_record_index],DueDate[current_last_record_index],ProductRequired[current_last_record_index],quantity[current_last_record_index]);

	fclose(fileout);
	current_last_record_index++;
}

void addBatchOrder(char InputFile[60]){ //InputFile[20] is the file name e.g. testpj.dat
	int k=0;
	int integ;

	filein = fopen(InputFile, "r");
	fileout = fopen("formattedForALS.log", "a");
	if (filein==NULL) {
		printf("Error! File not exist or it cannot be opened!\n");
		return;
	}
	while(!feof(filein)) {
		fscanf(filein, "%s %s %s %s %s", OrderNumber[current_last_record_index], StartDate[current_last_record_index], DueDate[current_last_record_index], ProductRequired[current_last_record_index], quantity[current_last_record_index]);
		if (current_last_record_index==MAX_ORDER_NOS) {
			printf("Max Orders reached! Not accepting more order!\n");
			break;
		}
		if (OrderNumber[current_last_record_index][0]=='R') {
			char temp[20];
			strcpy(temp, OrderNumber[current_last_record_index]+1);
			strcpy(OrderNumber[current_last_record_index], temp);
			// printf("Order No.:%s\n", OrderNumber[current_last_record_index]);
		}else{
			printf("Row %d: Incorrect format of data!\n",k+1);
			k++;
			continue;
		}

		if (StartDate[current_last_record_index][0]=='D') {
			char temp[20];
			strcpy(temp, StartDate[current_last_record_index]+1);
			strcpy(StartDate[current_last_record_index], temp);
			// printf("Start Date:%s\n", StartDate[current_last_record_index]);
		}else{
			printf("Row %d: Incorrect format of data!\n",k+1);
			k++;
			continue;
		}

		if (DueDate[current_last_record_index][0]=='D') {
			char temp[20];
			strcpy(temp, DueDate[current_last_record_index]+1);
			strcpy(DueDate[current_last_record_index], temp);
			if(atoi(DueDate[current_last_record_index])<atoi(StartDate[current_last_record_index])) {
				printf("Row %d: Incorrect Date! Due date is earlier than start date!\n",k+1);
				k++;
				continue;
			}
			if(atoi(DueDate[current_last_record_index])>TOTAL_DAYS||atoi(StartDate[current_last_record_index])>TOTAL_DAYS){
				printf("Row %d: Incorrect Date! Start date or Due date is after the period!\n",k+1);
				k++;
				continue;
			}

			// printf("Due Date:%s\n",DueDate[current_last_record_index]);
		}else{
			printf("Row %d: Incorrect format of data!\n",k+1);
			k++;
			continue;
		}
		// printf("No. %d\n",strncmp(ProductRequired[current_last_record_index],"Product_",9));
		if (strncmp(ProductRequired[current_last_record_index],"Product_",8)==0) {
			// printf("Match!\n");
			char temp[20];
			strcpy(temp, ProductRequired[current_last_record_index]+8);
			strcpy(ProductRequired[current_last_record_index], temp);
			// printf("Product:%s\n",ProductRequired[current_last_record_index]);

		}else{
			printf("Row %d: Incorrect format of data!\n",k+1);
			k++;
			continue;
		}
		fprintf(fileout,"%s %s %s %s %s\n",OrderNumber[current_last_record_index],StartDate[current_last_record_index],DueDate[current_last_record_index],ProductRequired[current_last_record_index],quantity[current_last_record_index]);
		current_last_record_index++;
		k++;
	}
	fclose(filein);
	fclose(fileout);
}

void input_standard_call(){
	printf("Please enter:\n");
	printf("> ");
}

void parse(char *line, char **argv){
	while (*line != '\0') {
		while (*line == ' ' || *line == '\t' || *line == '\n') {
			*line++ = '\0';
		}
		*argv++ = line;
		while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
			line++;
		}
	}
	*argv = '\0';                  /* mark the end of argument list  */
}

int check_argv_numbers(char **argv,int beginning,int ending){ //beginning set 0, ending = argv numbers
	int i;
	for (i=beginning; i<=ending; i++) {
		if (*argv[i]=='\0'||argv[i]==NULL) {
			return 1;
		}
	}
	return 0;

}

void print_all_commands(){
	printf("\n");
	printf("Available Commands:\n");
	printf("addOrder [order number] [expected start date] [expected due date] [product required] [quantity required]\n");
	printf("addBatchOrder [name of the batch file]\n");
	printf("runALS –[algorithm] | printSchedule > [output file name]\n");
	printf("printReport > [name of the output file]\n");
	printf("endProgram\n");
	printf("\n");
}

void run_ALS(){
	int i,j,k,n;
	char buf[80]={0};
	char buf2[80]={0};
	char file_name[80];

	char *parameter_flag;
	char *algorithm_flag;


	algorithm_flag = argv[1];
	// parameter_flag = argv[2];

	if (current_last_record_index==0) {
		printf("There is no data!\n");
		return;
	}
	if (strcmp(argv[2],"|")==0 && strcmp(argv[3],"printSchedule")==0 && strcmp(argv[4],">")==0) {
		remove("ALSoutfile.log");
		if (strcmp(algorithm_flag,"-FCFS")==0) {
			// printf("Going to run FCFS!\n");
			strcpy(buf, "FCFS");
			n = strlen(buf);
			// printf("Buf: %s\n",buf);
			write(fd[1],buf,n);
			read(pfd[0],buf2,80);

			strcpy(buf, "formattedForALS.log");
			n = strlen(buf);
			write(fd[1],buf,n);
			read(pfd[0],buf2,80);

			strcpy(buf, "ALSFCFSoutfile.log");
			n = strlen(buf);
			write(fd[1],buf,n);
			read(pfd[0],buf2,80);

			// after this , it prints the report
			strcpy(buf, "schedule");
			n = strlen(buf);
			write(fd[5],buf,n);
			read(pfd[4],buf2,80);

			strcpy(buf, "ALSFCFSoutfile.log");
			n = strlen(buf);
			write(fd[5],buf,n);
			read(pfd[4],buf2,80);

			strcpy(buf, argv[5]);
			n = strlen(buf);
			write(fd[5],buf,n);
			read(pfd[4],buf2,80);

			strcpy(buf, "FCFS");
			n = strlen(buf);
			write(fd[5],buf,n);
			read(pfd[4],buf2,80);


			return;
		} else if (strcmp(algorithm_flag,"-SJF")==0) {
			// printf("Going to run SJF!\n");
			strcpy(buf, "SJF");
			n = strlen(buf);
			// printf("Buf: %s\n",buf);
			write(fd[3],buf,n);
			read(pfd[2],buf2,80);

			strcpy(buf, "formattedForALS.log");
			n = strlen(buf);
			write(fd[3],buf,n);
			read(pfd[2],buf2,80);

			strcpy(buf, "ALSSJFoutfile.log");
			n = strlen(buf);
			write(fd[3],buf,n);
			read(pfd[2],buf2,80);

			// after this , it prints the report
			strcpy(buf, "schedule");
			n = strlen(buf);
			write(fd[5],buf,n);
			read(pfd[4],buf2,80);

			strcpy(buf, "ALSSJFoutfile.log");
			n = strlen(buf);
			write(fd[5],buf,n);
			read(pfd[4],buf2,80);

			strcpy(buf, argv[5]);
			n = strlen(buf);
			write(fd[5],buf,n);
			read(pfd[4],buf2,80);

			strcpy(buf, "SJF");
			n = strlen(buf);
			write(fd[5],buf,n);
			read(pfd[4],buf2,80);


			return;

		}
	}
	print_all_commands();




	// for (i=1; i<current_last_record_index; i++) {
	//  for (j = i; j > 0 && atoi(StartDate[i]) < atoi(StartDate[order_index[j-1]]); j--) {
	//      order_index[j] = order_index[j-1];
	//  }
	//  order_index[j] = i;
	// }

	// printf("Argv 1: %s\n",argv[1]);
	// strcpy(buf, argv[1]);
	// n = strlen(buf);
	// for (i=0; i<TOTAL_CHILD_PROCESS; i++) {
	//  write(fd[2*i+1],buf,n);
	//  read(pfd[2*i],buf2,80);
	// }
	//
	//
	//
	// order_index[0] = 0;
}

void run_FCFS(char InFileName[60], char OutFileName[60]){
	int record_count =0;
	int i,j;
	int day = 1;
	char OrderNumber[MAX_ORDER_NOS][20]={0};
	char StartDate[MAX_ORDER_NOS][20]={0};
	char EndDate[MAX_ORDER_NOS][20]={0};
	char DueDate[MAX_ORDER_NOS][20]={0};
	char ProductRequired[MAX_ORDER_NOS][20]={0};
	char quantity[MAX_ORDER_NOS][10]={0};
	int orderStatus[MAX_ORDER_NOS]={0};
	int order_index[MAX_ORDER_NOS];
	// char *assembly_line_dates[MAX_PRODUCTION_LINES][5];
	int assembly_line_dates[MAX_PRODUCTION_LINES]={1};
	// printf("%s\n",InputFile);
	filein = fopen(InFileName, "r");
	fileout = fopen(OutFileName,"w");
	if (filein==NULL) {
		printf("Error! File not exist or it cannot be opened!\n");
		return;
	}
	while(!feof(filein)) {
		fscanf(filein, "%s %s %s %s %s", OrderNumber[record_count], StartDate[record_count], DueDate[record_count], ProductRequired[record_count], quantity[record_count]);
		record_count++;
	}
	record_count--;
	// printf("Record count: %d\n",record_count);

	order_index[0] = 0;

	for (i=1; i<record_count; i++) {
		for (j = i; j > 0 && atoi(StartDate[i]) < atoi(StartDate[order_index[j-1]]); j--) {
			order_index[j] = order_index[j-1];
		}
		order_index[j] = i;
	}

	// for (i=0; i<record_count; i++) {
	// 	printf("%d ",order_index[i]);
	// }
	// printf("\n");
	// printf("Status: ");
	// for (i=0; i<record_count; i++) {
	// 	printf("%d ",orderStatus[i]);
	// }
	// printf("\n");
	for (day=1; day<TOTAL_DAYS; day++) {
		for (i=0; i<record_count; i++) {
			int days_needed;
			// printf("Start date of %d: %d\n",i,atoi(StartDate[order_index[i]]));
			// printf("Qty of %d: %s\n",i,quantity[order_index[i]]);
			// printf("Qty of %d: %d\n",i,atoi(quantity[order_index[i]]));

			if (atoi(StartDate[order_index[i]]) <= day && orderStatus[order_index[i]] == 0) { //the order is going ot take that!
				if(atoi(quantity[order_index[i]]) > ((atoi(DueDate[order_index[i]]) - day) * DAILY_PRODUCTION_CAPACITY)) {
					orderStatus[order_index[i]] = -1; //Reject orders that is unable to make.
					sprintf(EndDate[order_index[i]], "%03d", -1);

				}else{
					if (atoi(quantity[order_index[i]])%DAILY_PRODUCTION_CAPACITY !=0) {
						days_needed = atoi(quantity[order_index[i]])/DAILY_PRODUCTION_CAPACITY+1;
					}else{
						days_needed = atoi(quantity[order_index[i]])/DAILY_PRODUCTION_CAPACITY;
					}
					// printf("Days needed by %d: %d\n",i,days_needed);
					for (j=0; j<MAX_PRODUCTION_LINES; j++) {
						if (assembly_line_dates[j]<=day){
						// if (atoi(StartDate[order_index[i]])>=assembly_line_dates[j]) {
							assembly_line_dates[j]=day+days_needed;
							orderStatus[order_index[i]]=(j+1);
							sprintf(StartDate[order_index[i]], "%03d", day);
							sprintf(EndDate[order_index[i]], "%03d", assembly_line_dates[j]);
							// printf("Production line %d: Order %03d, production lines at Day:  %d \n",j,order_index[i],assembly_line_dates[j]);
							break;
						}
					}

					if (orderStatus[order_index[i]]==0) {
						sprintf(EndDate[order_index[i]], "%03d", -1);
					}
				}

			}
		}
	}
	// printf("Status: ");
	// for (i=0; i<record_count; i++) {
	// 	printf("%d ",orderStatus[i]);
	// }
	// printf("\n");

	// for (i=1; i<record_count; i++) {
	//  printf("%d ",order_index[i]);
	// }



	printf("\n");

	for (i=0; i<record_count; i++) {
		fprintf(fileout,"%s %s %s %s %s %d\n",OrderNumber[i],StartDate[i],EndDate[i],DueDate[i],quantity[i],orderStatus[i]);
	}
	fclose(filein);
	fclose(fileout);

}

void run_SJF(char InFileName[60], char OutFileName[60]){
	int record_count =0;
	int i,j,k;
	int day = 1;
	char OrderNumber[MAX_ORDER_NOS][20]={0};
	char StartDate[MAX_ORDER_NOS][20]={0};
	char EndDate[MAX_ORDER_NOS][20]={0};
	char DueDate[MAX_ORDER_NOS][20]={0};
	char ProductRequired[MAX_ORDER_NOS][20]={0};
	char quantity[MAX_ORDER_NOS][10]={0};
	int orderStatus[MAX_ORDER_NOS]={0};
	int order_index[MAX_ORDER_NOS];
	// char *assembly_line_dates[MAX_PRODUCTION_LINES][5];
	int assembly_line_dates[MAX_PRODUCTION_LINES]={1};
	// printf("%s\n",InputFile);
	filein = fopen(InFileName, "r");
	fileout = fopen(OutFileName,"w");
	if (filein==NULL) {
		printf("Error! File not exist or it cannot be opened!\n");
		return;
	}
	while(!feof(filein)) {
		fscanf(filein, "%s %s %s %s %s", OrderNumber[record_count], StartDate[record_count], DueDate[record_count], ProductRequired[record_count], quantity[record_count]);
		record_count++;
	}
	record_count--;
	printf("Record count: %d\n",record_count);

	order_index[0] = 0;

	//TODO: change this to quantity
	for (i=1; i<record_count; i++) {
		for (j = i; j > 0 && atoi(StartDate[i]) < atoi(StartDate[order_index[j-1]]); j--) {
			order_index[j] = order_index[j-1];
		}
		order_index[j] = i;
	}

	// for (i=0; i<record_count; i++) {
	// 	printf("%d ",order_index[i]);
	// }

	// for (i=0; i<record_count; i++) {
	// 	printf("%s %s %s %s %s %d\n",OrderNumber[i],StartDate[i],EndDate[i],DueDate[i],quantity[i],orderStatus[i]);
	// }
	// printf("\n");
	// printf("Status: ");
	// for (i=0; i<record_count; i++) {
	// 	printf("%d ",orderStatus[i]);
	// }
	// printf("\n");

	//here
	for (day=1; day<TOTAL_DAYS; day++) {
		// int lines_status[MAX_PRODUCTION_LINES]={0};
		int i;
		 //HERE!
		for (i=0; i<record_count;i++){
			if (atoi(StartDate[order_index[i]]) <= day && orderStatus[order_index[i]] == 0) { //the order is going ot take that!
				if(atoi(quantity[order_index[i]]) > ((atoi(DueDate[order_index[i]]) - day) * DAILY_PRODUCTION_CAPACITY)) {
					orderStatus[order_index[i]] = -1; //Reject orders that is unable to make.
					sprintf(EndDate[order_index[i]], "%03d", -1);
				}
			}
		}

		for (i=0;i<MAX_PRODUCTION_LINES;i++){
			if (assembly_line_dates[i]<=day){ //available
				// printf("Line %d available in day %d!\n",i+1,day);
				int least_quantity_index = -1; //indicates no one is the least quantity.
				for (j=0; j<record_count; j++) {
					if (orderStatus[order_index[j]] == 0&&(atoi(StartDate[order_index[j]])<=day)) {
						if (least_quantity_index == -1){
							least_quantity_index=order_index[j];
						}else if (atoi(quantity[order_index[j]])<atoi(quantity[least_quantity_index])){
							least_quantity_index=order_index[j];
						}
					}
				}
				if (least_quantity_index!=-1){ //got available!
					int days_needed;
					if (atoi(quantity[least_quantity_index])%DAILY_PRODUCTION_CAPACITY !=0) {
						days_needed = atoi(quantity[least_quantity_index])/DAILY_PRODUCTION_CAPACITY+1;
					}else{
						days_needed = atoi(quantity[least_quantity_index])/DAILY_PRODUCTION_CAPACITY;
					}
					assembly_line_dates[i]=day+days_needed;
					orderStatus[least_quantity_index]=(i+1);
					sprintf(StartDate[least_quantity_index], "%03d", day);
					sprintf(EndDate[least_quantity_index], "%03d", assembly_line_dates[i]);
					// printf("Production line %d took order %d\n",i,least_quantity_index);
				}
			}
		}


		// int least_quantity_index = -1; //indicates no one is the least quantity.
		//
		// for (i=0; i<record_count; i++) {
		// 	int days_needed;
		// 	//TODO: check how many orders on that day, let quantity smaller goes first.
		// 	if (atoi(StartDate[order_index[i]]) <= day && orderStatus[order_index[i]] == 0) { //the order is going ot take that!
		// 		if(atoi(quantity[order_index[i]]) > ((atoi(DueDate[order_index[i]]) - day) * DAILY_PRODUCTION_CAPACITY)) {
		// 			orderStatus[order_index[i]] = -1; //Reject orders that is unable to make.
		// 			sprintf(EndDate[order_index[i]], "%03d", -1);
		//
		// 		}else{
		// 			if (least_quantity_index==-1){
		// 				least_quantity_index=i;
		// 			}else if (){
		//
		// 			}
		//
		// 			if (atoi(quantity[order_index[i]])%DAILY_PRODUCTION_CAPACITY !=0) {
		// 				days_needed = atoi(quantity[order_index[i]])/DAILY_PRODUCTION_CAPACITY+1;
		// 			}else{
		// 				days_needed = atoi(quantity[order_index[i]])/DAILY_PRODUCTION_CAPACITY;
		// 			}
		// 			// printf("Days needed by %d: %d\n",i,days_needed);
		// 			for (j=0; j<MAX_PRODUCTION_LINES; j++) {
		// 				if (atoi(StartDate[order_index[i]])>=assembly_line_dates[j]) {
		// 					assembly_line_dates[j]=day+days_needed;
		// 					orderStatus[order_index[i]]=(j+1);
		// 					sprintf(EndDate[order_index[i]], "%03d", assembly_line_dates[j]);
		// 					// printf("Production line %d: Order %03d, production lines at Day:  %d \n",j,order_index[i],assembly_line_dates[j]);
		// 					break;
		// 				}
		// 			}
		//
		// 			if (orderStatus[order_index[i]]==0) {
		// 				sprintf(EndDate[order_index[i]], "%03d", -1);
		// 			}
		// 		}
		//
		// 	}
		// }
	}
	printf("\n");
	// printf("Status: ");
	// for (i=0; i<record_count; i++) {
	// 	printf("%d ",orderStatus[i]);
	// }
	// printf("\n");

	for (i=0; i<record_count; i++) {
		fprintf(fileout,"%s %s %s %s %s %d\n",OrderNumber[i],StartDate[i],EndDate[i],DueDate[i],quantity[i],orderStatus[i]);
	}


	fclose(filein);
	fclose(fileout);
}

void run_print_report(){

}
void print_schedule(char infilename[60],char outfilename[60],char *algorithm){
	int i,j;
	// char *temp;
	int record_count =0;

	char OrderNumber[MAX_ORDER_NOS][20]={0};
	char StartDate[MAX_ORDER_NOS][20]={0};
	char EndDate[MAX_ORDER_NOS][20]={0};
	char DueDate[MAX_ORDER_NOS][20]={0};
	char ProductRequired[MAX_ORDER_NOS][20]={0};
	char quantity[MAX_ORDER_NOS][10]={0};
	int orderStatus[MAX_ORDER_NOS]={0};
	int order_index[MAX_ORDER_NOS];
	// char *assembly_line_dates[MAX_PRODUCTION_LINES][5];
	int assembly_line_dates[MAX_PRODUCTION_LINES]={1};



	filein = fopen(infilename,"r");
	fileout = fopen(outfilename,"w");
	while(!feof(filein)) {
		fscanf(filein, "%s %s %s %s %s %d", OrderNumber[record_count], StartDate[record_count], EndDate[record_count], DueDate[record_count], quantity[record_count], &orderStatus[record_count]);
		record_count++;
	}
	record_count--;


	for (i=0; i<MAX_PRODUCTION_LINES; i++) {
		int days_in_use = 0;
		int orders_accepted=0;

		char temp_order_no[10];
		char temp_start_date[10];
		char temp_end_date[10];
		char temp_due_date[10];
		fprintf(fileout,"Assembly Line %d\n",i+1);
		fprintf(fileout,"\n");
		fprintf(fileout,"Algorithm: %s\n",algorithm);
		fprintf(fileout,"Start date: D001\n");
		fprintf(fileout,"End date: D060\n");
		fprintf(fileout,"\n");
		fprintf(fileout,"%15s %15s %15s %15s %25s %25s\n","Order Number","Start Date","End Date","Due Date","Quantity Required","Quantity Produced");
		fprintf(fileout,"-------------------------------------------------------------------------------------------------------------------\n");
		for (j=0; j<record_count; j++) {
			if (orderStatus[j]==i+1) {
				strcpy(temp_order_no,"R");
				strcat(temp_order_no,OrderNumber[j]);
				strcpy(temp_start_date,"D");
				strcat(temp_start_date,StartDate[j]);
				strcpy(temp_end_date,"D");
				strcat(temp_end_date,EndDate[j]);
				strcpy(temp_due_date,"D");
				strcat(temp_due_date,DueDate[j]);
				fprintf(fileout,"%15s %15s %15s %15s %25s %25s\n",temp_order_no, temp_start_date, temp_end_date, temp_due_date, quantity[j], quantity[j]);
				days_in_use+=(atoi(EndDate[j])-atoi(StartDate[j]));
				orders_accepted++;
			}
		}
		fprintf(fileout,"\n");

		fprintf(fileout,"%-35s %d\n","Total number of working days:",TOTAL_DAYS);
		fprintf(fileout,"%-35s %d\n","Orders accepted:",orders_accepted);
		fprintf(fileout,"%-35s %d\n","Days not in use:",TOTAL_DAYS-days_in_use);
		fprintf(fileout,"%-35s %d\n","Days in use:",days_in_use);
		fprintf(fileout,"%-35s %.2f%%\n","Utilisation:",((float)days_in_use/TOTAL_DAYS*100));

		fprintf(fileout,"\n");
		fprintf(fileout,"\n");
		fprintf(fileout,"\n");

	}
	fclose(filein);
	fclose(fileout);
	return;
}


void print_report(char infile_name[],char outfile_name[],char *algorithm,char *fileout_flag){
	// FILE *fp;
	int i,j;
	int record_count=0;
	int total_working_days=0;
	int accepted_order_nos =0 ;
	float average_working_days;
	float total_utilisation=0;

	char OrderNumber[MAX_ORDER_NOS][20]={0};
	char StartDate[MAX_ORDER_NOS][20]={0};
	char EndDate[MAX_ORDER_NOS][20]={0};
	char DueDate[MAX_ORDER_NOS][20]={0};
	char ProductRequired[MAX_ORDER_NOS][20]={0};
	char quantity[MAX_ORDER_NOS][10]={0};
	int orderStatus[MAX_ORDER_NOS]={0};
	int order_index[MAX_ORDER_NOS];
	// char *assembly_line_dates[MAX_PRODUCTION_LINES][5];
	int assembly_line_dates[MAX_PRODUCTION_LINES]={1};

	//run FCFS first
	filein = fopen(infile_name, "r");
	if (filein ==NULL){
		if(strcmp(fileout_flag,"w")==0){
			remove(outfile_name);
		}
		fclose(filein);
		return;
	}
	fileout= freopen (outfile_name, fileout_flag, stdout);



	while(!feof(filein)) {
		fscanf(filein, "%s %s %s %s %s %d", OrderNumber[record_count], StartDate[record_count], EndDate[record_count], DueDate[record_count], quantity[record_count], &orderStatus[record_count]);
		if (orderStatus[record_count]>0){
			accepted_order_nos++;
		}
		record_count++;
	}
	record_count--;

	printf("***Summary of Schedules*** \n\n");
	printf("Algorithm used: %s \n\n",algorithm);
	printf("There are %d orders scheduled in total. Details are as follows: \n\n",accepted_order_nos);
	printf("Assembly Line | Order Accepted | Working Day | Utilization \n");
	printf("========================================================================================= \n");

	for (i=0;i<MAX_PRODUCTION_LINES;i++){
		int this_working_days=0;
		int this_orders_accepted=0;
		float this_utilisation=0.0;
		for (j=0;j<record_count;j++){
			if (orderStatus[j]==i+1) {
				this_working_days+=(atoi(EndDate[j])-atoi(StartDate[j]));
				this_orders_accepted++;
			}
		}
		total_working_days +=this_working_days;
		this_utilisation=(float)this_working_days/TOTAL_DAYS;
		total_utilisation+=this_utilisation;


		printf("Line_%d		%0d		%0d		%0.1lf%% \n\n",i+1,this_orders_accepted,this_working_days,this_utilisation*100);
	}

	// printf("Line_1		%0.0lf		%0.0lf		%0.1lf \n\n",line_detail[0],line_detail[1],line_detail[2]);
	// printf("Line_2		%0.0lf		%0.0lf		%0.1lf \n\n",line_detail[3],line_detail[4],line_detail[5]);
	// printf("Line_3		%0.0lf		%0.0lf		%0.1lf \n",line_detail[6],line_detail[7],line_detail[8]);
	printf("========================================================================================= \n\n");

	printf("***PERFORMANCE***\n\n");
	printf("AVERAGE OF WORKING DAYS FOR THE %d ASSEMBLY LINES:  %0.1lf DAYS \n", MAX_PRODUCTION_LINES,(float)total_working_days/MAX_PRODUCTION_LINES );
	printf("AVERAGE OF UTILIZATION:                            %0.1lf%%  \n\n",((float)total_utilisation/3*100));
	printf("TOTAL WORKING DAYS OF THE %d ASSEMBLY LINES:        %d DAYS \n",MAX_PRODUCTION_LINES,total_working_days);
	printf("UTILIZATION OF THE %d ASSEMBLY LINES:               %0.1lf%%  \n\n",MAX_PRODUCTION_LINES,((float)total_utilisation/3*100));

	printf("***Order Rejected List***\n\n");
	printf(" - ORDER ACCEPTED:  %d\n", accepted_order_nos);
	printf(" - ORDER REJECTED:  %d\n\n", record_count-accepted_order_nos);

	printf("REJECTED ORDER LIST\n");
	printf("=================================\n");
	for (i = 0; i < record_count; i++ ) {
		if (orderStatus[i]==0 || orderStatus[i]==-1) {
			printf("%-6s %-5s %-5s %-6s\n",OrderNumber[i],StartDate[i],DueDate[i],quantity[i]);
		}
	}
	printf("\n\nThere are %d Orders rejected.\n\n\n",record_count-accepted_order_nos);

	fclose (fileout);
	freopen("/dev/tty", "w", stdout);
}
