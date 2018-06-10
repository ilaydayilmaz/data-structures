#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 50 //Stack Size

typedef struct STACK{
	int top;
	int st[SIZE];
}STACK;

void open_file();
void parse();
void initialize(STACK *);
int is_empty(STACK *);
int is_full(STACK *);
void push(STACK *, int );
void postfix(STACK *);
int get_priority(char );
void print_stack(STACK *, int);
void evaluate(int, char *);

char expression[50];
int variables[10]; //updated values of variables
int tmp[20][2]; //values of variables every step
int k = 0; //index to tmp

FILE *finput;

int main() {
	int n; //Input file line count
	int i;	
	STACK *stack;
	stack = (STACK *)malloc(sizeof(STACK));
	initialize(stack);
	
	open_file();
	printf("Enter step count: ");
	scanf("%d", &n);

	for(i=0; i<n; i++){
		parse();
		postfix(stack);
		printf("______________________________________________________\n");
	}

	
	for(i=0; i<k; i++)
		printf("%c=%d\t", tmp[i][0], tmp[i][1]);
	
	
	return 0;
}


void open_file(){
	finput = fopen("input.txt", "r");
	if(finput == NULL){
		printf("Error loading file..");
		exit(1);
	}
}

void parse(){ //Scans one line of input file
	char tmp = fgetc(finput);
	int i = 0;
	while(tmp!= ';'){
		if(tmp != '\n' && tmp != ' '){
			expression[i] = tmp;
			i++;
		}
		tmp = fgetc(finput);	
	}
	expression[i] = '\0';	
}

//Stack Functions
void initialize(STACK *stack){
	stack->top = 0;
}

int is_empty(STACK *stack){
	if(stack->top == 0)
		return 1;
	else
		return 0;
}

int is_full(STACK *stack){
	if(stack->top == SIZE)
		return 1;
	else 
		return 0;
}

void push(STACK *stack, int item){
	if(is_full(stack))
		printf("Stack is full\n");
	else{
		stack->st[stack->top] = item;
		stack->top++;
	}
}

int pop(STACK *stack){
	if(is_empty(stack)){
		printf("Stack is empty\n");
		return 0;
	}
	else{
		stack->top--;
		return stack->st[stack->top];
	}

}


void postfix(STACK *stack){ //Converts infix to postfix
	int i = 2, j = 0;
	char postfix[20] = {'\0'};
	int var = expression[0]; //Variable on the left side of equation
	printf("\nInput: %s\n", expression);
	
	while(expression[i] != '\0'){
		if(expression[i] =='(')
			push(stack, expression[i]);
		else if(expression[i] == '+' || expression[i] == '-' || expression[i] == '*' 
				|| expression[i] == '/' || expression[i] == '^'){
		//if the stack is empty
			if(stack->top == 0)
				push(stack, expression[i]);
			else{
				while(get_priority(stack->st[stack->top]) > get_priority(expression[i])){
					postfix[j] = stack->st[stack->top-1];
					j++;
					pop(stack);
				}
				push(stack, expression[i]);
			}
		}
		else if(expression[i] == ')'){
			while(stack->st[stack->top-1] != '('){
				postfix[j] = stack->st[stack->top-1];
				j++;	
				pop(stack);
			}
			pop(stack); 
		}
		else{
			postfix[j] = expression[i];
			j++;
		}
		i++; 
		printf("Postfix:%s\t\t\t\t",postfix);
		print_stack(stack, 1); 
             
	}
      
	while(stack->top > 0){
		postfix[j] = stack->st[stack->top-1];
		j++;
		pop(stack);
		printf("Postfix:%s\t\t\t\t",postfix);
		print_stack(stack, 1); 
	}
      
	postfix[j] = '\0'; //postfix is created
      
	printf("Current Output: %s\n\n",postfix);
	evaluate(var, postfix);
	
	
	
}


int get_priority(char c){ //returns the priority order of operand
	if(c == '*' || c == '/')
		return 2;
	else if(c == '+' || c == '-')
		return 1;
	else return -1;
}


void print_stack(STACK *stack, int isChar) { //prints all elements in stack
	int i = 0;
	if(is_empty(stack)){
        printf("Stack: Empty\n");
        return;      
    }
    printf("Stack: ");
    
	while(i != stack->top){
		if(isChar) //controls the content of stack
			printf("%c ", stack->st[i]);
		else
			printf("%d ",stack->st[i]);
		i++;              
    }
    printf("\n");
}


void evaluate(int var, char *exp){ //function to evaluate a given expression in postfix 
	STACK *stack2;
	stack2 = (STACK *)malloc(sizeof(STACK));
	initialize(stack2);
	
	int op1,op2,op3; //operands

	int i = 0;
	
	while(exp[i] != '\0'){
		if(isalpha(exp[i])){ //if the character(exp[i]) is alphabetic
			//pushes the correspondent value of exp[i]
			push(stack2, variables[exp[i]-'a']);
			print_stack(stack2, 0);
		}
		
		else if(isdigit(exp[i])){ //if exp[i] is a digit
			int num = 0;
			
			while(isdigit(exp[i])){ //calculates and pushes the value
				num = (int)(exp[i]-'0') + num*10;
                i++;
            }
            i--;
             
            //push the element in the stack
            push(stack2,num);
			
		}
		else if(exp[i] != ' '){
			op1 = pop(stack2);
			op2 = pop(stack2);
			switch(exp[i]){
				case '+': {
					op3 = op1 + op2;
                    break;
                }
                case '-': {
					op3 = op2 - op1;
					break;
				}
				case '*': {
					op3 = op1 * op2;
					break;
				}
				case '/': {
					op3 = op2 / op1;
					break;
				}
			}
			push(stack2, op3);
		}
		i++;
	}
	print_stack(stack2, 0);
	int result = pop(stack2);
	tmp[k][0] = var;
	tmp[k][1] = result;
	k++;
	variables[var-'a'] = result; //value of var is updated

}

