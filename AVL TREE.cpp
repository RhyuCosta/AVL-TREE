#include <iostream>
#include <string>
using namespace std;

struct Contato {
    string nome;
    string telefone;
    string email;
    bool favorito;

    Contato(string n = "", string t = "", string e = "", bool f = false)
        : nome(n), telefone(t), email(e), favorito(f) {}
};

struct Node {
    Contato contato;
    Node* left;
    Node* right;
    int height;

    Node(Contato c) : contato(c), left(nullptr), right(nullptr), height(1) {}
};

// ---------- Funções AVL ----------

int height(Node* n) {
    return n ? n->height : 0;
}

int balanceFactor(Node* n) {
    return height(n->left) - height(n->right);
}

void updateHeight(Node* n) {
    n->height = 1 + max(height(n->left), height(n->right));
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    updateHeight(y);
    updateHeight(x);
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    updateHeight(x);
    updateHeight(y);
    return y;
}

Node* balance(Node* n) {
    updateHeight(n);
    int bf = balanceFactor(n);

    if (bf > 1) {
        if (balanceFactor(n->left) < 0)
            n->left = rotateLeft(n->left);
        return rotateRight(n);
    } else if (bf < -1) {
        if (balanceFactor(n->right) > 0)
            n->right = rotateRight(n->right);
        return rotateLeft(n);
    }
    return n;
}

// ---------- Inserção ----------

Node* insert(Node* root, Contato c) {
    if (!root) return new Node(c);
    if (c.nome < root->contato.nome)
        root->left = insert(root->left, c);
    else if (c.nome > root->contato.nome)
        root->right = insert(root->right, c);
    else
        return root; // nome duplicado não permitido

    return balance(root);
}

// ---------- Remoção ----------

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Node* remove(Node* root, string nome) {
    if (!root) return nullptr;

    if (nome < root->contato.nome)
        root->left = remove(root->left, nome);
    else if (nome > root->contato.nome)
        root->right = remove(root->right, nome);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            delete root;
            return temp;
        }
        Node* temp = minValueNode(root->right);
        root->contato = temp->contato;
        root->right = remove(root->right, temp->contato.nome);
    }

    return balance(root);
}

// ---------- Buscas e Listagens ----------

Node* buscar(Node* root, string nome) {
    if (!root) return nullptr;
    if (nome < root->contato.nome)
        return buscar(root->left, nome);
    else if (nome > root->contato.nome)
        return buscar(root->right, nome);
    return root;
}

void inOrder(Node* root) {
    if (!root) return;
    inOrder(root->left);
    cout << root->contato.nome << " - " << root->contato.telefone
         << " - " << root->contato.email
         << (root->contato.favorito ? " [Favorito]" : "") << endl;
    inOrder(root->right);
}

void listarFavoritos(Node* root) {
    if (!root) return;
    listarFavoritos(root->left);
    if (root->contato.favorito)
        cout << root->contato.nome << " - " << root->contato.telefone << endl;
    listarFavoritos(root->right);
}

// ---------- Menu CLI ----------

void menu() {
    cout << "\n1. Adicionar contato\n2. Buscar contato\n3. Listar todos\n";
    cout << "4. Listar favoritos\n5. Remover contato\n6. Sair\nEscolha: ";
}

// ---------- Função Principal ----------

int main() {
    Node* root = nullptr;
    int op;
    do {
        menu();
        cin >> op;
        cin.ignore();

        if (op == 1) {
            string nome, tel, email;
            char fav;
            cout << "Nome: "; getline(cin, nome);
            cout << "Telefone: "; getline(cin, tel);
            cout << "Email: "; getline(cin, email);
            cout << "Favorito? (s/n): "; cin >> fav; cin.ignore();
            root = insert(root, Contato(nome, tel, email, fav == 's'));
        } else if (op == 2) {
            string nome;
            cout << "Buscar por nome: "; getline(cin, nome);
            Node* res = buscar(root, nome);
            if (res)
                cout << "Encontrado: " << res->contato.nome << ", "
                     << res->contato.telefone << ", "
                     << res->contato.email
                     << (res->contato.favorito ? " [Favorito]" : "") << endl;
            else
                cout << "Contato não encontrado.\n";
        } else if (op == 3) {
            cout << "\nContatos em ordem alfabética:\n";
            inOrder(root);
        } else if (op == 4) {
            cout << "\nFavoritos:\n";
            listarFavoritos(root);
        } else if (op == 5) {
            string nome;
            cout << "Digite o nome do contato a ser removido: ";
            getline(cin, nome);
            if (buscar(root, nome)) {
                root = remove(root, nome);
                cout << "Contato removido com sucesso.\n";
            } else {
                cout << "Contato não encontrado.\n";
            }
        }
    } while (op != 6);

    cout << "Encerrando o programa...\n";
    return 0;
}
