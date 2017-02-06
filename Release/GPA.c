//
//  GPA.c
//  Lab1
//
//  Created by chungonion on 25/1/2017.
//  Copyright © 2017年 chungonion.
//
// This code is a bit hard-coded in some extent.
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char *argv[]){
	int num_subj;
	float in_gp, sum_gp = 0.0;
	char in_grade, grade[10];
	int i;

	printf("This program name is %s\n",argv[0]); //argv[0] are the first argv, that's the program file name itself
	num_subj = argc -1;     //argc stored the number of arguments provided from the compiler
	//and we have to exclude the argv[0], since that's the program name

	printf("PolyU:\n");
	int num_valid_subj = 0;
	for (i=1; i<=num_subj; i++) { //excluding argv[0], and therefore begins from argv[1]
		int valid = 1;
		printf("Grade for the subject %d is %s, GP ", i, argv[i]);
		in_grade = argv[i][0];
		switch (in_grade) {
		case 'A':
			in_gp = 4.0; //PolyU Scale, A=4, B=3, C=2, D=1, and F=0
			break;
		case 'B':
			in_gp = 3.0;
			break;
		case 'C':
			in_gp = 2.0;
			break;
		case 'D':
			in_gp = 1.0;
			break;
		case 'F':
			in_gp = 0.0;
			break;
		default:
			valid = 0; //Other grades consider as invalid.
		}
		if (argv[i][2]==0 || argv[i][1]==0) {
			if (argv[i][1]=='+' && valid==1) { //If there's a "+", and valid, add GPA by1
				in_gp = in_gp+0.5;
				sum_gp = sum_gp+in_gp;
				num_valid_subj++;
				printf("%5.2f",in_gp);
			}else if (argv[i][1]==0 && valid==1) {
				sum_gp = sum_gp+in_gp;
				num_valid_subj++;
				printf("%5.2f",in_gp);
			}else{
				printf("invalid");
			}
		}else{
			printf("invalid");
		}
		printf("\n");

	}
	if (num_valid_subj!=0) {
		printf("Your GPA for %d subjects is %5.2f\n", num_valid_subj, sum_gp/num_valid_subj);
	}else{
		printf("Your GPA for 0 subjects is 0\n");
	}

	printf("Some U:\n");
	num_valid_subj = 0;
	sum_gp = 0;
	for (i=1; i<=num_subj; i++) {
		int valid = 1;
		printf("Grade for the subject %d is %s, GP ", i, argv[i]);
		if (argv[i][2]==0 || argv[i][1]==0) {
			switch (argv[i][0]) {
			case 'A':
				switch(argv[i][1]) {
				case '+':
					in_gp=12.0;
					break;
				case '-':
					in_gp=10.0;
					break;
				default:
					in_gp=11.0;
				}
				break;
			case 'B':
				switch(argv[i][1]) {
				case '+':
					in_gp=9.0;
					break;
				case '-':
					in_gp=7.0;
					break;
				default:
					in_gp=8.0;
				}
				break;
			case 'C':
				switch(argv[i][1]) {
				case '+':
					in_gp=6.0;
					break;
				case '-':
					in_gp=4.0;
					break;
				default:
					in_gp=5.0;
				}
				break;
			case 'D':
				switch(argv[i][1]) {
				case '+':
					valid = 0;
					break;
				case '-':
					valid = 0;
					break;
				default:
					in_gp = 2.0;
				}
				break;
			case 'F':
				in_gp = 0.0;
				break;
			default:
				valid = 0;
			}
			if (valid == 1) {
				sum_gp +=in_gp;
				printf("%5.2f",in_gp);
				num_valid_subj++;
			}else{
				printf("invalid");
			}
		}else{
			printf("invalid");
		}
		printf("\n");
	}
	if (num_valid_subj!=0) {
		printf("Your GPA for %d subjects is %5.2f\n", num_valid_subj, sum_gp/num_valid_subj);
	}else{
		printf("Your GPA for 0 subjects is 0\n");
	}

	return 0;

}
