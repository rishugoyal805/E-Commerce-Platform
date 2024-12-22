#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <windows.h>

using namespace std;

const int MAX_USERS = 100;

// Function prototypes for the payment-related functions
bool payment(float amt);
double add_to_wallet();
void buy_cart();

// Placeholder functions for programmer menu
void modifyQuantity();
void removeProduct();
void searchProduct_id(std::string id) ;
void searchProduct_name(std::string name);
void searchProduct_type(std::string type) ;

void wallet();
void refund_replace();
void remove_customer();

// Function to display products
void c_displayProducts();
void p_displayProducts();

// Functions for programmer and customer menus
void displayProgrammerMenu();
void displayCustomerMenu();

class Cart;

class Product {
public:
    int s_no;
    string p_id;
    string name;
    string type;
    string c_name;
    string refund;
    string cod;
    int p_quantity;
    float price;
    int discount;

    static vector<Product> products;
    static int lastSno;

    // Member function to add a new product
    static void addProduct() {
        Product newProduct;
        newProduct.s_no = ++lastSno;
        cout << "\nEnter Product Id: ";
        cin >> newProduct.p_id;
        cout << "Enter Product Name: ";
        cin.ignore();
        getline(cin, newProduct.name);
        cout << "Enter Product Type: ";
        getline(cin, newProduct.type);
        cout << "Enter Company Name: ";
        getline(cin, newProduct.c_name);
        cout << "Enter Refundable Days: ";
        cin >> newProduct.refund;
        cout << "Does the product have COD? Y/N: ";
        char ch;
        cin >> ch;
        newProduct.cod = (ch == 'Y' || ch == 'y') ? "Yes" : "No";
        cout << "Enter Product Quantity: ";
        cin >> newProduct.p_quantity;
        cout << "Enter Product Price: ";
        cin >> newProduct.price;
        cout << "Enter Discount in %: ";
        cin >> newProduct.discount;
        products.push_back(newProduct);
        sleep(1);
        cout << "\nProduct added successfully!\n\n" << endl;
        sleep(1);
    }
};

class Cart:public Product{
public:
    class CartItem {
    public:
        const Product* product;
        int quantity;

        CartItem(const Product* p, int q) : product(p), quantity(q) {}

        const Product* getProduct() const { return product; }
        int getQuantity() const { return quantity; }
    };

    vector<CartItem> items;

    // Function to add a product to the cart
    void addToCart(const Product* product, int quantity) {
        auto iter = find_if(items.begin(), items.end(), [product](const CartItem& item) {
            return item.getProduct()->s_no == product->s_no;
        });

        if (iter != items.end()) {
            iter->quantity += quantity;
        } else {
            // Product is not in the cart, add it
            items.emplace_back(product, quantity);
        }
        cout << "Product added to cart successfully!" << endl;
    }

    // Function to remove a product from the cart
    bool removeFromCart(string id) {
    auto iter = find_if(items.begin(), items.end(), [id](const CartItem& item) {
        return item.getProduct()->p_id == id;
    });

    if (iter != items.end()) {
        cout << "Press \n 1. If you want to reduce the quantity?\n 2. If you want to delete this item?" << endl;
        int c;
        cout<<"Enter your choice: ";
        cin >> c;
        cout<<"\n";
        if (c == 1) {
            cout << "Enter the quantity by which you want to reduce the quantity:";
            int x;
            cin >> x;
            if ((iter->quantity) > x) {
                iter->quantity -= x;
                cout << "Quantity reduce successfully from the cart.\n\n";
                viewCart();
                return true;
            } else {
                cout << "You can't reduce the quantity more than the quantity in the cart\n\n" << endl;
                return false;
            }
        } else if (c == 2) {
            items.erase(iter);
            cout << "Product removed from cart successfully\n\n" << endl;
            return true;
        } else {
            cerr << "Invalid input.\nPlease enter either '1' or '2'\n\n" << endl;
            return false;
        }
        cout<<"\n\n\n";
    } else {
        cerr << "Error: The specified product was not found in your cart.\n\n" << endl;
        return false;
    }return true;}

