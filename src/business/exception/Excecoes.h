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
