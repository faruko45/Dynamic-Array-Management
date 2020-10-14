#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_SIZE_STRING 100
#define MAX_SIZE_INT 64

typedef struct department{ //department structure
	char name[MAX_SIZE_STRING];
	char manager_name[MAX_SIZE_STRING];
	unsigned int employee_count;
	int flag;  //keeps deleted or not
}Department;

typedef struct company{ //company structure
	char name[MAX_SIZE_STRING];
	char founder[MAX_SIZE_STRING];
	unsigned int foundationYear;
	int market_price;
	Department** departments; 
	int department_count; //number of departments
	int flag; //keeps deleted or not
}Company;

Company* add_company(char name[], char founder[], unsigned int foundationYear, int market_price)
{
	Company* new_company = (Company*)malloc(sizeof(Company));
	strcpy(new_company->name,name);
	strcpy(new_company->founder,founder);
	new_company->foundationYear = foundationYear;
	new_company->market_price = market_price;
	new_company->flag = 0;
	new_company->department_count = 0;
	new_company->departments = NULL; //prevents errors
	return new_company;
}
//searchs the company according to the name and returns its address. If there is not a company such that returns NULL
Company* search_company(Company** list, int company_count, char name[]){
	int i;
	for(i = 0; i < company_count; i++)
	{
		if(strcmp(list[i]->name,name) == 0) //search according to the title if finds then return the company address
		{
			return list[i];
		}
	}
	return NULL; //else returns null
}

void normalizeString(char s[]) //this function normalizes the strings which got from fgets()
{
	if(s[strlen(s) - 2] == '\r' || s[strlen(s) - 2] == '\n')
		s[strlen(s)-2] = '\0';
	else if(s[strlen(s)- 1] == '\r' || s[strlen(s) - 1] == '\n')
		s[strlen(s)- 1] = '\0';
}

//deletes the company according to the given name.
void delete_company(Company** company_list, char name[], int company_count)
{
	Company* temporal = NULL;//keeps the address of the company which will be deleted
	temporal = search_company(company_list, company_count, name);
	if(temporal)//if the company exists with given name
	{
		temporal->flag = 1; //delete
		printf("Succesfully deleted.\n");
	}
	else
	{
		printf("invalid name.\n");
	}
}

//undeletes the company according to the given name.
void undelete_company(Company** company_list, char name[], int company_count)
{
	Company* temporal = NULL;//keeps the address of the company which will be undeleted
	temporal = search_company(company_list, company_count, name);
	if(temporal)//if the company exists with given name
	{
		temporal->flag = 0; //undelete
		printf("Succesfully undeleted.\n");
	}
	else
	{
		printf("invalid name.\n");
	}
}

void add_departments(Company* company) //adds departments to the target company
{
	int dep_count; //keeps the number of departments
	printf("Please enter the count of departments which will be added.\n");
	scanf("%d",&dep_count);
	company->department_count = dep_count; //assings the department count to the related company
	int i, emp_count;
	char str1[MAX_SIZE_STRING], str2[MAX_SIZE_STRING], dummy;//dummy variable is used to eliminate some dangerous errors
	company->departments = (Department**)malloc(company->department_count*sizeof(Department*)); //memory for departments
	for(i = 0; i < company->department_count; i++)//iterates until department count
	{
		Department* new_department = (Department*)malloc(sizeof(Department)); //memory for one department
		printf("Please type the name of department\n");
		scanf("%c",&dummy); //this line is used to prevent an error 
		fgets(str1,MAX_SIZE_STRING,stdin);
		normalizeString(str1);
		strcpy(new_department->name,str1);
		printf("Please type the manager name of department\n");
		fgets(str2,MAX_SIZE_STRING,stdin);
		normalizeString(str2);
		strcpy(new_department->manager_name,str2);
		printf("Please enter the employee count of department\n");
		scanf("%d",&emp_count);
		new_department->employee_count = emp_count;
		new_department->flag = 0;
		company->departments[i] = new_department;
		//this for loop takes all department features, assigns them and adds to the related company
	}
}

