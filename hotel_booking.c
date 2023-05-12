#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listNode{
    char maYeuCau[8];
    int nguoi;
    int phong;
    char maKhachSan[4];
    int date, month, year;
    int kid;
    struct listNode* next;
}listNode;
listNode* first = NULL, *last = NULL;

typedef struct treeNode{
    char maYeuCau[8];
    int nguoi;
    int phong;
    char maKhachSan[4];
    int date, month, year;
    int kid;
    struct treeNode* leftChild;
    struct treeNode* rightChild;
}treeNode;
treeNode* root = NULL;

int listEmpty(){
    return first == NULL && last == NULL;
}

void listRelease(){
    listNode* p = first;
    while(p != NULL){
        listNode* np = p->next;
        free(p);
        p = np;
    }
}

void freeTree(treeNode *r){
    if (r == NULL) return;
    freeTree(r->leftChild);
    freeTree(r->rightChild);
    free(r);
}

void menu(){
    printf("\nChuong trinh quan ly dat phong\n");
    printf("Vui long chon mot trong cac chuc nang sau\n");
    printf("[1] Doc du lieu tu file log\n");
    printf("[2] Kiem tra so luong yeu cau khong hop le\n");
    printf("[3] Tra cuu theo ma yeu cau\n");
    printf("[4] Thay doi yeu cau\n");
    printf("[5] Huy yeu cau\n");
    printf("[6] Luu danh sach yeu cau ra file\n");
    printf("[7] Thoat chuong trinh\n");
    printf("Lua chon cua ban la: ");
}

listNode* makelistNode(char* maYeuCau, int nguoi, int phong, char* maKhachSan, int date, int month, int year, int kid){
    listNode* p = (listNode*)malloc(sizeof(listNode));
    strcpy(p->maYeuCau, maYeuCau);
    p->nguoi = nguoi;
    p->phong = phong;
    strcpy(p->maKhachSan, maKhachSan);
    p->date = date;
    p->month = month;
    p->year = year;
    p->kid = kid;
    p->next = NULL;
    return p;
}

void listInsert(char* maYeuCau, int nguoi, int phong, char* maKhachSan, int date, int month, int year, int kid){
    listNode *p = makelistNode(maYeuCau, nguoi, phong, maKhachSan, date, month, year, kid);
    if (listEmpty()){
        first = p; last = p;
    }else{
        last->next = p; last = p;
    }
}
//
void processLoad(char* filename){
    FILE *f = fopen(filename,"r");
    if(f == NULL){
        printf("Cannot open file!\n");
        return;
    }
    int yeucau;
    fscanf(f, "%d", &yeucau);
    while(!feof(f)){
        char maYeuCau[8];
        int nguoi;
        int phong;
        char maKhachSan[4];
        int date, month, year;
        int kid;
        fscanf(f, "%s %d %d %s %d-%d-%d %d", maYeuCau, &nguoi, &phong, maKhachSan, &date, &month, &year, &kid);
        listInsert(maYeuCau, nguoi, phong, maKhachSan, date, month, year, kid);
    }
    printf("So luong yeu cau: %d\n", yeucau);
}
//
void kthople(){
    int count = 0;
    for(listNode *i = first; i != NULL; i = i->next){
        if((i->nguoi - i->kid) <= 0){
            count++;
            printf("%s %d %03d %s %02d-%02d-%4d %d\n", i->maYeuCau, i->nguoi, i->phong, i->maKhachSan, i->date, i->month, i->year, i->kid);
        }
    }
    printf("So luong yeu cau khong hop le: %d\n", count);
}
//
treeNode* maketreeNode(char* maYeuCau, int nguoi, int phong, char* maKhachSan, int date, int month, int year, int kid){
    treeNode* p = (treeNode*)malloc(sizeof(treeNode));
    strcpy(p->maYeuCau, maYeuCau);
    p->nguoi = nguoi;
    p->phong = phong;
    strcpy(p->maKhachSan, maKhachSan);
    p->date = date;
    p->month = month;
    p->year = year;
    p->kid = kid;
    p->leftChild = NULL;
    p->rightChild = NULL;
    return p;
}

treeNode* treeInsert(treeNode* r, char* maYeuCau, int nguoi, int phong, char* maKhachSan, int date, int month, int year, int kid){
    if(r == NULL) return maketreeNode(maYeuCau, nguoi, phong, maKhachSan, date, month, year, kid);
    int c = strcmp(r->maYeuCau, maYeuCau);
    if(c == 0){
        return r;
    }else if(c > 0){
        r->leftChild = treeInsert(r->leftChild, maYeuCau, nguoi, phong, maKhachSan, date, month, year, kid);
        return r;
    }else{
        r->rightChild = treeInsert(r->rightChild, maYeuCau, nguoi, phong, maKhachSan, date, month, year, kid);
        return r;
    }
}