    // Function to view the contents of the cart
    void viewCart() const {
        if (items.empty()) {
            cout << "\nYour cart is empty.\n\n" << endl;
        } else {
            float i=0;
            cout<<"\n************************ Your Cart ***********************\t\n"<< endl;
            cout << "Product ID\tProduct Name\tQuantity\tPrice" << endl;
            for (const auto& item : items) {
                const Product* product = item.getProduct();
                cout << product->p_id << "\t\t" << product->name << "\t\t"
                     << item.getQuantity() << "\t\t$" << product->price << endl;
                     i += product->price * item.getQuantity();
                     }
            cout<<"\n**********************************************************\n"<< endl;
            cout<<"Total bill of cart is :- "<<i<<" Rupees\n\n\n";}
    }
    void buy_cart() {
    float total_bill = 0;
    if (items.empty()) {
        cout << "\nYour cart is empty.\n\n" << endl;
        return;
    }
    else {
            cout<<"\n************************ Your Cart ***********************\t\n"<< endl;
            cout << "Product ID\tProduct Name\tQuantity\tPrice" << endl;
            for (const auto& item : items) {
                const Product* product = item.getProduct();
                cout << product->p_id << "\t\t" << product->name << "\t\t"
                     << item.getQuantity() << "\t\t$" << product->price << endl;
                     total_bill += product->price * item.getQuantity();
                     }
            cout<<"\n**********************************************************\n"<< endl;
            cout<<"Total bill of cart is :- "<<total_bill<<" Rupees\n\n\n";}
    cout << "Do you want to purchase this whole cart? (y/n): ";
    string buy;
    cin >> buy;

    if (buy == "y" || buy == "Y") {
        cout << "Press:\n 1. To opt for COD.\n 2. For other options: \n Enter choice:";
        int D;
        cin >> D;
        cout<<endl;
        if (D == 1) {
            cout << "You cannot purchase this whole cart by COD.\n" << endl;
            cout << "Please use other methods to purchase this whole Cart or buy the items one by one through the view Products Option." << endl;
            cout << "Do you still want to purchase this whole Cart? (y/n): ";
            string D1;
            cin >> D1;
            if (D1 == "y" || D1 == "Y") {
                sleep(1);
                bool success = payment(total_bill);
                if (success) {
                    items.clear();
                    cout << "Cart has been cleared." << endl;
                }
            } else {
                cout << "Your order has been cancelled." << endl;
            }
        } else if (D == 2) {
            bool success = payment(total_bill);
            if (success) {
                items.clear();
                cout << "Cart has been cleared." << endl;
            }
            else {
                cout << "Your order has been cancelled." << endl;
            }
        }else{cout<< "Invalid choice \nYour order has been cancelled." << endl;}
    } else if (buy == "n" || buy == "N") {
        cout << "Total bill of cart is: $" << total_bill << endl;
        cout << "Thank you for shopping with us." << endl;
    } else {
        cout << "Invalid input. Please try again." << endl;
    }
    cout<<"\n\n";
}
};

int Product::lastSno = 0;
vector<Product> Product::products;
string userID;

void set(string username) {
    userID = username;
}

std::string getu() {
    return userID;
}

class Customer {
private:
    const Product* findProductById(const std::string& id) const {
    auto iter = std::find_if(Product::products.begin(), Product::products.end(), [id](const Product& product) {
        return product.p_id == id;
    });

    if (iter != Product::products.end()) {
        return &(*iter);
    }

    return nullptr;
}

public:
    string name;
    string lname;
    string mobile_no;
    string email_address;
    string address;
    string pincode;
    string city;
    string state;
    string district;
    string password;
    double money = 0;
    Cart cart;
    string userID;

    static vector<Customer> customers;

    // Function to create a new customer account.
    static void addCustomer() {
        Customer newCustomer;
        cout << "Enter your first name: ";
        cin >> newCustomer.name;
        cout << "Enter your last name: ";
        cin >> newCustomer.lname;
        cout << "Enter your password: ";
        cin >> newCustomer.password;
        cout << "Enter your Mobile number: ";
        cin >> newCustomer.mobile_no;
        cout << "Enter your E-mail address: ";
        cin >> newCustomer.email_address;
        cout << "Enter your address: ";
        cin.ignore();
        getline(cin, newCustomer.address);
        cout << "Enter your pincode: ";
        cin >> newCustomer.pincode;
        cout << "Enter your city: ";
        cin.ignore();
        getline(cin, newCustomer.city);
        cout << "Enter your district: ";
        cin >> newCustomer.district;
        cout << "Enter your state: ";
        cin.ignore();
        getline(cin, newCustomer.state);

        cout << "\n**************************\n\t\t\nYour Username Is: " << newCustomer.name << "\n\n"
             << "Your Password Is: "
             << newCustomer.password << "\n\n**************************" << endl;
        cout << "\nNote:-Remember it as it will be used later" << endl;
        customers.push_back(newCustomer);
        cout << "Signup completed." << "\n\n" << endl;
    }

    //  displaying customers
    static void displayAllCustomers() {
        cout << "\nList of all customers:\n";
        cout << "Username\tMobile Number\tpassword\tCity" << endl;
        if(customers.empty()){cout<<"\nNo Customer found.\n\n";}
        for (const auto& customer : customers) {
            cout << customer.name << "\t\t" << customer.mobile_no << "\t" << customer.password << "\t\t"
            << customer.city << endl;
        }
        cout << endl<<endl;
    }