void print_departments(Company* company) //prints departmants of a selected company
{
	int i;
	for(i = 0; i < company->department_count; i++)
	{
		if(company->departments[i]->flag == 0)
		{
			printf("Department name: %s Manager name: %s Employee count: %d\n",company->departments[i]->name,company->departments[i]->manager_name,company->departments[i]->employee_count);
		}
	}
}
//deletes the department with given name
void delete_department(Company* company, char name[])
{
	int i;
	for(i = 0; i < company->department_count; i++) //searchs for title
	{
		if(strcmp(company->departments[i]->name,name) == 0)
		{
			company->departments[i]->flag = 1;
			printf("Succesfully deleted.\n");
		}
	}
}
//undeletes the department with given name
void undelete_department(Company* company, char name[])
{
	int i;
	for(i = 0; i < company->department_count; i++)//searchs for title
	{
		if(strcmp(company->departments[i]->name,name) == 0)
		{
			company->departments[i]->flag = 0;
			printf("Succesfully undeleted.\n");
		}
	}
}

void import_exportCompanies(Company** comp_list, int* company_count, int mode) //file operations. This function writes and reads binary
{
	char file_name[MAX_SIZE_STRING], data[MAX_SIZE_STRING];
	printf("Please enter the file name\n");
	scanf("%s",file_name);
	if(mode == 0) //Reading
	{
		FILE* fp = fopen(file_name,"rb");
		if(fp)
		{
			Company* new_company = (Company*)malloc(sizeof(Company)); //memory for company
			int comp_count = 0;
			while(fread(new_company,sizeof(Company),1,fp) == 1) //read a company
			{
				new_company->departments = (Department**)malloc(new_company->department_count*sizeof(Department*)); // memory for departments of company
				int i;
				for(i = 0; i < new_company->department_count; i++) // reads departments
				{
					Department* new_department = (Department*)malloc(sizeof(Department)); // memory for department
					if(fread(new_department,sizeof(Department),1,fp) == 1)
					{
						new_company->departments[i] = new_department;
					}
				}
				comp_list[comp_count] = new_company;
				comp_count++;
				*company_count = comp_count; //update company count
			}
		}
		else
		{
			printf("File cannot be opened.\n");
		}
	}
	else if(mode == 1) //Writing
	{
		FILE* fp = fopen(file_name,"wb");
		if(fp)
		{
			int i;
			for(i = 0; i < *company_count; i++)
			{
				fwrite(comp_list[i],sizeof(Company),1,fp);//writes company
				int j;
				for(j = 0; j < comp_list[i]->department_count; j++)
				{
					fwrite(comp_list[i]->departments[j],sizeof(Department),1,fp);//writes deparments one by one
				}
			}
		}
		else
		{
			printf("File cannot be opened.\n");
		}
	
	}
}

void print_company(Company* item)
{
	printf("***********************************\n");
	printf("Company name: %s\nFounder Name: %s\nFoundation Year: %d\nMarket Price: %d\n",item->name, item->founder, item->foundationYear, item->market_price);
	printf("--------------------------------\n");
	print_departments(item);
	printf("***********************************\n");
}

//prints all company list according to their flag status
void print_list(Company** comp_list, int company_count)
{
	int i,j = 0;
	for(i = 0; i < company_count; i++)
	{
		if(comp_list[i]->flag == 0)
		{
			print_company(comp_list[i]);
			j++;
		}
	}
	if(j == 0)
	{
		printf("List is empty.\n");
	}
}

//deletes all list physically to avoid memory leaks
void delete_list(Company** comp_list, int comp_number)
{
	int i,j;
	for(i = 0; i < comp_number; i++)
	{
		for(j = 0; j < comp_list[i]->department_count; j++)
		{
			free(comp_list[i]->departments[j]);
		}
		free(comp_list[i]);
	}
}

