#include "spi.h"
#include "stdio.h"


void calibrate_gyr(t_pos_data *offset)
{
	int test_nb;
	uint8_t data[6] = {0};
	int sum_offset_x;
	int sum_offset_y;
	int sum_offset_z;
	int max_test_nb;
	t_pos_data value;

	test_nb = 0;
	max_test_nb = 1000;
	sum_offset_x = 0;
	sum_offset_y = 0;
	sum_offset_z = 0;
	while (test_nb < max_test_nb)
	{
		data[0] = L3GD20Gyro_read_register(0x28); //2000 DPS
		data[1] = L3GD20Gyro_read_register(0x29); //2000 DPS
		data[2] = L3GD20Gyro_read_register(0x2a); //2000 DPS
		data[3] = L3GD20Gyro_read_register(0x2b); //2000 DPS
		data[4] = L3GD20Gyro_read_register(0x2c); //2000 DPS
		data[5] = L3GD20Gyro_read_register(0x2d); //2000 DPS

		value.x = ((int16_t)(data[0])) + ((int16_t)(data[1] << 8));
		value.y = ((int16_t)(data[2])) + ((int16_t)(data[3] << 8));
		value.z = ((int16_t)(data[4])) + ((int16_t)(data[5] << 8));

		sum_offset_x += value.x;
		sum_offset_y += value.y;
		sum_offset_z += value.z;
		
		test_nb++;
	}
	offset->x = sum_offset_x / max_test_nb;
	offset->y = sum_offset_y / max_test_nb;
	offset->z = sum_offset_z / max_test_nb;

	printf("offset->x = [%d]\n", offset->x);
	printf("offset->y = [%d]\n", offset->y);
	printf("offset->z = [%d]\n", offset->z);
}

void read_data_gyr(uint8_t data[6], t_pos_data offset, t_pos_data *value)
{
	value->x = ((int16_t)(data[0])) + ((int16_t)(data[1] << 8));
	value->y = ((int16_t)(data[2])) + ((int16_t)(data[3] << 8));
	value->z = ((int16_t)(data[4])) + ((int16_t)(data[5] << 8));

	value->x -= offset.x;
	value->y -= offset.y;
	value->z -= offset.z;

	// printf("value->x = [%d]\n", value->x);
	// printf("value->y = [%d]\n", value->y);
	// printf("value->z = [%d]\n", value->z);
}

void convert_to_angle_gyr(t_angle_data *angle,
								t_pos_data value)
{
	// 2000 dps
	// 1 digit == 8.75 mdps
	// 1 digit == 0.00875 dps
	// 100 hz 12.5 cut off
	// 

	// Put f because default is double float
	// max digit is 7 for simple float
	angle->x += value.x * 0.01f * 0.00875f;
	angle->y += value.y * 0.01f * 0.00875f;
	angle->z += value.z * 0.01f * 0.00875f; 

	printf("angle->x = [%f]\n", angle->x);
	printf("angle->y = [%f]\n", angle->y);
	printf("angle->z = [%f]\n", angle->z);
}
