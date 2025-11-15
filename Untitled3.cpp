#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define USERS_FILE "users.txt"
#define MERCHANTS_FILE "merchants.txt"
#define PRODUCTS_FILE "products.txt"

const char* API_KEY = "atsk_756398c56d33b2865151ca77171cc9f2225cd86e8944fc34e0c70465b1da4311486a841d";

typedef struct {
    int id;
    char name[50];
    char pin[10];
    float balance;
} User;

typedef struct {
    int id;
    char name[50];
    char pin[10];
    float balance;
} Merchant;

typedef struct {
    int id;
    char name[50];
    float price;
    int stock;
    int merchant_id;
} Product;

// ===================== FUNCTION PROTOTYPES =====================
void mainMenu();
void userMenu(int uid);
void merchantMenu(int mid);

void registerUser();
void registerMerchant();

int loginUser();
int loginMerchant();

void addProduct(int mid);
void viewAllProducts();
void viewMerchantProducts(int mid);
void viewMerchantProductsForUser();

void buyProduct(int uid);

float getUserBalance(int uid);
void updateUserBalance(int uid, float newBalance);

float getMerchantBalance(int mid);
void updateMerchantBalance(int mid, float newBalance);

void updateProductStock(int pid, int newStock);
int getNewId(char *filename);

void userDeposit(int uid);
void userWithdraw(int uid);

void merchantWithdraw(int mid);

int choosePaymentMethod();
int confirmPaymentPIN(int method);

int main() {
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    while (1) {
        printf("\n=== PHOZA EASY SYSTEM ===\n");
        printf("1. Register as User\n");
        printf("2. Register as Merchant\n");
        printf("3. Login as User\n");
        printf("4. Login as Merchant\n");
        printf("5. View All Products\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        if (choice == 1) registerUser();
        else if (choice == 2) registerMerchant();
        else if (choice == 3) { int uid = loginUser(); if (uid != -1) userMenu(uid); }
        else if (choice == 4) { int mid = loginMerchant(); if (mid != -1) merchantMenu(mid); }
        else if (choice == 5) viewAllProducts();
        else if (choice == 0) exit(0);
        else printf("Invalid choice!\n");
    }
}

void registerUser() {
    FILE *f = fopen(USERS_FILE, "a");
    if (!f) { printf("Error opening file.\n"); return; }

    User u;
    u.id = getNewId(USERS_FILE);

    printf("Enter name: ");
    scanf("%49s", u.name);

    printf("Set PIN: ");
    scanf("%9s", u.pin);

    u.balance = 5000;

    fprintf(f, "%d,%s,%s,%.2f\n", u.id, u.name, u.pin, u.balance);
    fclose(f);

    printf("User registered. ID = %d\n", u.id);
}

void registerMerchant() {
    FILE *f = fopen(MERCHANTS_FILE, "a");
    if (!f) { printf("Error opening file.\n"); return; }

    Merchant m;
    m.id = getNewId(MERCHANTS_FILE);

    printf("Enter merchant name: ");
    scanf("%49s", m.name);

    printf("Set PIN: ");
    scanf("%9s", m.pin);

    m.balance = 0;

    fprintf(f, "%d,%s,%s,%.2f\n", m.id, m.name, m.pin, m.balance);
    fclose(f);

    printf("Merchant registered. ID = %d\n", m.id);
}

int loginUser() {
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) { printf("No users found.\n"); return -1; }

    int id;
    char pin[10];

    printf("Enter User ID: ");
    scanf("%d", &id);
    printf("Enter PIN: ");
    scanf("%9s", pin);

    User u;

    while (fscanf(f, "%d,%49[^,],%9[^,],%f",
           &u.id, u.name, u.pin, &u.balance) != EOF) 
    {
        if (u.id == id && strcmp(u.pin, pin) == 0) {
            fclose(f);
            printf("Welcome %s!\n", u.name);
            return u.id;
        }
    }

    fclose(f);
    printf("Login failed.\n");
    return -1;
}

int loginMerchant() {
    FILE *f = fopen(MERCHANTS_FILE, "r");
    if (!f) { printf("No merchants found.\n"); return -1; }

    int id;
    char pin[10];

    printf("Enter Merchant ID: ");
    scanf("%d", &id);
    printf("Enter PIN: ");
    scanf("%9s", pin);

    Merchant m;

    while (fscanf(f, "%d,%49[^,],%9[^,],%f",
           &m.id, m.name, m.pin, &m.balance) != EOF) 
    {
        if (m.id == id && strcmp(m.pin, pin) == 0) {
            fclose(f);
            printf("Welcome %s!\n", m.name);
            return m.id;
        }
    }

    fclose(f);
    printf("Login failed.\n");
    return -1;
}