    static bool userAuthenticate(string user, string pass) {
        for (const auto& customer : customers) {
            if (customer.name == user && customer.password == pass) {
                return true;
            }
        }
        return false;
    }

    static void deleteCustomer(const std::string& username, const std::string& password);
    void wallet() {
        cout << "Your current balance is: " << money << endl;
        cout << "Do you want to add money to the wallet:" << endl;
        cout << "1. Yes\n2. No" << endl;
        cout<<"Enter your choice: ";
        int c;
        cin >> c;

        if (c == 1) {
            double cash = add_to_wallet();
            money += cash;
            cout << "Your new balance is: " << money << endl;
        } else if (c == 2) {
            cout << "Your current balance is: " << money << endl;
        } else {
            cout << "Invalid choice" << endl;
        }
    }

    // Add product to cart
    void add_to_cart() {
    string id;
    int  quantity;
    cout << "Enter product ID to add to cart: ";
    cin >> id;
    const Product* product = findProductById(id);
    if (product == nullptr) {
        cout << "\nProduct not found. Please enter a valid product ID.\n\n" << endl;
        return;
    }
    cout << "Enter quantity to add to cart: ";
    cin >> quantity;
    if (quantity <= 0 || quantity > product->p_quantity) {
        cout << "\nInvalid quantity. Please enter a valid quantity.\n\n" << endl;
        return;
    }
    // Confirm the purchase
    cout << "Are you sure you want to add this product to your cart? (Y/N): ";
    char confirmation;
    cin >> confirmation;
    if (confirmation == 'Y' || confirmation == 'y') {
        cout << "\nAdding product to cart..." << endl;
        // Check if the product quantity is sufficient
        if (quantity > product->p_quantity) {
            cout << "\nNot enough quantity available. Please enter a valid quantity.\n\n" << endl;
            return;
        }
        // Check if the product is already in the cart
        auto iter = find_if(cart.items.begin(), cart.items.end(), [id](const Cart::CartItem& item) {
        return item.getProduct()->p_id == id;
        });
        if(iter != cart.items.end()) {
            // Product is already in the cart, update quantity
            iter->quantity += quantity;
        } else {
            // Product is not in the cart, add it
            cart.addToCart(product, quantity);
        }
        // Update product quantity
        const_cast<Product*>(product)->p_quantity -= quantity;
        cart.viewCart();
        //cout <<"\n\n" << endl;
    } else {
        cout<<"\n\n";
        cout << "\nProduct addition to cart canceled.\n\n" << endl;
    }}
    void view_cart() const {
        cart.viewCart();
    }
    void removeitemFromCart() {
    if (cart.items.empty()) {
        cout << "Your cart is empty.\n\n";
        return;
    }
    cart.viewCart();
    string id;
    cout << "Enter product ID to remove from cart: ";
    cin >> id;
    bool removed = cart.removeFromCart(id);
    if (!removed) {
        cout << "Failed to remove that product!" << endl;
    } else {
        cout<< endl;
    }
}
    void buy() {cart.buy_cart();}
};

vector<Customer> Customer::customers;

class Authenticate {
    static string usernames[MAX_USERS];
    static string passwords[MAX_USERS];
    static bool isProgrammerLoggedIn;
    static bool isCustomerLoggedIn;
    static string loggedInUser;

public:
    static bool checkAuthentication(std::string username, std::string password) {
        for (int i = 0; i < MAX_USERS; ++i) {
            if (usernames[i] == username && passwords[i] == password) {
                cout << "Authentication successful." << endl;

                if (username == "Programmer"||username == "Rishu"||username == "Swayam"||username == "Shreya") {
                    cout<<"\nyou are a programmer\n";
                    isProgrammerLoggedIn = true;
                    isCustomerLoggedIn = false;
                } else {
                    cout<<"welcome to our world of online shopping\n"<<endl;
                    isProgrammerLoggedIn = false;
                    isCustomerLoggedIn = true;
                }
                return true;
            }
        }
        return false;
    }

    static bool isProgrammerLogged() {
        return isProgrammerLoggedIn;
    }

    static bool isCustomerLogged() {
        return isCustomerLoggedIn;
    }

    static string getLoggedInUser() {
        return loggedInUser;
    }

    static void setProgrammerLoggedIn(bool value) {
        isProgrammerLoggedIn = value;
    }

    static void setCustomerLoggedIn(bool value) {
        isCustomerLoggedIn = value;
    }
};

