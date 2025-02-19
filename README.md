# Contador-bitdoglab

## Descrição

Este projeto implementa um contador utilizando o Raspberry Pi Pico. Ele exibe um número na matriz de LEDs e permite incrementar ou decrementar esse número através de botões.

## Funcionalidades
- Exibição de números em uma matriz de LEDs.
- Controle por dois botões:
  - **Botão A**: Incrementa o contador.
  - **Botão B**: Decrementa o contador.
- Utiliza um LED vermelho para indicar estado.
- Uso de interrupções para detecção eficiente dos botões.

## Estrutura do Projeto

### Arquivos Principais

- `main.c`: Contém o código principal do contador.
- `setup.h` e `setup.c`: Configuração inicial do hardware.
- `led.h` e `led.c`: Controle de LEDs individuais.
- `matriz_leds.h` e `matriz_leds.c`: Controle da matriz de LEDs.
- `ws2812.pio.h`: Controle dos LEDs WS2812.
- `CMakeLists.txt`: Arquivo de configuração para compilação com CMake.
- `.vscode/`: Configurações para desenvolvimento no VS Code.

### Componentes Utilizados

- **Raspberry Pi Pico**
- **Matriz de LEDs WS2812**
- **Botões para entrada de controle**
- **LED vermelho para sinalização**

## Como Funciona o Código

### Inicialização
No `main.c`, o código inicializa os pinos de controle, LEDs e configura a matriz de LEDs para exibir o contador.

### Controle dos Botões
Os botões usam debounce para evitar leituras erradas e modificam o valor do contador.

### Temporizador
O código usa a biblioteca `hardware/timer.h` para criar delays e controlar eventos periódicos.

### Interrupções Utilizadas
O projeto faz uso de interrupções para garantir um funcionamento eficiente da detecção dos botões:

- **Interrupção nos pinos dos botões**: 
  - Configurada para detectar borda de descida.
  - Evita polling constante, economizando recursos da CPU.
  - Quando um botão é pressionado, um callback é acionado para atualizar o contador.

- **Uso do temporizador**: 
  - Garante que as alterações sejam aplicadas com um pequeno atraso para evitar glitches.
  - Atualiza a matriz de LEDs de forma controlada.

## Módulos e Suas Funções

### `main.c`
- Inicializa o sistema e configura os pinos de entrada e saída.
- Configura o temporizador para atualização periódica do display.
- Processa as entradas dos botões e atualiza o contador na matriz de LEDs.

### `setup.h` e `setup.c`
- Contêm funções para inicializar os periféricos do Raspberry Pi Pico.
- Configuração dos pinos GPIO e inicialização da matriz de LEDs.

### `led.h` e `led.c`
- Controla LEDs individuais.
- Contém funções para acender e apagar LEDs específicos.

### `matriz_leds.h` e `matriz_leds.c`
- Implementa a exibição dos números na matriz de LEDs.
- Contém funções para converter valores numéricos em padrões de exibição.

### `ws2812.pio.h`
- Define o controle dos LEDs WS2812 utilizando PIO (Programmed Input/Output).
- Facilita a comunicação e atualização da matriz de LEDs.

### `CMakeLists.txt`
- Define as regras de compilação do projeto para o Raspberry Pi Pico.

## Como Compilar e Executar

1. Certifique-se de ter o **Pico SDK** instalado.
2. Clone este repositório e entre no diretório do projeto.
3. Execute os seguintes comandos:
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```
4. Envie o arquivo `.uf2` gerado para o Raspberry Pi Pico.
