//	SPI���C�u����	for ATmega�V���[�Y
//	�w����t�]�x�H�쎺	https://github.com/jsdiy
//	2020/03 - 2025/05	@jsdiy

#include <avr/io.h>
#include "spilib.h"

void	SPI_MasterInit(void)
{
	SPI_DDR |= (SPI_SCK | SPI_MOSI | SPI_SS_GROUP);	//�o�͕���
	SPI_DDR &= ~SPI_MISO;	//���͕���
	SPI_PORT &= ~(SPI_SCK | SPI_MOSI);	//Lo���o��
	SPI_PORT |= SPI_SS_GROUP;	//�f�o�C�X��I�������̏�ԂƂ��邽��SS��Hi���o�́iSS�͕��_���j
	
	SPCR = (1 << SPE) | (0 << DORD) | (1 << MSTR)	//SPI�L����(1)�AMSB��s(0)�A�}�X�^�[�f�o�C�X(1)
		| (1 << CPOL) | (1 << CPHA)		//SCK�̋ɐ��E�ʑ��̐ݒ�
		| (0 << SPR1) | (0 << SPR0);	//�N���b�N�����̐ݒ�
	SPSR = (1 << SPI2X);				//�N���b�N�����̐ݒ�
	/*	SCK�̋ɐ��E�ʑ��̐ݒ�<CPOL,CPHA>
	ST7735S�� <0,0> or <1,0> or <1,1>	SPI���[�h0,2,3
	ST7789V�� <1,0> or <1,1>				SPI���[�h2,3
	�c�œ��삵���B
	*/
	/*	�N���b�N�����̐ݒ�(SPR1:SPR0:SPI2X)
	0:0:1 = f_osc/2�i�ő��ݒ�j
	0:0:0 = f_osc/4
	0:1:1 = f_osc/8
	�c
	1:1:0 = f_osc/128�i�ł��x���ݒ�j
	ST7735S/ST7789V��ATmega328P�̃V�X�e���N���b�N(max:20MHz)�ɂ�炸�ő��ݒ��ok
	*/
}

//�X���[�u�f�o�C�X��I������
void	SPI_SlaveSelect(ESpiSs ss)
{
	SPI_PORT &= ~ss;	//Lo���o�́iSS�͕��_���j
}

//�X���[�u�f�o�C�X��I����������
void	SPI_SlaveDeselect(ESpiSs ss)
{
	SPI_PORT |= ss;		//Hi���o�́iSS�͕��_���j
}

//1byte���M�i�Ɠ�����1byte��M�j
void	SPI_MasterTransmit(uint8_t data)
{
	SPDR = data;	//�]���J�n�i���̏u�Ԃ�SCK���o�͊J�n����j
	while (!(SPSR & (1<<SPIF)));	//�]�������҂�
	//�]�������i���̎��_�Ń��W�X�^SPDR�ɂ͎�M�f�[�^���i�[����Ă���BSCK�͏o�͒�~����j
}

//��M�o�b�t�@����byte�f�[�^�����o��
uint8_t	SPI_MasterReceive(void)
{
	return SPDR;
}