string Authenticate::usernames[MAX_USERS] = {"Programmer", "Rishu", "Swayam", "Shreya"};
string Authenticate::passwords[MAX_USERS] = {"ProgrammerPwd", "246", "256", "245"};
bool Authenticate::isProgrammerLoggedIn = false;
bool Authenticate::isCustomerLoggedIn = false;
string Authenticate::loggedInUser = "";

bool payment(float amt) {
    int a;
    std::string upi_id, account_no, password;
    cout << "Which payment option do you want to opt: " << endl;
    cout << "1. UPI" << endl;
    cout << "2. NET BANKING" << endl;
    cout << "3. By WALLET" << endl;
    cout << "\nEnter your choice: ";
    cin >> a;
    cout << endl;

    if (a == 1) {
        // Process UPI payment
        // Assume payment is successful
        cout << "Enter UPI ID: ";
        cin >> upi_id;
        sleep(1);
        cout << "Your UPI ID has been verified" << endl;
        cout << "GO TO YOUR UPI PAYMENT APPLICATION AND COMPLETE THE TRANSACTION" << endl;
        sleep(5);
        cout << "YOUR TRANSACTION HAS BEEN COMPLETED" << endl;
        cout << "THANK YOU FOR YOUR PURCHASE" << endl;
        cout << "YOUR ORDER HAS BEEN PLACED\n\n" << endl;
        return true;
    } else if (a == 2) {
        // Process NET BANKING payment
        // Assume payment is successful
        cout << "ENTER YOUR ACCOUNT NUMBER: ";
        cin >> account_no;
        cout << "Enter your password: ";
        cin >> password;
        cout << "PROCESSING ----" << endl;
        sleep(5);
        cout << "YOU ARE GOING TO BE DIRECTED TO YOUR SPECIFIC BANK'S NET BANKING PORTAL" << endl;
        sleep(5);
        cout << "YOUR TRANSACTION HAS BEEN COMPLETED" << endl;
        cout << "THANK YOU FOR YOUR PURCHASE" << endl;
        cout << "YOUR ORDER HAS BEEN PLACED\n\n" << endl;
        return true;
    } else if (a == 3) {
        // Process WALLET payment
        Customer* customer = nullptr;
        for (auto& c : Customer::customers) {
            if (c.name == getu()) {
                customer = &c;
                break;
            }
        }
        if (customer->money < amt) {
            cout << "Your transaction amount is greater than the available balance in your wallet." << endl;
            cout << "Your bill in rupees: " << amt << endl;
            cout << "Your money in wallet is: Rs." << customer->money << "\nPlease recharge your wallet.\n";
            cout << endl << endl;
            return false;
        } else {
            // Payment successful, deduct amount from wallet balance
            cout << "Using Wallet" << endl;
            sleep(2);
            cout << "Your money in wallet is: Rs." << customer->money << endl;
            cout << "YOUR TRANSACTION HAS BEEN COMPLETED" << endl;
            customer->money -= amt;
            cout << "Your remaining money in wallet is: Rs." << customer->money << endl;
            cout << "THANK YOU FOR YOUR PURCHASE" << endl;
            cout << "YOUR ORDER HAS BEEN PLACED\n\n" << endl;
            return true;
        }
    } else {
        cout << "YOU HAVE CHOSEN THE WRONG OPTION\n\n" << endl;
        return false;
    }
    return false;
}

// add money to wallet
double add_to_wallet(){
    int a;
    std::string upi_id, account_no, password;
    double amount;
    cin.ignore();
    cout<<"\nEnter amount that you want to add to the Wallet: ";
    cin>>amount;
    cout << "\nWhich payment option do you want to opt: " << endl;
    cout << "1. UPI" << endl;
    cout << "2. NET BANKING\n";
    cout<<"Enter your choice: ";
    cin >> a;

    if (a == 1) {
        cout << "\nEnter UPI ID: ";
        cin >> upi_id;
        sleep(1);
        cout << "Your UPI ID has been verified" << endl;
        cout << "GO TO YOUR UPI PAYMENT APPLICATION AND COMPLETE THE TRANSACTION" << endl;
        sleep(3);
        cout << "YOUR TRANSACTION HAS BEEN COMPLETED" << endl;
        return amount;}
    else if (a == 2) {
        cout << "\nENTER YOUR ACCOUNT NUMBER: ";
        cin >> account_no;
        cout << "Enter your password: ";
        cin >> password;
        cout << "PROCESSING ----" << endl;
        sleep(3);
        cout << "YOU ARE GOING TO BE DIRECTED TO YOUR SPECIFIC BANKS NET BANKING PORTAL" << endl;
        sleep(3);
        cout << "YOUR TRANSACTION HAS BEEN COMPLETED" << endl;
        return amount;}
    else {
        cout << "YOU HAVE CHOSEN THE WRONG OPTION" << endl;}
    cout << "\n\n";
    return 0;
}

