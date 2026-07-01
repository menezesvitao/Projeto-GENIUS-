/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define MAX_RODADAS 100

// Identificadores de cada cor
#define VERDE    0
#define AZUL     1
#define VERMELHO 2
#define AMARELO  3

uint8_t sequencia[MAX_RODADAS]; // Guarda a sequência do jogo
uint8_t rodada_atual = 0;       // Quantidade de passos que o jogador já alcançou
/* USER CODE END PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */



void ligar_cor(uint8_t cor);
void piscar_cor(uint8_t cor, uint16_t tempo);
void som_game_over(void);
uint8_t ler_botoes(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // --- ADICIONE ISSO LOGO NO INÍCIO DO WHILE(1) ---
	  if (rodada_atual == 0) {
	      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_SET); // Liga todos os LEDs
	     // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // Desliga o buzzer
	      HAL_Delay(1000); // 1 segundo
	      uint8_t botao = 255;
	      while (botao == 255) {
	     	botao = ler_botoes();
	      }
	     apagar_leds();
	  }

	  // 1. SORTEIA UMA NOVA COR PARA A RODADA ATUAL
	      // Usamos o contador do HAL_GetTick() para gerar um número pseudo-aleatório de 0 a 3
	      sequencia[rodada_atual] = HAL_GetTick() % 4;
	      rodada_atual++;

	      // 2. MOSTRA A SEQUÊNCIA ATÉ AGORA PARA O JOGADOR
	      HAL_Delay(800); // Espera um pouco antes de começar a ditar a sequência
	      for (int i = 0; i < rodada_atual; i++) {
	          piscar_cor(sequencia[i], 400); // Pisca cada cor cadastrada por 400ms
	      }

	      // 3. AGUARDA A RESPOSTA DO JOGADOR (PASSO A PASSO)
	      for (int passo = 0; passo < rodada_atual; passo++) {
	          uint8_t botao_apertado = 255;

	          // Fica preso esperando o jogador apertar qualquer botão válido
	          while (botao_apertado == 255) {
	              botao_apertado = ler_botoes();

	          }

	          // VERIFICA SE ELE ERROU O PASSO ATUAL
	          if (botao_apertado != sequencia[passo]) {
	        	  HAL_Delay(500);
	              som_game_over();   // Toca som de erro e acende tudo
	              rodada_atual = 0;  // Reseta o jogo de volta para o nível 1
	              break;             // Abandona a checagem interna para recomeçar o while
	          }
	      }

	      // Se saiu do loop sem resetar a rodada_atual, ele passou de nível!
	      if (rodada_atual > 0) {
	          HAL_Delay(500); // Pequena folga de comemoração antes do próximo nível aumentar
	      }

  } //----------------------------------------------- WHILE
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA2 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA3 PA4 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Função que acende o LED correspondente e toca o Buzzer
void ligar_cor(uint8_t cor) {
    if (cor == VERDE) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); // 0 Liga
    } else if (cor == AZUL) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    } else if (cor == VERMELHO) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    } else if (cor == AMARELO) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Liga Buzzer
}





void apagar_leds(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET); // Apaga
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // Desliga Buzzer
}


// Mostra o LED piscando por um tempo padrão do jogo
void piscar_cor(uint8_t cor, uint16_t tempo) {
    ligar_cor(cor);
    HAL_Delay(tempo);
    apagar_leds();
    HAL_Delay(150); // Pausa entre uma cor e outra da sequência
}

// Animação e som caso o jogador erre a sequência
void som_game_over(void)
{
    for(int i = 0; i < 2; i++)
    {
        ligar_cor(VERDE);
        HAL_Delay(120);
        apagar_leds();

        ligar_cor(AZUL);
        HAL_Delay(120);
        apagar_leds();

        ligar_cor(VERMELHO);
        HAL_Delay(120);
        apagar_leds();

        ligar_cor(AMARELO);
        HAL_Delay(120);
        apagar_leds();
    }

    // Final
    HAL_GPIO_WritePin(GPIOA,
                      GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,
                      GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    HAL_Delay(700);

    apagar_leds();
}

// Captura qual botão foi apertado aplicando o Debounce robusto
// Retorna a cor apertada, ou 255 se nenhum botão foi pressionado
uint8_t ler_botoes(void) {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) { // Verde
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET){
            ligar_cor(VERDE);
            HAL_Delay(150);
            apagar_leds();
            while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET);
            HAL_Delay(20);
            return VERDE;
        }
    }
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) { // Azul
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET){
            ligar_cor(AZUL);
            HAL_Delay(150);
            apagar_leds();
            while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET);
            HAL_Delay(20);
            return AZUL;
        }
    }

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_RESET) { // Vermelho
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_RESET){
            ligar_cor(VERMELHO);
            HAL_Delay(150);
            apagar_leds();
            while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_RESET);
            HAL_Delay(20);
            return VERMELHO;
        }
    }
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET) { // AMARELO
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET){
            ligar_cor(AMARELO);
            HAL_Delay(150);
            apagar_leds();
            while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET);
            HAL_Delay(20);
            return AMARELO;
        }
    }

    return 255; // Código de "nenhum botão pressionado"
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
