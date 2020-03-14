#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 1000

/*
* Item: An item that is being pushed to or popped from the stack
* It may be float (to be used while evaluating the postfix)
* It may be char (to be used while converting infix to postfix)
*/

typedef union{
    float fData;
    char  cData;
}Item;
/*
*
*/
typedef struct
{
int top;
int capacity;
Item *items;
} Stack;
/*
*
*/
Stack * initialize()
{
        Stack *stack = malloc(sizeof(Stack));
        stack->top = 0;
        stack->capacity = SIZE;
        stack->items = (Item*)malloc(SIZE*sizeof(Item));
        return stack;
}
/*
*
*/
int isEmpty(Stack *stack)
{
        if(stack->top == 0){
                return 1;
        } else {
                return 0;
        }
        return -1;
}
/*
*
*/
int isFull(Stack *stack)
{
        if(stack->top < stack->capacity){
                return 0;
        }
        else {
                return 1;
        }
        return -1;
}
/*
*
*/
Item top(Stack *stack)
{
        return stack->items[(stack->top)-1];
}
/*
*
*/
Item pop(Stack *stack)
{
        return stack->items[--(stack->top)];
}
/*
*
*/
void push(Stack *stack, Item val)
{
        if(isFull(stack)){
                printf("Stack is full.\n");
                exit(0);
        }
        stack->items[stack->top++] = val;
}
/*
*
display : To be removed (a debugging function)
*/
void display(Stack *stack){
        int i;
        printf("|");
        for(i=0;i<stack->top;i++){
                printf("%f|",stack->items[i].fData);
        }
        printf("\n");
}
/*
*
*/
int isToken(char token){
        if(token == '+' || token == '-' || token == '*' || token == '/' || token == ')' || token == '(' || token == '^'){
                return 1;
        }
        return 0;
}
int prec(char c){
switch (c) {
       case '+' : case '-' : return 1;
       case '*' : case '/' : return 2;
       case '^' : return 3;
       case '(':  case '#': return 0;
        }
}
/*
* infixToPostfix: converts an expression in infix notation to
* to a postfix notation (Reverse-Polish Notation)
* ASSUME single-digit operands
* ASSUME ^*+-/ operators
* e.g., 22+31 --> 22 31 +
* e.g., (2+3)*4 --> 2 3 + 4 *
*/
void infixToPostfix(char* infix, char* postfix)
{
        Item item;
        int len = strlen(infix);
        int i=0;
        int j=0;
        int isPrevToken = 0;
        char prevToken;
        Stack *stack = initialize();
        for(i;i<len;i++){
                char ch = infix[i];
                if ((ch>='0' && ch<= '9') || ch == '.'){ postfix[j++]=ch; isPrevToken = 0;}
                else if(isToken(infix[i])){
                        if(j == 0 && ch == '-'){postfix[j++]=ch; isPrevToken = 0; continue;} //Negative handling
                        if(isPrevToken && ch == '-' && prevToken != ')'){postfix[j++]=ch; isPrevToken = 0; continue;} //Negative handling
                        prevToken = ch;
                        isPrevToken = 1;
                        postfix[j++]=' ';
                        if (ch=='('){item.cData = ch; push(stack,item);}
                        else if (ch==')'){
                                while(!isEmpty(stack) && top(stack).cData!='('){ postfix[j++]=pop(stack).cData; postfix[j++]=' ';};
                                pop(stack).cData;
                        }
                        else {
                                while(!isEmpty(stack) && top(stack).cData!='(' && prec(ch) <= prec(top(stack).cData) )
                                postfix[j++]=pop(stack).cData;
                                postfix[j++]=' ';
                                item.cData = ch;
                                push(stack,item);
                        }
                }
        }
        while(!isEmpty(stack)){
                postfix[j++]=pop(stack).cData;
        }
        postfix[j++] = '\0'; // Truncate the string
}
/*
* evaluatePostfix: Evaluates an expression in postfix notation
* (Reverse-Polish Notation)
* ASSUME single-digit operands
*/
float evaluate(char x,float op1,float op2)
{
        if(x=='+')
                return(op1+op2);
        if(x=='-')
                return(op1-op2);
        if(x=='*')
                return(op1*op2);
        if(x=='/')
                return(op1/op2);
        if(x=='^')
                return(pow(op1,op2));
}
/*
*
*/
int tokenNext(char* string){
        char ch;
        while((ch=*string++)){
                if((ch == '-' && (*string+1) >= '0' && (*string+1) <= '9' ) || (ch>='0' && ch<= '9') ){return 0;}
                else if(isToken(ch)){return (int)ch;}
        }
}
/*
*
*/
int tokNum(char* string){char ch;int count=0;while((ch=*string++)){if(ch=='+'||(ch=='-' && ((*string+1) < '0' || (*string+1) > '9'))||ch=='*'||ch=='/'||ch=='^'){count++;}} return count;}
/*
*
*/
float evaluatePostfix(char* postfix)
{
        Item item;
        Stack *stack = initialize();
        int prevToks = tokNum(postfix); // number of tokens in the postfix
        int maxIters = (tokNum(postfix)*2)+1;
        int i=0;
        for(i;i<2;i++){item.fData = strtof(postfix,&postfix); push(stack,item);}
        //      display(stack);
        while(maxIters-- && tokNum(postfix)){
                if(!tokenNext(postfix)){
                        item.fData = strtof(postfix,&postfix);
                        push(stack,item);
                }
                else {
                        float op2 = pop(stack).fData;
                        float op1 = pop(stack).fData;
                        item.fData = evaluate((char)tokenNext(postfix),op1,op2);
                        push(stack,item);
                        int s = 0;
                        while(tokNum(postfix) == prevToks){postfix[s] = ' ';s++;}
                        prevToks--;
                }
        //              display(stack);
        }
        return pop(stack).fData;
        return -1;
}
/*
*
*/
void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}
/*
*
*/
int main(int argc, char**argv)
{
    char infixExpr[256] = "";
    char postfixExpr[256] = "";
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(infixExpr);
        infixToPostfix(infixExpr, postfixExpr);
        printf("\nPostfix : %s\n", postfixExpr);
        float result = evaluatePostfix(postfixExpr);
        printf("Result: %f\n\n", result);
        printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    }
    return 0;

}