// modify product quantity
void modifyQuantity() {
    cout << "\nModifying product quantity\n";
    p_displayProducts();
    if(!Product::products.empty()){
    cout << "\nEnter the Product ID you want to modify: ";
    std::string productId;
    cin >> productId;

    bool productFound = false;
    for (auto& product : Product::products) {
        if (product.p_id == productId) {
            cout << "Product Details:\n";
            cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tQuantity\tPrice\tDiscount" << endl;
            cout << product.s_no << "\t" << product.p_id << "\t\t" << product.name << "\t\t" << product.type << "\t"
             << product.c_name << "\t\t" << product.refund << "\t\t" << product.cod << "\t" << product.p_quantity
             << "\t\t" << product.price <<"\t" << product.discount << endl;

            cout << "\nEnter the quantity to increase: ";
            int quantity;
            cin >> quantity;

            cout << "Enter Programmer Password: ";
            int pass;
            cin >> pass;

            if (pass == 256 || pass == 246 || pass == 245) {
                product.p_quantity += quantity;
                cout << "Quantity has been increased. Order placed successfully.\n\n" << endl;
            } else {
                cout << "Wrong Programmer Password! Quantity modification canceled.\n\n" << endl;
            }
            productFound = true;
            break;
        }
    }
    if (!productFound) {
        cout << "Product not found with ID '" << productId << "'\n\n" << endl;
    }}
}

//remove products
void removeProduct() {
    p_displayProducts();
    if(!Product::products.empty()){
    cout << "Enter the Product ID you want to remove: ";
    std::string productId;
    cin >> productId;
    cout<<endl;
    bool productFound = false;
    for (auto it = Product::products.begin(); it != Product::products.end(); ++it) {
        if (it->p_id == productId) {
            cout << "Product Details:\n";
            cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tPrice\tDiscount" << endl;
            cout << it->s_no << "\t" << it->p_id << "\t\t" << it->name << "\t\t" << it->type << "\t"
            << it->c_name << "\t\t" << it->refund << "\t\t" << it->cod << "\t" << it->price << "\t" << it->discount << endl;

            cout << "\nAre you sure you want to delete this product? (Y/N): ";
            char confirmation;
            cin >> confirmation;

            if (confirmation == 'Y' || confirmation == 'y') {
                cout << "Enter Programmer Password: ";
                int pass;
                cin >> pass;

                if (pass == 256 || pass == 246 || pass == 245) {
                    Product::products.erase(it);
                    cout << "Product with ID '" << productId << "' has been successfully deleted.\n" << endl;
                } else {
                    cout << "Wrong Programmer Password! Product deletion canceled.\n\n" << endl;
                }
            } else {
                cout << "Product deletion canceled.\n\n" << endl;
            }
            productFound = true;
            break;
        }
    }
    if (!productFound) {
        cout << "Product not found with ID '" << productId << "'\n\n" << endl;
    }}
}

