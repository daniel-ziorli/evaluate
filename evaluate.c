#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node{
    char operand[256];
    float number;
    char variable[256];

    struct node * left;
    struct node * right;
} node;

//appends char to end of string
void append_char(char * str,char c){
    int len = strlen(str);
    str[len] = c;    
    str[len+1] = '\0';
}

// recursive function for building each branch of the binary tree
node * build_branches(char * s){
    node * new = malloc(sizeof(node));
    if (isdigit(s[0]) && s[0] != '[' && s[0] != '('){
        new->number = atof(s);
        return new;
    }
    if (isalpha(s[0]) && s[0] != '[' && s[0] != '('){
        strcpy(new->variable, s);
        new->number = 0.0;
        return new;
    }

    int depth = 0;
    int i;
    int index = -1;

    char left_side[256] = "";
    char right_side[256] = "";

    for (i = 1; i < strlen(s); i++){
        append_char(left_side, s[i]);

        if (s[i] == '[' || s[i] == '('){
            depth += 1;
        }
        if (s[i] == ']' || s[i] == ')'){
            depth -= 1;
        }

        if (depth == 0){
            index = i + 1;
            break;
        }
    }
    if (i == 1){
        left_side[0] = '\0';
        for (i = 1; i < strlen(s); i++){
            if (s[i] == '*' || s[i] == '/' || s[i] == '+' || s[i] == '-'){
                index = i;
                break;
            }
            append_char(left_side, s[i]);
        }
    }

    char operand[2] = "";
    operand[0] = s[index];
    operand[1] = '\0';

    for (i = index + 1; i < strlen(s); i++){
        append_char(right_side, s[i]);
        if (s[i] == '[' || s[i] == '('){
            depth += 1;
        }
        if (s[i] == ']' || s[i] == ')'){
            depth -= 1;
        }
        if (depth == 0){
            break;
        }
    }

    if (i == index + 1){
        right_side[0] = '\0';
        for (i = index + 1; i < strlen(s); i++){
            if (s[i] == ']' || s[i] == ')'){
                break;
            }
            append_char(right_side, s[i]);
        }
    }
    
    strcpy(new->operand, operand);

    new->left = build_branches(left_side);
    new->right = build_branches(right_side);
    
    return new;
}

// function for creating the initial tree, catalyst for build_branches recursive function
void create_tree(char * s, node ** n){
    int depth = 0;
    int i;
    int index = -1;

    char left_side[256] = "";
    char right_side[256] = "";

    for (i = 1; i < strlen(s); i++){
        append_char(left_side, s[i]);

        if (s[i] == '[' || s[i] == '('){
            depth += 1;
        }
        if (s[i] == ']' || s[i] == ')'){
            depth -= 1;
        }

        if (depth == 0){
            index = i + 1;
            break;
        }
    }
    if (i == 1){
        left_side[0] = '\0';
        for (i = 1; i < strlen(s); i++){
            if (s[i] == '*' || s[i] == '/' || s[i] == '+' || s[i] == '-'){
                index = i;
                break;
            }
            append_char(left_side, s[i]);
        }
    }

    char operand[2] = "";
    operand[0] = s[index];
    operand[1] = '\0';

    for (i = index + 1; i < strlen(s); i++){
        append_char(right_side, s[i]);
        if (s[i] == '[' || s[i] == '('){
            depth += 1;
        }
        if (s[i] == ']' || s[i] == ')'){
            depth -= 1;
        }
        if (depth == 0){
            break;
        }
    }

    if (i == index + 1){
        right_side[0] = '\0';
        for (i = index + 1; i < strlen(s); i++){
            if (s[i] == ']' || s[i] == ')'){
                break;
            }
            append_char(right_side, s[i]);
        }
    }
    

    node * root = malloc(sizeof(node));
    
    strcpy(root->operand, operand);

    root->left = build_branches(left_side);
    root->right = build_branches(right_side);


    *n = root;
}

// prints the conents of a node
void print_node(node * n){
    if (strlen(n->operand) > 0){
        printf("%s", n->operand);
    }
    else if (strlen(n->variable) > 0){
        printf("%s", n->variable);
    }
    else{
        printf("%.2f", n->number);
    }
}

// prints expression in post fix format
void postfix(node * tree){
    if (tree){
        postfix (tree->left);
        postfix (tree->right);
        print_node(tree);
    }
}

// prints expression in pre fix format
void prefix(node * tree){
    if (tree){
        print_node(tree);
        prefix (tree->left);
        prefix (tree->right);
    }
}

// prints expression in infix format
void infix(node * tree){
    if (tree){
        if (strlen(tree->operand) > 0){
            printf("(");
        }
        infix (tree->left);
        print_node(tree);
        infix (tree->right);
        if (strlen(tree->operand) > 0)
           printf(")");
    }
    
}

// prints the binary tree to console
void print_tree(node * root, int padding){
    if (root == NULL) 
        return; 
  
    padding += 6; 
  
    print_tree(root->right, padding); 
  
    printf("\n"); 
    for (int i = 6; i < padding; i++) {
        printf(" "); 
    }

    printf("[");
    print_node(root);
    printf("]");
    printf(" <");

    print_tree(root->left, padding); 
}

// finds and sets variable in binary tree
void set_variable(node * tree, char * var, float val){
    if (tree){
        if (strcmp(tree->variable, var) == 0){
            tree->number = val;
        }
        set_variable (tree->left, var, val);
        set_variable (tree->right, var, val);
    }
}

// evaluates the result of the binary tree
float evaluate(node * tree){
    if (tree){
        if (strlen(tree->operand) == 0){
            return tree->number;
        }
        else{
            float l = evaluate(tree->left);
            float r = evaluate(tree->right);
            if (tree->operand[0] == '+'){
                return l + r;
            }
            if (tree->operand[0] == '-'){
                return l - r;
            }
            if (tree->operand[0] == '*'){
                return l * r;
            }
            if (tree->operand[0] == '/'){
                if (r == 0){
                    printf("division by zero error\n");
                    return -1;
                }
                return l / r;
            }
        }
    }
    return -1;
}

int main(int argc, char *argv[]){

    if (argc != 2){
        printf("usage: q1.c equation\n");
        return 0;
    }

    char * equation = argv[1];
    node * root = NULL;
    create_tree(equation, &root);
    char option;
    while(1){
        printf("1. Display\n");
        printf("2. Preorder\n");
        printf("3. Inorder\n");
        printf("4. Postorder\n");
        printf("5. Update\n");
        printf("6. Calculate\n");
        printf("7. Quit.\n");
        scanf("%c", &option);

        if (option == '1'){
            print_tree(root, 0);
            printf("\n");
        }
        else if (option == '2'){
            prefix(root);
            printf("\n");
        }
        else if (option == '3'){
            infix(root);
            printf("\n");
        }
        else if (option == '4'){
            postfix(root);
            printf("\n");
        }
        else if (option == '5'){
            char variable_name[256];
            char new_value[256];
            printf("Please Enter a Variable Name:\n");
            scanf("%s", variable_name);
            printf("Please Enter the New Value:\n");
            scanf("%s", new_value);

            set_variable(root, variable_name, atof(new_value));
        }
        else if (option == '6'){
            printf("%.2f\n", evaluate(root));
        }
        else if (option == '7'){
            printf("Closing\n");
            return 0;
        }
        scanf("%c", &option);
        
    }

    return 0;
}