#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    int id, height;
    char name[100];
    float gpa;
    struct node* left;
    struct node* right;
} node;

int heightt(node* n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

node* newnode(int id, float gpa, char* name)
{
    node* n = malloc(sizeof(node));
    n->id = id;
    n->gpa = gpa;
    int i;
   for( i=0;name[i];i++)
        n->name[i]=name[i];
    n->name[i]='\0';
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

node* rotateright(node* y)
{
    node* x  = y->left;
    node* t2 = x->right;
    x->right = y;
    y->left  = t2;
    y->height = 1 + max(heightt(y->left), heightt(y->right));
    x->height = 1 + max(heightt(x->left), heightt(x->right));
    return x;
}

node* rotateleft(node* x)
{
    node* y  = x->right;
    node* t2 = y->left;
    x->right = t2;
    y->left  = x;
    x->height = 1 + max(heightt(x->left), heightt(x->right));
    y->height = 1 + max(heightt(y->left), heightt(y->right));
    return y;
}

int getbalance(node* root)
{
    if (root == NULL)
        return 0;
    return heightt(root->left) - heightt(root->right);
}

node* insert(node* root, int id, float gpa, char* name)
{
    if (gpa < 0.0f || gpa > 4.0f)
    {
        printf("Error: GPA %.2f is out of range (0.0 - 4.0)\n", gpa);
        return root;
    }

    if (root == NULL)
        return newnode(id, gpa, name);

    if (id < root->id)
        root->left  = insert(root->left,  id, gpa, name);
    else if (id > root->id)
        root->right = insert(root->right, id, gpa, name);
    else
    {
        printf("Error: Student with ID %d already exists.\n", id);
        return root;
    }

    root->height = 1 + max(heightt(root->left), heightt(root->right));

    int balance = getbalance(root);

    if (balance > 1 && id < root->left->id)
        return rotateright(root);

    if (balance < -1 && id > root->right->id)
        return rotateleft(root);

    if (balance > 1 && id > root->left->id)
    {
        root->left = rotateleft(root->left);
        return rotateright(root);
    }

    if (balance < -1 && id < root->right->id)
    {
        root->right = rotateright(root->right);
        return rotateleft(root);
    }

    return root;
}

int treeSize(node* root)
{
    if (root == NULL) return 0;
    return 1 + treeSize(root->left) + treeSize(root->right);
}
int searchByID(node* root, int id)
{
    if (root == NULL)
    {
        printf("Error: Student with ID %d not found.\n", id);
        return 0;
    }

    if (root->id == id)
    {
        printf("Student Found:\n");
        printf("ID   : %d\n",   root->id);
        printf("Name : %s\n",   root->name);
        printf("GPA  : %.2f\n", root->gpa);
        return 1;
    }

    if (id < root->id)
        return searchByID(root->left,  id);

    return searchByID(root->right, id);
}
int sameName(char* a, char* b)
{
    int i = 0;

    while (a[i] && b[i])
    {
        char c1 = a[i];
        char c2 = b[i];

        if (c1 >= 'A' && c1 <= 'Z')
            c1 = c1 + 32;

        if (c2 >= 'A' && c2 <= 'Z')
            c2 = c2 + 32;

        if (c1 != c2)
            return 0;

        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

int searchByName(node* root, char* name)
{
    if (root == NULL)
        return 0;

    if (sameName(root->name, name))
    {
        printf("Student Found:\n");
        printf("ID   : %d\n", root->id);
        printf("Name : %s\n", root->name);
        printf("GPA  : %.2f\n", root->gpa);
        return 1;
    }

    if (searchByName(root->left, name))
        return 1;
   else
    return searchByName(root->right, name);
}
node* minValueNode(node* root)
{
    node* current = root;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}
node* deleteNode(node* root, int id)
{
    if (root == NULL)
        return NULL;

    if (id < root->id)
        root->left = deleteNode(root->left, id);

    else if (id > root->id)
        root->right = deleteNode(root->right, id);

    else
    {
        if (root->left == NULL || root->right == NULL)
        {
            node* temp = root->left ? root->left : root->right;

            free(root);
            return temp;
        }
        else
        {
            node* temp = minValueNode(root->right);

            root->id = temp->id;
            root->gpa = temp->gpa;

            int i;
            for (i = 0; temp->name[i]; i++)
                root->name[i] = temp->name[i];
            root->name[i] = '\0';

            root->right = deleteNode(root->right, temp->id);
        }
    }

    if (root == NULL)
        return NULL;

    root->height = 1 + max(heightt(root->left), heightt(root->right));

    int balance = getbalance(root);

    if (balance > 1 && getbalance(root->left) >= 0)
        return rotateright(root);

    if (balance > 1 && getbalance(root->left) < 0)
    {
        root->left = rotateleft(root->left);
        return rotateright(root);
    }

    if (balance < -1 && getbalance(root->right) <= 0)
        return rotateleft(root);

    if (balance < -1 && getbalance(root->right) > 0)
    {
        root->right = rotateright(root->right);
        return rotateleft(root);
    }

    return root;
}
node* updateStudent(node* root, int id)
{
  int choice;
    node* current = root;

    while (current != NULL)
    {
        if (id < current->id)
            current = current->left;

        else if (id > current->id)
            current = current->right;

        else
        {
            while (1)
            {
                printf("\nUpdate Menu\n");
                printf("1. Update ID\n");
                printf("2. Update Name\n");
                printf("3. Update GPA\n");
                printf("4. Exit Update Menu\n");
                printf("Enter Your Choice: ");
                scanf("%d", &choice);

                if (choice == 1)
                {
                    int newID;

                    printf("Enter New ID: ");
                    scanf("%d", &newID);

                    if (newID == current->id)
                    {
                        printf("Error: New ID is the same as current ID.\n");
                        continue;
                    }

                    int oldSize = treeSize(root);

                    root = insert(root, newID, current->gpa, current->name);

                    if (treeSize(root) == oldSize)
                        continue;

                    root = deleteNode(root, current->id);

                    current = root;

                    while (current != NULL)
                    {
                        if (newID < current->id)
                            current = current->left;

                        else if (newID > current->id)
                            current = current->right;

                        else
                            break;
                    }

                    printf("Student ID updated successfully.\n");
                }

                else if (choice == 2)
                {
                    char newName[100];

                    printf("Enter New Name: ");
                    getchar();
                    gets(newName);

                    int i;

                    for(i = 0; newName[i]; i++)
                        current->name[i] = newName[i];

                    current->name[i] = '\0';

                    printf("Student Name updated successfully.\n");
                }

                else if (choice == 3)
                {
                    float newGPA;

                    printf("Enter New GPA: ");
                    scanf("%f", &newGPA);

                    if (newGPA < 0.0f || newGPA > 4.0f)
                    {
                        printf("Error: GPA %.2f is out of range (0.0 - 4.0)\n", newGPA);
                        continue;
                    }

                    current->gpa = newGPA;

                    printf("Student GPA updated successfully.\n");
                }

                else if (choice == 4)
                {
                    return root;
                }

                else
                {
                    printf("Invalid Choice.\n");
                }
            }
        }
    }

    printf("Error: Student with ID %d not found.\n", id);

    return root;
}
void displayAllStudents(node* root)
{
    if (root == NULL)
        return;

    displayAllStudents(root->left);

    printf("ID   : %d | ", root->id);
    printf("Name : %s | ", root->name);
    printf("GPA  : %.2f\n", root->gpa);

    displayAllStudents(root->right);
}
int treeHeight(node* root)
{
    if (root == NULL)
        return 0;

    int leftH = treeHeight(root->left);
    int rightH = treeHeight(root->right);

    return 1 + max(leftH, rightH);
}

void display_statical(node *root)
{
    if (root == NULL)
    {
        printf("Tree is empty.\n");
        return;
    }
printf("Tree Height: %d\n", treeHeight(root));
printf("Total Number of Students: %d\n", treeSize(root));
}
void minGPA(node* root, float* min)
{
    if (root == NULL)
        return;

    if (root->gpa < *min)
        *min = root->gpa;

    minGPA(root->left, min);
    minGPA(root->right, min);
}

void maxGPA(node* root, float* max)
{
    if (root == NULL)
        return;

    if (root->gpa > *max)
        *max = root->gpa;

    maxGPA(root->left, max);
    maxGPA(root->right, max);
}

void sumGPA(node* root, float* sum)
{
    if (root == NULL)
        return;

    *sum += root->gpa;

    sumGPA(root->left, sum);
    sumGPA(root->right, sum);
}

void countAboveAverage(node* root, float average, int* count)
{
    if (root == NULL)
        return;

    if (root->gpa > average)
        (*count)++;

    countAboveAverage(root->left, average, count);
    countAboveAverage(root->right, average, count);
}

void countBelowAverage(node* root, float average, int* count)
{
    if (root == NULL)
        return;

    if (root->gpa < average)
        (*count)++;

    countBelowAverage(root->left, average, count);
    countBelowAverage(root->right, average, count);
}
void displayGPAStatistics(node* root)
{
    if (root == NULL)
    {
        printf("Tree is empty.\n");
        return;
    }

    float min = root->gpa;
    float max = root->gpa;
    float sum = 0;

    int above = 0, below = 0;

    int total = treeSize(root);

    minGPA(root, &min);
    maxGPA(root, &max);
    sumGPA(root, &sum);

    float average = sum / total;

    countAboveAverage(root, average, &above);
    countBelowAverage(root, average, &below);

    printf("Minimum GPA: %.2f\n", min);
    printf("Maximum GPA: %.2f\n", max);
    printf("Average GPA: %.2f\n", average);
    printf("Above Avg: %d\n", above);
    printf("Below Avg: %d\n", below);
}
node* deleteAllStudents(node* root)
{
    if (root == NULL)
        return NULL;

    root->left = deleteAllStudents(root->left);
    root->right = deleteAllStudents(root->right);

    free(root);

    return NULL;
}
node* maxValueNode(node* root)
{
    node* current = root;

    while (current && current->right != NULL)
        current = current->right;

    return current;
}

void findPredecessorSuccessor(node* root, int id)
{
    if (root == NULL)
    {
        printf("The tree is empty.\n");
        return;
    }

    node* current = root;
    node* predecessor = NULL;
    node* successor = NULL;

    while (current != NULL)
    {
        if (id < current->id)
        {
            successor = current;
            current = current->left;
        }

        else if (id > current->id)
        {
            predecessor = current;
            current = current->right;
        }

        else
        {
            if (current->left != NULL)
                predecessor = maxValueNode(current->left);

            if (current->right != NULL)
                successor = minValueNode(current->right);

            printf("Student Found:\n");
            printf("ID   : %d\n", current->id);
            printf("Name : %s\n", current->name);
            printf("GPA  : %.2f\n", current->gpa);

            if (predecessor != NULL)
            {
                printf("\nPredecessor:\n");
                printf("ID   : %d\n", predecessor->id);
                printf("Name : %s\n", predecessor->name);
                printf("GPA  : %.2f\n", predecessor->gpa);
            }

            else
            {
                printf("\nThis is the first record.\n");
            }

            if (successor != NULL)
            {
                printf("\nSuccessor:\n");
                printf("ID   : %d\n", successor->id);
                printf("Name : %s\n", successor->name);
                printf("GPA  : %.2f\n", successor->gpa);
            }

            else
            {
                printf("\nThis is the last record.\n");
            }

            return;
        }
    }

    printf("Error: Student with ID %d not found.\n", id);
}
int main()
{
    node* root = NULL;

    int choice;
    int id;
    float gpa;
    char name[100];

    while (1)
    {
        printf("\n========== AVL Student Management System ==========\n");
        printf("1. Insert Student\n");
        printf("2. Search Student By ID\n");
        printf("3. Search Student By Name\n");
        printf("4. Delete Student\n");
        printf("5. Update Student\n");
        printf("6. Display All Students\n");
        printf("7. Display Tree Statistics\n");
        printf("8. Display GPA Statistics\n");
        printf("9. Delete All Students\n");
        printf("10. Find Predecessor and Successor\n");
        printf("11. Exit\n");

        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("Enter Student ID: ");
            scanf("%d", &id);

            printf("Enter Student full Name: ");
            getchar();
            gets(name);

            printf("Enter Student GPA: ");
            scanf("%f", &gpa);

            root = insert(root, id, gpa, name);
        }

        else if (choice == 2)
        {
            if (root == NULL)
            {
                printf("The tree is empty.\n");
                continue;
            }

            printf("Enter Student ID: ");
            scanf("%d", &id);

            searchByID(root, id);
        }

        else if (choice == 3)
        {
            if (root == NULL)
            {
                printf("The tree is empty.\n");
                continue;
            }

            printf("Enter Student Name: ");
            getchar();
            gets(name);

            if (!searchByName(root, name))
                printf("Error: Student with name %s not found.\n", name);
        }

        else if (choice == 4)
        {
            if (root == NULL)
            {
                printf("The tree is empty.\n");
                continue;
            }

            printf("Enter Student ID to delete: ");
            scanf("%d", &id);

            root = deleteNode(root, id);
        }

        else if (choice == 5)
        {
            if (root == NULL)
            {
                printf("The tree is empty.\n");
                continue;
            }

            printf("Enter Student ID to update: ");
            scanf("%d", &id);

            root = updateStudent(root, id);
        }

        else if (choice == 6)
        {
            if (root == NULL)
            {
                printf("The tree is empty.\n");
                continue;
            }

            displayAllStudents(root);
        }

        else if (choice == 7)
        {
            if (root == NULL)
            {
                printf("The tree is empty.\n");
                continue;
            }

            display_statical(root);
        }

        else if (choice == 8)
        {
            if (root == NULL)
            {
                printf("The tree is empty.\n");
                continue;
            }

            displayGPAStatistics(root);
        }

        else if (choice == 9)
        {
            if (root == NULL)
            {
                printf("The tree is already empty.\n");
                continue;
            }

            root = deleteAllStudents(root);

            printf("All students deleted successfully.\n");
        }

        else if (choice == 10)
        {
            if (root == NULL)
            {
                printf("The tree is empty.\n");
                continue;
            }

            printf("Enter Student ID: ");
            scanf("%d", &id);

            findPredecessorSuccessor(root, id);
        }

        else if (choice == 11)
        {
            root = deleteAllStudents(root);

            printf("Exiting Program...\n");
            break;
        }

        else
        {
            printf("Invalid Choice.\n");
        }
    }

    return 0;
}