// Search the products by ID
void searchProduct_id(std::string id) {
    for (const auto& product : Product::products) {
        if (product.p_id == id) {
            cout << "\nProducts detail:\n";
            cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tPrice\t Discount" << endl;
            cout << product.s_no << "\t" << product.p_id << "\t\t" << product.name << "\t\t" << product.type << "\t"
            << product.c_name << "\t\t" << product.refund << "\t\t" << product.cod << "\t"<< product.price <<"\t " << product.discount << endl;
            cout<<"\n\n";
            return;
        }
    }
    cout << "No product found with ID: '" << id << "'." << endl;
    cout << endl<<endl;
}
// Search the products by Name
void searchProduct_name(std::string name) {
    for (const auto& product : Product::products) {
        if (product.name == name) {
            cout << "\nProducts detail:\n";
            cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tPrice\t Discount" << endl;
            cout << product.s_no << "\t" << product.p_id << "\t\t" << product.name << "\t\t" << product.type << "\t"
            << product.c_name << "\t\t" << product.refund << "\t\t" << product.cod << "\t"<< product.price <<"\t " << product.discount << endl;
            cout<<"\n\n";
            return;
        }
    }
    cout << "No product found with Name: '" << name << "'." << endl;
    cout << endl<<endl;
}
// Search the products by Type
void searchProduct_type(const std::string type) {
        cout << "\nProducts detail:\n";
        cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tPrice\t Discount" << endl;
    for (const auto& product : Product::products) {
        if (product.type == type) {
            cout << product.s_no << "\t" << product.p_id << "\t\t" << product.name << "\t\t" << product.type << "\t"
            << product.c_name << "\t\t" << product.refund << "\t\t" << product.cod << "\t"<< product.price <<"\t " << product.discount << endl;
            cout<<"\n\n";
            return;
        }
    }
    cout << "No product found with Type: '" << type << "'." << endl;
    cout << endl<<endl;
}
void refund_replace() {
    string username, password;
    cout << "Enter your name: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;
    if(username!=getu()){
        cout << "Invalid customer details. Please try again.\n\n\n";
        return;
    }
    std::string n;
    cin.ignore();
    cout<<"Please enter Product id or name you are looking for : ";
    getline(cin,n);
    cout << "\nProducts detail:\n";
    cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tPrice\t Discount" << endl;
    for (const auto& product : Product::products) {
        if (product.p_id==n||product.name==n) {
            cout << product.s_no << "\t" << product.p_id << "\t\t" << product.name << "\t\t" << product.type << "\t"
            << product.c_name << "\t\t" << product.refund << "\t\t" << product.cod << "\t"<< product.price <<"\t " << product.discount << endl;
            cout<<"\n\n";
            std::string reason;
            cin.ignore();
            cout << "Enter the reason for refund/replacement (refund/replace): ";
            getline(cin,reason);
            if (reason == "refund" || reason == "REFUND") {
               cout << "\nRefund initiated. Our service agent will contact you soon for further step to be taken.\n";
            } else if (reason == "replace"  || reason == "REPLACE") {
               cout << "\nReplacement initiated. Our service agent will contact you soon for further step to be taken.\n";
            } else {
               cout << "\nOur agent will contact you soon.\n";
            }
            cout<<"\n\n";
            return;
        }
    }
    cout << "No product found with id / Name: '" << n << "'." << endl;
    cout<<"\n\n";
}
//finalizing the deletion
void Customer::deleteCustomer(const std::string& username, const std::string& password) {
    for (auto it = customers.begin(); it != customers.end(); ++it) {
        if (it->name == username && it->password == password) {
            cout << "\nCustomer Details:\n";
            cout << "Username: " << it->name << "\nMobile Number: " << it->mobile_no
                 << "\nEmail Address: " << it->email_address << "\nAddress: " << it->address
                 << "\nPincode: " << it->pincode << "\nCity: " << it->city
                 << "\nDistrict: " << it->district << "\nState: " << it->state << endl;

            cout << "\nAre you sure you want to delete this customer? (Y/N): ";
            char confirmation;
            cin >> confirmation;
            if (confirmation == 'Y' || confirmation == 'y') {
                customers.erase(it);
                cout << "\nCustomer '" << username << "' has been successfully deleted.\n\n" << endl;
            } else {
                cout << "\nDeletion canceled.\n\n" << endl;
            }
            return;
        }
    }
    cout << "\nCustomer not found or incorrect password.\n\n"<< endl;
}
//removing the customer
void remove_customer(){
    string username, pass1;
    int pass;
    cin.ignore();
    cout << "\nEnter Customer name: ";
    getline(cin, username);
    cin.ignore();
    cout << "Enter Customer password: ";
    getline(cin, pass1);
    cin.ignore();
    cout << "Enter Programmer Password: ";
    cin >> pass;
    if (pass == 256 || pass == 246 || pass == 245) {
        Customer::deleteCustomer(username, pass1);
    } else {
        cout << "Wrong Programmer Password!" << endl;
    }
}
void c_displayProducts() {
    string piid;
    char choice;
    if(Product::products.empty()){
        cout<<"No product available.\n\n\n";
        return;
    }
    else{
    cout << "\nList of all products:\n\n";
    cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tPrice\tDiscount" << endl;

    for (const auto& product : Product::products) {
        cout << product.s_no << "\t" << product.p_id << "\t\t" << product.name << "\t\t" << product.type << "\t"
            << product.c_name << "\t\t" << product.refund << "\t\t" << product.cod << "\t" << product.price << "\t" << product.discount << endl;
    }
    cout << endl;
    cout << "Do you want to buy any product? y/n: ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        cout << "Please enter product id you want to buy: ";
        cin >> piid;
        auto it = std::find_if(Product::products.begin(), Product::products.end(), [&](const auto& p) {
            return p.p_id == piid;
        });
        if (it != Product::products.end()) {
            cout << "\nProduct found:\n";
            cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tPrice\tDiscount" << endl;
            cout << (*it).s_no << "\t" << (*it).p_id << "\t\t" << (*it).name << "\t\t" << (*it).type << "\t"
                << (*it).c_name << "\t\t" << (*it).refund << "\t\t" << (*it).cod << "\t" << (*it).price << "\t" << (*it).discount << endl << endl;
            cout << "Do you want to opt Cash On Delivery? y/n: ";
            char d;
            cin >> d;
            if ((d == 'y' || d == 'Y') && ((*it).cod == "Yes")) {
                cout << "Cash On Delivery initiated.\nYour Order Product Name: " << (*it).name << "\nPrice: " << (*it).price << " has been successfully Placed.\n" << endl;
            }
            else if ((d == 'y' || d == 'Y') && ((*it).cod == "No")) {
                cout << "Sorry, Cash On Delivery option is not available for this product." << endl;
                cout << "Do you want to proceed with other payment options? y/n: ";
                char other_option;
                cin >> other_option;
                if (other_option == 'y' || other_option == 'Y') {
                    payment((*it).price);
                }
            }
            else {
                payment((*it).price);
            }
        }
        else {
            cout << "Product not found\n\n\n";
        }
    }
    else {
        cout << "\n\n\n";
        return;
    }
    }
}

