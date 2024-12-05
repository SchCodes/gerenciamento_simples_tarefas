#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <limits>
#include <ctime>

// Estrutura da tarefa
struct Tarefa {
    int id; // ID único 
    std::string titulo; // Título 
    std::string descricao; // Descrição
    std::string dataCriacao; // Data de criação
    std::string status; // Status  (pendente, em andamento, concluída)
    std::string prioridade; // Prioridade  (baixa, média, alta)
};

std::vector<Tarefa> tarefas; // Vetor que armazena todas as tarefas
int proximoId = 1; // Variável que armazena o próximo ID disponível para uma nova tarefa

bool validarData(const std::string& data) {
    if (data.size() != 8 || data[2] != '/' || data[5] != '/') return false;
    // Verifica se os caracteres são números
    for (std::string::size_type i = 0; i < data.size(); ++i) {
        if (i != 2 && i != 5 && !isdigit(data[i])) return false;
    }

    // Converte a data para struct tm
    int dia = std::stoi(data.substr(0, 2));
    int mes = std::stoi(data.substr(3, 2));
    int ano = std::stoi(data.substr(6, 2)) + 2000; // Assume que o ano está no formato "aa"

    std::tm dataInserida = {};
    dataInserida.tm_mday = dia;
    dataInserida.tm_mon = mes - 1;
    dataInserida.tm_year = ano - 1900;

    // Obtém a data atual
    std::time_t t = std::time(nullptr);
    std::tm dataAtual = *std::localtime(&t);

    // Ajusta a data atual para considerar um dia a menos
    dataAtual.tm_mday -= 1;
    std::mktime(&dataAtual);

    // Compara as datas
    if (std::mktime(&dataInserida) < std::mktime(&dataAtual)) {
        return false;
    }

    return true;
}

bool validarTitulo(const std::string& titulo) {
    return !titulo.empty() && std::any_of(titulo.begin(), titulo.end(), [](char c) { return !isdigit(c); });
}

// Função para adicionar uma nova tarefa
void adicionarTarefa() {
    Tarefa novaTarefa;
    novaTarefa.id = proximoId++; // Atribui um ID único à nova tarefa

    std::cout << "Digite o título: ";
    std::cin.ignore(); // Ignora o caractere de nova linha pendente no buffer de entrada
    std::getline(std::cin, novaTarefa.titulo); // Lê o título 
    while (!validarTitulo(novaTarefa.titulo)) {
        std::cout << "\nTítulo é obrigatório e não pode ser somente numérico. Digite o título: ";
        std::getline(std::cin, novaTarefa.titulo);
    }

    std::cout << "\nDigite a descrição (máximo 200 caracteres): ";
    std::getline(std::cin, novaTarefa.descricao); // Lê a descrição 
    if (novaTarefa.descricao.length() > 200) {
        novaTarefa.descricao = novaTarefa.descricao.substr(0, 200); // Limita a 200 caracteres
    }

    std::cout << "\nDigite a data (dd/mm/aa): ";
    std::getline(std::cin, novaTarefa.dataCriacao); // Lê a data de criação 
    while (!validarData(novaTarefa.dataCriacao)) {
        std::cout << "\nFormato de data inválido ou data anterior à data atual. Digite a data (dd/mm/aa): ";
        std::getline(std::cin, novaTarefa.dataCriacao);
    }

    std::cout << "\nDigite o status (1 para pendente, 2 para em andamento, 3 para concluída): ";
    int status;
    while (!(std::cin >> status) || status < 1 || status > 3) {
        std::cin.clear(); // Limpa o estado de erro
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora a entrada inválida
        std::cout << "\nOpção inválida. Digite o status (1 para pendente, 2 para em andamento, 3 para concluída): ";
    }
    novaTarefa.status = (status == 1) ? "pendente" : (status == 2) ? "em andamento" : "concluída";

    std::cout << "\nDigite a prioridade (1 para baixa, 2 para média, 3 para alta): ";
    int prioridade;
    while (!(std::cin >> prioridade) || prioridade < 1 || prioridade > 3) {
        std::cin.clear(); // Limpa o estado de erro
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora a entrada inválida
        std::cout << "\nOpção inválida. Digite a prioridade (1 para baixa, 2 para média, 3 para alta): ";
    }
    novaTarefa.prioridade = (prioridade == 1) ? "baixa" : (prioridade == 2) ? "média" : "alta";

    tarefas.push_back(novaTarefa); // Adiciona a nova tarefa ao vetor de tarefas
    std::cout << "\nTarefa adicionada com sucesso!\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer de entrada
}

