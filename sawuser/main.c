//****************************************************
//
//   EXAMPLE OF USING CS42L22 on STM32F4-Discovery
//
//   author: A.Finkelmeyer
//   http://www.mind-dump.net/configuring-the-stm32f4-discovery-for-audio
//
//****************************************************

#include "main.h"

volatile float_t note_frequency = 0.015;

const uint16_t USER_BUTTON = GPIO_Pin_0;

int main(void)
{
	SystemInit();

	fir_8 filt;

	//enables GPIO clock for PortD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    //enables GPIO clock for user button
    //https://www.badprog.com/electronics-stm32-using-the-user-button-to-play-with-the-lcd-screen-and-the-stm32f103ze-sk-board
    //https://github.com/k-code/stm32f4-examples/blob/master/Task-2-Buttons/src/main.c
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //setting User Button pin
    GPIO_InitTypeDef GPIO_InitUser;
    GPIO_InitUser.GPIO_Pin = USER_BUTTON;
    GPIO_InitUser.GPIO_Mode = GPIO_Mode_IN;

    GPIO_Init(GPIOD, &GPIO_InitUser);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	codec_init();
	codec_ctrl_init();

	I2S_Cmd(CODEC_I2S, ENABLE);

	initFilter(&filt);


    while(1)
    {

        if (GPIO_ReadInputDataBit(GPIOA, USER_BUTTON)) {
            note_frequency += 0.005;
            if (note_frequency >= 1) {
                note_frequency = 0.015;
            }
        }

    	if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE))
    	{
    		SPI_I2S_SendData(CODEC_I2S, sample);

    		//only update on every second sample to insure that L & R ch. have the same sample value
    		if (sampleCounter & 0x00000001)
    		{
    			sawWave += note_frequency;
    			if (sawWave > 1.0)
    				sawWave -= 2.0;

    			filteredSaw = updateFilter(&filt, sawWave);

    			sample = (int16_t)(NOTEAMPLITUDE*filteredSaw);
    		}
    		sampleCounter++;
    	}

    	if (sampleCounter==48000)
    	{
    		LED_BLUE_OFF;

    	}
    	else if (sampleCounter == 96000)
    	{
    		LED_BLUE_ON;
    		sampleCounter = 0;
    	}

    }
}

// a very crude FIR lowpass filter
float updateFilter(fir_8* filt, float val)
{
	uint16_t valIndex;
	uint16_t paramIndex;
	float outval = 0.0;

	valIndex = filt->currIndex;
	filt->tabs[valIndex] = val;

	for (paramIndex=0; paramIndex<8; paramIndex++)
	{
		outval += (filt->params[paramIndex]) * (filt->tabs[(valIndex+paramIndex)&0x07]);
	}

	valIndex++;
	valIndex &= 0x07;

	filt->currIndex = valIndex;

	return outval;
}

void initFilter(fir_8* theFilter)
{
	uint8_t i;

	theFilter->currIndex = 0;

	for (i=0; i<8; i++)
		theFilter->tabs[i] = 0.0;

	theFilter->params[0] = 0.01;
	theFilter->params[1] = 0.05;
	theFilter->params[2] = 0.12;
	theFilter->params[3] = 0.32;
	theFilter->params[4] = 0.32;
	theFilter->params[5] = 0.12;
	theFilter->params[6] = 0.05;
	theFilter->params[7] = 0.01;
}