//display all products for programmer
void p_displayProducts() {
    if (Product::products.empty()) {
    cout << "No product found\n\n\n";
} else {
    cout << "\nList of all products:\n";
    cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tQuantity\tPrice\tDiscount" << endl;
    for (const auto& product : Product::products) {
        cout << product.s_no << "\t" << product.p_id << "\t\t" << product.name << "\t\t" << product.type << "\t"
            << product.c_name << "\t\t" << product.refund << "\t\t" << product.cod << "\t" << product.p_quantity
            << "\t\t" << product.price <<"\t" << product.discount << endl;
    }
    cout<<endl;
}
}

//programmer menu
void displayProgrammerMenu() {
    if (Authenticate::isProgrammerLogged()) {
        int a;
        cout << "***********************\n\n"<<"     Programmer Menu     \n\n"<<"***********************\n"<<endl;
        cout << "1. Product list\n2. Add product\n3. Modify product (quantity)\n4. Remove product\n5. Search product\n6. Display customers\n7. Remove customer\n0. Logout" <<"\n"<< endl;
        cout << "Enter choice: ";
        cin >> a;

        switch (a) {
            case 1:
                cout << "\nViewing products\n";
                p_displayProducts();
                break;
            case 2:
                cout<<"\nAdding products\n";
                Product::addProduct();
                break;
            case 3:
                modifyQuantity();
                break;
            case 4:
                removeProduct();
                break;
            case 5:
                cout <<"\nSearching products\n\n";
                cout<<"By which property do you want to filter product?"<<endl;
                int  ch;
                cout<<"1. By ID \n2. By Name \n3. By Type \nEnter your choice: ";
                cin>>ch;
                cout<<endl;
                switch(ch){
                    case 1: {
                    std::string i;
                    cout<<"\nPlease enter Product Id you are looking for : ";
                    cin>>i;
                    searchProduct_id(i);
                    break;
                    }
                    case 2:  {
                    std::string n;
                    cin.ignore();
                    cout<<"Please enter Product name you are looking for : ";
                    getline(cin,n);
                    searchProduct_name(n);
                    break;
                    }
                    case 3:  {
                     std::string  T;
                     cin.ignore();
                     cout<<"Please enter Product Type you are looking for : ";
                     getline(cin,T);
                     searchProduct_type(T);
                     break;
                    }}
                    break;
            case 6:
                Customer::displayAllCustomers();
                break;
            case 7:
                remove_customer();
                break;
            case 0:{
                Authenticate::setProgrammerLoggedIn(false);
                cout<<"Programmer logged Out successfully!!!\n\n\n";
                break;
            }
            default:
                cout << "\nInvalid Choice\n";
        }
    } else {
        cout << "Access denied. You are not a programmer." << endl;
    }
}

