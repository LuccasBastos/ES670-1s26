# ES670-1s26
Repositório destinado para a matéria ES670: Projetos de Sistemas Embarcados.

## Padrão de codificação
Seguem as seguintes regras convencionais para padronização do código.

### Regra 01: Nome dos Arquivos
Utilizar notação camelo no nome dos arquivos
   
Ex.: automaticPilotControl.x

### Regra 02: Cabeçalho dos arquivos
```
/*
******************************************************************************
* @file    : nomeDoArquivo.x
* @brief   : Descrição da funcionalidade do arquivo
******************************************************************************
* @author  : Luccas Bastos e Vinicius Balbino
* @date    : Data de criação
* @version : Data da última versão (provavelmente hoje)
******************************************************************************
*/
```

### Regra 03: Conteúdo dos Arquivos
Siga a ordem no código de programação:
 1. Bloco de comentário (regra anterior)
 2. Inclusão de arquivos de cabeçalho (arquivos .h)
 3. Typedefs
 4. Defines
 5. Declaração de variáveis globais
 6. Métodos e funções

### Regra 04: Conteúdos dos arquivos .h
Todo arquivo .h tem que ter o header guard como nome do arquivo sem nenhum separador de palavras (espaços e _) e em letras maiúsculas, terminando como _H.

Ex.: #ifndef AUTOPILOTCONTROL_H

#define AUTOPILOTCONTROL_H

### Regra 05: Comentários
O código deve conter cometários breves e significativos no seguinte formato:
 
 Ex.: //comentário explicando a próxima linha de código
 
 Linha do código 

### Regra 06: Nome de variáveis
As variáveis devem ser iniciadas com prefixo que indica o tipo da variável em minúsculo, seguindo a notação camelo
 
 Ex.: unsigned int uiPitchGain
| Tipo                                  | Prefixo |
|---------------------------------------|---------|
|  char                                 |  c      |
|  unsigned char                        |  uc     |
|  int                                  |  i      |
|  unsigned int                         |  ui     |
|  long int                             |  l      |
|  unsigned long int                    |  ul     |
|  short int                            |  s      |
|  unsigned short int                   |  us     | 
|  long long                            |  ll     |
|  unsigned long long                   |  ull    |
|  float                                |  f      |
|  double                               |  d      |
|  long double                          |  ld     |
|  ponteiros                            |  p      |
|  outros tipos (structs, unions, etc)  |  x      |

### Regra 07: Nome de funções
O nome de funções deve ser formado por: (i) prefixo que indica o tipo de retorno, (ii) nome do arquivo, (iii) nome da função (seguindo notação camelo)
 
 Ex.: fTemperatureSensorGetTemperature()
 
 vPIDControllerSetGains(float fP, float fI, float fD)

### Regra 08: Definição de métodos e funções
```
/*
******************************************************************************
* @method : nomeDoMetodo.x
* @brief  : Descrição da funcionalidade do método
* @param  : Descrição dos parâmetros de entrada
* @return : Descrição dos parâmetros de saída
******************************************************************************
*/
```

### Regra 09: Indentação
No código, utilizar indentação com 2 espaços. 

### Regra 10: Nomes de Constantes
Nomes de constantes devem ser em maiúsculo

Ex.: #define TRUE 1

### Regra 11: Limite de caracteres por linha
A linha deve ter no máximo oitenta caracteres, se for ultrapassado esse limite, a linha deve ser quebrada

80 caracteres:

```
********************************************************************************
```

## Projetos desenvolvidos na disciplina
### Aula 2 - Familiarização com o kit de desenvolvimento e com a IDE
Atividade 1 - Piscar LED.

### Aula 4 - Apresentação dos requisitos do projeto prático da disciplina e modelagem
Atividade 2 - Análise de requisitos do projeto e início da modelagem.

### Aula 6 - Desenvolvimento de uma biblioteca para leitura dos botões e controle dos LEDs por GPIO. Uso de GPIO com registradores.
Atividade 3 - Controlar os botões e leds através dos registradores da placa STM.

### Aula 8 - Uso de timers e interrupções. Desenvolvimento de uma biblioteca para o teclado matricial utilizando multiplexação do tempo.
Atividade 4 - Controlar através do teclado matricial o uso de timers.

### Aula 10 - Interrupções e Debouncing de GPIOs
Atividade 5 - 

### Aula 12 - Implementação do protocolo de comunicação
Atividade 6 - 