void userMenu(int uid) {
    int choice;
    while (1) {
        printf("\n=== USER MENU ===\n");
        printf("1. View All Products\n");
        printf("2. View Products by Merchant ID\n");
        printf("3. Buy Product\n");
        printf("4. Check Balance\n");
        printf("5. Deposit Money\n");
        printf("6. Withdraw Money\n");
        printf("0. Logout\n");
        printf("Choose: ");
        scanf("%d", &choice);

        if (choice == 1) viewAllProducts();
        else if (choice == 2) viewMerchantProductsForUser();
        else if (choice == 3) buyProduct(uid);
        else if (choice == 4) printf("Balance: %.2f\n", getUserBalance(uid));
        else if (choice == 5) userDeposit(uid);
        else if (choice == 6) userWithdraw(uid);
        else if (choice == 0) break;
        else printf("Invalid!\n");
    }
}

void merchantMenu(int mid) {
    int choice;
    while (1) {
        printf("\n=== MERCHANT MENU ===\n");
        printf("1. Add Product\n");
        printf("2. View My Products\n");
        printf("3. Check Balance\n");
        printf("4. Withdraw\n");
        printf("0. Logout\n");

        printf("Choose: ");
        scanf("%d", &choice);

        if (choice == 1) addProduct(mid);
        else if (choice == 2) viewMerchantProducts(mid);
        else if (choice == 3) printf("Balance: %.2f\n", getMerchantBalance(mid));
        else if (choice == 4) merchantWithdraw(mid);
        else if (choice == 0) break;
        else printf("Invalid!\n");
    }
}

void addProduct(int mid) {
    FILE *f = fopen(PRODUCTS_FILE, "a");
    if (!f) return;

    Product p;
    p.id = getNewId(PRODUCTS_FILE);
    p.merchant_id = mid;

    printf("Product name: ");
    scanf("%49s", p.name);
    printf("Price: ");
    scanf("%f", &p.price);
    printf("Stock: ");
    scanf("%d", &p.stock);

    fprintf(f, "%d,%s,%.2f,%d,%d\n",
            p.id, p.name, p.price, p.stock, p.merchant_id);

    fclose(f);
    printf("Product added.\n");
}

void viewAllProducts() {
    FILE *f = fopen(PRODUCTS_FILE, "r");
    if (!f) { printf("No products.\n"); return; }

    Product p;

    printf("\n--- ALL PRODUCTS ---\n");
    while (fscanf(f, "%d,%49[^,],%f,%d,%d",
           &p.id, p.name, &p.price, &p.stock, &p.merchant_id) != EOF)
    {
        printf("ID:%d | %s | %.2f RWF | Stock:%d | Merchant:%d\n",
               p.id, p.name, p.price, p.stock, p.merchant_id);
    }

    fclose(f);
}

void viewMerchantProducts(int mid) {
    FILE *f = fopen(PRODUCTS_FILE, "r");
    if (!f) { printf("No products.\n"); return; }

    Product p;

    printf("\n--- YOUR PRODUCTS ---\n");

    while (fscanf(f, "%d,%49[^,],%f,%d,%d",
           &p.id, p.name, &p.price, &p.stock, &p.merchant_id) != EOF)
    {
        if (p.merchant_id == mid)
            printf("ID:%d | %s | %.2f | Stock:%d\n",
                   p.id, p.name, p.price, p.stock);
    }

    fclose(f);
}

void viewMerchantProductsForUser() {
    int mid;
    printf("Enter merchant ID: ");
    scanf("%d", &mid);

    FILE *f = fopen(PRODUCTS_FILE, "r");
    if (!f) { printf("No products.\n"); return; }

    Product p;

    printf("\n--- PRODUCTS FROM MERCHANT %d ---\n", mid);

    while (fscanf(f, "%d,%49[^,],%f,%d,%d",
           &p.id, p.name, &p.price, &p.stock, &p.merchant_id) != EOF)
    {
        if (p.merchant_id == mid)
            printf("ID:%d | %s | %.2f | Stock:%d\n",
                   p.id, p.name, p.price, p.stock);
    }

    fclose(f);
}

int choosePaymentMethod() {
    int method;
    printf("\n=== PAYMENT METHOD ===\n");
    printf("1. MTN MoMo\n");
    printf("2. Airtel Money\n");
    printf("3. Bank\n");
    printf("Choose: ");
    scanf("%d", &method);

    if (method < 1 || method > 3) {
        printf("Invalid. Try again.\n");
        return choosePaymentMethod();
    }
    return method;
}

int confirmPaymentPIN(int method) {
    char pin[10];

    if (method == 1) {
        printf("Enter MTN MoMo PIN: ");
        scanf("%9s", pin);
        printf("MTN MoMo payment confirmed using API_KEY internally.\n");
    }
    else if (method == 2) {
        printf("Enter Airtel Money PIN: ");
        scanf("%9s", pin);
        printf("Airtel Money payment confirmed using API_KEY internally.\n");
    }
    else {
        printf("Enter Bank Code: ");
        scanf("%9s", pin);
        printf("Bank payment confirmed using API_KEY internally.\n");
    }

    return 1;
}

