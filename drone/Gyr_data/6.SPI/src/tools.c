#include "spi.h"

void			ft_print_hexa(uint8_t data)
{
	char	hex_char;

	if (((data & 0b11110000) >> 4) > 9)
		hex_char = ((data & 0b11110000) >> 4) + 55;
	else
		hex_char = ((data & 0b11110000) >> 4) + 48;
	_write(0, &hex_char, 1);
	if ((data & 0b1111) > 9)
		hex_char = (data & 0b1111) + 55;
	else
		hex_char = (data & 0b1111) + 48;
	_write(0, &hex_char, 1);
}

void	print_data(uint8_t *data, uint8_t len)
{
	_write(0, "DATA\n", 5);
	int a = 0;
	while (a < len)
	{
		ft_print_hexa(data[a]);	
		a++;
	}
	_write(0, "\n----\n", 6);
}