treeNode* find(treeNode* r, char* maYeuCau){
    if(r == NULL) return NULL;
    int c = strcmp(r->maYeuCau, maYeuCau);
    if (c == 0) return r;
    if (c > 0) return find(r->leftChild, maYeuCau);
    return find(r->rightChild, maYeuCau);
}

treeNode* findMin(treeNode* r){
    if(r == NULL) return NULL;
    treeNode* lmin = findMin(r->leftChild);
    if(lmin != NULL) return lmin;
    return r;
}

treeNode* removeNode(treeNode* r, char* maYeuCau){
    if(r == NULL) return NULL;
    int c = strcmp(r->maYeuCau, maYeuCau);
    if(c > 0) r->leftChild = removeNode(r->leftChild, maYeuCau);
    else if(c < 0) r->rightChild = removeNode(r->rightChild, maYeuCau);
    else{
        if(r->leftChild != NULL && r->rightChild != NULL){
            treeNode* tmp = findMin(r->rightChild);
            strcpy(r->maYeuCau, tmp->maYeuCau);
            r->nguoi = tmp->nguoi;
            r->phong = tmp->phong;
            strcpy(r->maKhachSan, tmp->maKhachSan);
            r->date = tmp->date;
            r->month = tmp->month;
            r->year = tmp->year;
            r->kid = tmp->kid;
            r->rightChild = removeNode(r->rightChild, tmp->maYeuCau);
        }else{
            treeNode* tmp = r;
            if(r->leftChild == NULL) r = r->rightChild;
            else r = r->leftChild;
            free(tmp);
        }
    }
    return r;
}

void ReadtoTree(){
    for(listNode *i = first; i != NULL; i = i->next){
        root = treeInsert(root, i->maYeuCau, i->nguoi, i->phong, i->maKhachSan, i->date, i->month, i->year, i->kid);
    }
}
//
void processFind(){
    char maYeuCau[8];
    printf("Nhap ma yeu cau: ");
    scanf("%s", maYeuCau);
    treeNode* p = find(root, maYeuCau);
    if(p != NULL){
        printf("%s %d %03d %s %02d-%02d-%4d %d\n", p->maYeuCau, p->nguoi, p->phong, p->maKhachSan, p->date, p->month, p->year, p->kid);
    }else printf("NOT FOUND !\n");
}
//
void processUpdate(){
    char maYeuCau[8];
    printf("Nhap ma yeu cau: ");
    scanf("%s", maYeuCau);
    treeNode *p = find(root, maYeuCau);
    if(p == NULL) printf("NOT FOUND !\n");
    else{
        int nguoi;
        int kid;
        int date, month, year;
        printf("Nhap so luong nguoi: ");
        scanf("%d", &nguoi);
        printf("Nhap so luong tre em: ");
        scanf("%d", &kid);
        if (nguoi - kid <= 0){
            printf("INVALID !\n");
            return;
        }else{
            printf("Nhap thoi gian (DD-MM-YYYY): ");
            scanf("%d-%d-%d", &date, &month, &year);
            p->nguoi = nguoi;
            p->kid = kid;
            p->date = date;
            p->month = month;
            p->year = year;
            printf("UPDATE !\n");
            printf("%s %d %03d %s %02d-%02d-%4d %d\n", p->maYeuCau, p->nguoi, p->phong, p->maKhachSan, p->date, p->month, p->year, p->kid);
            return;
        }
    }
}
//
void processRemove(){
    char maYeuCau[8];
    printf("Nhap ma yeu cau: ");
    scanf("%s", maYeuCau);
    treeNode *p = find(root, maYeuCau);
    if(p == NULL){
        printf("NOT FOUND !\n");
        return;
    }else{
        root = removeNode(root, maYeuCau);
        printf("DELETE !\n");
        return;
    }
}
//
int slout = 0;
void fileStore(FILE* k, treeNode *p){
    if(p == NULL) return;
    fileStore(k, p->leftChild);
    fprintf(k, "%s %d %03d %s %02d-%02d-%4d %d\n", p->maYeuCau, p->nguoi, p->phong, p->maKhachSan, p->date, p->month, p->year, p->kid);
    slout++;
    fileStore(k, p->rightChild);
}
void processStore(char* filename){
    FILE *f = fopen(filename, "w+");
    if(f == NULL){
        printf("Cannot open file!\n");
        return;
    }
    fileStore(f, root);
    printf("So luong ma yeu cau da ghi: %d\n", slout);
}
//
int main(){
    int option;
    int flag = 0;
    do{
        menu();
        scanf("%d", &option);
        switch(option){
            case 1: processLoad("booking_orders.txt"); break;
            case 2: kthople(); break;
            case 3:
                if(flag == 0){
                    ReadtoTree();
                    flag = 1;
                }
                processFind();
                break;
            case 4: processUpdate(); break;
            case 5: processRemove(); break;
            case 6: processStore("booking_output.txt");
        }
    }while(option != 7);
    listRelease();
    freeTree(root);
    return 0;
}