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
        : LoginInvalidoException("Login muito longo (maximo 20 caracteres).") {}
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
        : SenhaInvalidaException("A senha nao pode ser igual ao nome de usuario.") {}
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