int print_menu(Company** comp_list, int* company_count)
{
	printf("1.\tAdd a company with its departments to list.\n2.\tDelete a company.\n3.\tUndelete a company.\n4.\tSearch for a company.\n5.\tDelete deparments from a company\n6.\tUndelete deparments from a company\n7.\tPrint whole company list\n8.\tImport list from a file.\n9.\tExport list to a file\nType -1 to exit program.\n");
	int choice;
	char dummy;
	scanf("%d",&choice);
	if(choice == 1){ //asks values and adds company with its departments to the list
		char comp_name[MAX_SIZE_STRING], founder_name[MAX_SIZE_STRING];
		int foundationYear, marketPrice;
		char dummy;
		Company* new_company = (Company*)malloc(sizeof(Company));
		printf("Please enter the name of company.\n");
		scanf("%c",&dummy);
		fgets(comp_name,MAX_SIZE_STRING,stdin);
		normalizeString(comp_name);
		printf("Please enter the founder name of company.\n");
		fgets(founder_name,MAX_SIZE_STRING,stdin);
		normalizeString(founder_name);
		printf("Please enter the foundation year and market price of company.\n");
		scanf("%d %d",&foundationYear,&marketPrice);
		new_company->market_price = marketPrice;
		new_company->foundationYear = foundationYear;
		strcpy(new_company->name,comp_name);
		strcpy(new_company->founder,founder_name);
		new_company->departments = NULL;
		new_company->department_count = 0;
		add_departments(new_company);
		comp_list[*company_count] = new_company;
		(*company_count) = (*company_count) + 1;
	}
	else if(choice == 2){ //deletes company by using delete_company() function
		char target_name[MAX_SIZE_STRING];
		printf("Please enter the name of company.\n");
		scanf("%c",&dummy);
		fgets(target_name,MAX_SIZE_STRING,stdin);
		normalizeString(target_name);
		delete_company(comp_list, target_name, *company_count);
	}
	else if(choice == 3) //undeletes company by using undelete_company() function
	{
		char target_name[MAX_SIZE_STRING];
		printf("Please enter the name of company.\n");
		scanf("%c",&dummy);
		fgets(target_name,MAX_SIZE_STRING,stdin);
		normalizeString(target_name);
		undelete_company(comp_list, target_name, *company_count);
	}
	else if(choice == 4){ //searches for company with given name
		char target_name[MAX_SIZE_STRING];
		printf("Please enter the name of company.\n");
		scanf("%c",&dummy);
		fgets(target_name,MAX_SIZE_STRING,stdin);
		normalizeString(target_name);
		Company* t_comp = search_company(comp_list,*company_count,target_name);
		if(t_comp)
		{
			printf("Company has been found.\n");
		}
		else
		{
			printf("Invalid name.\n");
		}
	}
	else if(choice == 5){ //deletes department according to the given name by using delete_department() function
		char target_name[MAX_SIZE_STRING];
		printf("Please enter the name of company.\n");
		scanf("%c",&dummy);
		fgets(target_name,MAX_SIZE_STRING,stdin);
		normalizeString(target_name);
		Company* target_company = search_company(comp_list,*company_count,target_name);
		printf("Please enter the name of department.\n");
		fgets(target_name,MAX_SIZE_STRING,stdin);
		normalizeString(target_name);
		delete_department(target_company,target_name);
	}
	else if(choice == 6) //undeletes department according to the given name by using undelete_department() function
	{
		char target_name[MAX_SIZE_STRING];
		printf("Please enter the name of company.\n");
		scanf("%c",&dummy);
		fgets(target_name,MAX_SIZE_STRING,stdin);
		normalizeString(target_name);
		Company* target_company = search_company(comp_list,*company_count,target_name);
		printf("Please enter the name of department.\n");
		fgets(target_name,MAX_SIZE_STRING,stdin);
		normalizeString(target_name);
		undelete_department(target_company,target_name);
	}
	else if(choice == 7){ //prints whole list
		print_list(comp_list, *company_count);
	}
	else if(choice == 8){ //reads from file 
		import_exportCompanies(comp_list, company_count, 0);
	}
	else if(choice == 9){ //writes to file
		import_exportCompanies(comp_list, company_count, 1);
	}
	return choice;
}

int main()
{
	int listSize = 64; //Max list size
	int company_count = 0; //keeps current company count
	char selection,selection1,dummy;
	Company** companyList = (Company**)malloc(listSize*sizeof(Company*)); //List structure
	do{
		int choice = print_menu(companyList, &company_count);
		printf("Do you want to exit?(y/n)");
		if(choice != 2 && choice != 3 && choice != 5 && choice != 6 && choice != 4)//is used to prevent errors
		{
			scanf("%c",&dummy);
		}
		scanf("%c",&selection1);
		selection = selection1;
	}while(selection == 'n');
	
	delete_list(companyList,company_count); // deletes all list to avoid memory leaks
	
	return 0;
}
