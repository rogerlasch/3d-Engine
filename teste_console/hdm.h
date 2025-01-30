
#ifndef HDM_H
#define HDM_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

using namespace std;

// Interface base para comandos
class CommandInterface {
public:
    virtual void execute() = 0;
    virtual ~CommandInterface() = default; // Adicionado destrutor virtual
};

// Comando: CreateAccount
class CreateAccount : public CommandInterface {
public:
    void execute() override { // Corrigido "virtual" redundante
        cout << "Criando conta..." << endl;
    }
};

// Comando: Login
class Login : public CommandInterface {
public:
    void execute() override { // Corrigido "virtual" redundante
        cout << "Fazendo login..." << endl;
    }
};

// Fábrica de comandos
class CommandFactory {
private:
    unordered_map<string, function<shared_ptr<CommandInterface>()>> hcommands; // Corrigida a assinatura do mapa

public:
    CommandFactory() {
        // Registrando comandos na fábrica
        hcommands["createAccount"] = []() -> shared_ptr<CommandInterface> {
            return make_shared<CreateAccount>();
        };
        hcommands["login"] = []() -> shared_ptr<CommandInterface> {
            return make_shared<Login>();
        };
    }

    void execute(const string& name) {
        auto it = hcommands.find(name);
        if (it != hcommands.end()) {
            auto fnc = it->second(); // Executa a função para criar o comando
            fnc->execute();
        } else {
            cout << "O comando " << name << " não existe." << endl;
        }
    }
};

#endif // HDM_H