// Customer menu
void displayCustomerMenu() {
    if (Authenticate::isCustomerLogged()) {
        int b,op;
        cout << "***********************\n\n" << "     Customer Menu     \n\n" << "***********************\n\n" << endl;
        cout << "1. Product list\n2. Cart \n3. Wallet\n4. Refund and replace\n5. Search product\n0. Logout" << "\n" << endl;
        cout << "Enter choice: ";
        cin >> b;
        cout<<endl;
        if (b != 0) {
            // Find the current customer
            Customer *customer = nullptr;
            for (auto& c : Customer::customers) {
                if (c.name == getu()) {
                    customer = &c;
                    break;
                }
            }
            if (customer == nullptr) {
                cout << "Error: Customer not found. Please log in again." << endl;
                return;
            }
            switch (b) {
                case 1:
                    c_displayProducts();
                   break;
                case 2:
                    cout << "Do you want to : \n 1. Add to cart \n 2. Remove from cart \n 3. View Cart\n 4. Buy the cart\n";
                    cout<<"Enter your choice: ";
                    cin>> op ;
                    cout<<endl;
                    if(op==1){
                        if(Product::products.empty()){
                            cout<<"No Product available.\n\n\n";
                            return;
                        }
                        else{
                        cout << "Displaying product details:\n\n";
                        cout << "S.No\tProduct Id\tProduct Name\tProduct Type\tCompany Name\tRefundable\tCOD\tPrice\tDiscount" << endl;
                        for (const auto& product : Product::products) {
                        cout << product.s_no << "\t" << product.p_id << "\t\t" << product.name << "\t\t" << product.type << "\t"
                        << product.c_name << "\t\t" << product.refund << "\t\t" << product.cod << "\t" << product.price << "\t" << product.discount << endl;
                    }
                    cout << "\nAdding to the cart\n";
                    customer->add_to_cart();}}
                    else if (op==2){
                        customer->removeitemFromCart();}
                    else if(op==3){
                    customer->view_cart();}
                    else if(op==4){
                        customer->buy();}
                    else{
                        cout << "Invalid option.\nPlease enter a valid number.";
                    }
                    break;
                case 3:
                    customer->wallet();
                    cout<<endl<<endl;
                    break;
                case 4:
                    refund_replace();
                    break;
                case 5:
                    cout << "By which property do you want to filter product?";
                    int ch;
                    cout << "\n1.By ID \n2.By Name \n3.By Type\nEnter your choice: ";
                    cin >> ch;
                    cout<<endl;
                    switch (ch) {
                        case 1: {
                            std::string i;
                            cout << "Please enter Product Id you are looking for : ";
                            cin >> i;
                            searchProduct_id(i);
                            break;
                        }
                        case 2: {
                            std::string n;
                            cin.ignore();
                            cout << "Please enter Product name you are looking for : ";
                            getline(cin, n);
                            searchProduct_name(n);
                            break;
                        }
                        case 3: {
                            std::string T;
                            cin.ignore();
                            cout << "Please enter Product Type you are looking for : ";
                            getline(cin, T);
                            searchProduct_type(T);
                            break;
                        }
                    }
                break;
                default:
                    cout << "\nInvalid Choice\n\n\n";
            }
        } else {
            Authenticate::setCustomerLoggedIn(false);
            cout<<"Logged out Successfully"<<"\nThank you for Shopping with us!!!\n\n\n";
        }
    } else {
        cout << "Access denied. You are not a customer." << endl;
    }
}

//header
void displayHeader() {
    cout << "************************************************************" << endl;
    cout << "*                    WELCOME TO OUR                        *" << endl;
    cout << "*                     ONLINE STORE!                        *" << endl;
    cout << "*                                                          *" << endl;
    cout << "* Submitted to:                             Programmed by: *" << endl;
    cout << "* Archana Purwar                                     Rishu *" << endl;
    cout << "* Sangeeta Mittal                             Swayam Gupta *" << endl;
    cout << "*                                          Shreya Varshney *" << endl;
    cout << "*                                                          *" << endl;
    cout << "************************************************************" <<"\n"<<endl;
}

int main() {
    int choice;
    Authenticate auth;
    while (true) {
        if (Authenticate::isProgrammerLogged() || Authenticate::isCustomerLogged()) {
            if (Authenticate::isProgrammerLogged()) {
                displayProgrammerMenu();
            } else if (Authenticate::isCustomerLogged()) {
                displayCustomerMenu();
            }
        } else {
            displayHeader();
            cout << "1. Signup   2. Login   3. Exit" <<"\n"<< endl;
            cout << "Enter your choice: ";
            cin >> choice;
            cout<<endl;
            if (choice==1||choice==2||choice==3) {

                switch (choice) {
                    case 1:
                        Customer::addCustomer();
                        break;
                    case 2:
                    {
                        string username, password;
                        cout << "Enter username: ";
                        cin >> username;
                        cout << "Enter password: ";
                        cin >> password;

                        if (auth.checkAuthentication(username, password)) {
                            Authenticate::setProgrammerLoggedIn(true);
                            Authenticate::setCustomerLoggedIn(false);
                            cout << "Programmer login successful.\n" << endl;
                        } else if (Customer::userAuthenticate(username, password)) {
                            Authenticate::setProgrammerLoggedIn(false);
                            Authenticate::setCustomerLoggedIn(true);
                            cout << "Customer login successful." << endl;
                            set(username);
                           std::string loggedInCustomer = username;
                           } else {
                          cout << "Invalid login credentials." << endl;
                          }
                          cout<<endl;
                        break;
                    }
                    case 3:
                        cout << "Exiting the program." << endl;
                        return 0;
                    default:
                        cout << "Invalid choice" << endl;
                }
            } else {
                cout << "Invalid input. Please enter a number." << endl;
                cin.clear();
                cin.ignore();
            }
        }
    }
   return 0;
}