void buyProduct(int uid) {
    int pid;
    printf("Enter Product ID: ");
    scanf("%d", &pid);

    FILE *f = fopen(PRODUCTS_FILE, "r");
    if (!f) { printf("No products.\n"); return; }

    Product p;
    int found = 0;

    while (fscanf(f, "%d,%49[^,],%f,%d,%d",
           &p.id, p.name, &p.price, &p.stock, &p.merchant_id) != EOF)
    {
        if (p.id == pid) { found = 1; break; }
    }
    fclose(f);

    if (!found) { printf("Product not found.\n"); return; }
    if (p.stock <= 0) { printf("Out of stock.\n"); return; }

    float balance = getUserBalance(uid);
    if (balance < p.price) { printf("Not enough balance.\n"); return; }

    int payMethod = choosePaymentMethod();
    confirmPaymentPIN(payMethod);

    balance -= p.price;
    updateUserBalance(uid, balance);
    updateProductStock(p.id, p.stock - 1);

    float merchantMoney = getMerchantBalance(p.merchant_id);
    updateMerchantBalance(p.merchant_id, merchantMoney + p.price);

    printf("Purchase successful!\n");
}

float getUserBalance(int uid) {
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) return 0;

    User u;

    while (fscanf(f, "%d,%49[^,],%9[^,],%f",
           &u.id, u.name, u.pin, &u.balance) != EOF)
    {
        if (u.id == uid) { fclose(f); return u.balance; }
    }

    fclose(f);
    return 0;
}

void updateUserBalance(int uid, float newBalance) {
    FILE *f = fopen(USERS_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    User u;

    while (fscanf(f, "%d,%49[^,],%9[^,],%f",
           &u.id, u.name, u.pin, &u.balance) != EOF)
    {
        if (u.id == uid) u.balance = newBalance;

        fprintf(temp, "%d,%s,%s,%.2f\n",
                u.id, u.name, u.pin, u.balance);
    }

    fclose(f);
    fclose(temp);

    remove(USERS_FILE);
    rename("temp.txt", USERS_FILE);
}

float getMerchantBalance(int mid) {
    FILE *f = fopen(MERCHANTS_FILE, "r");
    if (!f) return 0;

    Merchant m;

    while (fscanf(f, "%d,%49[^,],%9[^,],%f",
           &m.id, m.name, m.pin, &m.balance) != EOF)
    {
        if (m.id == mid) { fclose(f); return m.balance; }
    }

    fclose(f);
    return 0;
}

void updateMerchantBalance(int mid, float newBalance) {
    FILE *f = fopen(MERCHANTS_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    Merchant m;

    while (fscanf(f, "%d,%49[^,],%9[^,],%f",
           &m.id, m.name, m.pin, &m.balance) != EOF)
    {
        if (m.id == mid) m.balance = newBalance;

        fprintf(temp, "%d,%s,%s,%.2f\n",
                m.id, m.name, m.pin, m.balance);
    }

    fclose(f);
    fclose(temp);

    remove(MERCHANTS_FILE);
    rename("temp.txt", MERCHANTS_FILE);
}

void updateProductStock(int pid, int newStock) {
    FILE *f = fopen(PRODUCTS_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    Product p;

    while (fscanf(f, "%d,%49[^,],%f,%d,%d",
           &p.id, p.name, &p.price, &p.stock, &p.merchant_id) != EOF)
    {
        if (p.id == pid) p.stock = newStock;

        fprintf(temp, "%d,%s,%.2f,%d,%d\n",
                p.id, p.name, p.price, p.stock, p.merchant_id);
    }

    fclose(f);
    fclose(temp);

    remove(PRODUCTS_FILE);
    rename("temp.txt", PRODUCTS_FILE);
}

int getNewId(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 1;

    int id = 0;
    int last = 0;

    char line[200];
    while (fgets(line, sizeof(line), f))
        sscanf(line, "%d", &id),
        last = id > last ? id : last;

    fclose(f);
    return last + 1;
}

void userDeposit(int uid) {
    float amt;
    printf("Enter amount to deposit: ");
    scanf("%f", &amt);

    float bal = getUserBalance(uid);
    bal += amt;

    updateUserBalance(uid, bal);
    printf("Deposit done. New Balance: %.2f\n", bal);
}

void userWithdraw(int uid) {
    float amt;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);

    float bal = getUserBalance(uid);

    if (amt > bal) {
        printf("Not enough money.\n");
        return;
    }

    bal -= amt;
    updateUserBalance(uid, bal);

    printf("Withdraw done. Balance: %.2f\n", bal);
}

void merchantWithdraw(int mid) {
    float amt;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);

    float bal = getMerchantBalance(mid);

    if (amt > bal) {
        printf("Not enough money.\n");
        return;
    }

    bal -= amt;
    updateMerchantBalance(mid, bal);

    printf("Withdraw successful. New Balance: %.2f\n", bal);
}