// Função para visualizar todas as tarefas
void visualizarTarefas() {
    for (const auto& tarefa : tarefas) {
        std::cout << "\nID: " << tarefa.id << "\n"
                  << "Título: " << tarefa.titulo << "\n"
                  << "Descrição: " << tarefa.descricao << "\n"
                  << "Data de Criação: " << tarefa.dataCriacao << "\n"
                  << "Status: " << tarefa.status << "\n"
                  << "Prioridade: " << tarefa.prioridade << "\n\n";
    }
}

// Função para buscar uma tarefa pelo ID
void buscarTarefaPorId() {
    int id;
    std::cout << "\nDigite o ID da tarefa: ";
    std::cin >> id;

    for (const auto& tarefa : tarefas) {
        if (tarefa.id == id) {
            std::cout << "ID: " << tarefa.id << "\n"
                      << "Título: " << tarefa.titulo << "\n"
                      << "Descrição: " << tarefa.descricao << "\n"
                      << "Data de Criação: " << tarefa.dataCriacao << "\n"
                      << "Status: " << tarefa.status << "\n"
                      << "Prioridade: " << tarefa.prioridade << "\n\n";
            return;
        }
    }
    std::cout << "\nTarefa não encontrada.\n";
}

// Função para editar uma tarefa existente
void editarTarefa() {
    int id;
    std::cout << "\nDigite o ID da tarefa para editar: ";
    std::cin >> id;

    for (auto& tarefa : tarefas) {
        if (tarefa.id == id) {
            std::cout << "\nDigite o novo título: ";
            std::cin.ignore(); // Ignora o caractere de nova linha pendente no buffer de entrada
            std::getline(std::cin, tarefa.titulo); // Lê o novo título da tarefa
            while (!validarTitulo(tarefa.titulo)) {
                std::cout << "\nTítulo é obrigatório e não pode ser somente numérico. Digite o título: ";
                std::getline(std::cin, tarefa.titulo);
            }

            std::cout << "Digite a nova descrição (máximo 200 caracteres): ";
            std::getline(std::cin, tarefa.descricao); // Lê a nova descrição da tarefa
            if (tarefa.descricao.length() > 200) {
                tarefa.descricao = tarefa.descricao.substr(0, 200); // Limita a 200 caracteres
            }

            std::cout << "Digite a nova data (dd/mm/aa): ";
            std::getline(std::cin, tarefa.dataCriacao); // Lê a nova data de criação da tarefa
            while (!validarData(tarefa.dataCriacao)) {
                std::cout << "\nFormato de data inválido ou data anterior à data atual. Digite a data (dd/mm/aa): ";
                std::getline(std::cin, tarefa.dataCriacao);
            }

            std::cout << "Digite o novo status (1 para pendente, 2 para em andamento, 3 para concluída): ";
            int status;
            while (!(std::cin >> status) || status < 1 || status > 3) {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora a entrada inválida
                std::cout << "\nOpção inválida. Digite o status (1 para pendente, 2 para em andamento, 3 para concluída): ";
            }
            tarefa.status = (status == 1) ? "pendente" : (status == 2) ? "em andamento" : "concluída";

            std::cout << "Digite a nova prioridade (1 para baixa, 2 para média, 3 para alta): ";
            int prioridade;
            while (!(std::cin >> prioridade) || prioridade < 1 || prioridade > 3) {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora a entrada inválida
                std::cout << "\nOpção inválida. Digite a prioridade (1 para baixa, 2 para média, 3 para alta): ";
            }
            tarefa.prioridade = (prioridade == 1) ? "baixa" : (prioridade == 2) ? "média" : "alta";

            std::cout << "Tarefa atualizada com sucesso!\n";
            return;
        }
    }
    std::cout << "\nTarefa não encontrada.\n";
}

// Função para excluir uma tarefa pelo ID
void excluirTarefa() {
    int id;
    std::cout << "\nDigite o ID da tarefa para excluir: ";
    std::cin >> id;

    // Remove a tarefa com o ID especificado do vetor de tarefas
    auto it = std::remove_if(tarefas.begin(), tarefas.end(), [id](const Tarefa& tarefa) {
        return tarefa.id == id;
    });

    if (it != tarefas.end()) {
        tarefas.erase(it, tarefas.end()); // Apaga a tarefa do vetor
        std::cout << "\nTarefa excluída com sucesso!\n";
    } else {
        std::cout << "\nTarefa não encontrada.\n";
    }
}

