#pragma once

#include <stdexcept>
#include <string>

// ---------- Excecoes de login (LoginInvalidoException) ----------
class LoginInvalidoException : public std::runtime_error {
public:
    explicit LoginInvalidoException(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

class LoginLongoException : public LoginInvalidoException {
public:
    LoginLongoException()
        : LoginInvalidoException("O login deve ter no maximo 12 caracteres.") {}
};

class LoginVazioException : public LoginInvalidoException {
public:
    LoginVazioException()
        : LoginInvalidoException("O login nao pode ser vazio.") {}
};

class LoginComNumeroException : public LoginInvalidoException {
public:
    LoginComNumeroException()
        : LoginInvalidoException("O login nao pode conter numeros.") {}
};

class LoginEmUsoException : public LoginInvalidoException {
public:
    LoginEmUsoException()
        : LoginInvalidoException("Login ja esta em uso.") {}
};

// Falha de autenticacao (RF07, NF-SEG-02): a mensagem nao diz se o erro
// foi no login ou na senha.
class CredenciaisInvalidasException : public std::runtime_error {
public:
    CredenciaisInvalidasException()
        : std::runtime_error("Login ou senha invalidos.") {}
};

// ---------- Excecoes de senha (SenhaInvalidaException) ----------
class SenhaInvalidaException : public std::runtime_error {
public:
    explicit SenhaInvalidaException(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

class SenhaIgualUsuarioException : public SenhaInvalidaException {
public:
    SenhaIgualUsuarioException()
        : SenhaInvalidaException("A senha nao pode ser igual ao login.") {}
};

class SenhaCaracteresInsuficientesException : public SenhaInvalidaException {
public:
    SenhaCaracteresInsuficientesException()
        : SenhaInvalidaException("A senha deve conter ao menos 3 dos 4 tipos: maiusculas, minusculas, numeros e especiais.") {}
};

class SenhaTamanhoInvalidoException : public SenhaInvalidaException {
public:
    SenhaTamanhoInvalidoException()
        : SenhaInvalidaException("A senha deve ter entre 8 e 128 caracteres.") {}
};

// ---------- Excecoes de armazenamento ----------
// Equivalente a IOException do Java: qualquer falha de leitura, gravacao
// ou arquivo corrompido nos repositorios e relancada com este tipo.
class FalhaArmazenamentoException : public std::runtime_error {
public:
    explicit FalhaArmazenamentoException(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

// ---------- Excecao de identificador (IdInvalidoException) ----------
// ID que nao corresponde a nenhum cadastro (ex.: dono ou personagem inexistente).
class IdInvalidoException : public std::runtime_error {
public:
    explicit IdInvalidoException(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

// ---------- Excecoes da ficha do personagem (FichaInvalidaException) ----------
class FichaInvalidaException : public std::runtime_error {
public:
    explicit FichaInvalidaException(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

class NomePersonagemInvalidoException : public FichaInvalidaException {
public:
    NomePersonagemInvalidoException()
        : FichaInvalidaException("O nome do personagem deve ter de 1 a 30 caracteres.") {}
};

class NivelInvalidoException : public FichaInvalidaException {
public:
    NivelInvalidoException()
        : FichaInvalidaException("O nivel deve estar entre 1 e 20.") {}
};

class PontosDeVidaInvalidosException : public FichaInvalidaException {
public:
    PontosDeVidaInvalidosException()
        : FichaInvalidaException("Os pontos de vida devem ser maiores que zero.") {}
};

class AtributoInvalidoException : public FichaInvalidaException {
public:
    explicit AtributoInvalidoException(const std::string& atributo)
        : FichaInvalidaException("O atributo " + atributo + " deve estar entre 1 e 30.") {}
};
