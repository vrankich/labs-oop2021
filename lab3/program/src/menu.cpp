#include "menu.h" 

const char *closed_file::what() const throw()
{
    return "Can't open that file";
}

const char *OUTPUT[] = {"0. Quit", "1. Output", "2. Write to new file", "3. Append file"};
const char *TYPE[] = {"0. Quit", "1. Electric charge", "2. Dipole"};
const char *FUNCS[] = {"0. Quit",
    "1. Coulomb force",
    "2. Electric field",
    "3. Potential",
    "4. Potential energy"};

const int OUTPUT_SIZE = sizeof(OUTPUT) / sizeof(OUTPUT[0]);
const int TYPE_SIZE = sizeof(TYPE) / sizeof(TYPE[0]);
const int FUNCS_SIZE = sizeof(FUNCS) / sizeof(FUNCS[0]);

int dialog(const char *funcs[], int n)
{
    const char *err = "";
    int choice;
    do {
        std::cout << std::endl << err << std::endl;
        err = "Incorrect input...";
        for (int i = 0; i < n; i++) {
            std::cout << funcs[i] << std::endl;
        }
        std::cout << "\nMake a choice: ";
        choice = getchar() - '0';
        while (getchar() != '\n') {}
    } while (choice < 0 || choice >= n);
    return choice;
}

const char *get_file_name()
{
    std::cout << "Enter name of the file: ";
    const int buf_size = 81;
    char buf[buf_size];
    char *temp = nullptr, *res = nullptr;
    int len = 0, chunk_len = 0, str_len = 0;
    
    do {
        std::cin.get(buf, buf_size, '\n');
        if (!std::cout.good()) {
            delete [] res;
            return nullptr;
        } else if (std::cin.gcount() > 0) {
            chunk_len = strlen(buf);
            str_len = len + chunk_len;
            temp = res;
            res = new char[str_len + 1];
            memcpy(res, temp, len);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
            delete [] temp;
        } else {
            std::cout.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while(std::cin.gcount() == buf_size - 1);
    
    if (len > 0) {
        res[len] = '\0';
    } else {
        res = new char[1];
    }
    
    return res;
}

std::ostream& choose_output(std::ofstream &choice)
{
    const char *file_name = nullptr;
    int c = dialog(OUTPUT, OUTPUT_SIZE);
    switch(c) {
    case 0:
        choice.close();
        break;
    case 1:
        return std::cout;
    case 2:
        file_name = get_file_name();
        choice.open(file_name, std::ios::out);
        delete [] file_name;
        if (!choice.is_open()) {
            throw closed_file();
        }
        break;
    case 3:
        file_name = get_file_name();
        choice.open(file_name, std::ios::app);
        delete [] file_name;
        if (!choice.is_open()) {
            throw closed_file();
        }
        break;
    }
    return choice;
}

void choose_func(type t)
{
    if (t == QUIT) { return; }
    
    std::ofstream out;
    try {
        int c  = 0;
        Vector v;
        double q;
        
        do {
            c = dialog(FUNCS, FUNCS_SIZE);
            switch(c) {
            case 0:
                out.close();
                break;
            case 1:
                double q1, q2;
                std::cout << "Enter radius-vector: ";
                if (!getNum(v, std::cin)) { return; }
                std::cout << "Enter two charges:" << std::endl;
                if (!getNum(q1, std::cin)) { return; }
                if (!getNum(q2, std::cin)) { return; }
                if (t == EL_CHARGE) {
                    ech::coulomb_force(choose_output(out), q1, q2, v);
                } else {
                    dip::coulomb_force(choose_output(out), q1, q2, v);
                }
                out.close();
                break;
            case 2:
                if (t == EL_CHARGE) {
                    double q;
                    std::cout << "Enter radius-vector: ";
                    if (!getNum(v, std::cin)) { return; }
                    std::cout << "Enter charge:";
                    if (!getNum(q, std::cin)) { return; }
                    ech::electric_field(choose_output(out), q, v);
                } else {
                    Vector p;
                    std::cout << "Enter radius-vector: ";
                    if (!getNum(v, std::cin)) { return; }
                    std::cout << "Enter momentum:";
                    if (!getNum(p, std::cin)) { return; }
                    dip::electric_field(choose_output(out), p, v);
                }
                out.close();
                break;
            case 3:
                if (t == EL_CHARGE) {
                    double q;
                    std::cout << "Enter radius-vector: ";
                    if (!getNum(v, std::cin)) { return; }
                    std::cout << "Enter charge:" << std::endl;
                    if (!getNum(q, std::cin)) { return; }
                    ech::potential(choose_output(out), q, v);
                } else {
                    Vector p;
                    std::cout << "Enter radius-vector: ";
                    if (!getNum(v, std::cin)) { return; }
                    std::cout << "Enter momentum:";
                    if (!getNum(p, std::cin)) { return; }
                    dip::potential(choose_output(out), p, v);
                }
                out.close();
                break;
            case 4:
                if (t == EL_CHARGE) {
                    double q;
                    std::cout << "Enter radius-vector: ";
                    if (!getNum(v, std::cin)) { return; }
                    std::cout << "Enter charge:";
                    if (!getNum(q, std::cin)) { return; }
                    ech::potential(choose_output(out), q, v);
                } else {
                    Vector p;
                    std::cout << "Enter radius-vector: ";
                    if (!getNum(v, std::cin)) { return; }
                    std::cout << "Enter momentum:";
                    if (!getNum(p, std::cin)) { return; }
                    dip::potential(choose_output(out), p, v);
                }
                out.close();
                break;
            }
        } while (c != 0);
    } catch(std::exception &e) {
        std::cout << std::endl << e.what() << std::endl;
    }
}

void menu()
{
    int c = 0;
    do {
        c = dialog(TYPE, TYPE_SIZE);
        switch (c) {
        case 0:
            break;
        case 1:
            choose_func(EL_CHARGE);
            break;
        case 2:
            choose_func(DIPOLE);
            break;
        }
    } while (c != 0);
}