// Função para ordenar as tarefas por prioridade
void ordenarTarefasPorPrioridade() {
    std::sort(tarefas.begin(), tarefas.end(), [](const Tarefa& a, const Tarefa& b) {
        return a.prioridade < b.prioridade; // Compara as prioridades das tarefas
    });
    std::cout << "Tarefas ordenadas por prioridade.\n";
}

// Função para ordenar as tarefas por ID
void ordenarTarefasPorId() {
    std::sort(tarefas.begin(), tarefas.end(), [](const Tarefa& a, const Tarefa& b) {
        return a.id < b.id; // Compara os IDs das tarefas
    });
    std::cout << "\nTarefas ordenadas por ID.\n";
}

// Função para salvar as tarefas em um arquivo
void salvarTarefasEmArquivo() {
    std::ofstream arquivo("tarefas.txt"); // Abre o arquivo para escrita
    for (const auto& tarefa : tarefas) {
        arquivo << tarefa.id << "\n"
                << tarefa.titulo << "\n"
                << tarefa.descricao << "\n"
                << tarefa.dataCriacao << "\n"
                << tarefa.status << "\n"
                << tarefa.prioridade << "\n";
    }
    arquivo.close(); // Fecha o arquivo
    std::cout << "\nTarefas salvas em arquivo.\n";
}

// Função para carregar as tarefas de um arquivo
void carregarTarefasDoArquivo() {
    std::ifstream arquivo("tarefas.txt"); // Abre o arquivo para leitura
    if (arquivo.is_open()) {
        tarefas.clear(); // Limpa o vetor de tarefas atual
        Tarefa tarefa;
        int maxId = 1;
        while (arquivo >> tarefa.id) {
            arquivo.ignore(); // Ignora o caractere de nova linha pendente no buffer de entrada
            std::getline(arquivo, tarefa.titulo);
            std::getline(arquivo, tarefa.descricao);
            std::getline(arquivo, tarefa.dataCriacao);
            std::getline(arquivo, tarefa.status);
            std::getline(arquivo, tarefa.prioridade);
            tarefas.push_back(tarefa); // Adiciona a tarefa ao vetor
            if (tarefa.id > maxId) {
                maxId = tarefa.id; // Atualiza o maior ID encontrado
            }
        }
        proximoId = maxId + 1; // Define o próximo ID disponível
        arquivo.close(); // Fecha o arquivo
        std::cout << "\nTarefas carregadas do arquivo.\n";
    } else {
        std::cout << "\nNenhuma tarefa salva encontrada.\n";
    }
}

// Função que exibe o menu e processa as escolhas do usuário
void mostrarMenu() {
    int escolha;
    do {
        // Exibe as opções do menu
        std::cout << "\n1. Adicionar Tarefa\n"
                  << "2. Visualizar Tarefas\n"
                  << "3. Buscar Tarefa por ID\n"
                  << "4. Editar Tarefa\n"
                  << "5. Excluir Tarefa\n"
                  << "6. Ordenar Tarefas por Prioridade\n"
                  << "7. Ordenar Tarefas por ID\n"
                  << "8. Salvar Tarefas em Arquivo\n"
                  << "9. Carregar Tarefas do Arquivo\n"
                  << "10. Sair\n"
                  << "\nDigite sua escolha: ";
        while (!(std::cin >> escolha)) {
            std::cin.clear(); // Limpa o estado de erro
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora a entrada inválida
            std::cout << "\nEscolha inválida. Por favor, digite um número dentro das opções de 1 a 10: ";
        }

        // Processa a escolha do usuário
        switch (escolha) {
            case 1:
                adicionarTarefa();
                break;
            case 2:
                visualizarTarefas();
                break;
            case 3:
                buscarTarefaPorId();
                break;
            case 4:
                editarTarefa();
                break;
            case 5:
                excluirTarefa();
                break;
            case 6:
                ordenarTarefasPorPrioridade();
                break;
            case 7:
                ordenarTarefasPorId();
                break;
            case 8:
                salvarTarefasEmArquivo();
                break;
            case 9:
                carregarTarefasDoArquivo();
                break;
            case 10:
                std::cout << "\nSaindo...\n";
                break;
            default:
                std::cout << "\nEscolha inválida. Por favor, tente novamente.\n";
        }
    } while (escolha != 10); // Continua exibindo o menu até o usuário escolher sair
}

int main() {
    // carregarTarefasDoArquivo(); // carrega o arquivo na inicialização
    mostrarMenu(); // Exibe o menu para o usuário
    // salvarTarefasEmArquivo(); // Salva as tarefas em arquivo ao sair
    return 0; // Encerra o programa
}
