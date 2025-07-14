# Trabalho de Espectro de áudio com OpenGL
**Realizado por Mateus Cavalcanti**


## Resumo

Este trabalho tem como objetivo realizar um plot polar do espectro de uma faixa áudio utilizando as ferramentas de OpenGL
aprendidas durante a disciplina de computação gráfica durante o semestre 2025.1

## Instalação

Para instalar as dependências para realização do trabalho pode-se utilizar o script em bash na pasta raíz do projeto.

**Exemplo**
```sh
$ chmod +x install.sh
$ ./install
```

## Compilação

Para compilar o programa em C++ deve-se linkar as bibliotecas de FFTW, SDL, Math e OpenGL, conforme o exemplo a seguir:

**Exemplo**
```sh
$ g++ spectrosdl.cpp -o spectro -lSDL2 -lSDL2_mixer -lfftw3 -lGL -lGLU -lglut -lm
```

Isto irá gerar um binário executável.

## Execução

Para a execução basta passar como argumento do binário executável o caminho para o diretório contendo um arquivo MP3 de áudio.

```sh
$ ./spectro arquivo.mp3
```